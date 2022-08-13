#define NM_STACKTRACE_IMPL
#define NMST_USE_THREADS
#include "nmStacktrace.hpp"


void Test()
{NM_ENTRY()

    nmStackTrace::DumpStacktrace();
}

int main(int argc, char const *argv[])
{NM_ENTRY()

    Test();
    
    
    return 0;
}
