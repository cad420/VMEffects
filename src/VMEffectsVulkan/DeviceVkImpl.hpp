
#pragma once

#include <VMEffectsVulkan/IDeviceVk.h>
#include <VMEffectsVulkan/VulkanWrapper/InstanceVk.h>
#include <VMEffectsVulkan/VulkanWrapper/LogicalDevickVk.h>
#include <VMEffectsVulkan/VulkanWrapper/PhysicalDeviceVk.h>
#include <DeviceBase.hpp>

namespace vm
{
namespace fx
{
using namespace vkwrapper;
class DeviceVkImpl : public Device<IDeviceVk>
{
public:
	DeviceVkImpl( IRefCnt *cnt,
				  std::shared_ptr<InstanceVkWrapper> instance,
				  std::unique_ptr<VkPhysicalDeviceWrapper> physicalDevice,
				  std::shared_ptr<VkLogicalDeviceWrapper> logicalDevice,
				  void *reservedAllocator ) :
	  Device<IDeviceVk>( cnt ),
	  m_instance( std::move( instance ) ),
	  m_physicalDevice( std::move( physicalDevice ) ),
	  m_logicalDevice( std::move( logicalDevice ) ) {}

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

	std::shared_ptr<const InstanceVkWrapper> GetVkInstanceWrapper()const
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

	
private:
	std::shared_ptr<InstanceVkWrapper> m_instance;
	std::unique_ptr<VkPhysicalDeviceWrapper> m_physicalDevice;
	std::shared_ptr<VkLogicalDeviceWrapper> m_logicalDevice;
};
}  // namespace fx
}  // namespace vm
