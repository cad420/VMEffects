
#include <iostream>
#include <GLFW/glfw3.h>
#include <VMUtils/ref.hpp>
#include <VMUtils/fmt.hpp>

#include <VMEffectsVulkan/IEngineFactoryVk.h>
#include <VMFoundation/pluginloader.h>

#include <VMEffects/IDevice.h>
#include <VMEffects/IContext.h>

int main()
{
	using namespace vm::fx;
	using namespace vm;

	println("program started");

	PluginLoader::LoadPlugins( "backends" );
	Ref<IEngineFactoryVk> p = PluginLoader::GetPluginLoader()->CreatePlugin<IEngineFactoryVk>( "Vulkan" );
	if ( !p ){
		println("Failed to load engine");
		return 0;
	} 
		IDevice *pDev =nullptr;
		IContext *pCtx = nullptr;
		EngineVkDesc desc;
		SwapChainDesc swapChainDesc;
		swapChainDesc.Width = 1024;
		swapChainDesc.Height = 768;
		p->CreateDeviceAndContexts( desc, &pDev, &pCtx );
		p->CreateSwapChainVk( pDev, pCtx, nullptr, swapChainDesc );

	return 0;
}