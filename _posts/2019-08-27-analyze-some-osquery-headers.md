---
layout: post
title: Analyze some osquery headers
tags: [framework]
---

[Osquery 4.0.0]([https://github.com/osquery/osquery/releases/tag/4.0.0](https://github.com/osquery/osquery/releases/tag/4.0.0)) is released on Jun 29, 2019

Because of its complexity, there is many cycle dependency between the headers. (registry, event, logger, registry ... logger).  

Let's look at some of the detachable headers. The other headers are tangled too much.
Below are the basic utilities.

*osquery/ suffix is omitted for conciseness.*  

|HEADER                |BRIEF                          |
|-------------------|---------------------------------|
|utils/error/error.h|1. Represent error code. <br> 2. Convert enum class to error class. <br> 3. Provide helper function for #2. |
|utils/conversions/to.h|1. Conversion either scoped or unscoped enum value to std::string of human readable representation. <br> 2. Use "boost::core::demangle" to get symbol name(Enum Class). |
|utils/expected/expected.h| 1. Utility class that should be used in function that **return either error or value.** <br> 2. **Expected enforce developer to test for success and check error if any.**|
|utils/status/status.h| A utility class which is used to express the state of operations.
|utils/json/json.h| 1. Provide a small wrapper around constructing JSON objects. <br> 2. This is made of rapidjson internally.|
|core/sql/row.h| 1. A single row, type from a database query. <br> 2. Provide conversion between a Row and a JSON document.|
|core/sql/query_data.h| 1. **The result set returned from a osquery SQL query.** <br> 2. It's just a vector of Rows. <br> 3. Provide conversion between a QueryData and a JSON document.|
|core/sql/table_row.h| 1. Interface for accessing a table row. <br> 2. Include "sqlite.h"|
|core/sql/table_rows.h| 1. Provide conversion between a TableRows and a JSON document. <br> 2. TableRows is a vector of TableRowHolder.|
|core/plugins/plugin.h| 1. Plugin is a item of registry. <br> 2. A request is a thrift-serializable object. <br> 3. A response is optional but the API for using a plugin's call is defined by the registry. <br> 4. **Plugins act by being called, using a request, returning a response.** <br> 5.  Convert TableRows object to a PluginResponse.|


---

# Usage details
### utils/error/error.h
```cpp  
enum class TestError {                                                                     
   SomeError = 1,                                                                           
   AnotherError = 2,                                                                        
   MusicError,                                                                              
 };   
//
const auto msg = std::string{"Error message."};
auto err = osquery::createError(TestError::AnotherError) << msg;
// err.getErrorCode() == TestError::SomeError
// err.getMessage() contains "TestError[2]"(AnotherError) and "Error message."
```

### utils/conversions/to.h
```cpp
enum class En {
  First = 1,
};
// to<std::string>(En::First) == "En::First[1]"
 ```

### utils/expected/expected.h
```cpp
enum class TestError { SomeError = 1, AnotherError = 2 };
// return Expected object
Expected<std::string, TestError> function() {
  if (${expression})
    return "ok";
  else
    return createError(TestError::SomeError) << "one more error message";
// usage
auto result = function();
if (result) {
  use *result
} else {
  switch (result.getErrorCode()) {
    case TestError::SomeError:
      // do something with it
      break;
    case TestError::AnotherError:
      // do something with it
     break;
    }
}
```
### utils/status/status.h
```cpp
osquery::Status foobar() {
  auto na = doSomeWork();
  if (na->itWorked()) {
    // return Status::success();
    return osquery::Status(0, "OK");
  } else {
    // Status::failure(1, "Some proper error message.");
    return osquery::Status(1, "Some proper error message.");
  }
}
// usageprovides a small wrapper around constructing JSON objects.
auto s = foobar();
if (!s.ok())
  // do something
else
  // s.getMessage() == s.toString() == "Error message"
  // s.getCode() == 1
```
### utils/json/json.h
```cpp
auto doc = JSON::newArray();
{
  auto obj = doc.getObject();
  size_t value = 10;
  doc.add("key", value, obj);
  int value2 = -10;
  doc.add("key2", value2, obj);
  int64_t value3 = (uint64_t(1)) << 48;
  doc.add("key3", value3, obj);
  double value4 = 3.14159265359;
  doc.add("key4", value4, obj);
  doc.push(obj);
}
size_t value = 11;
doc.push(value);
// doc.toString(result) ==
//      "[{\"key\":10,\"key2\":-10,\"key3\":281474976710656,\"key4\":3."
//      "14159265359},11]";
// JSON::valueToSize(doc.doc()["key"]) == size_t{10}
 ```
