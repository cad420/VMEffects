
#pragma once

#include <VMEffectsVulkan/IBufferVk.h>
#include <VMUtils/ieverything.hpp>
#include <VMEffectsVulkan/VulkanWrapper/LogicalDevickVk.h>
#include <VMEffectsVulkan/VulkanWrapper/VulkanObjectWrapper.h>
#include <VMat/numeric.h>

#include "DeviceVkImpl.hpp"

namespace vm
{
namespace fx
{
using namespace vkwrapper;
class BufferVkImpl : public EverythingBase<IBufferVk>
{
public:
	BufferVkImpl( IRefCnt *cnt,
				  DeviceVkImpl *device,
				  const BufferDesc &desc,
				  const BufferData &bufferData ) :
	  EverythingBase<vm::fx::IBufferVk>(cnt),m_device(device),m_desc(desc)
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

		createInfo.size = desc.BufferSize;

		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;	// 
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;


		// Create Buffer
		m_buffer =  logicalDevice.CreateVkBuffer(createInfo,m_desc.Info);

		// Query Memory Requirements
		VkMemoryRequirements req = logicalDevice.GetBufferMemoryRRequirements(m_buffer);
		// Containts size and offset 


		// We need to decide the memory type used by the buffer here
		VkMemoryPropertyFlags flags = 0;
		if(m_desc.Usage == VM_USG_STAGING){
			// host visible is required if it is a staging buffer
			flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			// Coherent bit is important
		}else{
			flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		}

		// Memory Allocation
		m_bufferMemory = m_device->AllocateMemory(req,flags);
		const auto VkMemoryHandle = m_bufferMemory.GetHeap()->GetGPUMemory();
		const auto alignedOffset = vm::Align(VkDeviceSize(m_bufferMemory.Offset),req.alignment); 
		
		assert(m_bufferMemory.Size < alignedOffset - m_bufferMemory.Offset + req.size);
		// Bind Memory
		logicalDevice.BindBufferMemory(m_buffer,VkMemoryHandle,alignedOffset);

		// Initialization


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
	DeviceVkImpl * m_device;
	BufferDesc m_desc;
	MemoryAllocation m_bufferMemory;
};
}  // namespace fx
}  // namespace vm
