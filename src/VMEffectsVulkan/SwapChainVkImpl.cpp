
#include "SwapChainVkImpl.hpp"

namespace vm
{
namespace fx
{
SwapChainVkImpl::SwapChainVkImpl( IRefCnt *cnt, const SwapChainDesc &desc, DeviceVkImpl * device,
								  ContextVkImpl * context,
								  void *reservedNativeWindowHandle ) :
  SwapChain<ISwapChainVk>( cnt,device,context )
{
	bool swapChainAdquate = false;
	const auto swapChainSupport = querySwapChainSupport( device->GetVkPhysicalDevice() );
	swapChainAdquate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();

	if ( swapChainAdquate == false ) 
	{
		throw std::runtime_error( "The device doesn't support the surface present" );
	}

	// We use GLFW to manage the cross-platform window


	// [1] Create Surface
	glfwInit();
	glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
	m_window = glfwCreateWindow( desc.Width, desc.Height, "Vulkan", nullptr, nullptr );
	
	auto ret = glfwCreateWindowSurface( device->GetVkInstanceWrapper()->GetVkInstanceNativeHandle(), 
		m_window, nullptr, &m_surface );

	if ( ret != VK_SUCCESS ) {
		throw std::runtime_error( "can not create display surface" );
	}


	// [2] Create SwapChain
	

	
}
}  // namespace fx
}  // namespace vm
