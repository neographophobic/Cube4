# Cube4

This library is for the Freetronics [4x4x4 RGB LED Kit](http://www.freetronics.com.au/products/cube4-4x4x4-rgb-led-cube). It has been forked from the [original library](https://github.com/freetronics/Cube4) to add support for user defined functions, improve the documentation, and to use less memory.

## Installation
### Step 1: Download
* To get the latest released version, goto the [Releases](https://github.com/neographophobic/Cube4/releases) page, and then click the "Source Code (zip)" link for the latest release.  
![Download Releases Link](https://secretcodeninja-public.s3-ap-southeast-2.amazonaws.com/github_readme_resources/Cube4/Step1-Download_Release.png)

* To get the latest development version, click on the "Download ZIP" button on the Github page for this [library](https://github.com/neographophobic/Cube4). If you are unsure which version to get, get the one from the "Releases" page as it is the latest stable release.  
![Download Link](https://secretcodeninja-public.s3-ap-southeast-2.amazonaws.com/github_readme_resources/Cube4/Step1-Download.png)

### Step 2: Unzip 
Once the file has downloaded, browse to it on your computer and unzip it.  
![Unzip](https://secretcodeninja-public.s3-ap-southeast-2.amazonaws.com/github_readme_resources/Cube4/Step2-Unzip.png)

### Step 3: Rename
Rename the newly unzipped folder to from "Cube4-modified_library_master" to "Cube4".  
![Rename](https://secretcodeninja-public.s3-ap-southeast-2.amazonaws.com/github_readme_resources/Cube4/Step3-Rename.png)

### Step 4: Move to "Arduino" -> "libraries" folder
Browse to your "Documents" folder, and within it there should be an "Arduino" folder. Open the "Arduino" folder, and if there isn't a "libraries" folder create one. Now move the renamed folder from step 3, into the "libraries" folder.

If you are unsure as to where your "Arduino"folder is, you can find it from the Arduino IDE by going "Arduino" -> "Preferences" on a Mac, and "File" -> "Preferences" on a Windows PC, and note the "Sketchbook location".

## Simple Sketch

```
#include "Cube.h"

Cube cube;

void setup(void) {
  // Serial port options for control of the Cube using serial commands are:
  // 0: Control via the USB connector (most common).
  // 1: Control via the RXD and TXD pins on the main board.
  // -1: Don't attach any serial port to interact with the Cube.
  cube.begin(0, 115200); // Start on serial port 0 (USB) at 115200 baud
}

void loop(void) {
  cube.all(RED);
}
```

## Notes
### LED Coordinates
Each LED in the 4x4x4 grid has a location that is tracked by the X, Y, and Z values. 

With the "freetronics Cube" and URL text facing the front, the LEDs are numbered from 0 to 3 on each axis as follows:-

* X - Left to Right
* Y - Front to Back
* Z - Bottom to Top

User **ponch58** on the freetronic forums has created a PDF that shows the coordinates for each LED. It can be found at [http://forum.freetronics.com/viewtopic.php?f=32&t=5562#p13900](http://forum.freetronics.com/viewtopic.php?f=32&t=5562#p13900).

### Colours

The cube uses RGB colour values. When a colour value is required, you can provide RGB(red, green, blue), where "red", "green", and "blue" are numbers from 0 to 255, or 0x00 to 0xFF in HEX.

Additionally a number of colours are predefined, and can be used by name. These colours are listed in `color.h` and are as follows:-

* BLACK (Off)
* BLUE
* GREEN
* ORANGE
* PINK
* PURPLE
* RED
* WHITE
* YELLOW

## API
The cube can be instructed to display different patterns via the API. This can either be done via commands issued within a sketch, or if enabled, via a serial interface. Please ensure that the cube has been properly initialised with `cube.begin(options);` as shown above in the simple sketch.

These major API commands are specified below.

### Entire Cube
#### all
* Sketch: `cube.all(colour);`
* Serial: `all colour;`

This will cause all LEDs to be set to the same `colour`. 

#### shift
* Sketch: `cube.all(axis, direction);`
* Serial: `shift axis direction;`

Move the current LED patten along the specified axis in either a positive or negative direction.

* `axis` can be X, Y, or Z
* `direction` can be either + or -

### Single LED
#### set
* Sketch: `cube.set(X, Y, Z, colour);`
* Serial: `set XYZ colour;`

Set a single LED at the coordinates identified by `X`, `Y`, `Z` to the specified `colour`.

#### next
* Sketch: `cube.next(colour);`
* Serial: `next colour;`

Will set the next LED after the end point of the previous command to the provided `colour`. The next LED is determined by moving along the X axis, then the Y axis and finally the Z axis.

### Specific Plane
#### setplane
* Sketch: `cube.setplane(axis, position, colour);`
* Serial: `setplane axis position colour;`

Will set every LED on the given `axis` at the specified `position` to the provided `colour`.

* `axis` can be X, Y, or Z
* `position` can be 0, 1, 2, or 3

#### moveplane
* Sketch: `cube.moveplane(axis, fromPosition, toPosition, colour);`
* Serial: `moveplane axis fromPosition toPosition colour;`

Will move every LED on the given `axis` from the specified `fromPosition` to the `toPosition`, and then set the `fromPosition` to the provided `colour`.

* `axis` can be X, Y, or Z
* `fromPosition` and `toPosition` can be 0, 1, 2, or 3

#### copyplane
* Sketch: `cube.copyplane(axis, fromPosition, toPosition);`
* Serial: `copyplane axis fromPosition toPosition;`

Will copy every LED on the given `axis` from the specified `fromPosition` to the `toPosition`.

* `axis` can be X, Y, or Z
* `fromPosition` and `toPosition` can be 0, 1, 2, or 3

### Graphics
#### line
* Sketch: `cube.line(X1, Y1, Z1, X2, Y2, Z2, colour);`
* Serial: `line XYZ XYZ colour;`

This will draw a line from the first coordinate (`X1`, `Y1`, `Z1`) to the second coordinate (`X2`, `Y2`, `Z2`) with the provided `colour`.

#### box
* Sketch: `cube.box(X1, Y1, Z1, X2, Y2, Z2, colour, style, fill);`
* Serial: `box XYZ XYZ colour style fill;`

This will draw a box from the first coordinate (`X1`, `Y1`, `Z1`) to the second coordinate (`X2`, `Y2`, `Z2`), with the provided `colour` using the optionally specified `style` and additional `fill` colour.

* `style` is optional, and can be one of the following:-
	* `0` Solid Colour
	* `1` Walls Only
	* `2` Edges Only
	* `3` Walls Filled (requires `fill`)
	* `4` Edges Filled (requires `fill`)
* `fill` the `colour` to use for the two styles that require fill colours.

#### sphere
* Sketch: `cube.sphere(X, Y, Z, size, colour, fill);`
* Serial: `sphere XYZ size colour fill;`

Will attempt to draw a sphere, with the centre located at `X`, `Y`, `Z` with a diameter of `size` in `colour`. If the optional `fill` colour is provided, then the inner parts of the sphere will have that colour.

* `size` is the diameter of the sphere. Valid sizes are 1 to 4.
* `fill` the optional `colour` to for the inner parts of the cube.

### Other
#### hasReceivedSerialCommand
* Sketch: `cube.hasReceivedSerialCommand();`

Returns `true` if the cube has received a command via the serial interface, otherwise returns `false`.

#### help
* Serial: `help;`

Prints a guide to using most of the above commands to the serial console.

## User Defined Functions for use via Serial Interface
The serial interface has had a `user` command added to it to allow user specified functions to be executed. This means that multiple animations could be stored within the sketch, and a specific one executed on a command via the serial interface.

> When programming animations that include the `delay();` function, the cube will not respond to new serial instructions until after the delay is finished. The included example `UserDefinedFunction.ino` and associated classes have been programmed in a way to avoid this using a concept called a state machine.

* Serial: `user # colour;`

This tells the cube to run the user defined function that corresponds to `#` (identified by an integer) with the optional `colour`.

### `cube.setDelegate(function)`
To utilise the user defined function support, you need to create a function that responds to the user input, and then tell the cube which function should be called when the user asks for a user defined function via the serial interface.

In the `setup` part of the sketch, after `cube.begin();` make a call to `cube.setDelegate(function);` where `function` is the name of a function that you have defined within the sketch. In this example we will use `userFunctionHandler`.

### `void userFunctionHandler(int itemID, rgb_t selectedColour)`
This is the function that the library calls. It parses the `user` command and gets the itemID, and colour if provided.

You should store both of these in global variables that can be accessed from the run `loop` of the sketch.

### inUserMode
Sketch: `cube.inUserMode();`

Returns `true` if the last received command from the serial interface was `user # colour;`.

## Examples
Various example sketches are included within this library. The can be found in the `examples` directory, or from within the Arduino IDE at "File" -> "Examples" -> "Cube4".

Additionally members of the community occasionally post sketches to the [Freetronics 4x4x4 RGB Cube Forum](http://forum.freetronics.com/viewforum.php?f=32).
