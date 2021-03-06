
#include "EngineFactoryVkImpl.hpp"
//#include <VMEffectsVulkan/IEngineFactoryVk.h>
#include "VMUtils/vmnew.hpp"

namespace vm
{
namespace fx
{
static const std::vector<const char *> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

EngineFactoryVkImpl::EngineFactoryVkImpl( IRefCnt *cnt ) :
  EverythingBase<vm::fx::IEngineFactoryVk>( cnt )
{
}

void EngineFactoryVkImpl::CreateDeviceAndContexts( const EngineVkDesc &engineDesc,
												   IDevice ** device,
												   IContext ** context )
{
	using namespace vkwrapper;

	auto instance = InstanceVkWrapper::CreateInstance();

	// Create VkPhysicalDevice
	const auto physicalDeviceHandle = instance->GetAPhysicalDevice();
	auto physicalDeviceVk = VkPhysicalDeviceWrapper::CreatePhysicalDevice( physicalDeviceHandle );

	// Create Logical Device
	std::set<uint32_t> queueFamilies = { physicalDeviceVk->FindQueueFamily( VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT ) };

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

	for ( const auto &index : queueFamilies ) 
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = index;
		queueCreateInfo.queueCount = 1;
		float quePriority = 1.0f;
		queueCreateInfo.pQueuePriorities = &quePriority;
		queueCreateInfos.push_back( queueCreateInfo );
	}

	// Setup device features
	VkPhysicalDeviceFeatures supportFeatures;
	VkPhysicalDeviceFeatures requiredFeatures = {};
	vkGetPhysicalDeviceFeatures( physicalDeviceHandle, &supportFeatures );
	requiredFeatures.multiDrawIndirect = supportFeatures.multiDrawIndirect;
	requiredFeatures.tessellationShader = VK_TRUE;
	//requiredFeatures.geometryShader = VK_TRUE;

	const VkDeviceCreateInfo deviceCreateInfo = {
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		nullptr,
		0,
		static_cast<uint32_t>( queueCreateInfos.size() ),
		( queueCreateInfos.data() ),
		0,
		nullptr,
		static_cast<uint32_t>( deviceExtensions.size() ),
		( deviceExtensions.data() ),
		&requiredFeatures
	};

	auto logicalDeviceVk = VkLogicalDeviceWrapper::CreateLogicalDevice( physicalDeviceHandle, deviceCreateInfo, nullptr );

	auto pDev = VM_NEW<DeviceVkImpl>( engineDesc,
									  instance,
									  std::move( physicalDeviceVk ),
									  logicalDeviceVk, nullptr );
	auto pCtx = VM_NEW<ContextVkImpl>( pDev, engineDesc );

	*device = pDev;
	*context = pCtx;
}

ISwapChain *EngineFactoryVkImpl::CreateSwapChainVk( IDevice *device,
													IContext *context,
													void *nativeWindowHandle,
													const SwapChainDesc &desc )
{
	return nullptr;
}



}  // namespace fx
}  // namespace vm



std::vector<std::string> VulkanEngineFactory_PluginFactory::Keys() const
{
	return { "Vulkan" };
}

::vm::IEverything *VulkanEngineFactory_PluginFactory::Create( const std::string &key )
{
	return VM_NEW<vm::fx::EngineFactoryVkImpl>();
}

VM_REGISTER_PLUGIN_FACTORY_IMPL(VulkanEngineFactory_PluginFactory)

EXPORT_PLUGIN_FACTORY_IMPLEMENT( VulkanEngineFactory_PluginFactory )