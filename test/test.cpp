
#include <iostream>
#include <GLFW/glfw3.h>

#include <VMEffectsVulkan/IEngineFactoryVk.h>
#include <VMFoundation/pluginloader.h>

#include <VMEffects/IDevice.h>
#include <VMEffects/IContext.h>


int main()
{
	using namespace vm::fx;
	ysl::PluginLoader::LoadPlugins( "backends" );
	vm::Ref<vm::fx::IEngineFactoryVk> p = ysl::PluginLoader::GetPluginLoader()->CreatePlugin<vm::fx::IEngineFactoryVk>( "Vulkan" );
	if ( p ) {
		IDevice *pDev =nullptr;
		IContext *pCtx = nullptr;
		EngineVkDesc desc;
		p->CreateDeviceAndContexts( desc, &pDev, &pCtx );
	}
	
	return 0;
}