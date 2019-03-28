---
layout: post
title: The term of sqlite virtual table
tags: [sqlite]
---

# Sqlite Virtual Table

> [The Virtual Table Mechanism Of SQLite](https://sqlite.org/vtab.html)  
> The virtual table mechanism allows an application to publish interfaces that are accessible from SQL statements as if they were tables.

- A virtual table might represent an in-memory data structures.
- Or it might represent a view of data on disk that is not in the SQLite format.
- Or the application might compute the content of the virtual table on demand.
- Individual virtual table implementations might impose additional constraints. (read-only tables, allow INSERT or DELETE but not UPDATE)
- Postulated use: Access the filesystem of the host computer as if it were a database table


## [OsQuery with Virtual Table](https://sqlite.org/vtablist.html)
Hundreds of virtual tables that publish various aspects of the host computer, such as the process table, user lists, active network connections, and so forth.

## [json_tree with Virtual Table](https://sqlite.org/json1.html#jtree)
A table-valued function for decomposing a JSON string.


## Creating New Virtual Table Implementations

Follow these steps to create your own virtual table:

1. Write all necessary methods.
2. Create an instance of the sqlite3_module structure containing pointers to all the methods from step 1.
3. Register your sqlite3_module structure using one of the sqlite3_create_module() or sqlite3_create_module_v2() interfaces.
4. Run a CREATE VIRTUAL TABLE command that specifies the new module in the USING clause.

### Structure used by the virtual table implementation
```cpp
typedef struct sqlite3_vtab sqlite3_vtab;
typedef struct sqlite3_index_info sqlite3_index_info;
typedef struct sqlite3_vtab_cursor sqlite3_vtab_cursor;
typedef struct sqlite3_module sqlite3_module;
```

## sqlite3_module 
- Defines a module object used to implement a virtual table.
- Think of a module as a class from which one can construct multiple virtual tables having similar properties.
	- For example, one might have a module that provides read-only access to comma-separated-value (CSV) files on disk. That one module can then be used to create several virtual tables where each virtual table refers to a different CSV file.

- **The module structure contains methods that are invoked by SQLite** to perform various actions on the virtual table.
	- such as creating new instances of a virtual table or destroying old ones, reading and writing data, searching for and deleting, updating, or inserting rows. The module structure is explained in more detail below.

##  sqlite3_vtab
- Represents virtual table instance.
```cpp
struct sqlite3_vtab {
  const sqlite3_module *pModule;
  int nRef; // should not be altered 
  char *zErrMsg;
};
```
- The virtual table implementation may pass error message text to the core by putting an error message string in zErrMsg.
- Space to hold this error message string must be obtained from an SQLite memory allocation function such as sqlite3_mprintf() or sqlite3_malloc(). 

## sqlite3_vtab_cursor
- Represents a pointer to a specific row of a virtual table.
