
#include "CommandQueueVkImpl.hpp"

namespace vm
{
namespace fx
{
CommandQueueVkImpl::CommandQueueVkImpl( IRefCnt *cnt, std::shared_ptr<VkLogicalDeviceWrapper> logicalDevice, uint32_t queueFamily ) :
  EverythingBase<vm::fx::ICommandQueueVk>( cnt ),
  m_logicalDevice( logicalDevice ),
  m_vkQueue( logicalDevice->GetQueue( queueFamily, 0 ) ),
  m_queueFamilyIndex( queueFamily ),
  m_nextFence( 1 )
{
}
//

}  // namespace fx
}  // namespace vm