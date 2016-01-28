/*
 * File:    UserDefinedFunctions.ino
 * Version: 1.1
 * Author:  Adam Reed (adam@secretcode.ninja)
 * License: GPLv3
 * 
 * Notes:   Using the Random Colours, Random Primaries, and 
 *          Random Pastels examples by Jonathan Oxer 
 *          (jon@freetronics.com)
 */

// Include required libraries
#include "SPI.h"
#include "Cube.h"

// Include header files for animations
#include "ColourFader.h"
#include "RandomColours.h"
#include "Wave.h"
#include "ZigZag.h"

// Create an instance of the cube class
Cube cube;

// Create an instance of the ColourFader class,
// with a delay of 10 milliseconds between colour
// changes
ColourFader colourfader(cube, 10);

// Create an instance of the RandomColours class,
// with a delay of 2 milliseconds between settings
// the next random LED to a random colour
RandomColours randomcolours(cube, 2);

// Create an instance of the Wave class, with a 
// delay of 100 milliseconds between frames in
// the animation
Wave wave(cube, 100);

// Create an instance of the ZigZag class, with a 
// delay of 300 milliseconds between frames in
// the animation
ZigZag zigzag(cube, 300);

// Track which user defined function to run
byte action = 0; 

// Track the colour to use with user defined function
rgb_t theColour;

// Set a default colour
rgb_t defaultColour = BLUE;

void setup(void) {
  // Serial port options for control of the Cube using serial commands are:
  // 0: Control via the USB connector (most common).
  // 1: Control via the RXD and TXD pins on the main board.
  // -1: Don't attach any serial port to interact with the Cube.
  cube.begin(0, 115200); // Start on serial port 0 (USB) at 115200 baud

  // Tell the cube library that the function 'userFunctionHandler' should be
  // called if the user uses the 'user ### colour;' serial command line
  // instruction
  setDelegate(userFunctionHandler);
}

void userFunctionHandler(int itemID, rgb_t selectedColour)
{
  /*  This function is called when the user askes for a user defined item via the
   *  serial command line interface. The format is 'user ### colour;', where ### is
   *  a integer, and colour is either the predefined colours, or a HTML colour code.
   *  The colour variable is optional, (so 'user 1;' is valid) but if it is not present
   *  and expected, black is used in it's place.
   */

  // Set the global variables for action and colour based on
  // what the user requested. This will cause them to run
  // repeateadly in the main loop, until the user requests a
  // different action
  action = itemID;
  theColour = selectedColour;

  // Inform the user which action was selected
  switch (action)
  {
    case 1:
      serial->println("Colour Fader");
      break;
    case 2:
      serial->println("Random Pastels");
      break;
    case 3:
      serial->println("Random Colours");
      break;
    case 4:
      serial->println("Random Primaries");
      break;
    case 5:
      serial->println("Wave");
      break;
    case 6:
      serial->println("ZigZag");
      break;
  }
}

void loop(void) {
  if (!cube.hasReceivedSerialCommand())
  {
    // User hasn't sent a serial command, so just flash a 
    // LED like a blinking cursor waiting for input
    cube.set(0, 0, 0, WHITE);
    delay(200);
    cube.set(0, 0, 0, BLACK);
    delay(300);
  } else {
    // A serial command has been received
    if (cube.inUserMode())
    {
      // Check the colour that was set
      if (theColour.color[0] == 0 && theColour.color[1] == 0 && theColour.color[2] == 0) {
        // There was no user provided colour, so set it to the default
        theColour = defaultColour;
      }

      // The last processed serial command was for a user
      // defined function, so run the action based on the
      // users input
      switch (action)
      {
        case 1:
          colourfader.update();
          break;
        case 2:
          randomcolours.pastels();
          break;
        case 3:
          randomcolours.allColours();
          break;
        case 4:
          randomcolours.primary();
          break;
        case 5:
          wave.update(theColour);
          break;
        case 6:
          zigzag.update(theColour);
          break;
      }
    }    
  }
}

