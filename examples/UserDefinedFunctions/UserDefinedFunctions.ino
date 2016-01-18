/*
 * File:    UserDefinedFunctions.ino
 * Version: 1.0
 * Author:  Adam Reed (adam@secretcode.ninja)
 * License: GPLv3
 * 
 * Notes:   Using the Random Colours, Random Primaries, and 
 *          Random Pastels examples by Jonathan Oxer 
 *          (jon@freetronics.com)
 */

#include "SPI.h"
#include "Cube.h"

Cube cube;

byte action = 0; // Track which user defined function to run
rgb_t theColour; // Track the colour to use with user defined function

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
      // The last processed serial command was for a user
      // defined function, so run the action based on the
      // users input
      switch (action)
      {
        case 1:
          zigzag();
          break;
        case 2:
          randomPastels();
          break;
        case 3:
          randomColours();
          break;
        case 4:
          randomPrimaries();
          break;
        case 5:
          faceSweep();
          break;
      }
    }    
  }
}

void zigzag()
{
  // Zig Zag style pattern

  // The Zig
  cube.all(BLACK);
  cube.set(0,0,0, theColour);
  cube.set(2,0,0, theColour);
  cube.set(1,1,0, theColour);
  cube.set(3,1,0, theColour);
  cube.set(0,2,0, theColour);
  cube.set(2,2,0, theColour);
  cube.set(1,3,0, theColour);
  cube.set(3,3,0, theColour);
  cube.copyplane(Z, 0, 2);    
  cube.set(1,0,1, theColour);
  cube.set(3,0,1, theColour);
  cube.set(0,1,1, theColour);
  cube.set(2,1,1, theColour);
  cube.set(1,2,1, theColour);
  cube.set(3,2,1, theColour);
  cube.set(0,3,1, theColour);
  cube.set(2,3,1, theColour);
  cube.copyplane(Z, 1, 3);  
  delay(300);

  // The Zag
  cube.all(BLACK);
  cube.set(1,0,0, theColour);
  cube.set(3,0,0, theColour);
  cube.set(0,1,0, theColour);
  cube.set(2,1,0, theColour);
  cube.set(1,2,0, theColour);
  cube.set(3,2,0, theColour);
  cube.set(0,3,0, theColour);
  cube.set(2,3,0, theColour);
  cube.copyplane(Z, 0, 2);    
  cube.set(0,0,1, theColour);
  cube.set(2,0,1, theColour);
  cube.set(1,1,1, theColour);
  cube.set(3,1,1, theColour);
  cube.set(0,2,1, theColour);
  cube.set(2,2,1, theColour);
  cube.set(1,3,1, theColour);
  cube.set(3,3,1, theColour);
  cube.copyplane(Z, 1, 3);   
  delay(300);
}

void randomPastels()
{
  // Function by Jonathan Oxer (jon@freetronics.com), from 
  // the RandomPastels example sketch
  cube.set(random(4), random(4), random(4), RGB(random(255), random(255), random(255)));
  delay(2);
}

void randomColours()
{
  // Function by Jonathan Oxer (jon@freetronics.com), from 
  // the RandomColours example sketch
  byte xPos;
  byte yPos;
  byte zPos;
  byte rr;
  byte gg;
  byte bb;

  xPos = random(4);
  yPos = random(4);
  zPos = random(4);
  rr = random(0, 2) * 255;
  gg = random(0, 2) * 255;
  bb = random(0, 2) * 255;


  cube.set(xPos, yPos, zPos, RGB(rr, gg, bb));
  delay(2);
}

void randomPrimaries()
{
  // Function by Jonathan Oxer (jon@freetronics.com), from 
  // the RandomPrimaries example sketch
  rgb_t colours[3] = {RED, GREEN, BLUE};
  byte xPos;
  byte yPos;
  byte zPos;

  xPos = random(0, 4);
  yPos = random(0, 4);
  zPos = random(0, 4);
  byte i = random(0, 3);

  cube.set(xPos, yPos, zPos, colours[i]);
  delay(2);
}

void faceSweep() 
{
  cube.all(BLACK);
  // Move 1
  cube.setplane(Y, 0, BLUE);
  delay(100);

  // Move 2
  cube.set(0,0,0, BLACK);
  cube.set(1,0,0, BLACK);
  cube.set(2,0,0, BLACK);
  cube.set(3,0,0, BLACK);
  cube.set(0,1,0, BLUE);
  cube.set(1,1,0, BLUE);
  cube.set(2,1,0, BLUE);
  cube.set(3,1,0, BLUE);
  delay(100);

  // Move 3
  cube.set(0,1,0, BLACK);
  cube.set(1,1,0, BLACK);
  cube.set(2,1,0, BLACK);
  cube.set(3,1,0, BLACK);
  cube.set(0,0,1, BLACK);
  cube.set(1,0,1, BLACK);
  cube.set(2,0,1, BLACK);
  cube.set(3,0,1, BLACK);
  cube.set(0,2,0, BLUE);
  cube.set(1,2,0, BLUE);
  cube.set(2,2,0, BLUE);
  cube.set(3,2,0, BLUE);
  cube.set(0,1,1, BLUE);
  cube.set(1,1,1, BLUE);
  cube.set(2,1,1, BLUE);
  cube.set(3,1,1, BLUE);
  delay(100);

  // Move 4
  cube.set(0,2,0, BLACK);
  cube.set(1,2,0, BLACK);
  cube.set(2,2,0, BLACK);
  cube.set(3,2,0, BLACK);
  cube.set(0,1,1, BLACK);
  cube.set(1,1,1, BLACK);
  cube.set(2,1,1, BLACK);
  cube.set(3,1,1, BLACK);
  cube.set(0,0,2, BLACK);
  cube.set(1,0,2, BLACK);
  cube.set(2,0,2, BLACK);
  cube.set(3,0,2, BLACK);
  cube.set(0,3,0, BLUE);
  cube.set(1,3,0, BLUE);
  cube.set(2,3,0, BLUE);
  cube.set(3,3,0, BLUE);
  cube.set(0,2,1, BLUE);
  cube.set(1,2,1, BLUE);
  cube.set(2,2,1, BLUE);
  cube.set(3,2,1, BLUE);
  cube.set(0,1,2, BLUE);
  cube.set(1,1,2, BLUE);
  cube.set(2,1,2, BLUE);
  cube.set(3,1,2, BLUE);
  delay(100);

  // Move 5
  cube.set(0,3,0, BLACK);
  cube.set(1,3,0, BLACK);
  cube.set(2,3,0, BLACK);
  cube.set(3,3,0, BLACK);
  cube.set(0,2,1, BLACK);
  cube.set(1,2,1, BLACK);
  cube.set(2,2,1, BLACK);
  cube.set(3,2,1, BLACK);
  cube.set(0,1,2, BLACK);
  cube.set(1,1,2, BLACK);
  cube.set(2,1,2, BLACK);
  cube.set(3,1,2, BLACK);
  cube.set(0,1,3, BLUE); 
  cube.set(1,1,3, BLUE); 
  cube.set(2,1,3, BLUE); 
  cube.set(3,1,3, BLUE); 
  cube.set(0,2,2, BLUE);
  cube.set(1,2,2, BLUE);
  cube.set(2,2,2, BLUE);
  cube.set(3,2,2, BLUE);
  cube.set(0,3,1, BLUE);
  cube.set(1,3,1, BLUE);
  cube.set(2,3,1, BLUE);
  cube.set(3,3,1, BLUE);
  delay(100);

  // Move 6 
  cube.set(0,2,2, BLACK);
  cube.set(1,2,2, BLACK);
  cube.set(2,2,2, BLACK);
  cube.set(3,2,2, BLACK);
  cube.set(0,3,1, BLACK);
  cube.set(1,3,1, BLACK);
  cube.set(2,3,1, BLACK);
  cube.set(3,3,1, BLACK);
  cube.set(0,2,3, BLUE);
  cube.set(1,2,3, BLUE);
  cube.set(2,2,3, BLUE);
  cube.set(3,2,3, BLUE);
  cube.set(0,3,2, BLUE);
  cube.set(1,3,2, BLUE);
  cube.set(2,3,2, BLUE);
  cube.set(3,3,2, BLUE);
  delay(100);

  // Move 7
  cube.set(0,3,2, BLACK);
  cube.set(1,3,2, BLACK);
  cube.set(2,3,2, BLACK);
  cube.set(3,3,2, BLACK);
  cube.set(0,3,3, BLUE);
  cube.set(1,3,3, BLUE);
  cube.set(2,3,3, BLUE);
  cube.set(3,3,3, BLUE);
 
  delay(500);
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
      serial->println("ZigZag");
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
      serial->println("Face Sweep");
      break;
  }
}
