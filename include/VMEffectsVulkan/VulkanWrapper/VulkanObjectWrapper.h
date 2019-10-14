
#pragma once

#include <VMUtils/concepts.hpp>
#include <memory>

#include "LogicalDevickVk.h"

namespace vm
{
namespace fx
{
namespace vkwrapper
{
template <typename VkObjectType>
class VkObjectWrapper : NoCopy
{
public:
	VkObjectWrapper() :
	  m_logicalDevice( nullptr ),
	  m_object( VK_NULL_HANDLE ) {}

	VkObjectWrapper( std::shared_ptr<const VkLogicalDeviceWrapper> logicalDevice,
					 VkObjectType &&object ) :
	  m_logicalDevice( logicalDevice ),
	  m_object( object )
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
}  // namespace vkwrapper
}  // namespace fx
}  // namespace vm