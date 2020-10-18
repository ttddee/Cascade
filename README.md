# Cascade

Cascade is a node-based image editor with GPU-acceleration.

This is still an early prototype and not ready for production use.

## Features

- Node-based non-destructive editing workflow
- All image processing is done on the GPU, except for IO
- 32 bit linear color pipeline
- Easy extensibility due to separation of processing pipeline from application logic

## Shortcuts

- **F1** - View selected node front input
- **F2** - View selected node back input
- **F3** - View selected node alpha input
- **F4** - Toggle between selected node RGB output and alpha output
- **Delete** - Delete selected node
