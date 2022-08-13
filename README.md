# nmStacktrace
nmStacktrace is a debugging utility that keeps track of call stack


## Usage
To include implementation:
```cpp
    #define NM_STACKTRACE_IMPL
```
before including `nmStacktrace.hpp`<br>
You should define this in **one** C or C++ file.

Files should look like this.
```cpp
    // onefile.cpp
    #define NM_STACKTRACE_IMPL
    #include "nmStackTrace.hpp"
    
    /* ------------------------- */
    
    // otherfiles.cpp
    #include "nmStackTrace.hpp"
```

### Threading
nmStacktrace doesnt use threads by default. You can
```cpp
    #define NMST_USE_THREADS
```
defining `NMST_USE_THREADS` will include `<pthread.h>`

it is okay to define it even if program doesn't have multithreading.




### Documentation
- nmStacktrace basically holds records of all function entries. Adding entry to a function is simple as
```cpp
    void function()
    {
        NM_ENTRY()
        
        ...
    }
```

- To dump stacktrace
```cpp
    void nmStacktrace::DumpStacktrace();  // will print all entries on all used threads,
```
```cpp
    // threadit_t being thread id type used in threading implementation (for pthread, its pthread_t)
    void nmStacktrace::DumpThread(threadid_t id); // will only print stacktrace of given id
```

- Accessing stacktrace
```cpp
    nmStacktrace::s_StackTrace; // unordered_map<threadid_t, CallStack>
```
When threads are not defined, stacktrace will have one value with key 0