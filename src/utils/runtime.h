//
// Created by user on 07.08.2024.
//

#ifndef RUNTIME_H
#define RUNTIME_H

#include <windows.h>

namespace utils {
    class runtime {
    public:
        static bool isAlreadyRunning();
        static bool createRunLock();
        static void destroyRunLock();
    private:
        static inline HANDLE mux = nullptr;
    };
}


#endif //RUNTIME_H
