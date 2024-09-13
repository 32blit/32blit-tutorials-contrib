# Loading images for PicoSystem - MicroPython

## Fetching the necessary files

First, you will need to download the two images which we need for "Ninja Thief":

* [Background image](TODO: Must be PicoSystem background, i.e. 120x120)
* [Spritesheet image](TODO: Need link to image)

You can then save these files inside a new folder called `assets` within the project directory.

## Using the convertor program

The PicoSystem only uses 4 bits per colour channel, resulting in each pixel requiring 2 bytes of memory. This means that standard images (which use 4 bytes per pixel) have to be converted into a format which the PicoSystem can display.

In order to convert images to the PicoSystem format, you can download [this Python script](https://github.com/pimoroni/picosystem/blob/main/tools/convert-image.py). For ease of use, ensure it is saved to the `assets` folder in your project directory. You can then convert the `spritesheet.png` image by running:

```
cd assets
python3 convert-image.py spritesheet.png
```

> If you get an error message saying that Python was not found, try using `python` instead of `python3`, and ensure that Python 3 is installed.

If you are using Linux or macOS, you can also use:

```
cd assets
./convert-image.py spritesheet.png
```

This will create a new file called `spritesheet.16bpp`, which is ready to be copied to the PicoSystem via `ampy`:

```
ampy -p <port> mkdir assets
ampy -p <port> put spritesheet.16bpp assets/spritesheet.16bpp
```

## Loading the images

The PicoSystem SDK uses the `Buffer` class to store pixel data, which requires the width and height of the image to be specified, along with the path to the image data.

At the moment we are only going to load our background. We know that this has the same dimensions as the screen, so we can use the screen size constants defined in `constants.py`:

```py
# We declare our global variables at the top of the file, after the import statements

# Create buffer with the correct dimensions, and load our background image data into it
background = Buffer(Constants.SCREEN_WIDTH, Constants.SCREEN_HEIGHT, "assets/background.16bpp")
```