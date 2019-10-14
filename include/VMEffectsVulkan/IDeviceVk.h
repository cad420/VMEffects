#pragma once

#include <VMEffects/IDevice.h>
#include <vulkan/vulkan.h>
#include <memory>

namespace vm
{
namespace fx
{
class IDeviceVk : public IDevice
{
public:
	virtual VkDevice GetVkDeviceHandle() = 0;

	virtual VkPhysicalDevice GetVkPhysicalDevice() = 0;

	
	
	virtual ITexture *CreateTextureFromVkImage( VkImage vkImage, const TextureDesc &desc ) = 0;
	
	virtual IBuffer *CreateBufferFromVkBuffer( VkBuffer vkBuffer, const BufferDesc &desc ) = 0;
};

}  // namespace fx
}  // namespace vm