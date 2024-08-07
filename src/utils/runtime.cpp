//
// Created by user on 07.08.2024.
//

#include "runtime.h"

bool utils::runtime::isAlreadyRunning() {
    mux = CreateMutex(nullptr, TRUE, "boykisser");

    auto const ret = GetLastError() == ERROR_ALREADY_EXISTS;

    if(mux)
        CloseHandle(mux);

    return ret;
}

bool utils::runtime::createRunLock() {
    mux = CreateMutex(nullptr, TRUE, "boykisser");

    return GetLastError() != ERROR_ALREADY_EXISTS;
}

void utils::runtime::destroyRunLock() {
    if(mux)
        CloseHandle(mux);
}