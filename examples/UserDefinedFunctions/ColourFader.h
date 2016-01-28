/*
   File:      ColourFader.h
   Purpose:   Colour Fader pattern for the Freetronics 4x4x4 Cube (non blocking)

   Original Author:   ADA/THOMAS
   This example was contributed by sparky-nz on the Freetronics forums:
   http://forum.freetronics.com/viewtopic.php?f=32&t=5577&p=11085

   Modified by:       Adam Reed (adam@secretcode.ninja)
*/

// Ensure that this header file is only processed once, regardless of how
// many time it is included
#ifndef ColourFader_h
#define ColourFader_h

// Include for Arduino Library
#include <Arduino.h>

// Include for Cube Library
#include "Cube.h"

// Declare out class, with public and private variables and functions
class ColourFader {
  public:
    // Constructor method, requiring the cube class, and the time in
    // milliseconds between frames
    ColourFader(Cube cube, int theDelay);

    // Function to determine what the current state of the animation
    // should be, and to update the cube to that state if appropriate
    void update();

  private:
    // Reference to the cube
    Cube _cube;

    // Delay between animation states in milliseconds
    int _theDelay;

    // The state the animation is in
    int _state;

    // Tracks the time the code was last run
    unsigned long _previousMillis;

    // Track the Red, Green and Blue colour values
    int redValue = 0;
    int greenValue = 0;
    int blueValue = 255;
};

#endif

