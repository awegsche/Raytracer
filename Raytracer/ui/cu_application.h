#ifndef _CU_APPLICATION_H
#define _CU_APPLICATION_H

#include "utils/cuda_helpers.h"
#include "utils/optix_helpers.h"

class CUApplication {
    
    // cuda
    CUcontext m_cuda_context;
    CUstream m_cuda_stream;


    public:
    static std::unique_ptr<CUApplication> make_application();

};

#endif
