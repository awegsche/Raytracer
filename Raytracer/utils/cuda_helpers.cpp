#include "cuda_helpers.h"

bool cuda_helpers::check(CUresult result, char const *message) {
    if (result != CUDA_SUCCESS) {
        const char *cu_err_name = nullptr;
        // this can also fail
        auto _res = cuGetErrorName(result, &cu_err_name);
        if (_res != CUDA_SUCCESS) {
            spdlog::error("checking CUDA result failed with error code {}. This might happen because CUDA failed to initialise prior to this call",
                          static_cast<int>(_res));
            return false;
        }
        if (message)
            spdlog::error("{}", message);
        spdlog::error("cu_check failed: {}", cu_err_name);
        return false;
    }
    return true;
}
