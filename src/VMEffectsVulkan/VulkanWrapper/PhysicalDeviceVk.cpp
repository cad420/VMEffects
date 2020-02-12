
#include <VMEffectsVulkan/VulkanWrapper/PhysicalDeviceVk.h>
#include <VMUtils/log.hpp>
#include <set>

namespace vm
{
namespace fx
{
namespace vkwrapper
{

std::unique_ptr<VkPhysicalDeviceWrapper> VkPhysicalDeviceWrapper::CreatePhysicalDevice( VkPhysicalDevice device )
{
	const auto pDev = new VkPhysicalDeviceWrapper( device );
	
	return std::unique_ptr<VkPhysicalDeviceWrapper>( pDev );
}

VkPhysicalDevice VkPhysicalDeviceWrapper::GetPhysicalDeviceNativeHandle() const
{
	return m_device;
}

uint32_t VkPhysicalDeviceWrapper::FindMemoryType( uint32_t typeFilter, VkMemoryPropertyFlags properties )const
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

uint32_t VkPhysicalDeviceWrapper::FindQueueFamily( VkQueueFlags QueueFlags ) const
{
	for ( auto i = 0; i < m_queueFamiliyProperties.size(); i++ ) {
		const auto &que = m_queueFamiliyProperties[ i ];
		if ( que.queueCount > 0 && que.queueFlags & QueueFlags ) 
		{
			return i;
		}
	}
	return VK_DEVICE_QUEUE_CREATE_FLAG_BITS_MAX_ENUM;
}


VkPhysicalDeviceWrapper::VkPhysicalDeviceWrapper( VkPhysicalDevice device ):m_device( device )
{
	
	vkGetPhysicalDeviceProperties( m_device, &m_properties );
	vkGetPhysicalDeviceFeatures( m_device, &m_features );
	vkGetPhysicalDeviceMemoryProperties( m_device, &m_memoryProperties );
	
	//
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties( m_device, &queueFamilyCount, nullptr );
	m_queueFamiliyProperties.resize( queueFamilyCount );
	vkGetPhysicalDeviceQueueFamilyProperties( m_device, &queueFamilyCount, m_queueFamiliyProperties.data() );

	// Get Extensions Support
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(m_device,nullptr,&extensionCount,nullptr);
	if(extensionCount > 0){
		m_extensions.resize(extensionCount);
		const auto ret = vkEnumerateDeviceExtensionProperties(m_device,nullptr,&extensionCount,m_extensions.data());
		if(ret != VK_SUCCESS){
			throw std::runtime_error("This physical device does not support any extension.");
		}
	}
	
}


}  // namespace vkwrapper
}  // namespace fx
}  // namespace vm
