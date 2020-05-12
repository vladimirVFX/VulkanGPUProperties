#include <iostream>
#include "vulkan\vulkan.h"

using namespace std;

#define ASSERT_VULKAN(val)\
    if(val != VK_SUCCESS){\
        __debugbreak();\
    }

VkInstance instance;

void printStats(VkPhysicalDevice & device) {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, &properties);
    cout << "DeviceName:         " << properties.deviceName << endl;
    uint32_t apiVer = properties.apiVersion;
    cout << "API Version:        " << VK_VERSION_MAJOR(apiVer) << "."\
         << VK_VERSION_MINOR(apiVer) << "." << VK_VERSION_PATCH(apiVer) << endl;
    cout << "DeviceID:           " << properties.deviceID << endl;
    cout << "DeviceType:         " << properties.deviceType << endl;
    cout << "DriverVersion:      " << properties.driverVersion << endl;
    cout << "VendorID:           " << properties.vendorID << endl;

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(device, &features);
    cout << "Geometry Shader:    " << features.geometryShader << endl;
    cout << "Tesselation Shader: " << features.tessellationShader << endl;

    VkPhysicalDeviceMemoryProperties memProp;
    vkGetPhysicalDeviceMemoryProperties(device, &memProp);

    uint32_t amountOfQueueFamilies = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &amountOfQueueFamilies, NULL);
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

int main()
{   
    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = NULL;
    appInfo.pApplicationName = "Vulkan Tutorial";
    appInfo.applicationVersion = VK_MAKE_VERSION(0,0,0);
    appInfo.pEngineName = "Vulkan Version Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo instanceInfo = {};
    
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pNext = NULL;
    instanceInfo.flags = 0;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledLayerCount = 0;
    instanceInfo.ppEnabledLayerNames = NULL;
    instanceInfo.enabledExtensionCount = 0;

    VkResult result = vkCreateInstance(&instanceInfo, NULL, &instance);

    ASSERT_VULKAN(result);

    uint32_t amountOfPhysicalDevices = 0;
    result = vkEnumeratePhysicalDevices(instance, &amountOfPhysicalDevices, NULL);
    ASSERT_VULKAN(result);

    VkPhysicalDevice *physicalDevices = new VkPhysicalDevice[amountOfPhysicalDevices];
    result = vkEnumeratePhysicalDevices(instance, &amountOfPhysicalDevices, physicalDevices);
    ASSERT_VULKAN(result);
     
    for (int i = 0; i < amountOfPhysicalDevices; i++) {
        printStats(physicalDevices[i]);
    }

    return 0; 
}


