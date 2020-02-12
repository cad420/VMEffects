
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

	// [3] We need to query if the device supports present surface.
	bool swapChainAdquate = false;

	const auto swapChainSupport = querySwapChainSupport( device->GetVkPhysicalDevice() );

	swapChainAdquate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();

	if ( swapChainAdquate == false ) 
	{
		throw std::runtime_error( "The device doesn't support the surface present, failed to create swap chain of" + std::string(desc.Info) );
	}

	m_surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	m_presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	m_extent = chooseSwapExtent(swapChainSupport.capabilities);

	VkSwapchainCreateInfoKHR createInfo={};

	// [imageCount]
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	// if minImageCount == 0, it indicates that you can set the image count as large as possible
	if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount){
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = m_surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = m_surfaceFormat.format;
	createInfo.imageColorSpace = m_surfaceFormat.colorSpace;
	createInfo.imageExtent = m_extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	const auto graphicsQueueFamilyIndex = device->GetPhysicalDeviceVk().FindQueueFamily(VK_QUEUE_GRAPHICS_BIT);

	for(uint32_t i = 0;i<device->GetPhysicalDeviceVk().GetQueueFamiliyProperties().size();i++){
		VkBool32 present = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device->GetVkPhysicalDevice(),i,m_surface,&present);
		if(present)
		{
			m_presentQueueFamilyIndex = i;
			break;
		}
	}

	uint32_t indices[] = {graphicsQueueFamilyIndex,m_presentQueueFamilyIndex};
	if(m_presentQueueFamilyIndex != graphicsQueueFamilyIndex){
		 
		 // if the present and graphics queue index are not the same.
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices=indices;
	}else{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	// Swapchain is created from logical device though physical device decide to support it.
	if(vkCreateSwapchainKHR(device->GetVkDeviceHandle(),&createInfo,nullptr,&m_swapChain) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create swap chain");
	}	

	uint32_t imgCount;
	vkGetSwapchainImagesKHR(device->GetVkDeviceHandle(),m_swapChain,&imgCount,nullptr);
	m_swapChainImages.resize(imgCount);
	vkGetSwapchainImagesKHR(device->GetVkDeviceHandle(),m_swapChain,&imgCount,m_swapChainImages.data());
	
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
