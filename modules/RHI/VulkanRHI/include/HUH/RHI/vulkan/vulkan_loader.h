#pragma once

#ifndef VK_NO_PROTOTYPES
#define VK_NO_PROTOTYPES
#endif

#include <HUH/Windows/dynamic_library.h>
#include <vulkan/vulkan.h>

#define HUH_VULKAN_INSTANCE_FUNCS_1_0(Macro) \
    Macro(vkGetInstanceProcAddr)   \
    Macro(vkCreateInstance)        \
    Macro(vkEnumerateInstanceExtensionProperties) \
    Macro(vkEnumerateInstanceLayerProperties)

#if defined(VK_VERSION_1_1)
#define HUH_VULKAN_INSTANCE_FUNCS_1_1(Macro) \
    Macro(vkEnumerateInstanceVersion)
#else
#define HUH_VULKAN_INSTANCE_FUNCS_1_1(Macro)
#endif

#define HUH_VULKAN_FUNCS_1_0(Macro) \
    Macro(vkDestroyInstance) \
    Macro(vkEnumeratePhysicalDevices) \
    Macro(vkGetPhysicalDeviceFeatures) \
    Macro(vkGetPhysicalDeviceFormatProperties) \
    Macro(vkGetPhysicalDeviceImageFormatProperties) \
    Macro(vkGetPhysicalDeviceProperties) \
    Macro(vkGetPhysicalDeviceQueueFamilyProperties) \
    Macro(vkGetPhysicalDeviceMemoryProperties) \
    Macro(vkGetDeviceProcAddr) \
    Macro(vkCreateDevice) \
    Macro(vkDestroyDevice) \
    Macro(vkEnumerateDeviceExtensionProperties) \
    Macro(vkEnumerateDeviceLayerProperties) \
    Macro(vkGetDeviceQueue) \
    Macro(vkQueueSubmit) \
    Macro(vkQueueWaitIdle) \
    Macro(vkDeviceWaitIdle) \
    Macro(vkAllocateMemory) \
    Macro(vkFreeMemory) \
    Macro(vkMapMemory) \
    Macro(vkUnmapMemory) \
    Macro(vkFlushMappedMemoryRanges) \
    Macro(vkInvalidateMappedMemoryRanges) \
    Macro(vkGetDeviceMemoryCommitment) \
    Macro(vkBindBufferMemory) \
    Macro(vkBindImageMemory) \
    Macro(vkGetBufferMemoryRequirements) \
    Macro(vkGetImageMemoryRequirements) \
    Macro(vkGetImageSparseMemoryRequirements) \
    Macro(vkGetPhysicalDeviceSparseImageFormatProperties) \
    Macro(vkQueueBindSparse) \
    Macro(vkCreateFence) \
    Macro(vkDestroyFence) \
    Macro(vkResetFences) \
    Macro(vkGetFenceStatus) \
    Macro(vkWaitForFences) \
    Macro(vkCreateSemaphore) \
    Macro(vkDestroySemaphore) \
    Macro(vkCreateEvent) \
    Macro(vkDestroyEvent) \
    Macro(vkGetEventStatus) \
    Macro(vkSetEvent) \
    Macro(vkResetEvent) \
    Macro(vkCreateQueryPool) \
    Macro(vkDestroyQueryPool) \
    Macro(vkGetQueryPoolResults) \
    Macro(vkCreateBuffer) \
    Macro(vkDestroyBuffer) \
    Macro(vkCreateBufferView) \
    Macro(vkDestroyBufferView) \
    Macro(vkCreateImage) \
    Macro(vkDestroyImage) \
    Macro(vkGetImageSubresourceLayout) \
    Macro(vkCreateImageView) \
    Macro(vkDestroyImageView) \
    Macro(vkCreateShaderModule) \
    Macro(vkDestroyShaderModule) \
    Macro(vkCreatePipelineCache) \
    Macro(vkDestroyPipelineCache) \
    Macro(vkGetPipelineCacheData) \
    Macro(vkMergePipelineCaches) \
    Macro(vkCreateGraphicsPipelines) \
    Macro(vkCreateComputePipelines) \
    Macro(vkDestroyPipeline) \
    Macro(vkCreatePipelineLayout) \
    Macro(vkDestroyPipelineLayout) \
    Macro(vkCreateSampler) \
    Macro(vkDestroySampler) \
    Macro(vkCreateDescriptorSetLayout) \
    Macro(vkDestroyDescriptorSetLayout) \
    Macro(vkCreateDescriptorPool) \
    Macro(vkDestroyDescriptorPool) \
    Macro(vkResetDescriptorPool) \
    Macro(vkAllocateDescriptorSets) \
    Macro(vkFreeDescriptorSets) \
    Macro(vkUpdateDescriptorSets) \
    Macro(vkCreateFramebuffer) \
    Macro(vkDestroyFramebuffer) \
    Macro(vkCreateRenderPass) \
    Macro(vkDestroyRenderPass) \
    Macro(vkGetRenderAreaGranularity) \
    Macro(vkCreateCommandPool) \
    Macro(vkDestroyCommandPool) \
    Macro(vkResetCommandPool) \
    Macro(vkAllocateCommandBuffers) \
    Macro(vkFreeCommandBuffers) \
    Macro(vkBeginCommandBuffer) \
    Macro(vkEndCommandBuffer) \
    Macro(vkResetCommandBuffer) \
    Macro(vkCmdBindPipeline) \
    Macro(vkCmdSetViewport) \
    Macro(vkCmdSetScissor) \
    Macro(vkCmdSetLineWidth) \
    Macro(vkCmdSetDepthBias) \
    Macro(vkCmdSetBlendConstants) \
    Macro(vkCmdSetDepthBounds) \
    Macro(vkCmdSetStencilCompareMask) \
    Macro(vkCmdSetStencilWriteMask) \
    Macro(vkCmdSetStencilReference) \
    Macro(vkCmdBindDescriptorSets) \
    Macro(vkCmdBindIndexBuffer) \
    Macro(vkCmdBindVertexBuffers) \
    Macro(vkCmdDraw) \
    Macro(vkCmdDrawIndexed) \
    Macro(vkCmdDrawIndirect) \
    Macro(vkCmdDrawIndexedIndirect) \
    Macro(vkCmdDispatch) \
    Macro(vkCmdDispatchIndirect) \
    Macro(vkCmdCopyBuffer) \
    Macro(vkCmdCopyImage) \
    Macro(vkCmdBlitImage) \
    Macro(vkCmdCopyBufferToImage) \
    Macro(vkCmdCopyImageToBuffer) \
    Macro(vkCmdUpdateBuffer) \
    Macro(vkCmdFillBuffer) \
    Macro(vkCmdClearColorImage) \
    Macro(vkCmdClearDepthStencilImage) \
    Macro(vkCmdClearAttachments) \
    Macro(vkCmdResolveImage) \
    Macro(vkCmdSetEvent) \
    Macro(vkCmdResetEvent) \
    Macro(vkCmdWaitEvents) \
    Macro(vkCmdPipelineBarrier) \
    Macro(vkCmdBeginQuery) \
    Macro(vkCmdEndQuery) \
    Macro(vkCmdResetQueryPool) \
    Macro(vkCmdWriteTimestamp) \
    Macro(vkCmdCopyQueryPoolResults) \
    Macro(vkCmdPushConstants) \
    Macro(vkCmdBeginRenderPass) \
    Macro(vkCmdNextSubpass) \
    Macro(vkCmdEndRenderPass) \
    Macro(vkCmdExecuteCommands)

#if defined(VK_VERSION_1_1)
#define HUH_VULKAN_FUNCS_1_1(Macro) \
    Macro(vkBindBufferMemory2) \
    Macro(vkBindImageMemory2) \
    Macro(vkGetDeviceGroupPeerMemoryFeatures) \
    Macro(vkCmdSetDeviceMask) \
    Macro(vkCmdDispatchBase) \
    Macro(vkEnumeratePhysicalDeviceGroups) \
    Macro(vkGetImageMemoryRequirements2) \
    Macro(vkGetBufferMemoryRequirements2) \
    Macro(vkGetImageSparseMemoryRequirements2) \
    Macro(vkGetPhysicalDeviceFeatures2) \
    Macro(vkGetPhysicalDeviceProperties2) \
    Macro(vkGetPhysicalDeviceFormatProperties2) \
    Macro(vkGetPhysicalDeviceImageFormatProperties2) \
    Macro(vkGetPhysicalDeviceQueueFamilyProperties2) \
    Macro(vkGetPhysicalDeviceMemoryProperties2) \
    Macro(vkGetPhysicalDeviceSparseImageFormatProperties2) \
    Macro(vkTrimCommandPool) \
    Macro(vkGetDeviceQueue2) \
    Macro(vkCreateSamplerYcbcrConversion) \
    Macro(vkDestroySamplerYcbcrConversion) \
    Macro(vkCreateDescriptorUpdateTemplate) \
    Macro(vkDestroyDescriptorUpdateTemplate) \
    Macro(vkUpdateDescriptorSetWithTemplate) \
    Macro(vkGetPhysicalDeviceExternalBufferProperties) \
    Macro(vkGetPhysicalDeviceExternalFenceProperties) \
    Macro(vkGetPhysicalDeviceExternalSemaphoreProperties) \
    Macro(vkGetDescriptorSetLayoutSupport)
#else
#define HUH_VULKAN_FUNCS_1_1(Macro)
#endif

#if defined(VK_VERSION_1_2)
#define HUH_VULKAN_FUNCS_1_2(Macro) \
	Macro(vkCmdDrawIndirectCount) \
	Macro(vkCmdDrawIndexedIndirectCount) \
	Macro(vkCreateRenderPass2) \
	Macro(vkCmdBeginRenderPass2) \
	Macro(vkCmdNextSubpass2) \
	Macro(vkCmdEndRenderPass2) \
	Macro(vkResetQueryPool) \
	Macro(vkGetSemaphoreCounterValue) \
	Macro(vkWaitSemaphores) \
	Macro(vkSignalSemaphore) \
	Macro(vkGetBufferDeviceAddress) \
	Macro(vkGetBufferOpaqueCaptureAddress) \
	Macro(vkGetDeviceMemoryOpaqueCaptureAddress)
#else
#define HUH_VULKAN_FUNCS_1_2(Macro)
#endif

#if defined(VK_VERSION_1_3)
#define HUH_VULKAN_FUNCS_1_3(Macro) \
	Macro(vkGetPhysicalDeviceToolProperties) \
	Macro(vkCreatePrivateDataSlot) \
	Macro(vkDestroyPrivateDataSlot) \
	Macro(vkSetPrivateData) \
	Macro(vkGetPrivateData) \
	Macro(vkCmdSetEvent2) \
	Macro(vkCmdResetEvent2) \
	Macro(vkCmdWaitEvents2) \
	Macro(vkCmdPipelineBarrier2) \
	Macro(vkCmdWriteTimestamp2) \
	Macro(vkQueueSubmit2) \
	Macro(vkCmdCopyBuffer2) \
	Macro(vkCmdCopyImage2) \
	Macro(vkCmdCopyBufferToImage2) \
	Macro(vkCmdCopyImageToBuffer2) \
	Macro(vkCmdBlitImage2) \
	Macro(vkCmdResolveImage2) \
	Macro(vkCmdBeginRendering) \
	Macro(vkCmdEndRendering) \
	Macro(vkCmdSetCullMode) \
	Macro(vkCmdSetFrontFace) \
	Macro(vkCmdSetPrimitiveTopology) \
	Macro(vkCmdSetViewportWithCount) \
	Macro(vkCmdSetScissorWithCount) \
	Macro(vkCmdBindVertexBuffers2) \
	Macro(vkCmdSetDepthTestEnable) \
	Macro(vkCmdSetDepthWriteEnable) \
	Macro(vkCmdSetDepthCompareOp) \
	Macro(vkCmdSetDepthBoundsTestEnable) \
	Macro(vkCmdSetStencilTestEnable) \
	Macro(vkCmdSetStencilOp) \
	Macro(vkCmdSetRasterizerDiscardEnable) \
	Macro(vkCmdSetDepthBiasEnable) \
	Macro(vkCmdSetPrimitiveRestartEnable) \
	Macro(vkGetDeviceBufferMemoryRequirements) \
	Macro(vkGetDeviceImageMemoryRequirements) \
	Macro(vkGetDeviceImageSparseMemoryRequirements)
#else
#define HUH_VULKAN_FUNCS_1_3(Macro)
#endif

#if defined(VK_VERSION_1_4)
#define HUH_VULKAN_FUNCS_1_4(Macro) \
	Macro(vkCmdSetLineStipple) \
	Macro(vkMapMemory2) \
	Macro(vkUnmapMemory2) \
	Macro(vkCmdBindIndexBuffer2) \
	Macro(vkGetRenderingAreaGranularity) \
	Macro(vkGetDeviceImageSubresourceLayout) \
	Macro(vkGetImageSubresourceLayout2) \
	Macro(vkCmdPushDescriptorSet) \
	Macro(vkCmdPushDescriptorSetWithTemplate) \
	Macro(vkCmdSetRenderingAttachmentLocations) \
	Macro(vkCmdSetRenderingInputAttachmentIndices) \
	Macro(vkCmdBindDescriptorSets2) \
	Macro(vkCmdPushConstants2) \
	Macro(vkCmdPushDescriptorSet2) \
	Macro(vkCmdPushDescriptorSetWithTemplate2) \
	Macro(vkCopyMemoryToImage) \
	Macro(vkCopyImageToMemory) \
	Macro(vkCopyImageToImage) \
	Macro(vkTransitionImageLayout)
#else
#define HUH_VULKAN_FUNCS_1_4(Macro)
#endif

#if defined(VK_KHR_surface)
#define HUH_VULKAN_FUNCS_KHR_surface(Macro) \
	Macro(vkDestroySurfaceKHR) \
	Macro(vkGetPhysicalDeviceSurfaceSupportKHR) \
	Macro(vkGetPhysicalDeviceSurfaceCapabilitiesKHR) \
	Macro(vkGetPhysicalDeviceSurfaceFormatsKHR) \
	Macro(vkGetPhysicalDeviceSurfacePresentModesKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_surface(Macro)
#endif

#if defined(VK_KHR_swapchain)
#define HUH_VULKAN_FUNCS_KHR_swapchain(Macro) \
	Macro(vkCreateSwapchainKHR) \
	Macro(vkDestroySwapchainKHR) \
	Macro(vkGetSwapchainImagesKHR) \
	Macro(vkAcquireNextImageKHR) \
	Macro(vkQueuePresentKHR) \
	Macro(vkGetDeviceGroupPresentCapabilitiesKHR) \
	Macro(vkGetDeviceGroupSurfacePresentModesKHR) \
	Macro(vkGetPhysicalDevicePresentRectanglesKHR) \
	Macro(vkAcquireNextImage2KHR)
#else
#define HUH_VULKAN_FUNCS_KHR_swapchain(Macro)
#endif

#if defined(VK_KHR_display)
#define HUH_VULKAN_FUNCS_KHR_display(Macro) \
	Macro(vkGetPhysicalDeviceDisplayPropertiesKHR) \
	Macro(vkGetPhysicalDeviceDisplayPlanePropertiesKHR) \
	Macro(vkGetDisplayPlaneSupportedDisplaysKHR) \
	Macro(vkGetDisplayModePropertiesKHR) \
	Macro(vkCreateDisplayModeKHR) \
	Macro(vkGetDisplayPlaneCapabilitiesKHR) \
	Macro(vkCreateDisplayPlaneSurfaceKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_display(Macro)
#endif

#if defined(VK_KHR_display_swapchain)
#define HUH_VULKAN_FUNCS_KHR_display_swapchain(Macro) \
	Macro(vkCreateSharedSwapchainsKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_display_swapchain(Macro)
#endif

#if defined(VK_KHR_video_queue)
#define HUH_VULKAN_FUNCS_KHR_video_queue(Macro) \
	Macro(vkGetPhysicalDeviceVideoCapabilitiesKHR) \
	Macro(vkGetPhysicalDeviceVideoFormatPropertiesKHR) \
	Macro(vkCreateVideoSessionKHR) \
	Macro(vkDestroyVideoSessionKHR) \
	Macro(vkGetVideoSessionMemoryRequirementsKHR) \
	Macro(vkBindVideoSessionMemoryKHR) \
	Macro(vkCreateVideoSessionParametersKHR) \
	Macro(vkUpdateVideoSessionParametersKHR) \
	Macro(vkDestroyVideoSessionParametersKHR) \
	Macro(vkCmdBeginVideoCodingKHR) \
	Macro(vkCmdEndVideoCodingKHR) \
	Macro(vkCmdControlVideoCodingKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_video_queue(Macro)
#endif

#if defined(VK_KHR_video_decode_queue)
#define HUH_VULKAN_FUNCS_KHR_video_decode_queue(Macro) \
	Macro(vkCmdDecodeVideoKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_video_decode_queue(Macro)
#endif

#if defined(VK_KHR_dynamic_rendering)
#define HUH_VULKAN_FUNCS_KHR_dynamic_rendering(Macro) \
	Macro(vkCmdBeginRenderingKHR) \
	Macro(vkCmdEndRenderingKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_dynamic_rendering(Macro)
#endif

#if defined(VK_KHR_get_physical_device_properties2)
#define HUH_VULKAN_FUNCS_KHR_get_physical_device_properties2(Macro) \
	Macro(vkGetPhysicalDeviceFeatures2KHR) \
	Macro(vkGetPhysicalDeviceProperties2KHR) \
	Macro(vkGetPhysicalDeviceFormatProperties2KHR) \
	Macro(vkGetPhysicalDeviceImageFormatProperties2KHR) \
	Macro(vkGetPhysicalDeviceQueueFamilyProperties2KHR) \
	Macro(vkGetPhysicalDeviceMemoryProperties2KHR) \
	Macro(vkGetPhysicalDeviceSparseImageFormatProperties2KHR)
#else
#define HUH_VULKAN_FUNCS_KHR_get_physical_device_properties2(Macro)
#endif

#if defined(VK_KHR_device_group)
#define HUH_VULKAN_FUNCS_KHR_device_group(Macro) \
	Macro(vkGetDeviceGroupPeerMemoryFeaturesKHR) \
	Macro(vkCmdSetDeviceMaskKHR) \
	Macro(vkCmdDispatchBaseKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_device_group(Macro)
#endif

#if defined(VK_KHR_maintenance1)
#define HUH_VULKAN_FUNCS_KHR_maintenance1(Macro) \
	Macro(vkTrimCommandPoolKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_maintenance1(Macro)
#endif

#if defined(VK_KHR_device_group_creation)
#define HUH_VULKAN_FUNCS_KHR_device_group_creation(Macro) \
	Macro(vkEnumeratePhysicalDeviceGroupsKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_device_group_creation(Macro)
#endif

#if defined(VK_KHR_external_memory_capabilities)
#define HUH_VULKAN_FUNCS_KHR_external_memory_capabilities(Macro) \
	Macro(vkGetPhysicalDeviceExternalBufferPropertiesKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_external_memory_capabilities(Macro)
#endif

#if defined(VK_KHR_external_memory_fd)
#define HUH_VULKAN_FUNCS_KHR_external_memory_fd(Macro) \
	Macro(vkGetMemoryFdKHR) \
	Macro(vkGetMemoryFdPropertiesKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_external_memory_fd(Macro)
#endif

#if defined(VK_KHR_external_semaphore_capabilities)
#define HUH_VULKAN_FUNCS_KHR_external_semaphore_capabilities(Macro) \
	Macro(vkGetPhysicalDeviceExternalSemaphorePropertiesKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_external_semaphore_capabilities(Macro)
#endif

#if defined(VK_KHR_external_semaphore_fd)
#define HUH_VULKAN_FUNCS_KHR_external_semaphore_fd(Macro) \
	Macro(vkImportSemaphoreFdKHR) \
	Macro(vkGetSemaphoreFdKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_external_semaphore_fd(Macro)
#endif

#if defined(VK_KHR_push_descriptor)
#define HUH_VULKAN_FUNCS_KHR_push_descriptor(Macro) \
	Macro(vkCmdPushDescriptorSetKHR) \
	Macro(vkCmdPushDescriptorSetWithTemplateKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_push_descriptor(Macro)
#endif

#if defined(VK_KHR_descriptor_update_template)
#define HUH_VULKAN_FUNCS_KHR_descriptor_update_template(Macro) \
	Macro(vkCreateDescriptorUpdateTemplateKHR) \
	Macro(vkDestroyDescriptorUpdateTemplateKHR) \
	Macro(vkUpdateDescriptorSetWithTemplateKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_descriptor_update_template(Macro)
#endif

#if defined(VK_KHR_create_renderpass2)
#define HUH_VULKAN_FUNCS_KHR_create_renderpass2(Macro) \
	Macro(vkCreateRenderPass2KHR) \
	Macro(vkCmdBeginRenderPass2KHR) \
	Macro(vkCmdNextSubpass2KHR) \
	Macro(vkCmdEndRenderPass2KHR)
#else
#define HUH_VULKAN_FUNCS_KHR_create_renderpass2(Macro)
#endif

#if defined(VK_KHR_shared_presentable_image)
#define HUH_VULKAN_FUNCS_KHR_shared_presentable_image(Macro) \
	Macro(vkGetSwapchainStatusKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_shared_presentable_image(Macro)
#endif

#if defined(VK_KHR_external_fence_capabilities)
#define HUH_VULKAN_FUNCS_KHR_external_fence_capabilities(Macro) \
	Macro(vkGetPhysicalDeviceExternalFencePropertiesKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_external_fence_capabilities(Macro)
#endif

#if defined(VK_KHR_external_fence_fd)
#define HUH_VULKAN_FUNCS_KHR_external_fence_fd(Macro) \
	Macro(vkImportFenceFdKHR) \
	Macro(vkGetFenceFdKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_external_fence_fd(Macro)
#endif

#if defined(VK_KHR_performance_query)
#define HUH_VULKAN_FUNCS_KHR_performance_query(Macro) \
	Macro(vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR) \
	Macro(vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR) \
	Macro(vkAcquireProfilingLockKHR) \
	Macro(vkReleaseProfilingLockKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_performance_query(Macro)
#endif

#if defined(VK_KHR_get_surface_capabilities2)
#define HUH_VULKAN_FUNCS_KHR_get_surface_capabilities2(Macro) \
	Macro(vkGetPhysicalDeviceSurfaceCapabilities2KHR) \
	Macro(vkGetPhysicalDeviceSurfaceFormats2KHR)
#else
#define HUH_VULKAN_FUNCS_KHR_get_surface_capabilities2(Macro)
#endif

#if defined(VK_KHR_get_display_properties2)
#define HUH_VULKAN_FUNCS_KHR_get_display_properties2(Macro) \
	Macro(vkGetPhysicalDeviceDisplayProperties2KHR) \
	Macro(vkGetPhysicalDeviceDisplayPlaneProperties2KHR) \
	Macro(vkGetDisplayModeProperties2KHR) \
	Macro(vkGetDisplayPlaneCapabilities2KHR)
#else
#define HUH_VULKAN_FUNCS_KHR_get_display_properties2(Macro)
#endif

#if defined(VK_KHR_get_memory_requirements2)
#define HUH_VULKAN_FUNCS_KHR_get_memory_requirements2(Macro) \
	Macro(vkGetImageMemoryRequirements2KHR) \
	Macro(vkGetBufferMemoryRequirements2KHR) \
	Macro(vkGetImageSparseMemoryRequirements2KHR)
#else
#define HUH_VULKAN_FUNCS_KHR_get_memory_requirements2(Macro)
#endif

#if defined(VK_KHR_SAMPLER_YCBCR_CONVERSION_SPEC_VERSION)
#define HUH_VULKAN_FUNCS_KHR_SAMPLER_YCBCR_CONVERSION_SPEC_VERSION(Macro) \
	Macro(vkCreateSamplerYcbcrConversionKHR) \
	Macro(vkDestroySamplerYcbcrConversionKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_SAMPLER_YCBCR_CONVERSION_SPEC_VERSION(Macro)
#endif

#if defined(VK_KHR_bind_memory2)
#define HUH_VULKAN_FUNCS_KHR_bind_memory2(Macro) \
	Macro(vkBindBufferMemory2KHR) \
	Macro(vkBindImageMemory2KHR)
#else
#define HUH_VULKAN_FUNCS_KHR_bind_memory2(Macro)
#endif

#if defined(VK_KHR_maintenance3)
#define HUH_VULKAN_FUNCS_KHR_maintenance3(Macro) \
	Macro(vkGetDescriptorSetLayoutSupportKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_maintenance3(Macro)
#endif

#if defined(VK_KHR_draw_indirect_count)
#define HUH_VULKAN_FUNCS_KHR_draw_indirect_count(Macro) \
	Macro(vkCmdDrawIndirectCountKHR) \
	Macro(vkCmdDrawIndexedIndirectCountKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_draw_indirect_count(Macro)
#endif

#if defined(VK_KHR_timeline_semaphore)
#define HUH_VULKAN_FUNCS_KHR_timeline_semaphore(Macro) \
	Macro(vkGetSemaphoreCounterValueKHR) \
	Macro(vkWaitSemaphoresKHR) \
	Macro(vkSignalSemaphoreKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_timeline_semaphore(Macro)
#endif

#if defined(VK_KHR_fragment_shading_rate)
#define HUH_VULKAN_FUNCS_KHR_fragment_shading_rate(Macro) \
	Macro(vkGetPhysicalDeviceFragmentShadingRatesKHR) \
	Macro(vkCmdSetFragmentShadingRateKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_fragment_shading_rate(Macro)
#endif

#if defined(VK_KHR_dynamic_rendering_local_read)
#define HUH_VULKAN_FUNCS_KHR_dynamic_rendering_local_read(Macro) \
	Macro(vkCmdSetRenderingAttachmentLocationsKHR) \
	Macro(vkCmdSetRenderingInputAttachmentIndicesKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_dynamic_rendering_local_read(Macro)
#endif

#if defined(VK_KHR_present_wait)
#define HUH_VULKAN_FUNCS_KHR_present_wait(Macro) \
	Macro(vkWaitForPresentKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_present_wait(Macro)
#endif

#if defined(VK_KHR_buffer_device_address)
#define HUH_VULKAN_FUNCS_KHR_buffer_device_address(Macro) \
	Macro(vkGetBufferDeviceAddressKHR) \
	Macro(vkGetBufferOpaqueCaptureAddressKHR) \
	Macro(vkGetDeviceMemoryOpaqueCaptureAddressKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_buffer_device_address(Macro)
#endif

#if defined(VK_KHR_deferred_host_operations)
#define HUH_VULKAN_FUNCS_KHR_deferred_host_operations(Macro) \
	Macro(vkCreateDeferredOperationKHR) \
	Macro(vkDestroyDeferredOperationKHR) \
	Macro(vkGetDeferredOperationMaxConcurrencyKHR) \
	Macro(vkGetDeferredOperationResultKHR) \
	Macro(vkDeferredOperationJoinKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_deferred_host_operations(Macro)
#endif

#if defined(VK_KHR_pipeline_executable_properties)
#define HUH_VULKAN_FUNCS_KHR_pipeline_executable_properties(Macro) \
	Macro(vkGetPipelineExecutablePropertiesKHR) \
	Macro(vkGetPipelineExecutableStatisticsKHR) \
	Macro(vkGetPipelineExecutableInternalRepresentationsKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_pipeline_executable_properties(Macro)
#endif

#if defined(VK_KHR_map_memory2)
#define HUH_VULKAN_FUNCS_KHR_map_memory2(Macro) \
	Macro(vkMapMemory2KHR) \
	Macro(vkUnmapMemory2KHR)
#else
#define HUH_VULKAN_FUNCS_KHR_map_memory2(Macro)
#endif

#define HUH_VULKAN_ALL_PROC_FUNCS(Macro) \
    HUH_VULKAN_FUNCS_1_0(Macro) \
    HUH_VULKAN_FUNCS_1_1(Macro) \
    HUH_VULKAN_FUNCS_1_2(Macro) \
    HUH_VULKAN_FUNCS_1_3(Macro) \
    HUH_VULKAN_FUNCS_1_4(Macro) \
    HUH_VULKAN_FUNCS_KHR_surface(Macro) \
    HUH_VULKAN_FUNCS_KHR_swapchain(Macro) \
    HUH_VULKAN_FUNCS_KHR_display(Macro) \
    HUH_VULKAN_FUNCS_KHR_display_swapchain(Macro) \
    HUH_VULKAN_FUNCS_KHR_video_queue(Macro) \
    HUH_VULKAN_FUNCS_KHR_video_decode_queue(Macro) \
    HUH_VULKAN_FUNCS_KHR_dynamic_rendering(Macro) \
    HUH_VULKAN_FUNCS_KHR_get_physical_device_properties2(Macro) \
    HUH_VULKAN_FUNCS_KHR_device_group(Macro) \
    HUH_VULKAN_FUNCS_KHR_maintenance1(Macro) \
    HUH_VULKAN_FUNCS_KHR_device_group_creation(Macro) \
    HUH_VULKAN_FUNCS_KHR_external_memory_capabilities(Macro) \
    HUH_VULKAN_FUNCS_KHR_external_memory_fd(Macro) \
    HUH_VULKAN_FUNCS_KHR_external_semaphore_capabilities(Macro) \
    HUH_VULKAN_FUNCS_KHR_external_semaphore_fd(Macro) \
    HUH_VULKAN_FUNCS_KHR_push_descriptor(Macro) \
    HUH_VULKAN_FUNCS_KHR_descriptor_update_template(Macro) \
    HUH_VULKAN_FUNCS_KHR_create_renderpass2(Macro) \
    HUH_VULKAN_FUNCS_KHR_shared_presentable_image(Macro) \
    HUH_VULKAN_FUNCS_KHR_external_fence_capabilities(Macro) \
    HUH_VULKAN_FUNCS_KHR_external_fence_fd(Macro) \
    HUH_VULKAN_FUNCS_KHR_performance_query(Macro) \
    HUH_VULKAN_FUNCS_KHR_get_surface_capabilities2(Macro) \
    HUH_VULKAN_FUNCS_KHR_get_display_properties2(Macro) \
    HUH_VULKAN_FUNCS_KHR_get_memory_requirements2(Macro) \
    HUH_VULKAN_FUNCS_KHR_SAMPLER_YCBCR_CONVERSION_SPEC_VERSION(Macro) \
    HUH_VULKAN_FUNCS_KHR_bind_memory2(Macro) \
    HUH_VULKAN_FUNCS_KHR_maintenance3(Macro) \
    HUH_VULKAN_FUNCS_KHR_draw_indirect_count(Macro) \
    HUH_VULKAN_FUNCS_KHR_timeline_semaphore(Macro) \
    HUH_VULKAN_FUNCS_KHR_fragment_shading_rate(Macro) \
    HUH_VULKAN_FUNCS_KHR_dynamic_rendering_local_read(Macro) \
    HUH_VULKAN_FUNCS_KHR_present_wait(Macro) \
    HUH_VULKAN_FUNCS_KHR_buffer_device_address(Macro) \
    HUH_VULKAN_FUNCS_KHR_deferred_host_operations(Macro) \
    HUH_VULKAN_FUNCS_KHR_pipeline_executable_properties(Macro) \
    HUH_VULKAN_FUNCS_KHR_map_memory2(Macro)

#define HUH_VULKAN_ALL_FUNCS(Macro) \
    HUH_VULKAN_INSTANCE_FUNCS_1_0(Macro) \
    HUH_VULKAN_INSTANCE_FUNCS_1_1(Macro) \
    HUH_VULKAN_FUNCS_1_0(Macro) \
    HUH_VULKAN_FUNCS_1_1(Macro) \
    HUH_VULKAN_FUNCS_1_2(Macro) \
    HUH_VULKAN_FUNCS_1_3(Macro) \
    HUH_VULKAN_FUNCS_1_4(Macro) \
    HUH_VULKAN_FUNCS_KHR_surface(Macro) \
    HUH_VULKAN_FUNCS_KHR_swapchain(Macro) \
    HUH_VULKAN_FUNCS_KHR_display(Macro) \
    HUH_VULKAN_FUNCS_KHR_display_swapchain(Macro) \
    HUH_VULKAN_FUNCS_KHR_video_queue(Macro) \
    HUH_VULKAN_FUNCS_KHR_video_decode_queue(Macro) \
    HUH_VULKAN_FUNCS_KHR_dynamic_rendering(Macro) \
    HUH_VULKAN_FUNCS_KHR_get_physical_device_properties2(Macro) \
    HUH_VULKAN_FUNCS_KHR_device_group(Macro) \
    HUH_VULKAN_FUNCS_KHR_maintenance1(Macro) \
    HUH_VULKAN_FUNCS_KHR_device_group_creation(Macro) \
    HUH_VULKAN_FUNCS_KHR_external_memory_capabilities(Macro) \
    HUH_VULKAN_FUNCS_KHR_external_memory_fd(Macro) \
    HUH_VULKAN_FUNCS_KHR_external_semaphore_capabilities(Macro) \
    HUH_VULKAN_FUNCS_KHR_external_semaphore_fd(Macro) \
    HUH_VULKAN_FUNCS_KHR_push_descriptor(Macro) \
    HUH_VULKAN_FUNCS_KHR_descriptor_update_template(Macro) \
    HUH_VULKAN_FUNCS_KHR_create_renderpass2(Macro) \
    HUH_VULKAN_FUNCS_KHR_shared_presentable_image(Macro) \
    HUH_VULKAN_FUNCS_KHR_external_fence_capabilities(Macro) \
    HUH_VULKAN_FUNCS_KHR_external_fence_fd(Macro) \
    HUH_VULKAN_FUNCS_KHR_performance_query(Macro) \
    HUH_VULKAN_FUNCS_KHR_get_surface_capabilities2(Macro) \
    HUH_VULKAN_FUNCS_KHR_get_display_properties2(Macro) \
    HUH_VULKAN_FUNCS_KHR_get_memory_requirements2(Macro) \
    HUH_VULKAN_FUNCS_KHR_SAMPLER_YCBCR_CONVERSION_SPEC_VERSION(Macro) \
    HUH_VULKAN_FUNCS_KHR_bind_memory2(Macro) \
    HUH_VULKAN_FUNCS_KHR_maintenance3(Macro) \
    HUH_VULKAN_FUNCS_KHR_draw_indirect_count(Macro) \
    HUH_VULKAN_FUNCS_KHR_timeline_semaphore(Macro) \
    HUH_VULKAN_FUNCS_KHR_fragment_shading_rate(Macro) \
    HUH_VULKAN_FUNCS_KHR_dynamic_rendering_local_read(Macro) \
    HUH_VULKAN_FUNCS_KHR_present_wait(Macro) \
    HUH_VULKAN_FUNCS_KHR_buffer_device_address(Macro) \
    HUH_VULKAN_FUNCS_KHR_deferred_host_operations(Macro) \
    HUH_VULKAN_FUNCS_KHR_pipeline_executable_properties(Macro) \
    HUH_VULKAN_FUNCS_KHR_map_memory2(Macro)

namespace HUH {
bool LoadVulkan();
bool LoadVulkanInstance(VkInstance instance);

extern HUH::DynamicLibrary s_vulkan_lib;

#define CreateVulkanFunc(Type) extern PFN_##Type Type;
HUH_VULKAN_ALL_FUNCS(CreateVulkanFunc)
#undef CreateVulkanFunc

}// namespace HUH
