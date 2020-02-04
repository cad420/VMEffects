
#include "SwapChainVkImpl.hpp"

namespace vm
{
namespace fx
{
SwapChainVkImpl::SwapChainVkImpl( IRefCnt *cnt, const SwapChainDesc &desc, DeviceVkImpl * device,
								  ContextVkImpl * context,
								  void *reservedNativeWindowHandle ) :
  EverythingBase<ISwapChainVk>( cnt),m_device( device ),m_context( context )
{

	(void)reservedNativeWindowHandle;
	// We use GLFW to create surface instead of native window now. But we need to reserve it for extension.									

	bool swapChainAdquate = false;

	const auto swapChainSupport = querySwapChainSupport( device->GetVkPhysicalDevice() );

	swapChainAdquate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();

	if ( swapChainAdquate == false ) 
	{
		throw std::runtime_error( "The device doesn't support the surface present, failed to create swap chain of" + std::string(desc.Info) );
	}

	// We use GLFW to manage the cross-platform window

	// [1] Create Surface
	if ( glfwInit() == GLFW_FALSE ) {
		throw std::runtime_error( "Failed to initialize GLFW for swap chain of " + std::string( desc.Info ) );
	}
	glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
	if ( ( m_window = glfwCreateWindow( desc.Width, desc.Height, "Vulkan", nullptr, nullptr ) ) == nullptr ) {
		throw std::runtime_error( "Failed to create glfw window for swap chain of " + std::string( desc.Info ) );
	}
	
	auto ret = glfwCreateWindowSurface( device->GetVkInstanceWrapper()->GetVkInstanceNativeHandle(), 
		m_window, nullptr, &m_surface );

	if ( ret != VK_SUCCESS ) {
		throw std::runtime_error( "can not create vulkan display surface for swap chain of "+std::string(desc.Info) );
	}
	
	// [2] Create SwapChain
	
}

ITextureView * SwapChainVkImpl::GetCurrentFramebuffer()
{
	return nullptr;
}

ITextureView * SwapChainVkImpl::GetDepthBuffer()
{
	return nullptr;
}
}  // namespace fx
}  // namespace vm
