# Cube

This library is for the Freetronics [4x4x4 RGB LED Kit](http://www.freetronics.com.au/products/cube4-4x4x4-rgb-led-cube).

To install the library, click on the "Download ZIP" button, and then in the Arduino IDE (v1.6 or later), select "Sketch" -> "Include Library" -> "Add .ZIP Library", then select the downloaded zip file.

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

* Black (Off)
* Blue
* Green
* Orange
* Pink
* Purple
* Red
* White
* Yellow

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

## Examples
Various example sketches are included within this library. The can be found in the `examples` directory, or from within the Arduino IDE at "File" -> "Examples" -> "Cube4".

Additionally members of the community occasionally post sketches to the [Freetronics 4x4x4 RGB Cube Forum](http://forum.freetronics.com/viewforum.php?f=32).
