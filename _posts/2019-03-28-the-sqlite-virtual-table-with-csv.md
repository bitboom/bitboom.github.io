---
layout: post
title: The sqlite virtual table with csv vtab
tags: [sqlite]
---

# Sqlite Virtual Table
A virtual table is an object that presents an SQL table interface but which is not stored in the database file, at least not directly.
> [The Virtual Table Mechanism Of SQLite](https://sqlite.org/vtab.html)  
> The virtual table mechanism allows an application to publish interfaces that are accessible from SQL statements as if they were tables.  
>  
> [Using SQLite by Jay A. Kreibich](https://www.oreilly.com/library/view/using-sqlite/9781449394592/ch10.html)  
> This chapter looks at how to use and write virtual tables. A virtual table is a custom extension to SQLite that allows a developer to define the structure and contents of a table through code. To the database engine, a virtual table looks like any other table—a virtual table can be queried, updated, and manipulated using the same SQL commands that are used on other tables. The main difference is where the table data comes from. When processing a normal table, the SQLite library might access a database file to retrieve row and column values. In the case of a virtual table, the SQLite library calls in to your code to perform these functions and return data values. Your functions, in turn, can gather or calculate the requested data from whatever sources you want.  
> Developing a virtual table implementation, which is known as an SQLite module, is a fairly advanced feature. This chapter should give you a good idea of what virtual tables are capable of doing and the basics of how to write your own module. We’ll be walking through the code for two different modules. The first is a fairly simple one that exposes some internal SQLite data as a table. The second example will allow read-only access to standard Apache httpd server logs.

- A virtual table might represent an in-memory data structures.
- Or it might represent a view of data on disk that is not in the SQLite format.
- Or the application might compute the content of the virtual table on demand.
- Individual virtual table implementations might impose additional constraints. (read-only tables, allow INSERT or DELETE but not UPDATE)
- Postulated use: Access the filesystem of the host computer as if it were a database table

---

# [List Of Virtual Tables](https://sqlite.org/vtablist.html)
## osquery
Hundreds of virtual tables that publish various aspects of the host computer, such as the process table, user lists, active network connections, and so forth.

## json_tree
A table-valued function for decomposing a JSON string.

## csv
A virtual table that represents a comma-separated-value or CSV file (RFC 4180) as a read-only table so that it can be used as part of a larger query.

---

# Core Structures
##  sqlite3_vtab - Table
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

## sqlite3_module - Table Lifecycle
- Defines a module object used to implement a virtual table.
- Think of a module as a class from which one can construct multiple virtual tables having similar properties.
	- For example, one might have a module that provides read-only access to comma-separated-value (CSV) files on disk. That one module can then be used to create several virtual tables where each virtual table refers to a different CSV file.

- **The module structure contains methods that are invoked by SQLite** to perform various actions on the virtual table.
	- such as creating new instances of a virtual table or destroying old ones, reading and writing data, searching for and deleting, updating, or inserting rows. The module structure is explained in more detail below.

## sqlite3_vtab_cursor - Row
- Represents a pointer to a specific row of a virtual table.


# Creating New Virtual Table Implementations
Follow these steps to create your own virtual table:
1. Write all necessary methods.
2. Create an instance of the sqlite3_module structure containing pointers to all the methods from step 1.
3. Register your sqlite3_module structure using one of the sqlite3_create_module() or sqlite3_create_module_v2() interfaces.
4. Run a CREATE VIRTUAL TABLE command that specifies the new module in the USING clause.

---

# Practice - The CSV Virtual Table
## Compile sqlite with comand-line interface
```sh
# Download source code
$ wget -O sqlite3.zip https://www.sqlite.org/2019/sqlite-amalgamation-3270200.zip
$ unzip sqlite3.zip
# Build sqlite3
$ cd sqlite3
$ gcc shell.c sqlite3.c -o sqlite3 -lpthread -ldl
```

## Compile CSV extension
1. Download [csv.c](https://www.sqlite.org/src/artifact?ci=trunk&filename=ext/misc/csv.c) before.
2. Change "#include <sqlite3ext.h> to #include "sqlite3ext.h" in csv.c
```sh
# Build csv as shared lib
gcc -g -fPIC -shared csv.c -o csv.so
```

## Run sqlite3 with csv extension
1. Download sample [csv file](https://people.sc.fsu.edu/~jburkardt/data/csv/addresses.csv)
```sh
$ ./sqlite3
sqlite> .load ./csv
sqlite> CREATE VIRTUAL TABLE temp.t1 USING csv(filename='./addresses.csv');
sqlite> PRAGMA table_info(t1);
0|c0|TEXT|0||0
1|c1|TEXT|0||0
2|c2|TEXT|0||0
3|c3|TEXT|0||0
4|c4|TEXT|0||0
5|c5|TEXT|0||0
sqlite> SELECT * FROM t1;
John|Doe|120 jefferson st.|Riverside| NJ| 08075
Jack|McGinnis|220 hobo Av.|Phila| PA|09119
John "Da Man"|Repici|120 Jefferson St.|Riverside| NJ|08075
Stephen|Tyler|7452 Terrace "At the Plaza" road|SomeTown|SD| 91234
|Blankman||SomeTown| SD| 00298
Joan "the bone", Anne|Jet|9th, at Terrace plc|Desert City|CO|00123
sqlite> 
```

# References
- https://www.sqlite.org/howtocompile.html
- https://www.sqlite.org/download.html
