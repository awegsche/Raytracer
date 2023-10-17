#pragma once

#include <cuda.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>

namespace cuda_helpers {
/// function for checkint of cuda related commands

bool check(CUresult result, char const *message = nullptr);
} // namespace cuda_helpers

/// macro for zero overhead launch commands
#define CU_CHECK(fun)                                                                               \
    {                                                                                               \
        CUresult __res = fun;                                                                       \
        if (__res != CUDA_SUCCESS) {                                                                \
            const char *__cu_err_name;                                                              \
            cuGetErrorName(__res, &__cu_err_name);                                                  \
            spdlog::error("{}", std::string{__cu_err_name});                                                     \
        }                                                                                           \
    }
