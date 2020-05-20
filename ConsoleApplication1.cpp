#define VK_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR

#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>
//#include "vulkan\vulkan.h"
#include <vector>

using namespace std;

//#define VK_KHR_win32_surface 1
//#define VK_KHR_WIN32_SURFACE_SPEC_VERSION 5
//#define VK_KHR_WIN32_SURFACE_EXTENSION_NAME "VK_KHR_win32_surface"

#define ASSERT_VULKAN(val)\
    if(val != VK_SUCCESS){\
        __debugbreak();\
    }

VkInstance instance;
VkSurfaceKHR surface;
VkDevice device;
GLFWwindow* window;

void printStats(VkPhysicalDevice & device) {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, &properties);
    cout << "DeviceName:              " << properties.deviceName << endl;
    uint32_t apiVer = properties.apiVersion;
    cout << "API Version:             " << VK_VERSION_MAJOR(apiVer) << "."\
         << VK_VERSION_MINOR(apiVer) << "." << VK_VERSION_PATCH(apiVer) << endl;
    cout << "DeviceID:                " << properties.deviceID << endl;
    cout << "DeviceType:              " << properties.deviceType << endl;
    cout << "discreteQueuePriorities: " << properties.limits.discreteQueuePriorities << endl;

    cout << "DriverVersion:           " << properties.driverVersion << endl;
    cout << "VendorID:                " << properties.vendorID << endl;

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(device, &features);
    cout << "Geometry Shader:         " << features.geometryShader << endl;
    cout << "Tesselation Shader:      " << features.tessellationShader << endl;

    VkPhysicalDeviceMemoryProperties memProp;
    vkGetPhysicalDeviceMemoryProperties(device, &memProp);

    uint32_t amountOfQueueFamilies = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &amountOfQueueFamilies, nullptr);
    VkQueueFamilyProperties* familyProperties = new VkQueueFamilyProperties[amountOfQueueFamilies];
    vkGetPhysicalDeviceQueueFamilyProperties(device, &amountOfQueueFamilies, familyProperties);
     
    cout << "Amount of Queue Families: " << amountOfQueueFamilies << endl;

    for (int i = 0; i < amountOfQueueFamilies; i++) {
        cout << endl;
        cout << "Queue Family #" << i << endl;
        cout << "VK_QUEUE_GRAPHICS_BIT:         " << ((familyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) << endl;
        cout << "VK_QUEUE_COMPUTE_BIT:          " << ((familyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0) << endl;
        cout << "VK_QUEUE_TRANSFER_BIT:         " << ((familyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0) << endl;
        cout << "VK_QUEUE_SPARSE_BINDING_BIT:   " << ((familyProperties[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) != 0) << endl;
        cout << "Queue Count:                   " << familyProperties[i].queueCount << endl;
        cout << "Timestamp Valid Bits:          " << familyProperties[i].timestampValidBits << endl;
        uint32_t width = familyProperties[i].minImageTransferGranularity.width;
        uint32_t height = familyProperties[i].minImageTransferGranularity.height;
        uint32_t depth = familyProperties[i].minImageTransferGranularity.depth;
        cout << "Min Image Timestamp Granularity: " << width << "." << height << "." << depth << endl;
    }



    cout << endl;
    delete[] familyProperties;
}

void startGlfw() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);
    
    window = glfwCreateWindow(400, 300, "Vulkan tutorial", nullptr, nullptr);
}

void startVulkan() {
    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.pApplicationName = "Vulkan Tutorial";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
    appInfo.pEngineName = "Vulkan Version Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    uint32_t amountOfLayers = 0;
    vkEnumerateInstanceLayerProperties(&amountOfLayers, nullptr);
    VkLayerProperties* layers = new VkLayerProperties[amountOfLayers];
    vkEnumerateInstanceLayerProperties(&amountOfLayers, layers);

    cout << "Amount of Instance Layers:" << amountOfLayers << endl;
    for (int i = 0; i < amountOfLayers; i++) {
        cout << endl;
        cout << "Name:                   " << layers[i].layerName << endl;
        cout << "Spec Version:           " << layers[i].specVersion << endl;
        cout << "Implementation Version: " << layers[i].implementationVersion << endl;
        cout << "Description:            " << layers[i].description << endl;
        cout << endl;
    }

    uint32_t amountOfExtensions = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &amountOfExtensions, nullptr);
    VkExtensionProperties* extensions = new VkExtensionProperties[amountOfExtensions];
    vkEnumerateInstanceExtensionProperties(nullptr, &amountOfExtensions, extensions);

    cout << endl;
    cout << "Amount of Extensions: " << amountOfExtensions << endl;
    for (int i = 0; i < amountOfExtensions; i++) {
        cout << endl;
        cout << "Name: " << extensions[i].extensionName << endl;
        cout << "Spec Version" << extensions[i].specVersion << endl;
    }

    const vector<const char*> validationLayers = {
        "VK_LAYER_LUNARG_standart_validation"
    };

    uint32_t amountOfGlfwExtensions = 0;
    auto glfwExtensions = glfwGetRequiredInstanceExtensions(&amountOfGlfwExtensions);

    //const vector<const char*> usedExtensions = {
    //    "VK_KHR_surface",
    //    VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    //};

    VkInstanceCreateInfo instanceInfo = {};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pNext = nullptr;
    instanceInfo.flags = 0;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledLayerCount = validationLayers.size();
    instanceInfo.ppEnabledLayerNames = validationLayers.data();
    instanceInfo.enabledExtensionCount = amountOfGlfwExtensions;
    instanceInfo.ppEnabledExtensionNames = glfwExtensions;

    VkResult result = vkCreateInstance(&instanceInfo, nullptr, &instance);
    ASSERT_VULKAN(result);

    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_DISPLAY_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.pNext = nullptr;
    surfaceCreateInfo.flags = 0;
    surfaceCreateInfo.hinstance = nullptr;
    surfaceCreateInfo.hwnd = nullptr;

    result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
    ASSERT_VULKAN(result);

    VkSurfaceKHR surface;
    result = vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface);
    ASSERT_VULKAN(result);

    uint32_t amountOfPhysicalDevices = 0;
    result = vkEnumeratePhysicalDevices(instance, &amountOfPhysicalDevices, nullptr);
    ASSERT_VULKAN(result);

    //VkPhysicalDevice *physicalDevices = new VkPhysicalDevice[amountOfPhysicalDevices];
    vector<VkPhysicalDevice> physicalDevices;
    physicalDevices.resize(amountOfPhysicalDevices);

    result = vkEnumeratePhysicalDevices(instance, &amountOfPhysicalDevices, physicalDevices.data());
    ASSERT_VULKAN(result);

    for (int i = 0; i < amountOfPhysicalDevices; i++) {
        printStats(physicalDevices[i]);
    }

    float queuePrios[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    VkDeviceQueueCreateInfo deviceQueueCreateInfo;
    deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueCreateInfo.pNext = nullptr;
    deviceQueueCreateInfo.flags = 0;
    deviceQueueCreateInfo.queueFamilyIndex = 0;
    deviceQueueCreateInfo.queueCount = 4;
    deviceQueueCreateInfo.pQueuePriorities = queuePrios;

    VkPhysicalDeviceFeatures usedFeatures = {};

    VkDeviceCreateInfo deviceCreateInfo;
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = nullptr;
    deviceCreateInfo.flags = 0;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
    deviceCreateInfo.enabledLayerCount = 0;
    deviceCreateInfo.ppEnabledLayerNames = nullptr;
    deviceCreateInfo.enabledExtensionCount = 0;
    deviceCreateInfo.ppEnabledExtensionNames = nullptr;
    deviceCreateInfo.pEnabledFeatures = &usedFeatures;

    result = vkCreateDevice(physicalDevices[0], &deviceCreateInfo, nullptr, &device);
    ASSERT_VULKAN(result);

    VkQueue queue;
    vkGetDeviceQueue(device, 0, 0, &queue);

    delete[] layers;
    delete[] extensions;

}

void gameLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

}

void shutdownVulkan() {
    vkDeviceWaitIdle(device);
    vkDestroyDevice(device, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
}

void shutdownGlfw() {
    glfwDestroyWindow(window);

}

int main()
{   
    startGlfw();
    startVulkan();
    gameLoop();
    shutdownVulkan();
    shutdownGlfw();

    return 0;
    //23
}


