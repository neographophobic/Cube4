/*
   File:      ZigZag.cpp
   Purpose:   Zig Zag pattern for the Freetronics 4x4x4 Cube (non blocking)
   Author:    Adam Reed (adam@secretcode.ninja)
   Licence:   BSD 3-Clause Licence
*/

// Include for Arduino Library
#include <Arduino.h>

// Include for Cube Library
#include "Cube.h"

// Include the header file for this class
#include "ZigZag.h"

ZigZag::ZigZag(Cube cube, int theDelay)
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

void ZigZag::update(rgb_t theColour)
{
  // Handles drawing the ZigZag animation.

  /* This code is designed to be non blocking, so instead of using
   * "delay()", it uses a state machine to track where it is upto in the
   * animation. It then uses the time and the difference between this run
   * and the last run to determine if it needs to change to a different
   * state
   */

  // Get the current time
  unsigned long currentMillis = millis();

  if ((_state == 1) && (currentMillis - _previousMillis >= _theDelay))
  {
    // Draw frame 1 of the animation
    _cube.all(BLACK);
    _cube.set(0, 3, 0, theColour);
    _cube.line(0, 1, 0, 2, 3, 0, theColour);
    _cube.line(1, 0, 0, 3, 2, 0, theColour);
    _cube.set(3, 0, 0, theColour);
    _cube.copyplane(Z, 0, 2);
    _cube.line(0, 2, 1, 1, 3, 1, theColour);
    _cube.line(0, 0, 1, 3, 3, 1, theColour);
    _cube.line(2, 0, 1, 3, 1, 1, theColour);
    _cube.copyplane(Z, 1, 3);

    // Flag that we need to move to the other state
    _state = 2;
    // Remember the time for future reference
    _previousMillis = currentMillis;
  }
  else if ((_state == 2) && (currentMillis - _previousMillis >= _theDelay))
  {
    // Draw frame 2 of the animation
    _cube.all(BLACK);
    _cube.line(0, 2, 0, 1, 3, 0, theColour);
    _cube.line(0, 0, 0, 3, 3, 0, theColour);
    _cube.line(2, 0, 0, 3, 1, 0, theColour);
    _cube.copyplane(Z, 0, 2);
    _cube.set(0, 3, 1, theColour);
    _cube.line(0, 1, 1, 2, 3, 1, theColour);
    _cube.line(1, 0, 1, 3, 2, 1, theColour);
    _cube.set(3, 0, 1, theColour);
    _cube.copyplane(Z, 1, 3);

    // Flag that we need to move to the other state
    _state = 1;
    // Remember the time for future reference
    _previousMillis = currentMillis;
  }
}

