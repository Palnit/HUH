#pragma once

#ifndef VK_NO_PROTOTYPES
#define VK_NO_PROTOTYPES
#endif

#include <HUH/dynamic_library.h>
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

#if defined(VK_KHR_video_encode_queue)
#define HUH_VULKAN_FUNCS_KHR_video_encode_queue(Macro) \
	Macro(vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR) \
	Macro(vkGetEncodedVideoSessionParametersKHR) \
	Macro(vkCmdEncodeVideoKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_video_encode_queue(Macro)
#endif

#if defined(VK_KHR_synchronization2)
#define HUH_VULKAN_FUNCS_KHR_synchronization2(Macro) \
	Macro(vkCmdSetEvent2KHR) \
	Macro(vkCmdResetEvent2KHR) \
	Macro(vkCmdWaitEvents2KHR) \
	Macro(vkCmdPipelineBarrier2KHR) \
	Macro(vkCmdWriteTimestamp2KHR) \
	Macro(vkQueueSubmit2KHR)
#else
#define HUH_VULKAN_FUNCS_KHR_synchronization2(Macro)
#endif

#if defined(VK_KHR_copy_commands2)
#define HUH_VULKAN_FUNCS_KHR_copy_commands2(Macro) \
	Macro(vkCmdCopyBuffer2KHR) \
	Macro(vkCmdCopyImage2KHR) \
	Macro(vkCmdCopyBufferToImage2KHR) \
	Macro(vkCmdCopyImageToBuffer2KHR) \
	Macro(vkCmdBlitImage2KHR) \
	Macro(vkCmdResolveImage2KHR)
#else
#define HUH_VULKAN_FUNCS_KHR_copy_commands2(Macro)
#endif

#if defined(VK_KHR_ray_tracing_maintenance1)
#define HUH_VULKAN_FUNCS_KHR_ray_tracing_maintenance1(Macro) \
	Macro(vkCmdTraceRaysIndirect2KHR)
#else
#define HUH_VULKAN_FUNCS_KHR_ray_tracing_maintenance1(Macro)
#endif

#if defined(VK_KHR_maintenance4)
#define HUH_VULKAN_FUNCS_KHR_maintenance4(Macro) \
	Macro(vkGetDeviceBufferMemoryRequirementsKHR) \
	Macro(vkGetDeviceImageMemoryRequirementsKHR) \
	Macro(vkGetDeviceImageSparseMemoryRequirementsKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_maintenance4(Macro)
#endif

#if defined(VK_KHR_maintenance5)
#define HUH_VULKAN_FUNCS_KHR_maintenance5(Macro) \
	Macro(vkCmdBindIndexBuffer2KHR) \
	Macro(vkGetRenderingAreaGranularityKHR) \
	Macro(vkGetDeviceImageSubresourceLayoutKHR) \
	Macro(vkGetImageSubresourceLayout2KHR)
#else
#define HUH_VULKAN_FUNCS_KHR_maintenance5(Macro)
#endif

#if defined(VK_KHR_pipeline_binary)
#define HUH_VULKAN_FUNCS_KHR_pipeline_binary(Macro) \
	Macro(vkCreatePipelineBinariesKHR) \
	Macro(vkDestroyPipelineBinaryKHR) \
	Macro(vkGetPipelineKeyKHR) \
	Macro(vkGetPipelineBinaryDataKHR) \
	Macro(vkReleaseCapturedPipelineDataKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_pipeline_binary(Macro)
#endif

#if defined(VK_KHR_cooperative_matrix)
#define HUH_VULKAN_FUNCS_KHR_cooperative_matrix(Macro) \
	Macro(vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_cooperative_matrix(Macro)
#endif

#if defined(VK_KHR_line_rasterization)
#define HUH_VULKAN_FUNCS_KHR_line_rasterization(Macro) \
	Macro(vkCmdSetLineStippleKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_line_rasterization(Macro)
#endif

#if defined(VK_KHR_calibrated_timestamps)
#define HUH_VULKAN_FUNCS_KHR_calibrated_timestamps(Macro) \
	Macro(vkGetPhysicalDeviceCalibrateableTimeDomainsKHR) \
	Macro(vkGetCalibratedTimestampsKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_calibrated_timestamps(Macro)
#endif

#if defined(VK_KHR_maintenance6)
#define HUH_VULKAN_FUNCS_KHR_maintenance6(Macro) \
	Macro(vkCmdBindDescriptorSets2KHR) \
	Macro(vkCmdPushConstants2KHR) \
	Macro(vkCmdPushDescriptorSet2KHR) \
	Macro(vkCmdPushDescriptorSetWithTemplate2KHR) \
	Macro(vkCmdSetDescriptorBufferOffsets2EXT) \
	Macro(vkCmdBindDescriptorBufferEmbeddedSamplers2EXT)
#else
#define HUH_VULKAN_FUNCS_KHR_maintenance6(Macro)
#endif

#if defined(VK_EXT_debug_report)
#define HUH_VULKAN_FUNCS_EXT_debug_report(Macro) \
	Macro(vkCreateDebugReportCallbackEXT) \
	Macro(vkDestroyDebugReportCallbackEXT) \
	Macro(vkDebugReportMessageEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_debug_report(Macro)
#endif

#if defined(VK_EXT_debug_marker)
#define HUH_VULKAN_FUNCS_EXT_debug_marker(Macro) \
	Macro(vkDebugMarkerSetObjectTagEXT) \
	Macro(vkDebugMarkerSetObjectNameEXT) \
	Macro(vkCmdDebugMarkerBeginEXT) \
	Macro(vkCmdDebugMarkerEndEXT) \
	Macro(vkCmdDebugMarkerInsertEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_debug_marker(Macro)
#endif

#if defined(VK_EXT_transform_feedback)
#define HUH_VULKAN_FUNCS_EXT_transform_feedback(Macro) \
	Macro(vkCmdBindTransformFeedbackBuffersEXT) \
	Macro(vkCmdBeginTransformFeedbackEXT) \
	Macro(vkCmdEndTransformFeedbackEXT) \
	Macro(vkCmdBeginQueryIndexedEXT) \
	Macro(vkCmdEndQueryIndexedEXT) \
	Macro(vkCmdDrawIndirectByteCountEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_transform_feedback(Macro)
#endif

#if defined(VK_NVX_binary_import)
#define HUH_VULKAN_FUNCS_NVX_binary_import(Macro) \
	Macro(vkCreateCuModuleNVX) \
	Macro(vkCreateCuFunctionNVX) \
	Macro(vkDestroyCuModuleNVX) \
	Macro(vkDestroyCuFunctionNVX) \
	Macro(vkCmdCuLaunchKernelNVX)
#else
#define HUH_VULKAN_FUNCS_NVX_binary_import(Macro)
#endif

#if defined(VK_NVX_image_view_handle)
#define HUH_VULKAN_FUNCS_NVX_image_view_handle(Macro) \
	Macro(vkGetImageViewHandleNVX) \
	Macro(vkGetImageViewHandle64NVX) \
	Macro(vkGetImageViewAddressNVX)
#else
#define HUH_VULKAN_FUNCS_NVX_image_view_handle(Macro)
#endif

#if defined(VK_AMD_draw_indirect_count)
#define HUH_VULKAN_FUNCS_AMD_draw_indirect_count(Macro) \
	Macro(vkCmdDrawIndirectCountAMD) \
	Macro(vkCmdDrawIndexedIndirectCountAMD)
#else
#define HUH_VULKAN_FUNCS_AMD_draw_indirect_count(Macro)
#endif

#if defined(VK_AMD_shader_info)
#define HUH_VULKAN_FUNCS_AMD_shader_info(Macro) \
	Macro(vkGetShaderInfoAMD)
#else
#define HUH_VULKAN_FUNCS_AMD_shader_info(Macro)
#endif

#if defined(VK_NV_external_memory_capabilities)
#define HUH_VULKAN_FUNCS_NV_external_memory_capabilities(Macro) \
	Macro(vkGetPhysicalDeviceExternalImageFormatPropertiesNV)
#else
#define HUH_VULKAN_FUNCS_NV_external_memory_capabilities(Macro)
#endif

#if defined(VK_EXT_conditional_rendering)
#define HUH_VULKAN_FUNCS_EXT_conditional_rendering(Macro) \
	Macro(vkCmdBeginConditionalRenderingEXT) \
	Macro(vkCmdEndConditionalRenderingEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_conditional_rendering(Macro)
#endif

#if defined(VK_NV_clip_space_w_scaling)
#define HUH_VULKAN_FUNCS_NV_clip_space_w_scaling(Macro) \
	Macro(vkCmdSetViewportWScalingNV)
#else
#define HUH_VULKAN_FUNCS_NV_clip_space_w_scaling(Macro)
#endif

#if defined(VK_EXT_direct_mode_display)
#define HUH_VULKAN_FUNCS_EXT_direct_mode_display(Macro) \
	Macro(vkReleaseDisplayEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_direct_mode_display(Macro)
#endif

#if defined(VK_EXT_display_surface_counter)
#define HUH_VULKAN_FUNCS_EXT_display_surface_counter(Macro) \
	Macro(vkGetPhysicalDeviceSurfaceCapabilities2EXT)
#else
#define HUH_VULKAN_FUNCS_EXT_display_surface_counter(Macro)
#endif

#if defined(VK_EXT_display_control)
#define HUH_VULKAN_FUNCS_EXT_display_control(Macro) \
	Macro(vkDisplayPowerControlEXT) \
	Macro(vkRegisterDeviceEventEXT) \
	Macro(vkRegisterDisplayEventEXT) \
	Macro(vkGetSwapchainCounterEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_display_control(Macro)
#endif

#if defined(VK_GOOGLE_display_timing)
#define HUH_VULKAN_FUNCS_GOOGLE_display_timing(Macro) \
	Macro(vkGetRefreshCycleDurationGOOGLE) \
	Macro(vkGetPastPresentationTimingGOOGLE)
#else
#define HUH_VULKAN_FUNCS_GOOGLE_display_timing(Macro)
#endif

#if defined(VK_EXT_discard_rectangles)
#define HUH_VULKAN_FUNCS_EXT_discard_rectangles(Macro) \
	Macro(vkCmdSetDiscardRectangleEXT) \
	Macro(vkCmdSetDiscardRectangleEnableEXT) \
	Macro(vkCmdSetDiscardRectangleModeEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_discard_rectangles(Macro)
#endif

#if defined(VK_EXT_hdr_metadata)
#define HUH_VULKAN_FUNCS_EXT_hdr_metadata(Macro) \
	Macro(vkSetHdrMetadataEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_hdr_metadata(Macro)
#endif

#if defined(VK_EXT_debug_utils)
#define HUH_VULKAN_FUNCS_EXT_debug_utils(Macro) \
	Macro(vkSetDebugUtilsObjectNameEXT) \
	Macro(vkSetDebugUtilsObjectTagEXT) \
	Macro(vkQueueBeginDebugUtilsLabelEXT) \
	Macro(vkQueueEndDebugUtilsLabelEXT) \
	Macro(vkQueueInsertDebugUtilsLabelEXT) \
	Macro(vkCmdBeginDebugUtilsLabelEXT) \
	Macro(vkCmdEndDebugUtilsLabelEXT) \
	Macro(vkCmdInsertDebugUtilsLabelEXT) \
	Macro(vkCreateDebugUtilsMessengerEXT) \
	Macro(vkDestroyDebugUtilsMessengerEXT) \
	Macro(vkSubmitDebugUtilsMessageEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_debug_utils(Macro)
#endif

#if defined(VK_EXT_sample_locations)
#define HUH_VULKAN_FUNCS_EXT_sample_locations(Macro) \
	Macro(vkCmdSetSampleLocationsEXT) \
	Macro(vkGetPhysicalDeviceMultisamplePropertiesEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_sample_locations(Macro)
#endif

#if defined(VK_EXT_image_drm_format_modifier)
#define HUH_VULKAN_FUNCS_EXT_image_drm_format_modifier(Macro) \
	Macro(vkGetImageDrmFormatModifierPropertiesEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_image_drm_format_modifier(Macro)
#endif

#if defined(VK_EXT_validation_cache)
#define HUH_VULKAN_FUNCS_EXT_validation_cache(Macro) \
	Macro(vkCreateValidationCacheEXT) \
	Macro(vkDestroyValidationCacheEXT) \
	Macro(vkMergeValidationCachesEXT) \
	Macro(vkGetValidationCacheDataEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_validation_cache(Macro)
#endif

#if defined(VK_NV_shading_rate_image)
#define HUH_VULKAN_FUNCS_NV_shading_rate_image(Macro) \
	Macro(vkCmdBindShadingRateImageNV) \
	Macro(vkCmdSetViewportShadingRatePaletteNV) \
	Macro(vkCmdSetCoarseSampleOrderNV)
#else
#define HUH_VULKAN_FUNCS_NV_shading_rate_image(Macro)
#endif

#if defined(VK_NV_ray_tracing)
#define HUH_VULKAN_FUNCS_NV_ray_tracing(Macro) \
	Macro(vkCreateAccelerationStructureNV) \
	Macro(vkDestroyAccelerationStructureNV) \
	Macro(vkGetAccelerationStructureMemoryRequirementsNV) \
	Macro(vkBindAccelerationStructureMemoryNV) \
	Macro(vkCmdBuildAccelerationStructureNV) \
	Macro(vkCmdCopyAccelerationStructureNV) \
	Macro(vkCmdTraceRaysNV) \
	Macro(vkCreateRayTracingPipelinesNV) \
	Macro(vkGetRayTracingShaderGroupHandlesKHR) \
	Macro(vkGetRayTracingShaderGroupHandlesNV) \
	Macro(vkGetAccelerationStructureHandleNV) \
	Macro(vkCmdWriteAccelerationStructuresPropertiesNV) \
	Macro(vkCompileDeferredNV)
#else
#define HUH_VULKAN_FUNCS_NV_ray_tracing(Macro)
#endif

#if defined(VK_EXT_external_memory_host)
#define HUH_VULKAN_FUNCS_EXT_external_memory_host(Macro) \
	Macro(vkGetMemoryHostPointerPropertiesEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_external_memory_host(Macro)
#endif

#if defined(VK_AMD_buffer_marker)
#define HUH_VULKAN_FUNCS_AMD_buffer_marker(Macro) \
	Macro(vkCmdWriteBufferMarkerAMD) \
	Macro(vkCmdWriteBufferMarker2AMD)
#else
#define HUH_VULKAN_FUNCS_AMD_buffer_marker(Macro)
#endif

#if defined(VK_EXT_calibrated_timestamps)
#define HUH_VULKAN_FUNCS_EXT_calibrated_timestamps(Macro) \
	Macro(vkGetPhysicalDeviceCalibrateableTimeDomainsEXT) \
	Macro(vkGetCalibratedTimestampsEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_calibrated_timestamps(Macro)
#endif

#if defined(VK_NV_mesh_shader)
#define HUH_VULKAN_FUNCS_NV_mesh_shader(Macro) \
	Macro(vkCmdDrawMeshTasksNV) \
	Macro(vkCmdDrawMeshTasksIndirectNV) \
	Macro(vkCmdDrawMeshTasksIndirectCountNV)
#else
#define HUH_VULKAN_FUNCS_NV_mesh_shader(Macro)
#endif

#if defined(VK_NV_scissor_exclusive)
#define HUH_VULKAN_FUNCS_NV_scissor_exclusive(Macro) \
	Macro(vkCmdSetExclusiveScissorEnableNV) \
	Macro(vkCmdSetExclusiveScissorNV)
#else
#define HUH_VULKAN_FUNCS_NV_scissor_exclusive(Macro)
#endif

#if defined(VK_NV_device_diagnostic_checkpoints)
#define HUH_VULKAN_FUNCS_NV_device_diagnostic_checkpoints(Macro) \
	Macro(vkCmdSetCheckpointNV) \
	Macro(vkGetQueueCheckpointDataNV) \
	Macro(vkGetQueueCheckpointData2NV)
#else
#define HUH_VULKAN_FUNCS_NV_device_diagnostic_checkpoints(Macro)
#endif

#if defined(VK_INTEL_performance_query)
#define HUH_VULKAN_FUNCS_INTEL_performance_query(Macro) \
	Macro(vkInitializePerformanceApiINTEL) \
	Macro(vkUninitializePerformanceApiINTEL) \
	Macro(vkCmdSetPerformanceMarkerINTEL) \
	Macro(vkCmdSetPerformanceStreamMarkerINTEL) \
	Macro(vkCmdSetPerformanceOverrideINTEL) \
	Macro(vkAcquirePerformanceConfigurationINTEL) \
	Macro(vkReleasePerformanceConfigurationINTEL) \
	Macro(vkQueueSetPerformanceConfigurationINTEL) \
	Macro(vkGetPerformanceParameterINTEL)
#else
#define HUH_VULKAN_FUNCS_INTEL_performance_query(Macro)
#endif

#if defined(VK_AMD_display_native_hdr)
#define HUH_VULKAN_FUNCS_AMD_display_native_hdr(Macro) \
	Macro(vkSetLocalDimmingAMD)
#else
#define HUH_VULKAN_FUNCS_AMD_display_native_hdr(Macro)
#endif

#if defined(VK_EXT_buffer_device_address)
#define HUH_VULKAN_FUNCS_EXT_buffer_device_address(Macro) \
	Macro(vkGetBufferDeviceAddressEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_buffer_device_address(Macro)
#endif

#if defined(VK_EXT_tooling_info)
#define HUH_VULKAN_FUNCS_EXT_tooling_info(Macro) \
	Macro(vkGetPhysicalDeviceToolPropertiesEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_tooling_info(Macro)
#endif

#if defined(VK_NV_cooperative_matrix)
#define HUH_VULKAN_FUNCS_NV_cooperative_matrix(Macro) \
	Macro(vkGetPhysicalDeviceCooperativeMatrixPropertiesNV)
#else
#define HUH_VULKAN_FUNCS_NV_cooperative_matrix(Macro)
#endif

#if defined(VK_NV_coverage_reduction_mode)
#define HUH_VULKAN_FUNCS_NV_coverage_reduction_mode(Macro) \
	Macro(vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV)
#else
#define HUH_VULKAN_FUNCS_NV_coverage_reduction_mode(Macro)
#endif

#if defined(VK_EXT_headless_surface)
#define HUH_VULKAN_FUNCS_EXT_headless_surface(Macro) \
	Macro(vkCreateHeadlessSurfaceEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_headless_surface(Macro)
#endif

#if defined(VK_EXT_line_rasterization)
#define HUH_VULKAN_FUNCS_EXT_line_rasterization(Macro) \
	Macro(vkCmdSetLineStippleEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_line_rasterization(Macro)
#endif

#if defined(VK_EXT_host_query_reset)
#define HUH_VULKAN_FUNCS_EXT_host_query_reset(Macro) \
	Macro(vkResetQueryPoolEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_host_query_reset(Macro)
#endif

#if defined(VK_EXT_extended_dynamic_state)
#define HUH_VULKAN_FUNCS_EXT_extended_dynamic_state(Macro) \
	Macro(vkCmdSetCullModeEXT) \
	Macro(vkCmdSetFrontFaceEXT) \
	Macro(vkCmdSetPrimitiveTopologyEXT) \
	Macro(vkCmdSetViewportWithCountEXT) \
	Macro(vkCmdSetScissorWithCountEXT) \
	Macro(vkCmdBindVertexBuffers2EXT) \
	Macro(vkCmdSetDepthTestEnableEXT) \
	Macro(vkCmdSetDepthWriteEnableEXT) \
	Macro(vkCmdSetDepthCompareOpEXT) \
	Macro(vkCmdSetDepthBoundsTestEnableEXT) \
	Macro(vkCmdSetStencilTestEnableEXT) \
	Macro(vkCmdSetStencilOpEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_extended_dynamic_state(Macro)
#endif

#if defined(VK_EXT_host_image_copy)
#define HUH_VULKAN_FUNCS_EXT_host_image_copy(Macro) \
	Macro(vkCopyMemoryToImageEXT) \
	Macro(vkCopyImageToMemoryEXT) \
	Macro(vkCopyImageToImageEXT) \
	Macro(vkTransitionImageLayoutEXT) \
	Macro(vkGetImageSubresourceLayout2EXT)
#else
#define HUH_VULKAN_FUNCS_EXT_host_image_copy(Macro)
#endif

#if defined(VK_EXT_swapchain_maintenance1)
#define HUH_VULKAN_FUNCS_EXT_swapchain_maintenance1(Macro) \
	Macro(vkReleaseSwapchainImagesEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_swapchain_maintenance1(Macro)
#endif

#if defined(VK_NV_device_generated_commands)
#define HUH_VULKAN_FUNCS_NV_device_generated_commands(Macro) \
	Macro(vkGetGeneratedCommandsMemoryRequirementsNV) \
	Macro(vkCmdPreprocessGeneratedCommandsNV) \
	Macro(vkCmdExecuteGeneratedCommandsNV) \
	Macro(vkCmdBindPipelineShaderGroupNV) \
	Macro(vkCreateIndirectCommandsLayoutNV) \
	Macro(vkDestroyIndirectCommandsLayoutNV)
#else
#define HUH_VULKAN_FUNCS_NV_device_generated_commands(Macro)
#endif

#if defined(VK_EXT_depth_bias_control)
#define HUH_VULKAN_FUNCS_EXT_depth_bias_control(Macro) \
	Macro(vkCmdSetDepthBias2EXT)
#else
#define HUH_VULKAN_FUNCS_EXT_depth_bias_control(Macro)
#endif

#if defined(VK_EXT_acquire_drm_display)
#define HUH_VULKAN_FUNCS_EXT_acquire_drm_display(Macro) \
	Macro(vkAcquireDrmDisplayEXT) \
	Macro(vkGetDrmDisplayEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_acquire_drm_display(Macro)
#endif

#if defined(VK_EXT_PRIVATE_DATA_SPEC_VERSION)
#define HUH_VULKAN_FUNCS_EXT_PRIVATE_DATA_SPEC_VERSION(Macro) \
	Macro(vkCreatePrivateDataSlotEXT) \
	Macro(vkDestroyPrivateDataSlotEXT) \
	Macro(vkSetPrivateDataEXT) \
	Macro(vkGetPrivateDataEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_PRIVATE_DATA_SPEC_VERSION(Macro)
#endif

#if defined(VK_NV_cuda_kernel_launch)
#define HUH_VULKAN_FUNCS_NV_cuda_kernel_launch(Macro) \
	Macro(vkCreateCudaModuleNV) \
	Macro(vkGetCudaModuleCacheNV) \
	Macro(vkCreateCudaFunctionNV) \
	Macro(vkDestroyCudaModuleNV) \
	Macro(vkDestroyCudaFunctionNV) \
	Macro(vkCmdCudaLaunchKernelNV)
#else
#define HUH_VULKAN_FUNCS_NV_cuda_kernel_launch(Macro)
#endif

#if defined(VK_EXT_descriptor_buffer)
#define HUH_VULKAN_FUNCS_EXT_descriptor_buffer(Macro) \
	Macro(vkGetDescriptorSetLayoutSizeEXT) \
	Macro(vkGetDescriptorSetLayoutBindingOffsetEXT) \
	Macro(vkGetDescriptorEXT) \
	Macro(vkCmdBindDescriptorBuffersEXT) \
	Macro(vkCmdSetDescriptorBufferOffsetsEXT) \
	Macro(vkCmdBindDescriptorBufferEmbeddedSamplersEXT) \
	Macro(vkGetBufferOpaqueCaptureDescriptorDataEXT) \
	Macro(vkGetImageOpaqueCaptureDescriptorDataEXT) \
	Macro(vkGetImageViewOpaqueCaptureDescriptorDataEXT) \
	Macro(vkGetSamplerOpaqueCaptureDescriptorDataEXT) \
	Macro(vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_descriptor_buffer(Macro)
#endif

#if defined(VK_NV_fragment_shading_rate_enums)
#define HUH_VULKAN_FUNCS_NV_fragment_shading_rate_enums(Macro) \
	Macro(vkCmdSetFragmentShadingRateEnumNV)
#else
#define HUH_VULKAN_FUNCS_NV_fragment_shading_rate_enums(Macro)
#endif

#if defined(VK_EXT_device_fault)
#define HUH_VULKAN_FUNCS_EXT_device_fault(Macro) \
	Macro(vkGetDeviceFaultInfoEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_device_fault(Macro)
#endif

#if defined(VK_EXT_vertex_input_dynamic_state)
#define HUH_VULKAN_FUNCS_EXT_vertex_input_dynamic_state(Macro) \
	Macro(vkCmdSetVertexInputEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_vertex_input_dynamic_state(Macro)
#endif

#if defined(VK_HUAWEI_subpass_shading)
#define HUH_VULKAN_FUNCS_HUAWEI_subpass_shading(Macro) \
	Macro(vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI) \
	Macro(vkCmdSubpassShadingHUAWEI)
#else
#define HUH_VULKAN_FUNCS_HUAWEI_subpass_shading(Macro)
#endif

#if defined(VK_HUAWEI_invocation_mask)
#define HUH_VULKAN_FUNCS_HUAWEI_invocation_mask(Macro) \
	Macro(vkCmdBindInvocationMaskHUAWEI)
#else
#define HUH_VULKAN_FUNCS_HUAWEI_invocation_mask(Macro)
#endif

#if defined(VK_NV_external_memory_rdma)
#define HUH_VULKAN_FUNCS_NV_external_memory_rdma(Macro) \
	Macro(vkGetMemoryRemoteAddressNV)
#else
#define HUH_VULKAN_FUNCS_NV_external_memory_rdma(Macro)
#endif

#if defined(VK_EXT_pipeline_properties)
#define HUH_VULKAN_FUNCS_EXT_pipeline_properties(Macro) \
	Macro(vkGetPipelinePropertiesEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_pipeline_properties(Macro)
#endif

#if defined(VK_EXT_extended_dynamic_state2)
#define HUH_VULKAN_FUNCS_EXT_extended_dynamic_state2(Macro) \
	Macro(vkCmdSetPatchControlPointsEXT) \
	Macro(vkCmdSetRasterizerDiscardEnableEXT) \
	Macro(vkCmdSetDepthBiasEnableEXT) \
	Macro(vkCmdSetLogicOpEXT) \
	Macro(vkCmdSetPrimitiveRestartEnableEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_extended_dynamic_state2(Macro)
#endif

#if defined(VK_EXT_color_write_enable)
#define HUH_VULKAN_FUNCS_EXT_color_write_enable(Macro) \
	Macro(vkCmdSetColorWriteEnableEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_color_write_enable(Macro)
#endif

#if defined(VK_EXT_multi_draw)
#define HUH_VULKAN_FUNCS_EXT_multi_draw(Macro) \
	Macro(vkCmdDrawMultiEXT) \
	Macro(vkCmdDrawMultiIndexedEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_multi_draw(Macro)
#endif

#if defined(VK_EXT_opacity_micromap)
#define HUH_VULKAN_FUNCS_EXT_opacity_micromap(Macro) \
	Macro(vkCreateMicromapEXT) \
	Macro(vkDestroyMicromapEXT) \
	Macro(vkCmdBuildMicromapsEXT) \
	Macro(vkBuildMicromapsEXT) \
	Macro(vkCopyMicromapEXT) \
	Macro(vkCopyMicromapToMemoryEXT) \
	Macro(vkCopyMemoryToMicromapEXT) \
	Macro(vkWriteMicromapsPropertiesEXT) \
	Macro(vkCmdCopyMicromapEXT) \
	Macro(vkCmdCopyMicromapToMemoryEXT) \
	Macro(vkCmdCopyMemoryToMicromapEXT) \
	Macro(vkCmdWriteMicromapsPropertiesEXT) \
	Macro(vkGetDeviceMicromapCompatibilityEXT) \
	Macro(vkGetMicromapBuildSizesEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_opacity_micromap(Macro)
#endif

#if defined(VK_HUAWEI_cluster_culling_shader)
#define HUH_VULKAN_FUNCS_HUAWEI_cluster_culling_shader(Macro) \
	Macro(vkCmdDrawClusterHUAWEI) \
	Macro(vkCmdDrawClusterIndirectHUAWEI)
#else
#define HUH_VULKAN_FUNCS_HUAWEI_cluster_culling_shader(Macro)
#endif

#if defined(VK_EXT_pageable_device_local_memory)
#define HUH_VULKAN_FUNCS_EXT_pageable_device_local_memory(Macro) \
	Macro(vkSetDeviceMemoryPriorityEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_pageable_device_local_memory(Macro)
#endif

#if defined(VK_VALVE_descriptor_set_host_mapping)
#define HUH_VULKAN_FUNCS_VALVE_descriptor_set_host_mapping(Macro) \
	Macro(vkGetDescriptorSetLayoutHostMappingInfoVALVE) \
	Macro(vkGetDescriptorSetHostMappingVALVE)
#else
#define HUH_VULKAN_FUNCS_VALVE_descriptor_set_host_mapping(Macro)
#endif

#if defined(VK_NV_copy_memory_indirect)
#define HUH_VULKAN_FUNCS_NV_copy_memory_indirect(Macro) \
	Macro(vkCmdCopyMemoryIndirectNV) \
	Macro(vkCmdCopyMemoryToImageIndirectNV)
#else
#define HUH_VULKAN_FUNCS_NV_copy_memory_indirect(Macro)
#endif

#if defined(VK_NV_memory_decompression)
#define HUH_VULKAN_FUNCS_NV_memory_decompression(Macro) \
	Macro(vkCmdDecompressMemoryNV) \
	Macro(vkCmdDecompressMemoryIndirectCountNV)
#else
#define HUH_VULKAN_FUNCS_NV_memory_decompression(Macro)
#endif

#if defined(VK_NV_device_generated_commands_compute)
#define HUH_VULKAN_FUNCS_NV_device_generated_commands_compute(Macro) \
	Macro(vkGetPipelineIndirectMemoryRequirementsNV) \
	Macro(vkCmdUpdatePipelineIndirectBufferNV) \
	Macro(vkGetPipelineIndirectDeviceAddressNV)
#else
#define HUH_VULKAN_FUNCS_NV_device_generated_commands_compute(Macro)
#endif

#if defined(VK_EXT_extended_dynamic_state3)
#define HUH_VULKAN_FUNCS_EXT_extended_dynamic_state3(Macro) \
	Macro(vkCmdSetDepthClampEnableEXT) \
	Macro(vkCmdSetPolygonModeEXT) \
	Macro(vkCmdSetRasterizationSamplesEXT) \
	Macro(vkCmdSetSampleMaskEXT) \
	Macro(vkCmdSetAlphaToCoverageEnableEXT) \
	Macro(vkCmdSetAlphaToOneEnableEXT) \
	Macro(vkCmdSetLogicOpEnableEXT) \
	Macro(vkCmdSetColorBlendEnableEXT) \
	Macro(vkCmdSetColorBlendEquationEXT) \
	Macro(vkCmdSetColorWriteMaskEXT) \
	Macro(vkCmdSetTessellationDomainOriginEXT) \
	Macro(vkCmdSetRasterizationStreamEXT) \
	Macro(vkCmdSetConservativeRasterizationModeEXT) \
	Macro(vkCmdSetExtraPrimitiveOverestimationSizeEXT) \
	Macro(vkCmdSetDepthClipEnableEXT) \
	Macro(vkCmdSetSampleLocationsEnableEXT) \
	Macro(vkCmdSetColorBlendAdvancedEXT) \
	Macro(vkCmdSetProvokingVertexModeEXT) \
	Macro(vkCmdSetLineRasterizationModeEXT) \
	Macro(vkCmdSetLineStippleEnableEXT) \
	Macro(vkCmdSetDepthClipNegativeOneToOneEXT) \
	Macro(vkCmdSetViewportWScalingEnableNV) \
	Macro(vkCmdSetViewportSwizzleNV) \
	Macro(vkCmdSetCoverageToColorEnableNV) \
	Macro(vkCmdSetCoverageToColorLocationNV) \
	Macro(vkCmdSetCoverageModulationModeNV) \
	Macro(vkCmdSetCoverageModulationTableEnableNV) \
	Macro(vkCmdSetCoverageModulationTableNV) \
	Macro(vkCmdSetShadingRateImageEnableNV) \
	Macro(vkCmdSetRepresentativeFragmentTestEnableNV) \
	Macro(vkCmdSetCoverageReductionModeNV)
#else
#define HUH_VULKAN_FUNCS_EXT_extended_dynamic_state3(Macro)
#endif

#if defined(VK_EXT_shader_module_identifier)
#define HUH_VULKAN_FUNCS_EXT_shader_module_identifier(Macro) \
	Macro(vkGetShaderModuleIdentifierEXT) \
	Macro(vkGetShaderModuleCreateInfoIdentifierEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_shader_module_identifier(Macro)
#endif

#if defined(VK_NV_optical_flow)
#define HUH_VULKAN_FUNCS_NV_optical_flow(Macro) \
	Macro(vkGetPhysicalDeviceOpticalFlowImageFormatsNV) \
	Macro(vkCreateOpticalFlowSessionNV) \
	Macro(vkDestroyOpticalFlowSessionNV) \
	Macro(vkBindOpticalFlowSessionImageNV) \
	Macro(vkCmdOpticalFlowExecuteNV)
#else
#define HUH_VULKAN_FUNCS_NV_optical_flow(Macro)
#endif

#if defined(VK_AMD_anti_lag)
#define HUH_VULKAN_FUNCS_AMD_anti_lag(Macro) \
	Macro(vkAntiLagUpdateAMD)
#else
#define HUH_VULKAN_FUNCS_AMD_anti_lag(Macro)
#endif

#if defined(VK_EXT_shader_object)
#define HUH_VULKAN_FUNCS_EXT_shader_object(Macro) \
	Macro(vkCreateShadersEXT) \
	Macro(vkDestroyShaderEXT) \
	Macro(vkGetShaderBinaryDataEXT) \
	Macro(vkCmdBindShadersEXT) \
	Macro(vkCmdSetDepthClampRangeEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_shader_object(Macro)
#endif

#if defined(VK_QCOM_tile_properties)
#define HUH_VULKAN_FUNCS_QCOM_tile_properties(Macro) \
	Macro(vkGetFramebufferTilePropertiesQCOM) \
	Macro(vkGetDynamicRenderingTilePropertiesQCOM)
#else
#define HUH_VULKAN_FUNCS_QCOM_tile_properties(Macro)
#endif

#if defined(VK_NV_cooperative_vector)
#define HUH_VULKAN_FUNCS_NV_cooperative_vector(Macro) \
	Macro(vkGetPhysicalDeviceCooperativeVectorPropertiesNV) \
	Macro(vkConvertCooperativeVectorMatrixNV) \
	Macro(vkCmdConvertCooperativeVectorMatrixNV)
#else
#define HUH_VULKAN_FUNCS_NV_cooperative_vector(Macro)
#endif

#if defined(VK_NV_low_latency2)
#define HUH_VULKAN_FUNCS_NV_low_latency2(Macro) \
	Macro(vkSetLatencySleepModeNV) \
	Macro(vkLatencySleepNV) \
	Macro(vkSetLatencyMarkerNV) \
	Macro(vkGetLatencyTimingsNV) \
	Macro(vkQueueNotifyOutOfBandNV)
#else
#define HUH_VULKAN_FUNCS_NV_low_latency2(Macro)
#endif

#if defined(VK_EXT_attachment_feedback_loop_dynamic_state)
#define HUH_VULKAN_FUNCS_EXT_attachment_feedback_loop_dynamic_state(Macro) \
	Macro(vkCmdSetAttachmentFeedbackLoopEnableEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_attachment_feedback_loop_dynamic_state(Macro)
#endif

#if defined(VK_NV_cluster_acceleration_structure)
#define HUH_VULKAN_FUNCS_NV_cluster_acceleration_structure(Macro) \
	Macro(vkGetClusterAccelerationStructureBuildSizesNV) \
	Macro(vkCmdBuildClusterAccelerationStructureIndirectNV)
#else
#define HUH_VULKAN_FUNCS_NV_cluster_acceleration_structure(Macro)
#endif

#if defined(VK_NV_partitioned_acceleration_structure)
#define HUH_VULKAN_FUNCS_NV_partitioned_acceleration_structure(Macro) \
	Macro(vkGetPartitionedAccelerationStructuresBuildSizesNV) \
	Macro(vkCmdBuildPartitionedAccelerationStructuresNV)
#else
#define HUH_VULKAN_FUNCS_NV_partitioned_acceleration_structure(Macro)
#endif

#if defined(VK_EXT_device_generated_commands)
#define HUH_VULKAN_FUNCS_EXT_device_generated_commands(Macro) \
	Macro(vkGetGeneratedCommandsMemoryRequirementsEXT) \
	Macro(vkCmdPreprocessGeneratedCommandsEXT) \
	Macro(vkCmdExecuteGeneratedCommandsEXT) \
	Macro(vkCreateIndirectCommandsLayoutEXT) \
	Macro(vkDestroyIndirectCommandsLayoutEXT) \
	Macro(vkCreateIndirectExecutionSetEXT) \
	Macro(vkDestroyIndirectExecutionSetEXT) \
	Macro(vkUpdateIndirectExecutionSetPipelineEXT) \
	Macro(vkUpdateIndirectExecutionSetShaderEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_device_generated_commands(Macro)
#endif

#if defined(VK_NV_cooperative_matrix2)
#define HUH_VULKAN_FUNCS_NV_cooperative_matrix2(Macro) \
	Macro(vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV)
#else
#define HUH_VULKAN_FUNCS_NV_cooperative_matrix2(Macro)
#endif

#if defined(VK_KHR_acceleration_structure)
#define HUH_VULKAN_FUNCS_KHR_acceleration_structure(Macro) \
	Macro(vkCreateAccelerationStructureKHR) \
	Macro(vkDestroyAccelerationStructureKHR) \
	Macro(vkCmdBuildAccelerationStructuresKHR) \
	Macro(vkCmdBuildAccelerationStructuresIndirectKHR) \
	Macro(vkBuildAccelerationStructuresKHR) \
	Macro(vkCopyAccelerationStructureKHR) \
	Macro(vkCopyAccelerationStructureToMemoryKHR) \
	Macro(vkCopyMemoryToAccelerationStructureKHR) \
	Macro(vkWriteAccelerationStructuresPropertiesKHR) \
	Macro(vkCmdCopyAccelerationStructureKHR) \
	Macro(vkCmdCopyAccelerationStructureToMemoryKHR) \
	Macro(vkCmdCopyMemoryToAccelerationStructureKHR) \
	Macro(vkGetAccelerationStructureDeviceAddressKHR) \
	Macro(vkCmdWriteAccelerationStructuresPropertiesKHR) \
	Macro(vkGetDeviceAccelerationStructureCompatibilityKHR) \
	Macro(vkGetAccelerationStructureBuildSizesKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_acceleration_structure(Macro)
#endif

#if defined(VK_KHR_ray_tracing_pipeline)
#define HUH_VULKAN_FUNCS_KHR_ray_tracing_pipeline(Macro) \
	Macro(vkCmdTraceRaysKHR) \
	Macro(vkCreateRayTracingPipelinesKHR) \
	Macro(vkGetRayTracingCaptureReplayShaderGroupHandlesKHR) \
	Macro(vkCmdTraceRaysIndirectKHR) \
	Macro(vkGetRayTracingShaderGroupStackSizeKHR) \
	Macro(vkCmdSetRayTracingPipelineStackSizeKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_ray_tracing_pipeline(Macro)
#endif

#if defined(VK_EXT_mesh_shader)
#define HUH_VULKAN_FUNCS_EXT_mesh_shader(Macro) \
	Macro(vkCmdDrawMeshTasksEXT) \
	Macro(vkCmdDrawMeshTasksIndirectEXT) \
	Macro(vkCmdDrawMeshTasksIndirectCountEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_mesh_shader(Macro)
#endif

#if defined(VK_KHR_android_surface)
#define HUH_VULKAN_FUNCS_KHR_android_surface(Macro) \
	Macro(vkCreateAndroidSurfaceKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_android_surface(Macro)
#endif

#if defined(VK_ANDROID_external_memory_android_hardware_buffer)
#define HUH_VULKAN_FUNCS_ANDROID_external_memory_android_hardware_buffer(Macro) \
	Macro(vkGetAndroidHardwareBufferPropertiesANDROID) \
	Macro(vkGetMemoryAndroidHardwareBufferANDROID)
#else
#define HUH_VULKAN_FUNCS_ANDROID_external_memory_android_hardware_buffer(Macro)
#endif

#if defined(VK_FUCHSIA_imagepipe_surface)
#define HUH_VULKAN_FUNCS_FUCHSIA_imagepipe_surface(Macro) \
	Macro(vkCreateImagePipeSurfaceFUCHSIA)
#else
#define HUH_VULKAN_FUNCS_FUCHSIA_imagepipe_surface(Macro)
#endif

#if defined(VK_FUCHSIA_external_memory)
#define HUH_VULKAN_FUNCS_FUCHSIA_external_memory(Macro) \
	Macro(vkGetMemoryZirconHandleFUCHSIA) \
	Macro(vkGetMemoryZirconHandlePropertiesFUCHSIA)
#else
#define HUH_VULKAN_FUNCS_FUCHSIA_external_memory(Macro)
#endif

#if defined(VK_FUCHSIA_external_semaphore)
#define HUH_VULKAN_FUNCS_FUCHSIA_external_semaphore(Macro) \
	Macro(vkImportSemaphoreZirconHandleFUCHSIA) \
	Macro(vkGetSemaphoreZirconHandleFUCHSIA)
#else
#define HUH_VULKAN_FUNCS_FUCHSIA_external_semaphore(Macro)
#endif

#if defined(VK_FUCHSIA_buffer_collection)
#define HUH_VULKAN_FUNCS_FUCHSIA_buffer_collection(Macro) \
	Macro(vkCreateBufferCollectionFUCHSIA) \
	Macro(vkSetBufferCollectionImageConstraintsFUCHSIA) \
	Macro(vkSetBufferCollectionBufferConstraintsFUCHSIA) \
	Macro(vkDestroyBufferCollectionFUCHSIA) \
	Macro(vkGetBufferCollectionPropertiesFUCHSIA)
#else
#define HUH_VULKAN_FUNCS_FUCHSIA_buffer_collection(Macro)
#endif

#if defined(VK_MVK_ios_surface)
#define HUH_VULKAN_FUNCS_MVK_ios_surface(Macro) \
	Macro(vkCreateIOSSurfaceMVK)
#else
#define HUH_VULKAN_FUNCS_MVK_ios_surface(Macro)
#endif

#if defined(VK_MVK_macos_surface)
#define HUH_VULKAN_FUNCS_MVK_macos_surface(Macro) \
	Macro(vkCreateMacOSSurfaceMVK)
#else
#define HUH_VULKAN_FUNCS_MVK_macos_surface(Macro)
#endif

#if defined(VK_EXT_metal_surface)
#define HUH_VULKAN_FUNCS_EXT_metal_surface(Macro) \
	Macro(vkCreateMetalSurfaceEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_metal_surface(Macro)
#endif

#if defined(VK_EXT_metal_objects)
#define HUH_VULKAN_FUNCS_EXT_metal_objects(Macro) \
	Macro(vkExportMetalObjectsEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_metal_objects(Macro)
#endif

#if defined(VK_EXT_external_memory_metal)
#define HUH_VULKAN_FUNCS_EXT_external_memory_metal(Macro) \
	Macro(vkGetMemoryMetalHandleEXT) \
	Macro(vkGetMemoryMetalHandlePropertiesEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_external_memory_metal(Macro)
#endif

#if defined(VK_NN_vi_surface)
#define HUH_VULKAN_FUNCS_NN_vi_surface(Macro) \
	Macro(vkCreateViSurfaceNN)
#else
#define HUH_VULKAN_FUNCS_NN_vi_surface(Macro)
#endif

#if defined(VK_KHR_wayland_surface)
#define HUH_VULKAN_FUNCS_KHR_wayland_surface(Macro) \
	Macro(vkCreateWaylandSurfaceKHR) \
	Macro(vkGetPhysicalDeviceWaylandPresentationSupportKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_wayland_surface(Macro)
#endif

#if defined(VK_KHR_win32_surface)
#define HUH_VULKAN_FUNCS_KHR_win32_surface(Macro) \
	Macro(vkCreateWin32SurfaceKHR) \
	Macro(vkGetPhysicalDeviceWin32PresentationSupportKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_win32_surface(Macro)
#endif

#if defined(VK_KHR_external_memory_win32)
#define HUH_VULKAN_FUNCS_KHR_external_memory_win32(Macro) \
	Macro(vkGetMemoryWin32HandleKHR) \
	Macro(vkGetMemoryWin32HandlePropertiesKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_external_memory_win32(Macro)
#endif

#if defined(VK_KHR_external_semaphore_win32)
#define HUH_VULKAN_FUNCS_KHR_external_semaphore_win32(Macro) \
	Macro(vkImportSemaphoreWin32HandleKHR) \
	Macro(vkGetSemaphoreWin32HandleKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_external_semaphore_win32(Macro)
#endif

#if defined(VK_KHR_external_fence_win32)
#define HUH_VULKAN_FUNCS_KHR_external_fence_win32(Macro) \
	Macro(vkImportFenceWin32HandleKHR) \
	Macro(vkGetFenceWin32HandleKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_external_fence_win32(Macro)
#endif

#if defined(VK_NV_external_memory_win32)
#define HUH_VULKAN_FUNCS_NV_external_memory_win32(Macro) \
	Macro(vkGetMemoryWin32HandleNV)
#else
#define HUH_VULKAN_FUNCS_NV_external_memory_win32(Macro)
#endif

#if defined(VK_EXT_full_screen_exclusive)
#define HUH_VULKAN_FUNCS_EXT_full_screen_exclusive(Macro) \
	Macro(vkGetPhysicalDeviceSurfacePresentModes2EXT) \
	Macro(vkAcquireFullScreenExclusiveModeEXT) \
	Macro(vkReleaseFullScreenExclusiveModeEXT) \
	Macro(vkGetDeviceGroupSurfacePresentModes2EXT)
#else
#define HUH_VULKAN_FUNCS_EXT_full_screen_exclusive(Macro)
#endif

#if defined(VK_NV_acquire_winrt_display)
#define HUH_VULKAN_FUNCS_NV_acquire_winrt_display(Macro) \
	Macro(vkAcquireWinrtDisplayNV) \
	Macro(vkGetWinrtDisplayNV)
#else
#define HUH_VULKAN_FUNCS_NV_acquire_winrt_display(Macro)
#endif

#if defined(VK_KHR_xcb_surface)
#define HUH_VULKAN_FUNCS_KHR_xcb_surface(Macro) \
	Macro(vkCreateXcbSurfaceKHR) \
	Macro(vkGetPhysicalDeviceXcbPresentationSupportKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_xcb_surface(Macro)
#endif

#if defined(VK_KHR_xlib_surface)
#define HUH_VULKAN_FUNCS_KHR_xlib_surface(Macro) \
	Macro(vkCreateXlibSurfaceKHR) \
	Macro(vkGetPhysicalDeviceXlibPresentationSupportKHR)
#else
#define HUH_VULKAN_FUNCS_KHR_xlib_surface(Macro)
#endif

#if defined(VK_EXT_directfb_surface)
#define HUH_VULKAN_FUNCS_EXT_directfb_surface(Macro) \
	Macro(vkCreateDirectFBSurfaceEXT) \
	Macro(vkGetPhysicalDeviceDirectFBPresentationSupportEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_directfb_surface(Macro)
#endif

#if defined(VK_EXT_acquire_xlib_display)
#define HUH_VULKAN_FUNCS_EXT_acquire_xlib_display(Macro) \
	Macro(vkAcquireXlibDisplayEXT) \
	Macro(vkGetRandROutputDisplayEXT)
#else
#define HUH_VULKAN_FUNCS_EXT_acquire_xlib_display(Macro)
#endif

#if defined(VK_GGP_stream_descriptor_surface)
#define HUH_VULKAN_FUNCS_GGP_stream_descriptor_surface(Macro) \
	Macro(vkCreateStreamDescriptorSurfaceGGP)
#else
#define HUH_VULKAN_FUNCS_GGP_stream_descriptor_surface(Macro)
#endif

#if defined(VK_QNX_screen_surface)
#define HUH_VULKAN_FUNCS_QNX_screen_surface(Macro) \
	Macro(vkCreateScreenSurfaceQNX) \
	Macro(vkGetPhysicalDeviceScreenPresentationSupportQNX)
#else
#define HUH_VULKAN_FUNCS_QNX_screen_surface(Macro)
#endif

#if defined(VK_QNX_external_memory_screen_buffer)
#define HUH_VULKAN_FUNCS_QNX_external_memory_screen_buffer(Macro) \
	Macro(vkGetScreenBufferPropertiesQNX)
#else
#define HUH_VULKAN_FUNCS_QNX_external_memory_screen_buffer(Macro)
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
    HUH_VULKAN_FUNCS_KHR_map_memory2(Macro) \
    HUH_VULKAN_FUNCS_KHR_video_encode_queue(Macro) \
    HUH_VULKAN_FUNCS_KHR_synchronization2(Macro) \
    HUH_VULKAN_FUNCS_KHR_copy_commands2(Macro) \
    HUH_VULKAN_FUNCS_KHR_ray_tracing_maintenance1(Macro) \
    HUH_VULKAN_FUNCS_KHR_maintenance4(Macro) \
    HUH_VULKAN_FUNCS_KHR_maintenance5(Macro) \
    HUH_VULKAN_FUNCS_KHR_pipeline_binary(Macro) \
    HUH_VULKAN_FUNCS_KHR_cooperative_matrix(Macro) \
    HUH_VULKAN_FUNCS_KHR_line_rasterization(Macro) \
    HUH_VULKAN_FUNCS_KHR_calibrated_timestamps(Macro) \
    HUH_VULKAN_FUNCS_KHR_maintenance6(Macro) \
    HUH_VULKAN_FUNCS_EXT_debug_report(Macro) \
    HUH_VULKAN_FUNCS_EXT_debug_marker(Macro) \
    HUH_VULKAN_FUNCS_EXT_transform_feedback(Macro) \
    HUH_VULKAN_FUNCS_NVX_binary_import(Macro) \
    HUH_VULKAN_FUNCS_NVX_image_view_handle(Macro) \
    HUH_VULKAN_FUNCS_AMD_draw_indirect_count(Macro) \
    HUH_VULKAN_FUNCS_AMD_shader_info(Macro) \
    HUH_VULKAN_FUNCS_NV_external_memory_capabilities(Macro) \
    HUH_VULKAN_FUNCS_EXT_conditional_rendering(Macro) \
    HUH_VULKAN_FUNCS_NV_clip_space_w_scaling(Macro) \
    HUH_VULKAN_FUNCS_EXT_direct_mode_display(Macro) \
    HUH_VULKAN_FUNCS_EXT_display_surface_counter(Macro) \
    HUH_VULKAN_FUNCS_EXT_display_control(Macro) \
    HUH_VULKAN_FUNCS_GOOGLE_display_timing(Macro) \
    HUH_VULKAN_FUNCS_EXT_discard_rectangles(Macro) \
    HUH_VULKAN_FUNCS_EXT_hdr_metadata(Macro) \
    HUH_VULKAN_FUNCS_EXT_debug_utils(Macro) \
    HUH_VULKAN_FUNCS_EXT_sample_locations(Macro) \
    HUH_VULKAN_FUNCS_EXT_image_drm_format_modifier(Macro) \
    HUH_VULKAN_FUNCS_EXT_validation_cache(Macro) \
    HUH_VULKAN_FUNCS_NV_shading_rate_image(Macro) \
    HUH_VULKAN_FUNCS_NV_ray_tracing(Macro) \
    HUH_VULKAN_FUNCS_EXT_external_memory_host(Macro) \
    HUH_VULKAN_FUNCS_AMD_buffer_marker(Macro) \
    HUH_VULKAN_FUNCS_EXT_calibrated_timestamps(Macro) \
    HUH_VULKAN_FUNCS_NV_mesh_shader(Macro) \
    HUH_VULKAN_FUNCS_NV_scissor_exclusive(Macro) \
    HUH_VULKAN_FUNCS_NV_device_diagnostic_checkpoints(Macro) \
    HUH_VULKAN_FUNCS_INTEL_performance_query(Macro) \
    HUH_VULKAN_FUNCS_AMD_display_native_hdr(Macro) \
    HUH_VULKAN_FUNCS_EXT_buffer_device_address(Macro) \
    HUH_VULKAN_FUNCS_EXT_tooling_info(Macro) \
    HUH_VULKAN_FUNCS_NV_cooperative_matrix(Macro) \
    HUH_VULKAN_FUNCS_NV_coverage_reduction_mode(Macro) \
    HUH_VULKAN_FUNCS_EXT_headless_surface(Macro) \
    HUH_VULKAN_FUNCS_EXT_line_rasterization(Macro) \
    HUH_VULKAN_FUNCS_EXT_host_query_reset(Macro) \
    HUH_VULKAN_FUNCS_EXT_extended_dynamic_state(Macro) \
    HUH_VULKAN_FUNCS_EXT_host_image_copy(Macro) \
    HUH_VULKAN_FUNCS_EXT_swapchain_maintenance1(Macro) \
    HUH_VULKAN_FUNCS_NV_device_generated_commands(Macro) \
    HUH_VULKAN_FUNCS_EXT_depth_bias_control(Macro) \
    HUH_VULKAN_FUNCS_EXT_acquire_drm_display(Macro) \
    HUH_VULKAN_FUNCS_EXT_PRIVATE_DATA_SPEC_VERSION(Macro) \
    HUH_VULKAN_FUNCS_NV_cuda_kernel_launch(Macro) \
    HUH_VULKAN_FUNCS_EXT_descriptor_buffer(Macro) \
    HUH_VULKAN_FUNCS_NV_fragment_shading_rate_enums(Macro) \
    HUH_VULKAN_FUNCS_EXT_device_fault(Macro) \
    HUH_VULKAN_FUNCS_EXT_vertex_input_dynamic_state(Macro) \
    HUH_VULKAN_FUNCS_HUAWEI_subpass_shading(Macro) \
    HUH_VULKAN_FUNCS_HUAWEI_invocation_mask(Macro) \
    HUH_VULKAN_FUNCS_NV_external_memory_rdma(Macro) \
    HUH_VULKAN_FUNCS_EXT_pipeline_properties(Macro) \
    HUH_VULKAN_FUNCS_EXT_extended_dynamic_state2(Macro) \
    HUH_VULKAN_FUNCS_EXT_color_write_enable(Macro) \
    HUH_VULKAN_FUNCS_EXT_multi_draw(Macro) \
    HUH_VULKAN_FUNCS_EXT_opacity_micromap(Macro) \
    HUH_VULKAN_FUNCS_HUAWEI_cluster_culling_shader(Macro) \
    HUH_VULKAN_FUNCS_EXT_pageable_device_local_memory(Macro) \
    HUH_VULKAN_FUNCS_VALVE_descriptor_set_host_mapping(Macro) \
    HUH_VULKAN_FUNCS_NV_copy_memory_indirect(Macro) \
    HUH_VULKAN_FUNCS_NV_memory_decompression(Macro) \
    HUH_VULKAN_FUNCS_NV_device_generated_commands_compute(Macro) \
    HUH_VULKAN_FUNCS_EXT_extended_dynamic_state3(Macro) \
    HUH_VULKAN_FUNCS_EXT_shader_module_identifier(Macro) \
    HUH_VULKAN_FUNCS_NV_optical_flow(Macro) \
    HUH_VULKAN_FUNCS_AMD_anti_lag(Macro) \
    HUH_VULKAN_FUNCS_EXT_shader_object(Macro) \
    HUH_VULKAN_FUNCS_QCOM_tile_properties(Macro) \
    HUH_VULKAN_FUNCS_NV_cooperative_vector(Macro) \
    HUH_VULKAN_FUNCS_NV_low_latency2(Macro) \
    HUH_VULKAN_FUNCS_EXT_attachment_feedback_loop_dynamic_state(Macro) \
    HUH_VULKAN_FUNCS_NV_cluster_acceleration_structure(Macro) \
    HUH_VULKAN_FUNCS_NV_partitioned_acceleration_structure(Macro) \
    HUH_VULKAN_FUNCS_EXT_device_generated_commands(Macro) \
    HUH_VULKAN_FUNCS_NV_cooperative_matrix2(Macro) \
    HUH_VULKAN_FUNCS_KHR_acceleration_structure(Macro) \
    HUH_VULKAN_FUNCS_KHR_ray_tracing_pipeline(Macro) \
    HUH_VULKAN_FUNCS_EXT_mesh_shader(Macro) \
    HUH_VULKAN_FUNCS_KHR_android_surface(Macro) \
    HUH_VULKAN_FUNCS_ANDROID_external_memory_android_hardware_buffer(Macro) \
    HUH_VULKAN_FUNCS_FUCHSIA_imagepipe_surface(Macro) \
    HUH_VULKAN_FUNCS_FUCHSIA_external_memory(Macro) \
    HUH_VULKAN_FUNCS_FUCHSIA_external_semaphore(Macro) \
    HUH_VULKAN_FUNCS_FUCHSIA_buffer_collection(Macro) \
    HUH_VULKAN_FUNCS_MVK_ios_surface(Macro) \
    HUH_VULKAN_FUNCS_MVK_macos_surface(Macro) \
    HUH_VULKAN_FUNCS_EXT_metal_surface(Macro) \
    HUH_VULKAN_FUNCS_EXT_metal_objects(Macro) \
    HUH_VULKAN_FUNCS_EXT_external_memory_metal(Macro) \
    HUH_VULKAN_FUNCS_NN_vi_surface(Macro) \
    HUH_VULKAN_FUNCS_KHR_wayland_surface(Macro) \
    HUH_VULKAN_FUNCS_KHR_win32_surface(Macro) \
    HUH_VULKAN_FUNCS_KHR_external_memory_win32(Macro) \
    HUH_VULKAN_FUNCS_KHR_external_semaphore_win32(Macro) \
    HUH_VULKAN_FUNCS_KHR_external_fence_win32(Macro) \
    HUH_VULKAN_FUNCS_NV_external_memory_win32(Macro) \
    HUH_VULKAN_FUNCS_EXT_full_screen_exclusive(Macro) \
    HUH_VULKAN_FUNCS_NV_acquire_winrt_display(Macro) \
    HUH_VULKAN_FUNCS_KHR_xcb_surface(Macro) \
    HUH_VULKAN_FUNCS_KHR_xlib_surface(Macro) \
    HUH_VULKAN_FUNCS_EXT_directfb_surface(Macro) \
    HUH_VULKAN_FUNCS_EXT_acquire_xlib_display(Macro) \
    HUH_VULKAN_FUNCS_GGP_stream_descriptor_surface(Macro) \
    HUH_VULKAN_FUNCS_QNX_screen_surface(Macro) \
    HUH_VULKAN_FUNCS_QNX_external_memory_screen_buffer(Macro)

#define HUH_VULKAN_ALL_FUNCS(Macro) \
    HUH_VULKAN_INSTANCE_FUNCS_1_0(Macro) \
    HUH_VULKAN_INSTANCE_FUNCS_1_1(Macro) \
    HUH_VULKAN_ALL_PROC_FUNCS(Macro)

namespace HUH {
bool HUH_VULKANLOADER_API LoadVulkan();
bool HUH_VULKANLOADER_API LoadVulkanInstance(VkInstance instance);

extern HUH_VULKANLOADER_API HUH::DynamicLibrary s_vulkan_lib;

#define CreateVulkanFunc(Type) extern HUH_VULKANLOADER_API PFN_## Type Type;
HUH_VULKAN_ALL_FUNCS(CreateVulkanFunc)
#undef CreateVulkanFunc

}// namespace HUH
