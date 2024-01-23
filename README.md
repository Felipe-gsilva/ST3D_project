# ST3D_project
## My first ever Vulkan application.
A simple (until now) usage of Vulkan API, with GLFW as window handler and old glsl for shadering.

## Compiling the code
I'm using cmake, so it must be really easy to run this application

### For linux
`cmake -S path/to/source -B path/to/build/`

Then, build it
`cmake --build path/to/build/`

Run it
`./path/to/build/ST3D_project.exe`
### For windows
First, let cmake generate the makefiles
`cmake -G "MinGW Makefiles" -S path/to/source -B path/to/build/`

Then, build it
`cmake --build path/to/build/`

Run it
`./path/to/build/ST3D_project.exe`
