## App struct
  This holds info on glfw window pointer and on the vulkan instance "object" itself

## Window handler
  Glfw functions, such as `glfwInit`, `glfwWindowHint` and `glfwCreateWindow` (which returns a pointer to the window created).

## Instance
  Instances are vulkan subsystems which link the Vulkan libs and the application. Vulkan does not have a global state, so all tracked state must be stored somewhere. This place is the `vkInstance` object. Each instance will be soon "inserted" with a bunch of logical devices depending on each function you do implement. The `VkInstanceCreateInfo` function creates a bunch of info inside this object (being treated as a struct in this C code) which describes the vulkan instance with .sType(type of struct), with .pNext(point to next) and so on.

## Validation Layers (Optional for basic codes)
Validation layers are the debug layer you, dev, can add to the application
  -Checking the values of parameters against the specification to detect misuse
  -Tracking creation and destruction of objects to find resource leaks
  -Checking thread safety by tracking the threads that calls originate from
  -Logging every call and its parameters to the standard output
  -Tracing Vulkan calls for profiling and replaying

## Physical devices and queues families
  VkPhysical Devices can handle the count and the physical device itself 
  It is important to get all gpu properties and features with functions like `vkGetPhysicalDeviceFeatures` or `vkGetPhysicalDeviceProperties`

## Logical devices
  

## Queues Families
  There are different types of queues that originate from different queue families and each family of queues allows only a subset of commands. For example, there could be a queue family that only allows processing of compute commands or one that only allows memory transfer related commands.

## Queues
