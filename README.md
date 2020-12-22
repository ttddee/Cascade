# Cascade Image Editor

[![Build Status](https://jenkins.marvao.net/buildStatus/icon?job=Cascade&subject=Linux%20build)](https://jenkins.marvao.net/view/job/Cascade) [![Build Status](https://jenkins.marvao.net/buildStatus/icon?job=CascadeWin&subject=Windows%20build)](https://jenkins.marvao.net/view/job/CascadeWin) [![Build Status](https://jenkins.marvao.net/buildStatus/icon?job=CascadeTests&subject=Tests)](https://jenkins.marvao.net/view/job/CascadeTests)

![Cascade](screenshots/csc-screen01.jpg)   

Cascade is a node-based image editor with GPU-acceleration.

This is still an early prototype.

## Download

Cascade is free software. If you want to try it out, the latest binaries for Windows are [here](https://github.com/ttddee/Cascade/releases).

If you want to get involved or have questions, please join our [Discord](https://discord.gg/SHPHqgKtFM).

## Features

- Non-destructive node-based editing workflow
- All image processing is done on the GPU
- 32 bit linear color pipeline
- Support for the most common color spaces and file formats

## Shortcuts

- **F1** - View selected node front input
- **F2** - View selected node back input
- **F3** - View selected node alpha input
- **F4** - Toggle between selected node RGB output and alpha output
- **Delete** - Delete selected node

- **Ctrl + Left Click** - Reset slider to default

## Build

## Linux

#### Clone the project

``` bash
git clone https://github.com/ttddee/Cascade
```

#### Install dependencies

Install the following with your package manager of choice:

- **Qt** >= 5.10
- **OpenImageIO**
- **Gmic**
- **CImg**
- **Vulkan Headers**
- **GTest**
- **Intel TBB**

The easiest is building the project in QtCreator. Open the project file **Cascade.pro** and you should be good to go.


