---
layout: post
title: Osquery's Event Framework
tags: [osquery]
---

본 글은 [How RocksDB is used in osquery](https://code.fb.com/security/how-rocksdb-is-used-in-osquery/)의 내용 중 Event Framework에 관한 것을 번역 및 추가 해석 한 것이다.  

Event Framework는 크게 2가지의 주체를 가진다.   

- Event Publisher: Event를 발생시키는 주체 
- Event Subscriber: Event를 받는 주체

이에 따라, Event Framework를 PubSub 구조/패턴이라고도 한다. Osquery에서 PubSub을 어떻게 정의하는지 알아보기 전에 필요한 내용들을 정리해보자.  

---

# Event Contexts
먼저 어떤 Event를 주고 받을 지를 정의해야 하는데, 그것을 Event Context에 정의한다. 즉, Event의 metadata를 정의한 것이고, 이는 Osqeury table의 Scheme와 대응 된다.  

**Osquery Event Framework의 목적은 Event가 발생 했을 때 그 Event를 RocksDB에 저장하고, 사용자가 질의(Query)했을 때 그 사이에 일어난 Event를 알 수 있게 하는 것이다.**

```cpp
struct UdevEventContext : public EventContext {
  /// A pointer to the device object, most subscribers will only use device.
  struct udev_device* device;
  
  /// The udev_event_action identifier.
  udev_event_action action;
  
  /// Action as a string (as given by udev).
  std::string action_string;

  // Additional metadata about the event
  std::string subsystem;
  std::string devnode;
  std::string devtype;
  std::string driver;
};
```

---

# Subscription Contexts
만약 Event Subscriber가 연관된 Event Publisher가 발생시키는 모든 Event에 대해 수신 한다면
비효율 적이다. 따라서 Event Subscriber는 Event Context중에 내가 수신 할 이벤트의 종류를 
지정할 수 있는데, 이를 하는 것이 Subscription Context이다.

```cpp
struct UdevSubscriptionContext : public SubscriptionContext {
  /// The hardware event action, add/remove/change.
  udev_event_action action;

  /// Restrict to a specific subsystem.
  std::string subsystem;
  /// Restrict to a specific devnode.
  std::string devnode;
  /// Restrict to a specific devtype.
  std::string devtype;
  /// Restrict to a specific driver name.
  std::string driver;
};
```

---

# Event Publisher
Event Context를 통해 어떤 Event를 주고 받을지 정의했고, Subscription Context를 통해
Event Publisher가 어떤 Event Subscriber에게 Event를 전달 해야하는지 정의했다.  

그럼 이 두가지 (Event Context, Subscription Context)를 가지고 Event Publisher를 정의해 보자.


```cpp
class UdevEventPublisher
    : public EventPublisher<UdevSubscriptionContext, UdevEventContext>

```

Event Publisher를 정의하기 위해서 몇가지 멤버 함수를 구현해야 한다.  

먼저 Event Publisher의 생성자/소멸자 역할을 하는 setUp()/tearDown()를 구현해야 한다. Event Publisher의 setUp() 함수가 호출 되고 나면 Event Framework는 run-loop를 통해 해당 Event Publisher의 run() 함수를 주기적으로 호출해 준다. (Event Publisher를 그만 동작 시켜야 할때 tearDown()이 호출 된다.)

```cpp
/**
 * @brief A Linux `udev` EventPublisher.
 *
 */
class UdevEventPublisher
    : public EventPublisher<UdevSubscriptionContext, UdevEventContext> {
  DECLARE_PUBLISHER("udev");

 public:
  /// set up the publisher
  Status setUp();

  /// start a run-loop which listens for udev events and publishes them
  /// to subscribers
  Status run();

  /// clean up resources
  void tearDown();

  // ...
};
```

---

# Event Subscriber
Event Subscriber는 Event Publisher에 비해 심플하다. 기본적으로 Event Subscriber는 하나의 Event Publisher를 지정하도록 설계 되었기에 Event Publisher는 유연한 구조이고, Event Subscriber는 심플한 구조이다.  

Event Subscriber는 Event 받으려 하는 Event Publisher를 지정해야한다. 아래는 Hardware Event를 받기 위해 Udev Event Publisher를 지정한 Event Subscriber이다.

```cpp
class HardwareEventSubscriber : public EventSubscriber<UdevEventPublisher> 
```

Event Publisher를 지정하고 나선, #1 Event Subscriber를 초기화할 init()함수와 #2 Event를 수신했을 때 실행 시킬 Callback 함수를 정의하기만 하면 된다.

```cpp
/**
 * @brief Track udev events in Linux
 */
class HardwareEventSubscriber : public EventSubscriber<UdevEventPublisher> {
 public:
  Status init();

  Status Callback(const UdevEventContextRef& ec, const void* user_data);
};
```

HardwareEventSubscriber의 초기화 과정은 간단하다. SubscriptionContext를 생성하여 어떤 종류의 Event를 받을 것인지(action)을 명시하고, 정의한 Callback 함수(멤버 함수)를 등록하면 된다. 다음은 모든 Event에대해 Callback 함수를 등록하는 초기화 과정이다.

```cpp
Status HardwareEventSubscriber::init() {
  auto subscription = createSubscriptionContext();
  subscription->action = UDEV_EVENT_ACTION_ALL;

  subscribe(&HardwareEventSubscriber::Callback, subscription, nullptr);
  return Status(0, "OK");
}
```

# Event Callback
EventCallback 함수는 Event를 수신하려는 Event Subscriber가 정의하는 것으로, Event Publisher에 의해 호출 된다. Event Subscriber의 init() 함수에서 호출 될 EventCallback의 때를 정의할 수 있는데, 위의 HardwareEventSubscriber::init()에선 "UDEV_EVENT_ACTION_ALL"를 사용함으로써 모든 Event를 수신하였다.  

Osquery에선 Callback의 주 목적은 SQL table의 행을 생성 하는 것으로, 아래와 같이 Callback 함수를 정의한다.

```cpp
Status HardwareEventSubscriber::Callback(const UdevEventContextRef& ec,
                                         const void* user_data) {
  Row r;
  // ...
  // redacted for brevity

  struct udev_device *device = ec->device;
  r["action"] = ec->action_string;
  r["path"] = ec->devnode;
  r["type"] = ec->devtype;
  r["driver"] = ec->driver;

  // UDEV properties.
  r["model"] = UdevEventPublisher::getValue(device, "ID_MODEL_FROM_DATABASE");
  r["model_id"] = INTEGER(UdevEventPublisher::getValue(device, "ID_MODEL_ID"));
  r["vendor"] = UdevEventPublisher::getValue(device, "ID_VENDOR_FROM_DATABASE");
  r["vendor_id"] =
      INTEGER(UdevEventPublisher::getValue(device, "ID_VENDOR_ID"));
  r["serial"] =
      INTEGER(UdevEventPublisher::getValue(device, "ID_SERIAL_SHORT"));
  r["revision"] = INTEGER(UdevEventPublisher::getValue(device, "ID_REVISION"));

  r["time"] = INTEGER(ec->time);

  // add the row to our backing datastore, RocksDB
  // more on this soon!
  add(r, ec->time);
  
  return Status(0, "OK");
}
```

또한 Osquery의 Callback 정의를 보면 RocksDB에 행을 저장하는 add() 함수를 호출하는 것을 볼 수 있는데, 이는 SQL query를 통해 질의하였을 때 Event가 발생했던 결과를 받기 위한 것이다.  

좀 더 자세히 정리하면, 실제 Event가 발생 했을 때 EventSubscriber가 정의한 EventCallback이 불리고, Osquery는 이 EventCallback에서 생성된 행(Row)를 RocksDB에 시간과 함께 저장한다. 시간과 함께 저장된 이 정보들은 SQL Query 사이에 일어 났던 모든 결과를 리턴 할 수 있게 된다.

# Registration
Event Subscriber를 정의했으면 REGISTER 매크로를 이용하여 등록하여야한다.
첫 번째 인자는 Event Subscriber의 Type이고, 두번 째는 Event Subscriber를 명시하는 고정 스트링, 세번 째는 Event와 대응되는 Table Schema 이름이다. *정확히 두번 째 파라미터는 Registry에 등록할 Plugin의 이름이다.*

```cpp
REGISTER(HardwareEventSubscriber, "event_subscriber", "hardware_events");
```

세번 째 파라미터인 테이블 스키마는 DSL(Domain Specific Language)로 아래와 같이 정의 될 수 있다.
```python
table_name("hardware_events")
description("Hardware (PCI/USB/HID) events from UDEV or IOKit.")
schema([
    Column("action", TEXT, "Remove, insert, change properties, etc"),
    Column("path", TEXT, "Local device path assigned (optional)"),
    Column("type", TEXT, "Type of hardware and hardware event"),
    Column("driver", TEXT, "Driver claiming the device"),
    Column("model", TEXT, "Hardware device model"),
    Column("model_id", INTEGER),
    Column("vendor", TEXT, "Hardware device vendor"),
    Column("vendor_id", INTEGER),
    Column("serial", TEXT, "Device serial (optional)"),
    Column("revision", INTEGER, "Device revision (optional)"),
    Column("time", INTEGER, "Time of hardware event"),
])
attributes(event_subscriber=True)
implementation("events/hardware_events@hardware_events::genTable")
```
---

**Event Factory**는 원문에서 설명하지는 않았지만, 구조적으로 중요한 부분이다. Subscription을 생성하고, 등록하고 하는 Event Framework의 핵심 부분이다. 이 부분은 추가적으로 코드를 살펴보는 것이 좋다.
