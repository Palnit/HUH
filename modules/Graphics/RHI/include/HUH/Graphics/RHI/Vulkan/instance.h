#pragma once

#include <volk.h>
#include <iostream>
#include "HUH/Graphics/RHI/instance.h"
namespace HUH::RHI {
class HUH_API VulkanInstance : public RHI::Instance {
public:
    ~VulkanInstance() override {}
    void Destroy() override;

protected:
    bool Init() override;

    VkInstance m_instance = VK_NULL_HANDLE;
};
}// namespace HUH::RHI
