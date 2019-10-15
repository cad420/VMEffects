
#pragma once

#include <VMEffectsVulkan/IBufferVk.h>
#include <BufferBase.hpp>
#include <VMEffectsVulkan/VulkanWrapper/LogicalDevickVk.h>
#include <VMEffectsVulkan/VulkanWrapper/VulkanObjectWrapper.h>

#include "DeviceVkImpl.hpp"

namespace vm
{
namespace fx
{
using namespace vkwrapper;
class BufferVkImpl : public BufferBase<IBufferVk, DeviceVkImpl, BufferDesc>
{
public:
	BufferVkImpl( IRefCnt *cnt,
				  DeviceVkImpl *device,
				  const BufferDesc &desc,
				  const BufferData &bufferData ) :
	  BufferBase<vm::fx::IBufferVk,
				 vm::fx::DeviceVkImpl,
				 vm::fx::BufferDesc>( cnt, device, desc )
	{
		const auto &logicalDevice = device->GetLogicalDeviceVk();

		VkBufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		if ( desc.BindFlag & VM_BT_UNORDERED_ACCESS ) {
			createInfo.usage |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
		}
		if ( desc.BindFlag & VM_BT_SHADER_RESOURCE ) {
			createInfo.usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
		}

		if ( desc.BindFlag & VM_BT_VERTEX_BUFFER )
			createInfo.usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		if ( desc.BindFlag & VM_BT_INDEX_BUFFER )
			createInfo.usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		if ( desc.BindFlag & VM_BT_INDIRECT_DRAW_ARGS )
			createInfo.usage |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
		if ( desc.BindFlag & VM_BT_UNIFORM_BUFFER ) {
			
			createInfo.usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		}

		if ( desc.Usage = VM_USG_DYNAMIC ) {
			
		}
		
	}

	IBufferView *CreateBufferView( const BufferViewDesc &desc ) override
	{
		return nullptr;
	}

	~BufferVkImpl() {}

	VkBuffer GetVkBuffer() const override
	{
		return m_buffer;
	}

	void *GetNativeHandle() override
	{
		return (void *)GetVkBuffer();
	}

private:
	VkBufferWrapper m_buffer;
	MemoryAllocation m_BufferMemoryInfo;
};
}  // namespace fx
}  // namespace vm
