
#include <VMEffectsVulkan/VulkanWrapper/InstanceVk.h>
#include <stdexcept>
#include <GLFW/glfw3.h>
#include <iostream>
#include <VMUtils/log.hpp>

namespace vm
{
namespace fx
{
namespace vkwrapper
{
#ifndef __APPLE__
static const std::vector<const char *> validationLayers = { "VK_LAYER_LUNARG_standard_validation" };
#else
static const std::vector<const char *> validationLayers = {};
#endif

#if !defined( NDEBUG )
static constexpr bool enableValidationLayers = true;
#else
static constexpr bool enableValidationLayers = false;
#endif

static const std::vector<const char *> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

static bool CheckValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties( &layerCount, nullptr );
	std::vector<VkLayerProperties> availableLayers( layerCount );
	vkEnumerateInstanceLayerProperties( &layerCount,
										availableLayers.data() );

	for ( const char *layerName : validationLayers ) {
		bool layerFound = false;

		for ( const auto &layerProperties : availableLayers ) {
			if ( strcmp( layerName, layerProperties.layerName ) == 0 ) {
				layerFound = true;
				break;
			}
		}

		if ( layerFound == false ) {
			return false;
		}
	}

	return true;
}

static std::vector<const char *> GetRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char **glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions( &glfwExtensionCount );
	std::vector<const char *> extensions( glfwExtensions,
										  glfwExtensions + glfwExtensionCount );

	if ( enableValidationLayers ) {
		extensions.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
	}

	return extensions;
}

InstanceVk::~InstanceVk()
{
	if ( enableValidationLayers ) {
		DestroyDebugUtilsMessengerEXT( m_instance, m_callback, nullptr );
	}
	vkDestroyInstance( m_instance, nullptr );
}

std::shared_ptr<InstanceVk> InstanceVk::CreateInstance()
{
	auto pIns = new InstanceVk();
	return std::shared_ptr<InstanceVk>(pIns);
}

VkInstance InstanceVk::GetVkInstance()
{
	return m_instance;
}

VkPhysicalDevice InstanceVk::GetAPhysicalDevice()
{
	VkPhysicalDevice device = nullptr;
	for ( const auto &d : m_physicalDevices ) {
		if ( IsRequired( d ) ) {
			device = d;
			break;
		}
	}

	return device;
}

InstanceVk::InstanceVk()
{
	if ( !glfwInit() ) {
		throw std::runtime_error( "failed to init glfw" );
	}

	if ( enableValidationLayers && CheckValidationLayerSupport() == false ) {
		throw std::runtime_error( "validation layers are not available" );
	}

	VkApplicationInfo appInfo;
	VkInstanceCreateInfo instanceInfo;

	//vkCreateWin32SurfaceKHR()

	appInfo.pNext = nullptr;
	appInfo.pApplicationName = "Vulkan Demo";
	appInfo.applicationVersion = 1;
	appInfo.apiVersion = VK_MAKE_VERSION( 1, 0, 0 );
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION( 1, 0, 0 );

	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pNext = nullptr;
	instanceInfo.ppEnabledExtensionNames = nullptr;
	instanceInfo.enabledExtensionCount = 0;
	instanceInfo.pApplicationInfo = &appInfo;

	if ( enableValidationLayers ) {
		instanceInfo.enabledLayerCount = validationLayers.size();
		instanceInfo.ppEnabledLayerNames = validationLayers.data();
	} else {
		instanceInfo.enabledLayerCount = 0;
	}

	// add extensions
	auto extensions = GetRequiredExtensions();

	instanceInfo.enabledExtensionCount = static_cast<uint32_t>( extensions.size() );
	instanceInfo.ppEnabledExtensionNames = extensions.data();

	auto ret = vkCreateInstance( &instanceInfo, nullptr, &m_instance );
	if ( ret != VK_SUCCESS ) {
		throw std::runtime_error( "Cannot create vulkan instance" );
	}

	SetupDebugCallback();

	// Enumerate physical device

	uint32_t dc = 0;
	if ( vkEnumeratePhysicalDevices( m_instance, &dc, nullptr ) != VK_SUCCESS ) {
		Debug( "There is no physical device" );
	}

	Log( "There is(are) {} physical device(s)\n", dc );

	m_physicalDevices.resize( dc );
	vkEnumeratePhysicalDevices( m_instance, &dc, m_physicalDevices.data() );
}

void InstanceVk::SetupDebugCallback()
{
	if ( enableValidationLayers == false ) {
		return;
	}

	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
								 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
								 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
							 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
							 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = nullptr;  // Optional

	if ( CreateDebugUtilsMessengerEXT( m_instance, &createInfo, nullptr, &m_callback ) != VK_SUCCESS ) {
		throw std::runtime_error( "failed to set up debug callback!" );
	}
}

VkResult InstanceVk::CreateDebugUtilsMessengerEXT( VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pCallback )
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
	  vkGetInstanceProcAddr( instance,
							 "vkCreateDebugUtilsMessengerEXT" );
	if ( func != nullptr ) {
		return func( instance, pCreateInfo, pAllocator, pCallback );
	} else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void InstanceVk::DestroyDebugUtilsMessengerEXT( VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks *pAllocator )
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)
	  vkGetInstanceProcAddr( instance,
							 "vkDestroyDebugUtilsMessengerEXT" );
	if ( func != nullptr ) {
		func( instance, callback, pAllocator );
	}
}

VkBool32 InstanceVk::debugCallback( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData )
{
	//std::cerr << "validation layer: " << pCallbackData->pMessage
	//	<< std::endl;

	//if(messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	//{
	//
	//}
	//

	return VK_FALSE;
}

bool InstanceVk::IsRequired( VkPhysicalDevice device )
{
	// queue family support
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount, nullptr );
	std::vector<VkQueueFamilyProperties> queueFamilies( queueFamilyCount );
	vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount, queueFamilies.data() );

	for ( auto i = 0; i < queueFamilies.size(); i++ ) {
		const auto &que = queueFamilies[ i ];
		if ( que.queueCount > 0 && que.queueFlags & VK_QUEUE_GRAPHICS_BIT ) {
			m_queueFamilyIndex.graphicsFamiliy = i;
		}

		// query the support for surface present queue

		//VkBool32 presentSupport = false;
		//vkGetPhysicalDeviceSurfaceSupportKHR( device, i, m_surface, &presentSupport );
		//if ( que.queueCount > 0 && presentSupport )
		//{
		//	m_queueFamilyIndex.presentFamiliy = i;
		//}
	}

	if ( m_queueFamilyIndex.isComplete() == false ) {
		Debug( "QueueFamilyIndex is not completed" );
	}

	// extension check
	auto extension = CheckExtensionSupport( device );

	//bool swapChainAdquate = false;
	//
	//if ( extension )
	//{
	//	const auto swapChainSupport = querySwapChainSupport( device );
	//	swapChainAdquate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	//}

	return m_queueFamilyIndex.isComplete() && extension;
}

bool InstanceVk::CheckExtensionSupport( VkPhysicalDevice device )
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties( device, nullptr, &extensionCount, nullptr );

	std::vector<VkExtensionProperties> availableExtensions( extensionCount );
	vkEnumerateDeviceExtensionProperties( device, nullptr,
										  &extensionCount,
										  availableExtensions.data() );

	std::set<std::string> requiredExtensions( deviceExtensions.begin(), deviceExtensions.end() );

	for ( const auto &extension : availableExtensions ) {
		requiredExtensions.erase( extension.extensionName );
	}
	return requiredExtensions.empty();
}
}  // namespace vkwrapper
}  // namespace fx
}  // namespace vm
