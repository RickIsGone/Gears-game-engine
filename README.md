<img align="left" src="media/gears-s.png" alt="Gears Logo" width="80px"/>
<h1>Gears Engine</h1>

# Building

first of all you need to clone the repo using *git*, to do so go to the desired location you want the project to be in and paste this line in your terminal:

```bash
git clone https://github.com/RickIsGone/Gears-game-engine
```

after doing this follow the instruction for the targeted OS

<details><summary><big>WINDOWS</big></summary><p>

  **WORK IN PROGRESS**  

</details>

<details><summary><big>LINUX</big></summary><p>  

  **WORK IN PROGRESS**  

  if you dont have a c++ installer on your machine you can get it by pasting this line in the terminal:

  ```bash
  sudo apt install clang
  ```

  To build the engine you first need to install the required libs, to do so you'll have to paste these lines in the terminal:  

  ```bash
  sudo apt update
  sudo apt upgrade
  sudo apt install libglfw3-dev
  sudo apt install libtinyobjloader-dev
  sudo apt install libglm-dev
  sudo apt install libvulkan-dev
  ```  

  after installing the libs you'll need to install glslc to compile the shaders, paste this in the terminal:

  ```bash
  sudo apt install glslc
  ```

  after you are done installing the libs and glslc if you dont have cmake installed get it by pasting this line in the terminal:

  ```bash
  sudo apt install cmake
  ```

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
