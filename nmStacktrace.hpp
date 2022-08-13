#ifndef __NM_STACK_TRACE_HPP__
#define __NM_STACK_TRACE_HPP__
#pragma once

#include <stack>
#include <unordered_map>
#include <iostream>
#ifdef NMST_USE_THREADS
    #include <pthread.h>
    typedef pthread_t threadid_t;

    #define GET_THREAD_ID() pthread_self()
#else
    typedef unsigned int threadid_t;

    #define GET_THREAD_ID() (0)
#endif

#define NM_ENTRY() nmStackTrace::FuncEntryRegister __nm_entry(__FUNCTION__);



namespace nmStackTrace {
    struct FuncEntry {
        std::string _funcName;

        FuncEntry();
        FuncEntry(const std::string &funcName);
        ~FuncEntry();
    };
    typedef std::stack<FuncEntry> CallStack;
    static std::unordered_map<threadid_t, CallStack> s_Stacktrace;

    // object that registers FuncEntry to stacktrace on construction.
    struct FuncEntryRegister {
        FuncEntryRegister() = delete;
        FuncEntryRegister(const std::string& funcName);
        ~FuncEntryRegister();
    };

#ifdef NMST_USE_THREADS
    // Dumps thread call stack with given thread id
    void DumpThread(threadid_t id);
#endif

    // Dumps call stack for all threads
    void DumpStacktrace();


/* Implementation */
#ifdef NM_STACKTRACE_IMPL

#ifdef NMST_USE_THREADS
    void DumpThread(threadid_t id)
    {
        std::cout << "--->Stacktrace Thread ID:" << id << "<---\n";
        // copy stack
        CallStack cStack = s_Stacktrace[id];
        while(!cStack.empty())
        {
            std::cout << "\t<call:" << cStack.top()._funcName << ">\n";
            cStack.pop();
        }
        std::cout << "---<Stacktrace Thread ID:" << id << ">---\n";
    }
#endif
    void DumpStacktrace()
    {
        std::cout << "--> Stacktrace <--\n";
        for( auto& [tid, stack] : s_Stacktrace)
        {
            std::cout << "--->Stacktrace Thread ID:" << tid << "<---\n";
            
            // copy stack
            CallStack cStack = stack;
            while(!cStack.empty())
            {
                std::cout << "\t<call:" << cStack.top()._funcName << ">\n";
                cStack.pop();
            }
            std::cout << "---<Stacktrace Thread ID:" << tid << ">---\n";
        }
        std::cout << "--< Stacktrace >--" << std::endl;
    }


    FuncEntry::FuncEntry() {}
    FuncEntry::FuncEntry(const std::string &funcName) : _funcName(funcName) {}
    FuncEntry::~FuncEntry() {}

#ifdef NMST_USE_THREADS
    // Internal method that locks s_Stacktrace. // dont use it
    void SettraceLock(bool lock)
    {
        static pthread_mutex_t trace_mutex;
        
        // initialize mutex
        static bool first_time = true;
        if (first_time)
        {
            if(pthread_mutex_init(&trace_mutex, NULL) != 0)
            {
                throw std::runtime_error("Failed to initialize mutex");
            }
            first_time = false;
        }

        if(lock)
            pthread_mutex_lock(&trace_mutex);
        else
            pthread_mutex_unlock(&trace_mutex);
    }
#else
    // Does nothing if threads are not defined
    void SettraceLock(bool lock) {}
#endif

    FuncEntryRegister::FuncEntryRegister(const std::string& funcName)
    {
        SettraceLock(true);
        s_Stacktrace[GET_THREAD_ID()].emplace(FuncEntry(funcName));
        SettraceLock(false);
    }
    FuncEntryRegister::~FuncEntryRegister()
    {
        SettraceLock(true);
        if(s_Stacktrace.size() > 0) 
            s_Stacktrace[GET_THREAD_ID()].pop();
        SettraceLock(false);
    }
} // namespace nmStacktrace

#endif // NM_STACKTRACE_IMPL
#endif // __NM_STACK_TRACE_HPP__