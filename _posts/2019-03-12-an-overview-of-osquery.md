---
layout: post
title: An overview of osquery
tags: [framework]
---

# [osquery](https://osquery.io/)
> facebook/osquery
> **Performant endpoint visibility**

- SQL-based operating system instrumentation, monitoring, and analytics framework
- It is a simple and easy-to-use operating system explorer
- These tools reveal an operating system as a high-performance relational database
- OS concepts are represented in tabular form
- Users to employ SQL commands to carry out system monitoring and analytics
- Osquery use a simple plugin and extensions API to implement SQL tables
- A collection of tables in existence ready for use, and more are being written.

---

# [High Level Features](https://osquery.readthedocs.io/en/stable/)

## osqueryd
- The high-performance and low-footprint distributed **host monitoring daemon**
- Allows you to schedule queries to be executed across your entire infrastructure.
- The daemon takes care of aggregating the query results over time and generates logs which indicate state changes in your infrastructure.

---

## osqueryi
- The interactive query console
- Gives you a SQL interface to try out new queries and explore your operating system.
- With dozens of useful tables built-in, osqueryi is an invaluable tool when performing incident response, diagnosing a systems operations problem, troubleshooting a performance issue, etc.

---

## [SDK](https://osquery.readthedocs.io/en/stable/development/osquery-sdk/)

- "public API" or SDK = the set of osquery headers + osquery core. 
	- The public headers can be found in [osquery/include/osquery/](https://github.com/facebook/osquery/tree/experimental/osquery/include/osquery)
- This SDK can be used to build osquery outside of our CMake build system with a minimum set of dependencies.
- The public API and SDK headers are documented via doxygen.
- Extensions are separate processes that communicate over a Thrift IPC channel to osquery core in order to **register one or more plugins or virtual tables**. 

---

## [Extensions](https://osquery.readthedocs.io/en/stable/development/osquery-sdk/)
- An extension allows you to develop independently: it may be compiled and linked using an external build system, **against proprietary code**.
- Extensions use osquery's Thrift API to communicate between osqueryi or osqueryd and the extension process.
- Extensions are commonly written in C++, but can also be developed in Python, in Go, or in really any language that supports Thrift.
- Only the osquery SDK provides the simple *startExtension* symbol that manages the life of your process, including the Thrift service threads and a watchdog.

---

## [Extension example](https://github.com/facebook/osquery/blob/master/osquery/examples/example_extension.cpp)
C++ extensions should link: boost, thrift, glog, gflags, and optionally rocksdb for eventing. 

```cpp
// Note 1: Include the sdk.h helper.
#include <osquery/sdk.h>

using namespace osquery;

// Note 2: Define at least one plugin or table.
class ExampleTablePlugin : public TablePlugin {
 private:
  TableColumns columns() const override {
    return {
      std::make_tuple("example_text", TEXT_TYPE, ColumnOptions::DEFAULT),
      std::make_tuple("example_integer", INTEGER_TYPE, ColumnOptions::DEFAULT),
    };
  }

  QueryData generate(QueryContext& request) override {
    QueryData results;
    Row r;

    r["example_text"] = "example";
    r["example_integer"] = INTEGER(1);
    results.push_back(r);
    return results;
  }
};

// Note 3: Use REGISTER_EXTERNAL to define your plugin or table.
REGISTER_EXTERNAL(ExampleTablePlugin, "table", "example");

int main(int argc, char* argv[]) {
  // Note 4: Start logging, threads, etc.
  osquery::Initializer runner(argc, argv, ToolType::EXTENSION);

  // Note 5: Connect to osqueryi or osqueryd.
  auto status = startExtension("example", "0.0.1");
  if (!status.ok()) {
    LOG(ERROR) << status.getMessage();
    runner.requestShutdown(status.getCode());
  }

  // Finally, shutdown.
  runner.waitForShutdown();
  return 0;
}
```
---

# [Under the Hood](https://blog.kolide.com/osquery-under-the-hood-c1a8df46bb7a)
> Osquery: Under the Hood, Zach Wasserman  

![Data flows within osquery](https://cdn-images-1.medium.com/max/1000/1*3qY3S09oUJC4ICnDXI7_FQ.png)

---

## Query Engine (SQLite Engine)
- Enable ordinary users to perform sophisticated analysis with a familiar SQL dialect.
- Osquery gets all of the query parsing, optimization and execution functionality from SQLite
- While osquery uses the SQLite query engine, **it does not actually use SQLite for data storage**. 
- Most data is generated on-the-fly at query execution time through a concept we call “Virtual Tables”.
- Osquery does need to store some data on the host, and for this it uses an embedded RocksDB database.

---

## Virtual Tables
- Gather all the data that we serve up for analytics.
- Most virtual tables generate their data at query time 
	- by parsing a file or calling a system API.
- At query time, the SQLite query engine will request the virtual table to generate data.
- The osquery developers have put a great deal of effort into making virtual table creation easy for community contributors.
- Create a simple spec file (using a custom DSL built in Python) and implement in C++ (or C/Objective-C as necessary).

---

## Virtual Tables DSL example
```python
table_name("etc_hosts", aliases=["hosts"])
description("Line-parsed /etc/hosts.")
schema([
    Column("address", TEXT, "IP address mapping"),
    Column("hostnames", TEXT, "Raw hosts mapping"),
])
attributes(cacheable=True)
implementation("etc_hosts@genEtcHosts")
```

---

## Event System
### Problem
- Not all of the data exposed by osquery fits well into the model of generating on-the-fly when the table is queried. 
- Take for example the common problem of file integrity monitoring (FIM).
- We need continuous visibility.

### Solution
- Osquery has an event publisher/subscriber system that can generate, filter and store data to be exposed when the appropriate virtual table is queried.
- Event publishers run in their own thread and can use whatever APIs they need to create a stream of events to publish.
- For FIM on Linux, the publisher generates events through inotify.
- It then publishes the events to one or more subscribers, which can filter and store the data (in RocksDB) as they see fit.

---

## Scheduler
- Consider deploying osquery on a massive scale, like the over 1 million production hosts in Facebook’s fleet.

- It could be a huge problem if each of these hosts ran the same query at the exact same time and caused a simultaneous spike in resource usage.

- So the scheduler provides a randomized “splay”, allowing queries to run on an approximate rather than exact interval.

- This simple design prevents resource spikes across the fleet.

---

## Diff Engine
- In order to optimize for large scale and bubble up the most relevant data
- Osquery provides facilities for **outputting differential query results**.
- Each time a query runs, the results of that query are stored in the internal RocksDB store.
- When logs are output, the results of the current query are compared with the results of the existing query, and a log of the added/removed rows can be provided.

---

## RocksDB
- Though much of the data that osquery presents is dynamically generated by the system state at query time, there are a myriad of contexts in which the agent stores data.
- To achieve this, osquery utilizes another Facebook open source project, RocksDB. This is a highly write-optimized, embedded key-value database that is compiled into the osquery binary.
- **RocksDB is used for storing events, results of scheduled queries for differential logging, configuration state, buffered logs, and more.**

---

## Configuration Plugins
- In the multitude of environments osquery runs in, it may need to pull configuration from a variety of sources.
- Configuration Plugins provides the necessary configuration to the daemon.
- Common sources for configuration are the filesystem or via a TLS server.
- A config plugin will return osquery configuration to osquery daemon(in JSON form) when called by the extension manager.

---

## Logger Plugins
- Like configuration, logging must also be compatible with a variety of architectures.
- Commonly used logging plugins are filesystem, TLS, or AWS Kinesis/Firehose.
- Whenever osquery generates logs (status or result logs), these will be written to the logger plugin

---

## Distributed Plugins
- Distributed plugins enable remote live querying of data from osquery.
- The only existing implementation of the distributed plugin is TLS.
- These APIs allow the osquery process to retrieve queries to execute from some out-of-process controller, and then write results to that controller.

---

## Static Compilation
- Osquery builds into a single executable, with most of its (many) dependencies statically linked.
- This executable includes osqueryi and osqueryd, along with all of the built-in plugins that enable a full osquery deployment.

![Shared library dependencies for osqueryd on Linux](https://cdn-images-1.medium.com/max/1000/1*m6nxBbXueds4Nn2iRcCDfw.png)

---

## Watchdog
- Osqueryd has a “Watchdog” that spawns a “worker” osquery process.
- This Watchdog process keeps tabs on the queries that are executing, and kills the worker process if it exceeds the predefined performance thresholds.
- Additionally, it can blacklist queries that are consistently causing performance problems, allowing users to return and debug these queries at a later time while addressing the immediate problem.

---

# References
- https://osquery.io/
- https://osquery.readthedocs.io/en/stable/
- https://osquery.readthedocs.io/en/stable/development/osquery-sdk/
- https://blog.kolide.com/osquery-under-the-hood-c1a8df46bb7a

---


