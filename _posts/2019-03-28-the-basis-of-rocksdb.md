---
layout: post
title: The basis of RocksDB
tags: [rocksdb]
---

# [RocksDB](https://rocksdb.org/)
> Rocksdb is an embeddable persistent key-value store for fast storage.

- High Performance (Write performance)
	- log-structured storage engines (Leveldb, Rocksdb, and Cassandra)
	- page-oriented storage engines (PostgreSQL, MySQL, and sqlite)
	- [Btree vs LSM benchmark](https://github.com/wiredtiger/wiredtiger/wiki/Btree-vs-LSM)
- Optimized for Fast Storage
	- high read/write rates offered by flash or RAM
- Adaptable
	- from database storage engines such as MyRocks to application data caching to embedded workloads
- Basic and Advanced Database Operations
	- basic operations(opening, closing, reading and writing)
	- advanced operations(merging and compaction filters)

# [Basic Operations](https://github.com/facebook/rocksdb/wiki/Basic-Operations) and [Examples](https://github.com/facebook/rocksdb/tree/master/examples)
## Opening & Closing
- DB is thread-safe
- Close() is not supported. (Just use 'delete'.)
### Definition
```cpp
// A DB is safe for concurrent access from multiple threads.
class DB {
 public:
  // Open the database with the specified "name".
  // Stores a pointer to a heap-allocated database in *dbptr and returns
  // OK on success.
  // Stores nullptr in *dbptr and returns a non-OK status on error.
  // Caller should delete *dbptr when it is no longer needed.
  static Status Open(const Options& options, const std::string& name,
                     DB** dbptr);
// virtual Status Close() { return Status::NotSupported(); }
```
### Example - Opening & Closing
```cpp
DB* db;
Options options;
// Create the DB if it's not already present
options.create_if_missing = true;
// Open DB
Status s = DB::Open(options, kDBPath, &db);
assert(s.ok());
// Close DB
delete db;
```

## Writing & Reading
### Definition
```cpp
// Set the database entry for "key" to "value".
// If "key" already exists, it will be overwritten.
// Returns OK on success, and a non-OK status on error.
// Note: consider setting options.sync = true.
virtual Status Put(const WriteOptions& options, const Slice& key,
                   const Slice& value);
                     
// If the database contains an entry for "key" store the
// corresponding value in *value and return OK.
//
// If there is no entry for "key" leave *value unchanged and return
// a status for which Status::IsNotFound() returns true.
//
// May return some other Status on an error.
virtual Status Get(const ReadOptions& options, const Slice& key,
                   std::string* value) {
```
### Example - Writing & Reading
```cpp
// Put key-value
s = db->Put(WriteOptions(), "key1", "value");
assert(s.ok());
std::string value;
//
// Get value
s = db->Get(ReadOptions(), "key1", &value);
assert(s.ok());
assert(value == "value");
//
// Delete value
if (s.ok()) s = db->Delete(rocksdb::WriteOptions(), key1);
assert(s.ok());
```

### Atomically writing
```cpp
// Atomically apply a set of updates
{
  WriteBatch batch;
  batch.Delete("key1");
  batch.Put("key2", value);
  s = db->Write(WriteOptions(), &batch);
}
```

## Slice (Efficient std::string)
- rocskdb::Slice is a simple structure that contains a length and a pointer to an external byte array.
- Returning a Slice is a cheaper alternative to returning a std::string since we do not need to copy potentially large keys and values.
- In addition, rocksdb methods do not return null-terminated C-style strings since rocksdb keys and values are allowed to contain '\0' bytes.

### Convert between std::string and rocksdb::Slice
```cpp
// Convert slice to string
rocksdb::Slice s1 = "hello";
//
std::string str("world");
rocksdb::Slice s2 = str;
// Convert string to slice
std::string str = s1.ToString();
assert(str == std::string("hello"));
// Careful
// it is up to the caller to ensure that the external byte array into which the Slice points remains live while the Slice is in use. For example, the following is buggy
rocksdb::Slice slice;
if (...) {
  std::string str = ...;
  slice = str;
}
Use(slice);
```

## PinnableSlice(The data pinned in memory)
- Similarly to Slice, PinnableSlice refers to some in-memory data so it does not incur the memcpy cost.
- The pinned data are released when PinnableSlice object is destructed or when ::Reset is invoked explicitly on it.
- Each Get results into at least a memcpy from the source to the value string.
- If the source is in the block cache, you can avoid the extra copy by using a PinnableSlice.
```cpp
std::string string_val;
// If it cannot pin the value, it copies the value to its internal buffer.
// The intenral buffer could be set during construction.
// string_val == internal buffer
PinnableSlice pinnable_val(&string_val);
db->Get(ReadOptions(), db->DefaultColumnFamily(), "key2", &pinnable_val);
assert(pinnable_val == "value");
//
// If the value is not pinned, the internal buffer must have the value.
assert(pinnable_val.IsPinned() || string_val == "value");
```

## Column Families
- Column Families provide a way to logically partition the database. 
- Users can provide atomic writes of multiple keys across multiple column families and read a consistent view from them.

### Definition
```cpp
struct ColumnFamilyOptions;
struct ColumnFamilyDescriptor; // Option + name
class ColumnFamilyHandle;
// Create a column_family and return the handle of column family
// through the argument handle.
virtual Status CreateColumnFamily(const ColumnFamilyOptions& options,
                                    const std::string& column_family_name,
                                    ColumnFamilyHandle** handle);
//
// Bulk create column families.
// Return the handles of the column families through the argument handles.
// In case of error, the request may succeed partially, and handles will
// contain column family handles that it managed to create, and have size
// equal to the number of created column families.
virtual Status CreateColumnFamilies(
    const std::vector<ColumnFamilyDescriptor>& column_families,
    std::vector<ColumnFamilyHandle*>* handles);
//
// Bulk create column families with the same column family options.
// Return the handles of the column families through the argument handles.
// In case of error, the request may succeed partially, and handles will
// contain column family handles that it managed to create, and have size
// equal to the number of created column families.
virtual Status CreateColumnFamilies(
    const ColumnFamilyOptions& options,
    const std::vector<std::string>& column_family_names,
    std::vector<ColumnFamilyHandle*>* handles);
```

### Example - Create ColumnFamilyhandle
```cpp
// open DB
Options options;
options.create_if_missing = true;
DB* db;
Status s = DB::Open(options, kDBPath, &db);
assert(s.ok());

// create column family
ColumnFamilyHandle* cf;
s = db->CreateColumnFamily(ColumnFamilyOptions(), "new_cf", &cf);
assert(s.ok());

// close DB
delete cf;
delete db;
```
### Example - Writing & Reading with ColumnFamily
```cpp
// open DB with two column families
std::vector<ColumnFamilyDescriptor> column_families;
//
// have to open default column family
column_families.push_back(ColumnFamilyDescriptor(
    kDefaultColumnFamilyName, ColumnFamilyOptions()));
//
// open the new one, too
column_families.push_back(ColumnFamilyDescriptor(
    "new_cf", ColumnFamilyOptions()));
std::vector<ColumnFamilyHandle*> handles;
s = DB::Open(DBOptions(), kDBPath, column_families, &handles, &db);
assert(s.ok());
//
// put and get from non-default column family
s = db->Put(WriteOptions(), handles[1], Slice("key"), Slice("value"));
assert(s.ok());
std::string value;
s = db->Get(ReadOptions(), handles[1], Slice("key"), &value);
assert(s.ok());
//
// atomic write
WriteBatch batch;
batch.Put(handles[0], Slice("key2"), Slice("value2"));
batch.Put(handles[1], Slice("key3"), Slice("value3"));
batch.Delete(handles[0], Slice("key"));
s = db->Write(WriteOptions(), &batch);
assert(s.ok());
//
// drop column family
s = db->DropColumnFamily(handles[1]);
assert(s.ok());
//
// close db
for (auto handle : handles) {
  delete handle;
}
delete db;
```

# References
- https://github.com/facebook/rocksdb/blob/master/include/rocksdb/db.h
- https://github.com/facebook/rocksdb/tree/master/examples
- https://rocksdb.org/blog/2017/08/24/pinnableslice.html
- https://github.com/facebook/rocksdb/wiki/Basic-Operations


