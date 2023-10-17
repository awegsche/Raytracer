
#include "cu_application.h"

std::unique_ptr<CUApplication> CUApplication::make_application() {

    auto app = std::make_unique<CUApplication>();
    // init CUDA and OptiX
    // note: here we call `cuda_helpers::check` which is slightly more overhead because we are
    // in initialisation which is only called once at the start and is certainly not hot code
    spdlog::info("setup CUDA and OptiX");
    if (!cuda_helpers::check(cuInit(0), "failed initialising CUDA"))
        return {};

    CUdevice device = 0;
    if (!cuda_helpers::check(cuCtxCreate(&app->m_cuda_context, CU_CTX_SCHED_SPIN, device),
                             "creating CUDA context failed"))
        return {};

    if (!cuda_helpers::check(cuStreamCreate(&app->m_cuda_stream, CU_STREAM_DEFAULT),
                             "creating CUDA stream failed"))
        return {};

#ifdef _WIN32
    void *handle = optix_helpers::optix_load_windows_dll();
    if (!handle) {
        spdlog::error("OptiX DLL not found");
        return;
    }
    void *symbol =
        reinterpret_cast<void *>(GetProcAddress((HMODULE)handle, "optixQueryFunctionTable"));
    if (!symbol) {
        spdlog::error("Optix Query Function Table Symbol not found");
        return;
    }
#else
    spdlog::error("optix DLL loading on non-WIN32 platforms not yet supported");
    return {};

#endif

    return app;
}
