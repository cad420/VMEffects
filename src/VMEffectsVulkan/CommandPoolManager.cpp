
#include "CommandPoolManager.hpp"

namespace vm
{
namespace fx
{
CommandPoolManager::CommandPoolManager( DeviceVkImpl &device, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags ) :
  m_device( device ),
  m_queueFamilyIndex( queueFamilyIndex ),
  m_flags( flags )
{

}


VkCommandPoolWrapper CommandPoolManager::AllocateCommandPool( const char *dbgInfo )
{
	std::lock_guard<std::mutex> lk( m_mtx );
	VkCommandPoolWrapper cmdPool;
	if ( m_cmdPools.empty() == false ) {
		cmdPool = std::move( m_cmdPools.front() );
		m_cmdPools.pop_front();
	}
	auto &logicalDevice = m_device.GetLogicalDeviceVk();
	if ( cmdPool == VK_NULL_HANDLE ) {
		VkCommandPoolCreateInfo createInfo = {};

		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.queueFamilyIndex = m_queueFamilyIndex;
		createInfo.flags = m_flags;

		cmdPool = logicalDevice.CreateCommandPool( createInfo, "create cmd pool" );
	}

	logicalDevice.ResetCommandPool( cmdPool, 0 );

	return std::move( cmdPool );
}

void CommandPoolManager::ReleaseCommandPool( VkCommandPoolWrapper cmdPool, uint32_t cmdQueueIndex, uint64_t fenceValue )
{
	std::lock_guard<std::mutex> lk( m_mtx );
	m_cmdPools.emplace_back( std::move( cmdPool ) );
}

void CommandPoolManager::DestroyPools()
{
	std::lock_guard<std::mutex> lk( m_mtx );
	m_cmdPools.clear();
}

}  // namespace fx
}  // namespace vm