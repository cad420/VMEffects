
#pragma once

#include <VMEffectsVulkan/IDeviceVk.h>
#include <VMEffectsVulkan/IEngineFactoryVk.h>
#include <VMEffectsVulkan/VulkanWrapper/InstanceVk.h>
#include <VMEffectsVulkan/VulkanWrapper/LogicalDevickVk.h>
#include <VMEffectsVulkan/VulkanWrapper/PhysicalDeviceVk.h>
#include <DeviceBase.hpp>
#include "CommandPoolManager.hpp"

namespace vm
{
namespace fx
{
using namespace vkwrapper;
class DeviceVkImpl : public Device<IDeviceVk>
{
public:
	DeviceVkImpl( IRefCnt *cnt,
	              const EngineVkDesc &engineDesc,
	              std::shared_ptr<InstanceVkWrapper> instance,
	              std::unique_ptr<VkPhysicalDeviceWrapper> physicalDevice,
	              std::shared_ptr<VkLogicalDeviceWrapper> logicalDevice,
	              void *reservedAllocator );

	IShader *CreateShader( const ShaderDesc &desc ) override
	{
		return nullptr;
	}

	IBuffer *CreateBuffer( const BufferDesc &desc ) override
	{
		return nullptr;
	}

	ITexture *CreateTexture( const TextureDesc &desc ) override
	{
		return nullptr;
	}

	ISampler *CreateSampler( const SamplerDesc &desc ) override
	{
		return nullptr;
	}

	IPipeline *CreatePipeline( const PipelineDesc &desc ) override
	{
		return nullptr;
	}

	void AllocateTransientCommandPool( VkCommandPoolWrapper &cmdPool, VkCommandBuffer &cmdBuff, const char *dbgInfo )
	{
		
	}

	VkDevice GetVkDeviceHandle() override
	{
		return m_logicalDevice->GetVkDeviceNativeHandle();
	}

	VkPhysicalDevice GetVkPhysicalDevice() override
	{
		return m_physicalDevice->GetPhysicalDeviceNativeHandle();
	}

	std::shared_ptr<InstanceVkWrapper> GetVkInstanceWrapper()
	{
		return m_instance;
	}

	std::shared_ptr<const InstanceVkWrapper> GetVkInstanceWrapper() const
	{
		return m_instance;
	}

	const VkPhysicalDeviceWrapper &GetPhysicalDeviceVk() const
	{
		return *m_physicalDevice;
	}

	const VkLogicalDeviceWrapper &GetLogicalDeviceVk() const
	{
		return *m_logicalDevice;
	}

	ITexture *CreateTextureFromVkImage( VkImage vkImage, const TextureDesc &desc ) override
	{
		return nullptr;
	}

	IBuffer *CreateBufferFromVkBuffer( VkBuffer vkBuffer, const BufferDesc &desc ) override
	{
		return nullptr;
	}

	MemoryAllocation AllocateMemory( const VkMemoryRequirements &req, VkMemoryPropertyFlags flags )
	{
		return m_deviceMemoryManager.Allocate( req, flags );
	}

private:
	std::shared_ptr<InstanceVkWrapper> m_instance;
	std::unique_ptr<VkPhysicalDeviceWrapper> m_physicalDevice;
	std::shared_ptr<VkLogicalDeviceWrapper> m_logicalDevice;
	VulkanMemoryArena m_deviceMemoryManager;


	// used to copy buffer and texture
	CommandPoolManager m_transientCmdPoolManager;
	
};
}  // namespace fx
}  // namespace vm
