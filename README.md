# Cascade Image Editor

![Cascade](screenshots/csc-screen01.jpg)

Cascade is a node-based image editor with GPU-acceleration.

This is still an early prototype and not ready for production use.

## Download

Cascade is free software. If you want to try it out, the latest binaries are [here](https://github.com/ttddee/Cascade/releases).

If you want to get involved or have questions, please join our [Discord](https://discord.gg/SHPHqgKtFM).

## Features

- Node-based non-destructive editing workflow
- All image processing is done on the GPU
- 32 bit linear color pipeline
- Easy extensibility due to separation of processing pipeline from application logic

## Shortcuts

- **F1** - View selected node front input
- **F2** - View selected node back input
- **F3** - View selected node alpha input
- **F4** - Toggle between selected node RGB output and alpha output
- **Delete** - Delete selected node

## Build

`git clone https://github.com/ttddee/Cascade`

Dependencies:
- [Qt](https://www.qt.io/) >= 5.10
- [OpenImageIO](https://github.com/OpenImageIO/oiio)
- [OpenColorIO](https://github.com/AcademySoftwareFoundation/OpenColorIO)
- [Qt Andvanced Docking System](https://github.com/githubuser0xFFFF/Qt-Advanced-Docking-System)

If you want to run the tests you will also need:
- [GTest](https://github.com/google/googletest)

You also need to have Vulkan and a Vulkan-capable graphics driver installed. [Here](https://vulkan.gpuinfo.org/) is a list of compatible devices and driver versions.

### Linux

The easiest way is to open the project with QtCreator and build from there with QMake.

### Windows

On Windows it can be a little bit tricky to get the dependencies built. [Here](https://drive.marvao.net/d/f/585884087212880337) is a dependency package that should get you going. Unzip and place into the root of the project.

All the binaries are built with MSVC2019.
