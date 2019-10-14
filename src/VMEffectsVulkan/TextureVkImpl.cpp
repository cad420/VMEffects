
#include <VMEffects/EngineType.h>

// internal header

#include "TextureVkImpl.hpp"
#include "EngineTypeMappingVk.hpp"

namespace vm
{
namespace fx
{
TextureVkImpl::TextureVkImpl( IRefCnt *cnt,
							  DeviceVkImpl *device,
							  const TextureDesc &desc,
							  const TextureData &texData ) :
  TextureBase<vm::fx::ITextureVk,
			  vm::fx::DeviceVkImpl,
			  vm::fx::TextureDesc>( cnt, device, desc )
{
	VkImageCreateInfo createInfo = {};

	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	createInfo.format = ETextureFormatToVkType( desc.TextureFormat );
	createInfo.extent.width = desc.Width;
	createInfo.extent.height = ( desc.TextureDim == VM_TD_1D || desc.TextureDim == VM_TD_1D_ARRAY ) ? 1 : desc.Height;
	createInfo.extent.depth = ( desc.TextureDim == VM_TD_3D ) ? desc.Depth : 1;

	createInfo.mipLevels = desc.MipMapLevels;
	createInfo.samples = static_cast<VkSampleCountFlagBits>( 1 << ( desc.SampleCount - 1 ) );
	createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;

	if ( desc.TextureDim == VM_TD_1D_ARRAY ||
		 desc.TextureDim == VM_TD_2D_ARRAY ||
		 desc.TextureDim == VM_TD_CUBE ||
		 desc.TextureDim == VM_TD_CUBE_ARRAY ) {
		createInfo.arrayLayers = desc.ArraySize;
	} else {
		createInfo.arrayLayers = 1;
	}

	createInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	if ( desc.BindFlag & VM_BT_RENDER_TARGET ) {
		// VK_IMAGE_USAGE_TRANSFER_DST_BIT is required for vkCmdClearColorImage()
		createInfo.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	}
	if ( desc.BindFlag & VM_BT_DEPTH_STENCIL ) {
		// VK_IMAGE_USAGE_TRANSFER_DST_BIT is required for vkCmdClearDepthStencilImage()
		createInfo.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	}
	if ( desc.BindFlag & VM_BT_UNORDERED_ACCESS ) {
		createInfo.usage |= VK_IMAGE_USAGE_STORAGE_BIT;
	}
	if ( desc.BindFlag & VM_BT_SHADER_RESOURCE ) {
		createInfo.usage |= VK_IMAGE_USAGE_SAMPLED_BIT;
	}

	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	createInfo.queueFamilyIndexCount = 0;
	createInfo.pQueueFamilyIndices = nullptr;
	createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	auto &logicalDevice = device->GetLogicalDeviceVk();
	m_image = logicalDevice.CreateVkImage( createInfo, "Create Image" );

	auto memReq = logicalDevice.GetImageMemoryRequirements( m_image );

	VkMemoryPropertyFlags flags;
	if ( desc.Usage == VM_USG_STAGING )
	{
		flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
				VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
				VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
	} else {
		flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	}
}

TextureVkImpl::~TextureVkImpl()
{
}
}  // namespace fx
}  // namespace vm