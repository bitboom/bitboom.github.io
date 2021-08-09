## Web Service

## Front-end

#### `http`를 통해 HTML 문서를 받는다.
우리는 `http` 혹은 `https`로 시작 되는 주소를 통해
웹사이트에 접근 한다.
HTTP(HyperText Transfer Protocol)는
W3(WWW, World Wide Web)상에서
클라이언트와 서버 사이에 정보를 주고 받는
프로토콜이다.
클라이언트-서버 구조는 요청-응답으로 구성 된다.
여기서 클라이언트는 브라우저를 의미하고, `http`로 시작되는 주소는 서버의 주소이다.

즉, 브라우저를 통해 서버에 있는 HTML 문서를 받아 브라우저에서 실행 시키는 것이다.

### 브라우저에서 동작하는 웹 애플리케이션 (Web Application)
안드로이드에서 동작하는 애플리케이션을 안드로이드 애플리케이션이라 부른다.
아이폰에서 동작하는 애플리케이션을 ios 애플리케이션이라 부른다.
**웹 애플리케이션은 브라우저에서 동작하는 애플리케이션이다.**

- 안드로이드 애플리케이션은 `java`와 `kotlin`으로 구현한다.
- ios 애플리케이션은 `swift`로 구현한다.
- 웹 애플리케이션은 `html, css, javascript`로 구현한다.

### 웹 클라이언트 애플리케이션은 자바스크립트로 개발한다.
웹 사이트는 클라이언트-서버 구조 이기에,
브라우저에서 클라이언트가 실행 되고 서버에 HTML 문서를 요청,
웹 클라이언트 애플리케이션, 웹 서버 애플리케이션으로 구분 할 수 있다.

웹 클라이언트는 자바스크립트로 개발하고, 웹 서버는 C#, 자바, 루비, 파이썬 등으로 개발한다.
Node.js를 사용하여 자바스크립트로 웹 서버를 개발 할 수도 있다.

### 프론트엔드의 구성
웹 클라이언트 애플리케이션을 FE(Front-end)라 부르고,
웹 서버 애플리케이션을 BE(Back-end)라 부른다.

FE는 주로 HTML, CSS, Javascript로 구성된다.
- HTML은 웹 애플케이션의 레이아웃을 표현
- CSS는 HTML로 작성 된 웹 애플리케이션을 디자인
- Javacript FE의 로직을 구현

아래는 기본적인 HTML5 페이지이다.

```HTML
<!DOCTYPE html>
<html lang="en">
  <head>
    <title> <!-- 브라우저 탭에 표시 되는 텍스트 --> </title>
    <script> <!-- 자바스크립트 코드 --> </script>
  </head>
  <body>
    <!-- 브라우저에 그려지는 요소 -->
  </body>
</html>
```

### 자바스크립트를 작동하게하는 브라우저 플랫폼
브라우저 객체 모델 (BOM, Browser Object Model)은 브라우저와 관련된 **자바스크립트 객체**의 집합을 의미.

- window: 브라우저 기반 최상위 객체
  - location: 주소와 관련된 객체
  - navigator: 브라우저와 관련된 객체
  - history: 기록과 관련된 객체
  - screen: 화면 전체와 관련된 객체
  - document: HTML 문서와 관련된 객체

```js
<!-- 브라우저 창을 열어, HTML 문서를 렌더링 -->
<script>
  var child = window.open('', '', 'width=300, height=200');
  child.document.write('<h1> HTML DOC </h1>')
</script>
```

### 브라우저가 HTML 문서를 인식하는 방식
DOM은 브라우저가 HTML을 인식하는 방식이다.
document와 관련되어 있다.

HTML 문서에 존재하는 html이나 body를 태그라고 부른다.
이 태그를 자바스크립트에서 이용할 수 있는 객체로 만들면, 그것이 document object이다.

### 정적 페이지, 동적 페이지
브라우저가 HTML 문서를 읽어서 그대로 보여주는 것은 정적으로 문서 객체를 생성한다고 표현한다.
자바스크립트로 HTML 문서에 없던 문서 객체(태그)를 생성 하는 것을 동적으로 문서 객체를 생성한다고 표현한다.


### JSON
자바스크립트의 기본 자료형은 숫자, 문자, 불, 함수, undefined이다.

배열 내부에 있는 값을 요소(element)라 부른다.
각체 내부에 있는 값을 속성(property)라 부른다.

다른 언어에서는 이렇게 요소와 속성을 구분하지만,
자바 스크립트에서는 요소와 속성이 같다. 배열도 객체로 분류하기 떄문이다.

### 함수와 메서드
객체의 속성 중 함수 자료형을 메서드라 부른다.

### 객체는 for문을 사용 할 수 없다.
`for in` 반복문을 사용하여 속성에 접근해야 한다.

```js
for (var key in product) {
  output += key + ':' + product[key] + '\n';
}
```

### Vue 는 웹 페이지를 개발하기 위한 FE 프레임워크
- UI 개발방법 중 MVVM 패턴에서 ViewModel에 해당하는 라이브러리
- 컴포넌트 기반 프레임워크

```
        View Model
View -> DOM Listener -> Model
     <- Data Binding <-
-----------------------------
DOM     Vue.js          js objects
                        data
```

- Button clink -> DOM listener

### Vue instance vs component
- new Vue(); 뷰 인스턴스
  - el: vue가 그려질 위치 <div id="app"> <=> #app (css 선택자)
  - data: data Binding
  - template: 화면에 표시할 HTML, CSS 등 마크업 요소 정
- Component; 화면을 구성할 수 있는 블록

### 컴포넌트 등록하기 전역, 지역
- 컴포넌트 => HTML 사용자 정의 태그
- Vue.componet('my-component') => <my-component>
- new Vue({ componets: })

### 상,하위 컴포넌트
```
           props 전달
상위 컴포넌트 ---------> 하위 컴포넌트
          <---------
           이벤트 발생

// 등록
compnent('이름', { props: ['props 속성 이름'] })

// 그리고 전달
<child-component v-bind:props 속성 이름="넘길 데이터">
```

- 하위컴포넌트에서 상위컴포넌트로 이벤트 전달 가능
- 이벤트 버스를 사용하면 지정한 2개의 컴포넌트 사이에 데이터 전달 가능

### 페이지간 이동 라우터
싱글 페이지 애플리케이션에서 사용하는 웹 페이지 간의 이동 방법
- <router-link to=""> 지정한 url로 이동
- <router-view> 변경되는 url에 따라 컴포넌트를 그려주는 영역

- 서버에서 데이터를 가져오는 HTTP 통-
