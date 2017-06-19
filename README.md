# Algorithm Visualiser

Simulates various sorting algorithms graphically.

![preview](https://user-images.githubusercontent.com/4589491/27299125-8aaf4b48-552b-11e7-93f9-baf32776797c.gif)

### Implemented Sorting Algorithms

- Bubble sort
- Insertion sort
- Quick sort
- Radix sort

## Building

Download and make sure you can compile [NSEngine.](https://github.com/Nuke928/NSEngine)

1. Create a folder inside the NSEngine repo, name it something like `build`.
2. Inside, issue the following CMake command: 
	```cmake -DSTANDALONE=ON -DSTANDALONE_GAME_DIR="<path/to/this/repo> -DSTANDALONE_GAME_NAME="Algorithm Visualiser" ..```
3. If everything went okay, compile the thing with the compiler you've specified and locate AlgorithmVisualiser.exe (e.g. inside build/NSEngine/Debug)
4. Launch it: `AlgorithmVisualiser.exe --basepath <again/path/to/this/repo>`

## TODO

- Moar sorting algorithms
- Allow for smooth window resizing
- Sound effects (optional)