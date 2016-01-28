/*
   File:      RandomColours.h
   Purpose:   Random Colour patterns for the Freetronics 4x4x4 Cube (non blocking)

   Original Author:   Jonathan Oxer (jon@freetronics.com)
   License:           GPLv3
   These examples are part of the Cube4 Library from Freetronics
   https://github.com/freetronics/Cube4.

   Modified by:       Adam Reed (adam@secretcode.ninja)
*/

// Ensure that this header file is only processed once, regardless of how
// many time it is included
#ifndef RandomColours_h
#define RandomColours_h

/*==========================================================================
    APPLICATION SETTINGS (USER EDITABLE)

    UNUSED_ANALOG_PIN   An unused analog pin that is used as a seed for the
                        random number generator to ensure that the random
                        pattern is different each time the cube is started
  ============================================================================ */
#define UNUSED_ANALOG_PIN      0
/*========================================================================== */

// Include for Arduino Library
#include "Arduino.h"

// Include for Cube Library
#include "Cube.h"

// Declare out class, with public and private variables and functions
class RandomColours {
  public:
    // Constructor method, requiring the cube class, the time in
    // milliseconds between animations, and the colour to use.
    RandomColours(Cube cube, int theDelay);

    // Functions to draw the three different animations that we have
    // in this class that randomly fill the cube with constantly
    // changing colours

    // Pastel Colours
    void pastels();

    // All Colours
    void allColours();

    // Primary (Red, Green, and Blue) Colours
    void primary();

  private:
    // Reference to the cube
    Cube _cube;

    // Delay between animation states in milliseconds
    int _theDelay;

    // Tracks the time the code was last run
    unsigned long _previousMillis;
};

#endif

