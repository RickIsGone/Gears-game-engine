<img align="left" src="media/gears-s.png" alt="Gears Logo" width="80px"/>
<h1>Gears Engine</h1>

# Building

**WORK IN PROGRESS**  

## Prerequisites

* cmake  
* git  
* Visual studio (Windows)

<ins> **1. Cloning the repo:** </ins>  
Start by cloning the repo with `git clone --recursive https://github.com/RickIsGone/Gears-game-engine`

after doing this follow the instruction for the targeted OS

<details><summary><big>WINDOWS</big></summary><p>

  <ins> **2. Downloading the required packages:** </ins>  

  To build the engine you'll need to install the Vulkan SDK, which you can download [here](https://vulkan.lunarg.com/sdk/home#windows)

  <ins> **3. Compiling the project:** </ins>  

  after you are done installing the Vulkan SDK go in the engine root and make a new directory called `build`  

  open a terminal in the directory and type `cmake..`

  after you've compiled the engine with Visual studio make sure to move the `shaders` directory in the same directory as the executable or it wont work  

  congratulation! the engine is now compiled and ready to be used

</details>

<details><summary><big>LINUX</big></summary><p>  

  <ins> **2. Downloading the required packages:** </ins>  

  To build the engine you'll first need to install the required packages, to do so you'll have to paste these lines in the terminal:  

  ```bash
  sudo apt update
  sudo apt upgrade
  sudo apt install libwayland-dev libvulkan-dev libxkbcommon-dev xorg-dev
  ```  

  after installing the packages you'll need to install glslc to compile the shaders, paste this in the terminal:

  ```bash
  sudo apt install glslc
  ```

  <ins> **3. Compiling the project:** </ins>  

  after installing cmake open the terminal and travel to the root of the engine and paste this in the terminal:

  ```bash
  mkdir build
  cd build
  cmake ..
  make
  ```

  congratulation! the engine is now compiled and ready to be used

</details><p>  

# Libraries

* [GLFW](https://github.com/glfw/glfw)
* [VULKAN](https://www.lunarg.com/vulkan-sdk/)
* [GLM](https://github.com/g-truc/glm/)
* [TINYOBJLOADER](https://github.com/tinyobjloader/tinyobjloader)
