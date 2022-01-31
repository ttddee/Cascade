# Cascade Image Editor

![Cascade](screenshots/csc-screen03.jpg)   

![Cascade](screenshots/csc-screen02.jpg) 

Cascade is a node-based image editor with GPU-acceleration.

## Features

- Non-destructive node-based editing workflow
- All image processing is done on the GPU
- 32 bit linear color pipeline
- Support for the most common color spaces and file formats

## Download

Cascade is free software. If you want to try it out, the latest binaries for Windows are [here](https://github.com/ttddee/Cascade/releases).

The release has been tested on Windows 10 and NVIDIA GPUs.

If you want to get involved or have questions, please join our [Discord](https://discord.gg/SHPHqgKtFM) or create an issue here on Github.

## Build

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
.\vcpkg\vcpkg --feature-flags="versions" install --triplet=x64-windows
```

It will take a while to compile but upon completion you should be able to open the project in Qt Creator, configure your compiler and build.

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


