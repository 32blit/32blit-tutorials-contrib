# Loading images for PicoSystem - C++

## Fetching the necessary files

First, you will need to download the two images which we need for "Ninja Thief":

* [Background image](TODO: Must be PicoSystem background, i.e. 120x120)
* [Spritesheet image](TODO: Need link to image)

You can then save these files inside a new folder called `assets` within the project directory.

## Using the online convertor

The PicoSystem only uses 4 bits per colour channel, resulting in each pixel requiring 2 bytes of memory. This means that standard images (which use 4 bytes per pixel) have to be converted into a format which the PicoSystem can display.

The [online convertor](https://wiki.picosystem.com/en/tools/image-converter) provides a simple way to convert an image into C++ code, as an array of bytes, where each pair of bytes repesents one pixel. Before we use this tool, we need to create a new file to store our assets in, called `assets.hpp`. We will paste the results of our converted images into this file, one after another.

To convert an image, drag the image from your file manager onto the box on the website. Make sure that the "C++" tab is selected, and click "Copy to clipboard". You can then paste the result into `assets.hpp`, making sure to rename the variables to `asset_background` and `asset_spritesheet`. Once you've converted both images, your `assets.hpp` file should look something like this:

```cpp
#pragma once

#include "picosystem.hpp"

using namespace picosystem;

color_t asset_background[14400] = {
    0x87f8, 0x87f8, 0x87f8, 0x87f8, 0x87f8, 0x87f8, 0x87f8, 0x87f8,
    0x87f8, 0x87f8, 0x87f8, 0x87f8, 0x87f8, 0x87f8, 0x87f8, 0x87f8,
    0x87f8, 0x87f8, 0x87f8, 0x87f8, 0x87f8, 0x87f8, 0x87f8, 0x87f8,
    // A lot more lines...
};

color_t asset_spritesheet[3072] = {
    0xacf9, 0x79f6, 0xacf9, 0xacf9, 0xacf9, 0x79f6, 0xacf9, 0xacf9,
    0xacf9, 0x79f6, 0xacf9, 0xacf9, 0xacf9, 0x79f6, 0xacf9, 0xacf9,
    0xacf9, 0x79f6, 0xacf9, 0xacf9, 0xacf9, 0x79f6, 0xacf9, 0xacf9,
    // A lot more lines...
};
```

## Accessing the images in our code

In our code, we can access the assets we just converted by including the `assets.hpp` header file (add this line in `ninja_thief.hpp`):

```cpp
#include "assets.hpp"
```

We will now be able to refer to the `asset_background` and `asset_spritesheet` byte arrays in our code.

## Buffers

The PicoSystem SDK uses the `buffer_t` type to store pixel data, along with the `buffer` function to create these buffers. When creating a new buffer for an image, we need to provide the width and height of the image, along with the pixel data (which is stored in `assets.hpp`).

At the moment we are only going to load our background. We know that this has the same dimensions as the screen, so we can use the screen size constants defined in `constants.hpp`:

```cpp
buffer_t* background = buffer(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, asset_background);
```

We will put this setup code into the `init` function, but since we need to access the buffer in the `render` function, we need to declare it first, as a global variable:

```cpp
// Outside of our functions, just after the includes
buffer_t* background = nullptr;

// Perform setup here
void init() {
    // Load the background from assets.hpp
    background = buffer(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, asset_background);
}
```