
#include "DeviceVkImpl.hpp"

namespace vm
{
namespace fx
{
vm::fx::DeviceVkImpl::DeviceVkImpl( IRefCnt *cnt, 
	const EngineVkDesc &engineDesc, 
	std::shared_ptr<InstanceVkWrapper> instance, 
	std::unique_ptr<VkPhysicalDeviceWrapper> physicalDevice, 
	std::shared_ptr<VkLogicalDeviceWrapper> logicalDevice, 
	void * reservedAllocator ) :
  Device<IDeviceVk>( cnt ),
  m_instance( std::move( instance ) ),
  m_physicalDevice( std::move( physicalDevice ) ),
  m_logicalDevice( std::move( logicalDevice ) ),
  m_deviceMemoryManager( *m_logicalDevice,
						 *m_physicalDevice,
						 engineDesc.DeviceLocalMemoryHeapSize,
						 engineDesc.HostVisibleMemoryHeapSize,
						 engineDesc.DeviceLocalMemoryReserveSize,
						 engineDesc.HostVisibleMemoryReserveSize ),m_transientCmdPoolManager(  )
{
}

}  // namespace fx
}  // namespace vm
