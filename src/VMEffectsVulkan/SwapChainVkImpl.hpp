
#pragma once

#include <vulkan/vulkan.h>

#include <VMEffectsVulkan/ISwapChinaVk.h>
#include <SwapChainImpl.hpp>
#include <VMUtils/log.hpp>
#include <GLFW/glfw3.h>

#include "DeviceVkImpl.hpp"
#include "ContextVkImpl.hpp"

namespace vm
{
namespace fx
{
class SwapChainVkImpl : public SwapChain<ISwapChainVk>
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

	GLFWwindow *m_window = nullptr;

	VkSurfaceKHR m_surface = VK_NULL_HANDLE;

	VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;

	std::shared_ptr<vkwrapper::InstanceVkWrapper> m_instance;

	uint32_t m_currentImageIndex = 0;
};
}  // namespace fx
}  // namespace vm