
#pragma once

#include <memory>
#include <vulkan/vulkan.h>
#include <VMUtils/log.hpp>
#include <stdexcept>

namespace vm
{
namespace fx
{
namespace vkwrapper
{


template<typename T>
class VkObjectWrapper;

using VkImageWrapper = VkObjectWrapper<VkImage>;
using VkImageViewWrapper = VkObjectWrapper<VkImageView>;
using VkBufferWrapper = VkObjectWrapper<VkBuffer>;
using VkBufferViewWrapper = VkObjectWrapper<VkBufferView>;
using VkDeviceMemoryWrapper = VkObjectWrapper<VkDeviceMemory>;
using VkCommandPoolWrapper = VkObjectWrapper<VkCommandPool>;
using VkFenceWrapper = VkObjectWrapper<VkFence>;

class VkLogicalDeviceWrapper : public std::enable_shared_from_this<VkLogicalDeviceWrapper>
{
public:
	static std::shared_ptr<VkLogicalDeviceWrapper> CreateLogicalDevice( VkPhysicalDevice device,
																		const VkDeviceCreateInfo &,
																		VkAllocationCallbacks *allocator );
	VkDevice GetVkDeviceNativeHandle();
	VkQueue GetQueue( uint32_t queueFamilyIndex, uint32_t queueIndex ) const;
	~VkLogicalDeviceWrapper();

	template <typename VulkanObjectType, typename CreateFuncName, typename CreateInfoType>
	VkObjectWrapper<VulkanObjectType> CreateVkObject(
	  CreateFuncName func,
	  const CreateInfoType &createInfo,
	  const char *dbgInfo )const
	{
		VulkanObjectType vkObject = VK_NULL_HANDLE;
		auto res = func( m_device, &createInfo, m_allocator, &vkObject );
		if ( res != VK_SUCCESS ) {
			Log( "Failed to create Vulkan object: {}", dbgInfo );
			throw std::runtime_error( "Failed to create Vulkan object" );
		}
		return VkObjectWrapper<VulkanObjectType>( shared_from_this(), std::move( vkObject ) );
	}

	VkImageWrapper CreateVkImage( const VkImageCreateInfo &createInfo, const char *dbgInfo ) const;
	void ReleaseVkObject( VkImageWrapper &&vkImageWrapper ) const;

	VkImageViewWrapper CreateVkImageView( const VkImageViewCreateInfo &createInfo, const char *dbgInfo ) const;
	void ReleaseVkObject( VkImageViewWrapper &&vkImageViewWrapper ) const;

	VkBufferWrapper CreateVkBuffer( const VkBufferCreateInfo &createInfo, const char *dbgInfo ) const;
	void ReleaseVkObject( VkBufferWrapper &&vkBufferWrapper ) const;

	VkBufferViewWrapper CreateVkBufferView( const VkBufferViewCreateInfo &createInfo, const char *dbgInfo ) const;
	void ReleaseVkObject( VkBufferViewWrapper &&vkBufferViewWrapper ) const;

	VkFenceWrapper CreateFence( const VkFenceCreateInfo &createInfo, const char *dbgInfo ) const;
	void ReleaseVkObject( VkFenceWrapper &&vkFencewrapper ) const;

	void ReleaseVkObject( VkCommandPoolWrapper &&vkCommandPoolWrapper ) const;

	void ReleaseVkObject( VkDeviceMemoryWrapper &&vkDeviceMemoryWrapper )const;

	VkCommandPoolWrapper CreateCommandPool( const VkCommandPoolCreateInfo &createInfo, const char *dbgInfo )const;

	VkResult ResetCommandPool( VkCommandPool cmdPool, VkCommandPoolResetFlags flags ) const;

	VkMemoryRequirements GetImageMemoryRequirements( VkImage image )const;
	VkMemoryRequirements GetBufferMemoryRRequirements( VkBuffer buffer)const;

	VkResult BindBufferMemory( VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize offset ) const;
	VkResult BindImageMemory( VkImage image, VkDeviceMemory memory, VkDeviceSize offset ) const;

	VkDeviceMemoryWrapper AllocateDeviceMemory( const VkMemoryAllocateInfo &allocInfo, const char *dbgInfo )const;
	VkCommandBuffer AllocateCommandBuffer( const VkCommandBufferAllocateInfo &allocInfo, const char *dbgInfo ) const;

	VkResult ResetFence( VkFence fence ) const;

	VkResult MapMemory( VkDeviceMemory devMemory, VkDeviceSize size, VkDeviceSize offset, VkMemoryMapFlags flags, void **data ) const;
	void UnmapMemory( VkDeviceMemory devMemory ) const;

private:
	VkLogicalDeviceWrapper( VkPhysicalDevice device, const VkDeviceCreateInfo &ci, VkAllocationCallbacks *allocator );

	VkDevice m_device = VK_NULL_HANDLE;
	VkAllocationCallbacks *m_allocator = nullptr;
	VkMemoryPropertyFlags m_memoryProperties;
};

template <typename VkObjectType>
class VkObjectWrapper : NoCopy
{
public:
	VkObjectWrapper() :
	  m_logicalDevice( nullptr ),
	  m_object( VK_NULL_HANDLE ) {}

	VkObjectWrapper( std::shared_ptr<const VkLogicalDeviceWrapper> logicalDevice,
					 VkObjectType && object):
	  m_object( object ),
	  m_logicalDevice( std::move(logicalDevice) )
	{
		object = VK_NULL_HANDLE;
	}

	operator VkObjectType()const
	{
		return m_object;
	}

	// Do not own the Vulkan object by this wrapper
	explicit VkObjectWrapper( VkObjectType object ) :
	  m_object( object ) {}

	VkObjectWrapper( VkObjectWrapper &&object ) noexcept :
	  m_logicalDevice( std::move( object.m_logicalDevice ) ),
	  m_object( object.m_object )
	{
		object.m_object = VK_NULL_HANDLE;
	}

	void Release()
	{
		if ( m_logicalDevice && m_object != VK_NULL_HANDLE ) 
		{
			// Relase Vulkan sources in logical device;
			m_logicalDevice->ReleaseVkObject( std::move( *this ) );
		}
		
		m_object = VK_NULL_HANDLE;
		m_logicalDevice = nullptr;
	}

	VkObjectWrapper &operator=( VkObjectWrapper &&object )
	{
		Release();
		m_logicalDevice = std::move( object.m_logicalDevice );
		m_object = object.m_object;
		object.m_object = VK_NULL_HANDLE;
		return *this;
	}

	~VkObjectWrapper()
	{
		Release();
	}

private:
	friend class VkLogicalDeviceWrapper;
	VkObjectType m_object;
	std::shared_ptr<const VkLogicalDeviceWrapper> m_logicalDevice;
};

using VkImageWrapper = VkObjectWrapper<VkImage>;
using VkImageViewWrapper = VkObjectWrapper<VkImageView>;
using VkBufferWrapper = VkObjectWrapper<VkBuffer>;
using VkBufferViewWrapper = VkObjectWrapper<VkBufferView>;
using VkDeviceMemoryWrapper = VkObjectWrapper<VkDeviceMemory>;
using VkCommandPoolWrapper = VkObjectWrapper<VkCommandPool>;
using VkFenceWrapper = VkObjectWrapper<VkFence>;


}  // namespace vkwrapper
}  // namespace fx
}  // namespace vm
