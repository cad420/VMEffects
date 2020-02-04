
#pragma once

#include <VMEffectsVulkan/VulkanWrapper/InstanceVk.h>
#include <VMUtils/log.hpp>
#include "ContextVkImpl.hpp"

namespace vm
{
namespace fx
{

/**
 * \brief
 *
 * \Warning: Factory class should be a singleton.
 * But there is no limit to create more than one factory
 * instances from heap based on the EverythingBase which is
 * only allowed to be allocated in heap so far.
 * Actually, we can issue this by implementing a FactoryBase template
 * class so that it is able to allow to allocate instance on stack.
 */
class EngineFactoryVkImpl : public EverythingBase<IEngineFactoryVk>
{
public:
	EngineFactoryVkImpl( IRefCnt *cnt );

	void CreateDeviceAndContexts( const EngineVkDesc &engineDesc,
	                              IDevice **device,
	                              IContext **context ) override;

	ISwapChain *CreateSwapChainVk( IDevice *device, IContext *context, void *nativeWindowHandle, const SwapChainDesc &desc ) override;
private:

};


}  // namespace fx
}  // namespace vm

class VulkanEngineFactory_PluginFactory : public vm::IPluginFactory
{
	DECLARE_PLUGIN_FACTORY( "VMEffects.Engine.Vulkan" )
public:
	std::vector<std::string> Keys() const override;
	vm::IEverything * Create( const std::string &key ) override;
};

VM_REGISTER_PLUGIN_FACTORY_DECL(VulkanEngineFactory_PluginFactory)

EXPORT_PLUGIN_FACTORY( VulkanEngineFactory_PluginFactory )