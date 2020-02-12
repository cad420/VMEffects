
#pragma once

#include <limits>
#include <vulkan/vulkan.h>

#include <VMEffectsVulkan/ISwapChainVk.h>
#include <SwapChainImpl.hpp>
#include <VMUtils/log.hpp>
#include <GLFW/glfw3.h>

#include "DeviceVkImpl.hpp"
#include "ContextVkImpl.hpp"

namespace vm
{
namespace fx
{
class SwapChainVkImpl : public EverythingBase<ISwapChainVk>
{
public:
	SwapChainVkImpl( IRefCnt *cnt,
					 const SwapChainDesc &desc,
					 DeviceVkImpl *device,
					 ContextVkImpl *context,
					 void *reservedNativeWindowHandle );

	void Present( uint32_t interval ) override
	{
		Warning( "Present(uint32_t interval) is not implement" );
	}
	void Resize( uint32_t w, uint32_t h ) override
	{
		Warning( "Resize( uint32_t w, uint32_t h ) is not implement" );
	}

	ITextureView *GetCurrentFramebuffer() override;

	ITextureView *GetDepthBuffer() override;

	VkSwapchainKHR GetVkSwapChain() override
	{
		return m_swapChain;
	}

private:
	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	SwapChainSupportDetails querySwapChainSupport( VkPhysicalDevice device ) const
	{
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR( device, m_surface, &details.capabilities );

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR( device, m_surface, &formatCount, nullptr );

		if ( formatCount != 0 ) {
			details.formats.resize( formatCount );
			vkGetPhysicalDeviceSurfaceFormatsKHR( device, m_surface,
												  &formatCount, details.formats.data() );
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR( device, m_surface,
												   &presentModeCount, nullptr );

		if ( presentModeCount != 0 ) {
			details.presentModes.resize( presentModeCount );
			vkGetPhysicalDeviceSurfacePresentModesKHR( device, m_surface,
													   &presentModeCount, details.presentModes.data() );
		}

		return details;
	}

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> & availableFormats){
		if(availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED){
			return {VK_FORMAT_B8G8R8A8_SNORM,VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
		}
		for(const auto & fmt:availableFormats)
		{
			if(fmt.format ==VK_FORMAT_B8G8R8A8_UNORM && fmt.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return fmt;
		}
		return availableFormats[0];
	}

	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
	{
		for(const auto & mode:availablePresentModes){
			if(mode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return mode;
			}
			else if(mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
			{
				return VK_PRESENT_MODE_IMMEDIATE_KHR;
			}
		}
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR & capabilities){
		if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}
		else
		{
			int width,height;
			glfwGetFramebufferSize(m_window,&width,&height);
			VkExtent2D extent = {static_cast<uint32_t>(width),static_cast<uint32_t>(height)};
			extent.width = (std::max)(capabilities.minImageExtent.width,(std::min)(capabilities.maxImageExtent.width,extent.width));
			extent.height= (std::max)(capabilities.minImageExtent.height,(std::min)(capabilities.maxImageExtent.height,extent.height));
			return extent;
		}
		
	}

	Ref<IDevice> m_device = nullptr;
	WeakRef<IContext> m_context = nullptr;

	GLFWwindow *m_window = nullptr;
	VkSurfaceKHR m_surface = VK_NULL_HANDLE;
	VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
	std::vector<VkImage> m_swapChainImages;
	std::shared_ptr<vkwrapper::InstanceVkWrapper> m_instance;
	uint32_t m_currentImageIndex = 0;
	uint32_t m_presentQueueFamilyIndex = -1;
	VkSurfaceFormatKHR m_surfaceFormat;
	VkPresentModeKHR m_presentMode;
	VkExtent2D m_extent;
};
}  // namespace fx
}  // namespace vm