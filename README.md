<hr>

<p align="center">
  <img src="design/logo/cascade-logo-full.png" width="500">
</p>

<h3 align="center">
  node-based image editor
</h3>

<hr>

![Cascade](screenshots/csc-screen03.jpg)   

![Cascade](screenshots/csc-screen02.jpg) 

Cascade is a node-based image editor with GPU-acceleration.

## Features

- Non-destructive node-based editing workflow
- All image processing is done on the GPU
- 32 bit linear color pipeline
- Support for the most common color spaces and file formats
- You can write your own image processing effects using GLSL. A quick introduction on how to do that is [here](https://cascadedocs.readthedocs.io/en/latest/writingshader.html)
- Or you can use existing [ISF shaders](https://cascadedocs.readthedocs.io/en/latest/isfshaders.html) to add effects to your images.

## Download

Cascade is free software. If you want to try it out, the latest binaries for Windows and a Linux AppImage are [here](https://github.com/ttddee/Cascade/releases).

If you want to get involved or have questions, please join our [Discord](https://discord.gg/SHPHqgKtFM) or create an issue here on Github.

## Windows Build

To build the project on Windows, you will have to install Qt and the Vulkan SDK manually. The rest is handled by vcpkg. 

Windows versions are compiled with MSVC 2019 64bit.

Install [Qt](https://www.qt.io/download) using the official installer. At the moment we are using version **5.15.0**, installed into `C:\Qt515`.

It's easiest to use Qt Creator as IDE, but feel free to use Visual Studio if you want.

Open a command prompt and clone the Cascade repo:

```
git clone https://github.com/ttddee/Cascade
```

Enter the project directory and install vcpkg:

``` 
cd Cascade
```
```
git clone https://github.com/microsoft/vcpkg
```
```
.\vcpkg\bootstrap-vcpkg.bat
```

Now you can install all other dependencies using the command below:

```
.\vcpkg\vcpkg --feature-flags="versions" --triplet=x64-windows  install
```

It will take a while to compile but upon completion you should be able to open the project in Qt Creator, configure your compiler and build.

## Ubuntu Build (21.10)

We are going to download the repository into the home directory.

```
cd ~
```

Clone the repository:

```
git clone https://github.com/ttddee/Cascade
```

Change to the Cascade directory and execute the `ubuntu-setup` bash script.

```
cd Cascade
```
```
chmod +x ubuntu-setup
```
```
sudo ./ubuntu-setup
```

This should install all the dependencies you need as well as QtCreator.

Now, open the project with QtCreator and configure it to use Qt5, GCC as compiler and GDB as debugger.

## Shortcuts

- **F1** - View selected node front input
- **F2** - View selected node back input
- **F3** - View selected node alpha input
- **F4** - Toggle between selected node RGB output and alpha output
- **Delete** - Delete selected node

- **Ctrl + Left Click** - Reset slider to default

## Tech

Cascade is written in C++ and runs on Windows and Linux. You will need a [Vulkan-compatible GPU](https://vulkan.gpuinfo.org/) to run the program.

The GUI was created in [Qt](https://www.qt.io/) and all rendering is done on the GPU through [Vulkan](https://www.vulkan.org/) with GLSL as shading language.

For IO and color space conversion we are using [OpenImageIO](https://github.com/OpenImageIO/oiio)/[OpenColorIO](https://opencolorio.org/).

Multithreaded computations on the CPU are done with [TBB](https://github.com/oneapi-src/oneTBB).


