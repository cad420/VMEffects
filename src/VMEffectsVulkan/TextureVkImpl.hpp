
#pragma once

#include <TextureBase.hpp>
#include <VMEffectsVulkan/ITextureVk.h>
#include <VMEffectsVulkan/VulkanWrapper/VulkanObjectWrapper.h>

#include "DeviceVkImpl.hpp"

namespace vm
{
namespace fx
{
class TextureVkImpl : public TextureBase<ITextureVk, DeviceVkImpl, TextureDesc>
{
public:
	TextureVkImpl( IRefCnt *cnt,
	               DeviceVkImpl *device,
	               const TextureDesc &desc,
	               const TextureData &texData );

	ITextureView *CreateTextureView( const TextureViewDesc &desc ) override final
	{
		return nullptr;
	}

	void * GetNativeHandle() override final
	{
		return (m_image);
	}

	VkImage GetVkImage() const override final
	{
		return m_image;
	}

	void SetImageLayout( VkImageLayout layout ) override final
	{
	}

	VkImageLayout GetImageLayout() override final
	{
		return VkImageLayout{};
	}

	~TextureVkImpl();

private:
	VkImageWrapper m_image;
	VkBufferWrapper m_stagingBuffer;
	VkDeviceSize m_size;
};

}  // namespace fx
}  // namespace vm
