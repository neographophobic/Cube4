/*
   File:      ColourFader.cpp
   Purpose:   Colour Fader pattern for the Freetronics 4x4x4 Cube (non blocking)

   Original Author:   ADA/THOMAS
   This example was contributed by sparky-nz on the Freetronics forums:
   http://forum.freetronics.com/viewtopic.php?f=32&t=5577&p=11085

   Modified by:       Adam Reed (adam@secretcode.ninja)
*/

// Include for Arduino Library
#include <Arduino.h>

// Include for Cube Library
#include "Cube.h"

// Include the header file for this class
#include "ColourFader.h"

ColourFader::ColourFader(Cube cube, int theDelay)
{
  // Retain the reference to the cube
  _cube = cube;

  // Retain the delay we will use
  _theDelay = theDelay;

  // Set the default initial state for the animation
  _state = 1;

  // Set the time we last ran the code to zero as it hasn't run yet
  _previousMillis = 0;
}

void ColourFader::update()
{
  // Handles drawing the Colour Fader animation.

  /* This code is designed to be non blocking, so instead of using
     "delay()", it uses a state machine to track where it is upto in the
     animation. It then uses the time and the difference between this run
     and the last run to determine if it needs to change to a different
     state
  */

  // Get the current time
  unsigned long currentMillis = millis();

  if ((_state == 1) && (currentMillis - _previousMillis >= _theDelay))
  {
    // Increase the red, while decreasing the blue
    redValue = redValue + 1;
    blueValue = blueValue - 1;
    _cube.all(RGB(redValue, greenValue, blueValue));

    // Remember the time for future reference
    _previousMillis = currentMillis;

    // Move to the next state if we have hit maximum red
    if (redValue == 255) {
      _state++;
    }
  }
  else if ((_state == 2) && (currentMillis - _previousMillis >= _theDelay))
  {
    // Increase the green, while decreasing the red
    greenValue = greenValue + 1;
    redValue = redValue - 1;
    _cube.all(RGB(redValue, greenValue, blueValue));

    // Remember the time for future reference
    _previousMillis = currentMillis;

    // Move to the next state if we have hit maximum green
    if (greenValue == 255) {
      _state++;
    }
  }
  else if ((_state == 3) && (currentMillis - _previousMillis >= _theDelay))
  {
    // Increase the blue, while decreasing the green
    blueValue = blueValue + 1;
    greenValue = greenValue - 1;
    _cube.all(RGB(redValue, greenValue, blueValue));

    // Remember the time for future reference
    _previousMillis = currentMillis;

    // Move back to the first state if we have hit maximum blue
    if (blueValue == 255) {
      _state = 1;
    }
  }
}

