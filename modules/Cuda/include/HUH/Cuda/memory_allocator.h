#pragma once

#include <HUH/RHI/device.h>
#include <driver_types.h>
#include <map>

namespace HUH::Cuda {

template<typename T>
class UniquePtr {
public:
    UniquePtr(T* ptr) : m_ptr(ptr) {}
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;
    UniquePtr(UniquePtr&& other) noexcept {
        m_ptr = other.m_ptr;
        other.m_ptr = nullptr;
    }
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        m_ptr = other.m_ptr;
        other.m_ptr = nullptr;
        return *this;
    }

    operator T*() const { return m_ptr; }
    T* Get() { return m_ptr; }
    T operator*() { return *m_ptr; }

    void Reset(T* ptr) {
        cudaFree(m_ptr);
        m_ptr = ptr;
    }

    T* Release() {
        T* ptr = m_ptr;
        m_ptr = nullptr;
        return ptr;
    }

    ~UniquePtr() {
        if (m_ptr) {
            cudaFree(m_ptr);
        }
    }

private:
    T* m_ptr = nullptr;
};

class MemoryAllocator {
public:
    HUH_NODISCARD void* MapRHIBuffer(HUH::RHI::Buffer* Buffer);

    struct Allocation {
        HUH::RHI::Buffer::SharedMemoryInfo::PlatformHandle PlatformHandle;
        cudaExternalMemory_t ExternalMemory = nullptr;

        HUH_NODISCARD void* MapMemory(const HUH::RHI::Buffer::SharedMemoryInfo& Shm) const;
    };
    ~MemoryAllocator();

protected:
    std::vector<Allocation> m_allocations;
};

}// namespace HUH::Cuda
