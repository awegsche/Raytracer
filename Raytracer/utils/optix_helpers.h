#pragma once



namespace optix_helpers {
#ifdef _WIN32

/// <summary>
/// Loads the OptiX dll for windows.
/// 
///     I copied this verbatim from the OptiX Samples, this code is 'write-only'
/// </summary>
/// <returns></returns>
void *optix_load_windows_dll();
#endif
} // namespace optix_helpers
