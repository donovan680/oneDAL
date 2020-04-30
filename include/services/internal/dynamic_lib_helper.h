/* file: dynamic_lib_helper.h */
/*******************************************************************************
* Copyright 2020 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/*
//++
// Helper for dynamic libs management
//--
*/

#ifndef __DAAL_DYNAMIC_LIB_HELPER_H__
#define __DAAL_DYNAMIC_LIB_HELPER_H__

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64)

    #ifdef __linux__

        #include <dlfcn.h>

    #elif defined(_WIN32) || defined(_WIN64)

        #include <windows.h>

    #endif // __linux__

namespace daal
{
namespace services
{
namespace internal
{
/** @ingroup services_internal
 * @{
 */

/**
 * <a name="DAAL-CLASS-SERVICES__INTERNAL__DYNAMICLIBHELPER"></a>
 * \brief  Class that provides routines for managing dynamic library
 */

class DynamicLibHelper final
{
public:
    DynamicLibHelper()                         = delete;
    DynamicLibHelper(const DynamicLibHelper &) = delete;
    DynamicLibHelper(const char * libName, int flag, services::Status * status = nullptr)
    {
        services::Status localStatus;
    #ifdef __linux__
        _handle = dlopen(libName, flag);
    #elif defined(_WIN32) || defined(_WIN64)
        _handle = LoadLibraryA(libName);
    #endif
        if (!_handle)
        {
            services::internal::tryAssignStatusAndThrow(status, services::ErrorCanNotLoadDynamicLibrary);
            return;
        }
    }

    ~DynamicLibHelper()
    {
    #ifdef __linux__
        dlclose(_handle);
    #elif defined(_WIN32) || defined(_WIN64)
        FreeLibrary((HMODULE)_handle);
    #endif
    };

    template <typename T>
    T getSymbol(const char * symName, services::Status * status = nullptr)
    {
    #ifdef __linux__
        void * sym   = dlsym(_handle, symName);
        char * error = dlerror();

        if (nullptr != error)
        {
            services::internal::tryAssignStatusAndThrow(status, services::ErrorCanNotLoadDynamicLibrarySymbol);
            return nullptr;
        }
    #elif defined(_WIN32) || defined(_WIN64)
        void * sym = GetProcAddress((HMODULE)_handle, symName);

        if (nullptr != sym)
        {
            services::internal::tryAssignStatusAndThrow(status, services::ErrorCanNotLoadDynamicLibrarySymbol);
            return nullptr;
        }
    #endif

        return (T)sym;
    }

private:
    void * _handle;
};

} // namespace internal
} // namespace services
} // namespace daal

#endif // #if defined(__linux__) || defined(_WIN32) || defined(_WIN64)
#endif // __DAAL_DYNAMIC_LIB_HELPER_H__
