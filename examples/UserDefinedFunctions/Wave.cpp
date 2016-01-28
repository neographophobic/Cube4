/*
    File:     Wave.cpp
    Purpose:  Wave pattern for the Freetronics 4x4x4 Cube (non blocking)
    Author:   Adam Reed (adam@secretcode.ninja)
    Licence:  BSD 3-Clause Licence
*/

// Include for Arduino Library
#include <Arduino.h>

// Include for Cube Library
#include "Cube.h"

// Include the header file for this class
#include "Wave.h"

Wave::Wave(Cube cube, int theDelay)
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

void Wave::update(rgb_t theColour)
{
  // Handles drawing the Wave animation.

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
    drawWaveAnimationFrame(0, 0, 1, 1, 2, 2, 3, 3, theColour);

    // Move to the next frame
    _state = 2;

    // Remember the time for future reference
    _previousMillis = currentMillis;
  }
  else if ((_state == 2) && (currentMillis - _previousMillis >= _theDelay))
  {
    drawWaveAnimationFrame(0, 1, 1, 0, 2, 1, 3, 2, theColour);

    // Move to the next state
    _state = 3;

    // Remember the time for future reference
    _previousMillis = currentMillis;
  }
  else if ((_state == 3) && (currentMillis - _previousMillis >= _theDelay))
  {
    drawWaveAnimationFrame(0, 2, 1, 1, 2, 0, 3, 1, theColour);

    // Move to the next state
    _state = 4;

    // Remember the time for future reference
    _previousMillis = currentMillis;
  }
  else if ((_state == 4) && (currentMillis - _previousMillis >= _theDelay))
  {
    drawWaveAnimationFrame(0, 3, 1, 2, 2, 1, 3, 0, theColour);

    // Move to the next state
    _state = 5;

    // Remember the time for future reference
    _previousMillis = currentMillis;
  }
  else if ((_state == 5) && (currentMillis - _previousMillis >= _theDelay))
  {
    drawWaveAnimationFrame(0, 2, 1, 3, 2, 2, 3, 1, theColour);

    // Move to the next state
    _state = 6;

    // Remember the time for future reference
    _previousMillis = currentMillis;
  }
  else if ((_state == 6) && (currentMillis - _previousMillis >= _theDelay))
  {
    drawWaveAnimationFrame(0, 1, 1, 2, 2, 3, 3, 2, theColour);

    // Move back to the next state
    _state = 1;

    // Remember the time for future reference
    _previousMillis = currentMillis;
  }
}

void Wave::drawWaveAnimationFrame(byte y1, byte z1, byte y2, byte z2, byte y3, byte z3, byte y4, byte z4, rgb_t theColour)
{
  _cube.all(BLACK);
  _cube.line(0, y1, z1, 3, y1, z1, theColour);
  _cube.line(0, y2, z2, 3, y2, z2, theColour);
  _cube.line(0, y3, z3, 3, y3, z3, theColour);
  _cube.line(0, y4, z4, 3, y4, z4, theColour);
}

