
#include <VMEffectsVulkan/VulkanWrapper/VulkanFencePool.h>
#include <vulkan/vulkan.h>

namespace vm
{
namespace fx
{
vkwrapper::MyVkFencePool::MyVkFencePool( std::shared_ptr<const VkLogicalDeviceWrapper> logicalDevice ) :
  m_logicalDevice( std::move( logicalDevice ) )
{
}

vkwrapper::VkFenceWrapper vkwrapper::MyVkFencePool::GetFence()
{
	VkFenceWrapper fence;
	if ( m_fences.empty() == false ) 
	{
		fence = std::move( m_fences.back() );
		m_logicalDevice->ResetFence( fence );
		m_fences.pop_back();
	} else {
		VkFenceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		fence = m_logicalDevice->CreateFence( createInfo,"create fence" );
	}
	return fence;
}

void vkwrapper::MyVkFencePool::DisposeFence( VkFenceWrapper &&fence )
{
	m_fences.emplace_back( std::move( fence ) );
}
}  // namespace fx
}  // namespace vm