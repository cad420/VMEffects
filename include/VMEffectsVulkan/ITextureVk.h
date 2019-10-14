
#pragma once
#include <VMEffects/ITexture.h>

#include <vulkan/vulkan.h>
namespace vm
{
namespace fx
{
class ITextureVk : public ITexture
{
public:
	virtual VkImage GetVkImage() const = 0;

	virtual void SetImageLayout( VkImageLayout layout ) = 0;

	virtual VkImageLayout GetImageLayout() = 0;
	
};
}  // namespace fx
}  // namespace vm