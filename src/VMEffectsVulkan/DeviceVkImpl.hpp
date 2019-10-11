
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
				  std::shared_ptr<InstanceVk> instance,
				  std::unique_ptr<PhysicalDeviceVk> physicalDevice,
				  std::shared_ptr<LogicalDeviceVk> logicalDevice,
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
		return m_logicalDevice->GetVkDevice();
	}

	VkPhysicalDevice GetVkPhysicalDevice() override
	{
		return m_physicalDevice->GetPhysicalDeviceHandle();
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
	std::shared_ptr<InstanceVk> m_instance;
	std::unique_ptr<PhysicalDeviceVk> m_physicalDevice;
	std::shared_ptr<LogicalDeviceVk> m_logicalDevice;
};
}  // namespace fx
}  // namespace vm
