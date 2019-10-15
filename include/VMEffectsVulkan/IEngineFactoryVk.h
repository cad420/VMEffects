
#include <VMEffects/IEngineFactory.h>
#include <VMEffects/IContext.h>
#include <VMEffects/IDevice.h>
#include <VMEffects/ISwapChain.h>

#include <VMCoreExtension/plugin.h>
#include <VMCoreExtension/plugindef.h>

namespace vm
{
namespace fx
{

struct EngineVkDesc
{
	bool EnableValidation = false;
	uint32_t ExtensionCount = 0;
	const char *const *ExtensionNames = nullptr;
	void *VkAllocator = nullptr;

	uint64_t DeviceLocalMemoryHeapSize = 16 << 20;
	uint64_t HostVisibleMemoryHeapSize = 16 << 20;
	uint64_t DeviceLocalMemoryReserveSize = 256 << 20;
	uint64_t HostVisibleMemoryReserveSize = 256 << 20;
	
};

class IEngineFactoryVk : public IEngineFactory
{
public:
	virtual void CreateDeviceAndContexts( const EngineVkDesc &engineDesc,
										  IDevice **device,
										  IContext **context ) = 0;

	virtual ISwapChain *CreateSwapChainVk( IDevice *device,
										   IContext *context,
										   void * nativeWindowHandle,
										   const SwapChainDesc &desc ) = 0;
};

}  // namespace fx
}  // namespace vm

DECLARE_PLUGIN_METADATA( vm::fx::IEngineFactoryVk,"VMEffects.Engine.Vulkan" )
