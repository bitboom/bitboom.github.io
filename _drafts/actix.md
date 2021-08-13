1. https://actix.rs/docs/getting-started/
2. curl --header "Content-Type: application/json"   --request POST   --data '{"username":"xyz","password":"xyz"}'   http://localhost:8080/echo

## Under the hood (https://actix.rs/docs/getting-started/)
- HttpServer는 특정 주소(url)로 Request를 받으면 Response를 반환
  - Request를 받으면 => `Request handler`
  - Response를 반환 => `impl Responder`
  - 특정 주소(url) => routing information with `built-in macros`
  
```rust

// Request handler App::service
#[get("/")] // routing information with `built-in macros
async fn hello() -> impl Responder {
    HttpResponse::Ok().body("Hello world!")
}

// Request handler for App::service
#[post("/echo")]
async fn echo(req_body: String) -> impl Responder {
    HttpResponse::Ok().body(req_body)
}

// Request handler for App::route
// routes that do not use a routing macro
async fn manual_hello() -> impl Responder {
    HttpResponse::Ok().body("Hey there!")
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    HttpServer::new(|| {    // HttpServer which will serve incoming requests to app
        App::new()          // create an App instance w/ application factory!
            .service(hello) // register the request handlers
            .service(echo)  // App::service for the handlers using routing macros
            .route("/hey", web::get().to(manual_hello)) // App::route for manually routed handlers, declaring the path and method
    })
    .bind("127.0.0.1:8080")?
    .run()
    .await
}
```

## application
> actix-web provides various primitives to build web servers and applications with Rust
- routing
- middleware
- pre-processing of requests
- post-processing of responses

- server는 App 인스턴스 중심으로 제작됨
- App 인스턴스는 resources and middleware를 등록 하는데 사용
- App 인스턴스는 handlers에서 접근 가능한 state를 저장

### [Scope](https://docs.rs/actix-web/3.3.2/actix_web/struct.Scope.html)
> Scope is a set of resources with common root path.
- application’s scope acts as a namespace for all routes
- scope have the same url path prefix.

```Rust
use actix_web::{web, App, HttpResponse};

fn main() {
    let app = App::new().service(
        web::scope("/{project_id}/")
            .service(web::resource("/path1").to(|| async { HttpResponse::Ok() }))
            .service(web::resource("/path2").route(web::get().to(|| HttpResponse::Ok())))
            .service(web::resource("/path3").route(web::head().to(|| HttpResponse::MethodNotAllowed())))
    );
}

// => /{project_id}/path1 - responds to all http method
// => /{project_id}/path2 - GET requests
// => /{project_id}/path3 - HEAD requests
```

### State
> Application state is shared with all routes and resources **within the same scope**

- 같은 scopte에서 접근할 수 있는 State(data)
- `web::Data<T>`를 통해 가져 올 수 있음

```Rust
use actix_web::{get, web, App, HttpServer};

// This struct represents state
struct AppState {
    app_name: String,
}

#[get("/")]
async fn index(data: web::Data<AppState>) -> String {
    let app_name = &data.app_name; // <- get app_name

    format!("Hello {}!", app_name) // <- response with app_name
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    HttpServer::new(|| {
        App::new()
            .data(AppState { // pass the state
                app_name: String::from("Actix-web"),
            })
            .service(index)
    })
    .bind("127.0.0.1:8080")?
    .run()
    .await
}
```

### Mutable State
- HttpServer는 각 스레드마다 application instance를 생성함
- 따라서, Application data는 여러번 생성 될 수 있음 
- thread간 shared data를 공유하고 싶으면, `Send + Sync`를 상속
  - Internally, web::Data uses Arc.
  - Thus, in order to avoid creating two Arcs,
  - we should create our Data before registering it using **App::app_data()**
  
```
use actix_web::{web, App, HttpServer};
use std::sync::Mutex;

struct AppStateWithCounter {
    counter: Mutex<i32>, // <- Mutex is necessary to mutate safely across threads
}

async fn index(data: web::Data<AppStateWithCounter>) -> String {
    let mut counter = data.counter.lock().unwrap(); // <- get counter's MutexGuard
    *counter += 1; // <- access counter inside MutexGuard

    format!("Request number: {}", counter) // <- response with count
}

// shared object among threads
#[actix_web::main]
async fn main() -> std::io::Result<()> {
    let counter = web::Data::new(AppStateWithCounter {
        counter: Mutex::new(0),
    });

    HttpServer::new(move || {
        // move counter into the closure **
        App::new()
            // Note: using app_data instead of data
            .app_data(counter.clone()) // <- register the created data
            .route("/", web::get().to(index))
    })
    .bind("127.0.0.1:8080")?
    .run()
    .await
}
```

### Application guards
- guard as a simple function that accepts a request object reference and returns true or false
- request 검사 할 때 사용

```Rust
.service(
    web::scope("/")
        .guard(guard::Header("Host", "www.rust-lang.org"))
        .route("", web::to(|| HttpResponse::Ok().body("www"))),
)
```

### Configure
```Rust
use actix_web::{web, App, HttpResponse, HttpServer};

// this function could be located in a different module
fn scoped_config(cfg: &mut web::ServiceConfig) {
    cfg.service(
        web::resource("/test")
            .route(web::get().to(|| HttpResponse::Ok().body("test")))
            .route(web::head().to(|| HttpResponse::MethodNotAllowed())),
    );
}

// this function could be located in a different module
fn config(cfg: &mut web::ServiceConfig) {
    cfg.service(
        web::resource("/app")
            .route(web::get().to(|| HttpResponse::Ok().body("app")))
            .route(web::head().to(|| HttpResponse::MethodNotAllowed())),
    );
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    HttpServer::new(|| {
        App::new()
            .configure(config)
            .service(web::scope("/api").configure(scoped_config))
            .route("/", web::get().to(|| HttpResponse::Ok().body("/")))
    })
    .bind("127.0.0.1:8080")?
    .run()
    .await
}

// => /         -> "/"
// => /app      -> "app"
// => /api/test -> "test"
```

## Server
- HttpServer type is responsible for serving HTTP requests.
- HttpServer accepts an **application factory** as a parameter,
- To bind to a specific socket address, 
  - bind() must be used, and it may be called multiple times
-  bind ssl socket, bind_openssl() or bind_rustls() should be used.
- HttpServer automatically starts a number of HTTP workers
