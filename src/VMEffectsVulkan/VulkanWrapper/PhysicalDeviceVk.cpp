
#include <VMEffectsVulkan/VulkanWrapper/PhysicalDeviceVk.h>
#include <exception>
#include <VMUtils/log.hpp>
#include <set>

namespace vm
{
namespace fx
{
namespace vkwrapper
{

std::unique_ptr<PhysicalDeviceVk> PhysicalDeviceVk::CreatePhysicalDevice( VkPhysicalDevice device )
{
	auto pDev = new PhysicalDeviceVk( device );
	
	return std::unique_ptr<PhysicalDeviceVk>( pDev );
}

VkPhysicalDevice PhysicalDeviceVk::GetPhysicalDeviceHandle() const
{
	return m_device;
}

uint32_t PhysicalDeviceVk::FindMemoryType( uint32_t typeFilter, VkMemoryPropertyFlags properties )const
{

	for ( uint32_t i = 0; i < i < m_memoryProperties.memoryTypeCount; i++ ) 
	{
		if ( ( typeFilter & ( 1 << i ) ) && ( m_memoryProperties.memoryTypes[ i ].propertyFlags & properties ) == properties ) 
		{
			return i;
		}
	}
	
	throw std::runtime_error( "failed to find suitable memory type!" );
}

uint32_t PhysicalDeviceVk::FindQueueFamily( VkQueueFlags QueueFlags ) const
{
	for ( auto i = 0; i < m_queueFamiliyProperties.size(); i++ ) {
		const auto &que = m_queueFamiliyProperties[ i ];
		if ( que.queueCount > 0 && que.queueFlags & QueueFlags ) 
		{
			return i;
		}

		// query the support for surface present queue

		//VkBool32 presentSupport = false;
		//vkGetPhysicalDeviceSurfaceSupportKHR( m_device, i, m_surface, &presentSupport );
		//if ( que.queueCount > 0 && presentSupport ) {
		//	m_queueFamilyIndex.presentFamiliy = i;
		//}
	}
	return VK_DEVICE_QUEUE_CREATE_FLAG_BITS_MAX_ENUM;
}

PhysicalDeviceVk::PhysicalDeviceVk( VkPhysicalDevice device ):m_device( device )
{
	
	vkGetPhysicalDeviceProperties( m_device, &m_properties );
	vkGetPhysicalDeviceFeatures( m_device, &m_features );
	
	vkGetPhysicalDeviceMemoryProperties( m_device, &m_memoryProperties );
	
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties( m_device, &queueFamilyCount, nullptr );
	m_queueFamiliyProperties.resize( queueFamilyCount );
	vkGetPhysicalDeviceQueueFamilyProperties( m_device, &queueFamilyCount, m_queueFamiliyProperties.data() );
	
}


}  // namespace vkwrapper
}  // namespace fx
}  // namespace vm