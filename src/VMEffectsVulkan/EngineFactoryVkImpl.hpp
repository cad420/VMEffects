
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


class VulkanEngineFactory_PluginFactory : public ysl::IPluginFactory
{
	DECLARE_PLUGIN_FACTORY( "VMEffects.Engine.Vulkan" )
public:
	std::vector<std::string> Keys() const override;
	::vm::IEverything * Create( const std::string &key ) override;
};

}  // namespace fx
}  // namespace vm

EXPORT_PLUGIN_FACTORY( vm::fx::VulkanEngineFactory_PluginFactory )