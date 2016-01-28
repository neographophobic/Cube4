/*
   File:      RandomColours.cpp
   Purpose:   Random Colour patterns for the Freetronics 4x4x4 Cube (non blocking)

   Original Author:   Jonathan Oxer (jon@freetronics.com)
   License:           GPLv3
   These examples are part of the Cube4 Library from Freetronics
   https://github.com/freetronics/Cube4.

   Modified by:       Adam Reed (adam@secretcode.ninja)
*/

// Include for Arduino Library
#include "Arduino.h"

// Include for Cube Library
#include "Cube.h"

// Include the header file for this class
#include "RandomColours.h"

RandomColours::RandomColours(Cube cube, int theDelay)
{
  // Retain the reference to the cube
  _cube = cube;

  // Retain the delay we will use
  _theDelay = theDelay;

  // Set the time we last ran the code to zero as it hasn't run yet
  _previousMillis = 0;

  // Seed the random number generator so that we get different results each
  // time the cube is started
  randomSeed(analogRead(UNUSED_ANALOG_PIN));
}

void RandomColours::pastels() {
  // Handles drawing the Random Pastels Colours animation.

  /* This code is designed to be non blocking, so instead of using
     "delay()", it uses the time and the difference between this run
     and the last run to determine if it needs to update the cube. As
     this animation doesn't have "states" we don't use a state machine.
  */

  // Get the current time
  unsigned long currentMillis = millis();

  if (currentMillis - _previousMillis >= _theDelay)
  {
    // Pick a random x, y, z, location on the cube, and set it's colour to
    // the mixture of random values of red, green and blue (each between 0 and 255)
    _cube.set(random(0, 4), random(0, 4), random(0, 4), RGB(random(0, 256), random(0, 256), random(0, 256)));

    // Remember the time for future reference
    _previousMillis = currentMillis;
  }
}

void RandomColours::allColours() {
  // Handles drawing the Random Colours animation.

  /* This code is designed to be non blocking, so instead of using
     "delay()", it uses the time and the difference between this run
     and the last run to determine if it needs to update the cube. As
     this animation doesn't have "states" we don't use a state machine.

      Notes:    This code was based on the RandomPastels, RandomColour,
                and RandomPrimary examples by Jonathan Oxer
                (jon@freetronics.com) that ship as part of the Cube
                Library code.
  */

  // Get the current time
  unsigned long currentMillis = millis();

  if (currentMillis - _previousMillis >= _theDelay)
  {
    // For three random numbers that are either 0 or 255 which
    // represents either fully on or fully off for each of the colour
    // channels
    byte rr = random(0, 2) * 255;
    byte gg = random(0, 2) * 255;
    byte bb = random(0, 2) * 255;

    if (!(rr == 0 && gg == 0 && bb == 0))
    {
      // Pick a random x, y, z, location on the cube, and set it's colour to
      // the mixture of red, green and blue random values from above as long
      // as it wouldn't be black.
      _cube.set(random(0, 4), random(0, 4), random(0, 4), RGB(rr, gg, bb));
    }

    // Remember the time for future reference
    _previousMillis = currentMillis;
  }
}

void RandomColours::primary()
{
  // Handles drawing the Random Primary Colours animation.

  /* This code is designed to be non blocking, so instead of using
     "delay()", it uses the time and the difference between this run
     and the last run to determine if it needs to update the cube. As
     this animation doesn't have "states" we don't use a state machine.
  */

  // Get the current time
  unsigned long currentMillis = millis();

  if (currentMillis - _previousMillis >= _theDelay)
  {
    // Define the three primary colours
    rgb_t colours[3] = {RED, GREEN, BLUE};

    // Randomly pick an index to use for the above colours array
    byte i = random(0, 3);

    // Pick a random x, y, z, location on the cube and set it's colour
    // to the colour at the randomly picked index
    _cube.set(random(0, 4), random(0, 4), random(0, 4), colours[i]);

    // Remember the time for future reference
    _previousMillis = currentMillis;
  }
}

