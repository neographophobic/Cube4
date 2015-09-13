/*
 * File:    parser.cpp
 * Version: 0.0
 * Author:  Andy Gelme (@geekscape)
 * License: GPLv3
 *
 * Cube message parser.
 *
 * ToDo
 * ~~~~
 * - Make parser understand colour aliases, such as RED
 * - Set global cursor position when writing to a location
 * - Decide how to represent the hidden location. Using -1 for now
 */

#ifndef CUBE_cpp
#define CUBE_cpp

#include "Cube.h"
#include "engine.h"
#include "parser.h"

byte parseCommand(
  char *message, byte length, byte *position, command_t **command
);

byte parseRGB(char *message, byte length, byte *position, rgb_t *rgb);
byte parsePosition(char *message, byte length, byte *position, byte *positionX, byte *positionY, byte *positionZ);
byte parseOffset(char *message, byte length, byte *position, byte *offset);
byte parseAxis(char *message, byte length, byte *position, byte *axis);
byte parseDirection(char *message, byte length, byte *position, byte *direction);

byte checkForHexadecimal(char *message, byte length, byte *position, byte *digit);
byte checkForOffset(char *message, byte length, byte *position, byte *digit);
byte checkForAxis(char *message, byte length, byte *position, byte *digit);
byte checkForDirection(char *message, byte length, byte *position, byte *digit);

void skipToken(char *message, byte length, byte *position);
void skipWhitespace(char *message, byte length, byte *position);
boolean stringCompare(char *source, char *target);
boolean stringDelimiter(char character);

byte parser(
  char       *message,
  byte        length,
  bytecode_t *bytecode) {

  byte errorCode = 0;
  byte position = 0;

  // Convert the message to lowercase
  int i = 0;
  while(message[i]) {
	  message[i] = tolower(message[i]);
	  i++;
  }

  skipWhitespace(message, length, & position);

  command_t *command;

  errorCode = parseCommand(message, length, & position, & command);

  if (errorCode == 0) {
    skipWhitespace(message, length, & position);

    errorCode =
      (command->parser)(message, length, & position, command, bytecode);

    if (errorCode == 0) {
      skipWhitespace(message, length, & position);
    }
  }

  return(errorCode);
}

byte parseCommand(
  char       *message,
  byte        length,
  byte       *position,
  command_t **command) {

  byte errorCode = 5;

  for (byte commandType = 0;  commandType < commandCount;  commandType ++) {
    *command = & commands[commandType];

    if (stringCompare((*command)->name, & message[*position])) {
      skipToken(message, length, position);
      errorCode = 0;
      break;
    }
  }

  return(errorCode);
}

byte parseCommandAll(
  char       *message,
  byte        length,
  byte       *position,
  command_t  *command,
  bytecode_t *bytecode) {

  byte errorCode = 0;
  bytecode->executer = command->executer;

  errorCode = parseRGB(message, length, position, & bytecode->u.lit.colorFrom);

  if (errorCode == 0) cubeAll(bytecode->u.lit.colorFrom);

  return(errorCode);
};

byte parseCommandShift(
  char       *message,
  byte        length,
  byte       *position,
  command_t  *command,
  bytecode_t *bytecode) {

  byte axis;
  byte direction;
  byte errorCode = 0;
  bytecode->executer = command->executer;

  errorCode = parseAxis(message, length, position, & axis);
  errorCode = parseDirection(message, length, position, & direction);

  if (errorCode == 0) cubeShift(axis, direction);

  return(errorCode);
};

byte parseCommandSet(
  char       *message,
  byte        length,
  byte       *position,
  command_t  *command,
  bytecode_t *bytecode) {

  byte positionX;
  byte positionY;
  byte positionZ;
  byte errorCode = 0;
  bytecode->executer = command->executer;

  errorCode = parsePosition(message, length, position, & positionX, & positionY, & positionZ);
  errorCode = parseRGB(message, length, position, & bytecode->u.lit.colorFrom);

  if (errorCode == 0) cubeSet( positionX, positionY, positionZ, bytecode->u.lit.colorFrom);

  return(errorCode);
};

byte parseCommandLine(
  char       *message,
  byte        length,
  byte       *position,
  command_t  *command,
  bytecode_t *bytecode) {

  byte positionX1;
  byte positionY1;
  byte positionZ1;
  byte positionX2;
  byte positionY2;
  byte positionZ2;
  byte errorCode = 0;
  bytecode->executer = command->executer;

  errorCode = parsePosition(message, length, position, & positionX1, & positionY1, & positionZ1);
  errorCode = parsePosition(message, length, position, & positionX2, & positionY2, & positionZ2);
  errorCode = parseRGB(message, length, position, & bytecode->u.lit.colorFrom);

  if (errorCode == 0) cubeLine( positionX1, positionY1, positionZ1, positionX2, positionY2, positionZ2, bytecode->u.lit.colorFrom);

  return(errorCode);
};

byte parseCommandBox(
  char       *message,
  byte        length,
  byte       *position,
  command_t  *command,
  bytecode_t *bytecode) {

  byte positionX1;
  byte positionY1;
  byte positionZ1;
  byte positionX2;
  byte positionY2;
  byte positionZ2;
  byte style = 0;
  byte errorCode = 0;
  bytecode->executer = command->executer;

  errorCode = parsePosition(message, length, position, & positionX1, & positionY1, & positionZ1);
  errorCode = parsePosition(message, length, position, & positionX2, & positionY2, & positionZ2);
  errorCode = parseRGB(message, length, position, & bytecode->u.lit.colorFrom);
  errorCode = parseOffset(message, length, position, & style);
  if (errorCode) {
    errorCode = 0;
    style = 0;
  }
  errorCode = parseRGB(message, length, position, & bytecode->u.lit.colorTo);
  if (errorCode) {
    errorCode = 0;
    bytecode->u.lit.colorTo = BLACK;
  }

  if (errorCode == 0) cubeBox( positionX1, positionY1, positionZ1, positionX2, positionY2, positionZ2, bytecode->u.lit.colorFrom, style, bytecode->u.lit.colorTo);

  return(errorCode);
};

byte parseCommandSphere(
  char       *message,
  byte        length,
  byte       *position,
  command_t  *command,
  bytecode_t *bytecode) {

  byte positionX1;
  byte positionY1;
  byte positionZ1;
  byte size;
  byte errorCode = 0;
  bytecode->executer = command->executer;

  errorCode = parsePosition(message, length, position, & positionX1, & positionY1, & positionZ1);
  errorCode = parseOffset(message, length, position, & size);
  errorCode = parseRGB(message, length, position, & bytecode->u.lit.colorFrom);
  errorCode = parseRGB(message, length, position, & bytecode->u.lit.colorTo);
  if (errorCode) {
    errorCode = 0;
    bytecode->u.lit.colorTo = BLACK;
  }

  if (errorCode == 0) cubeSphere( positionX1, positionY1, positionZ1, size, bytecode->u.lit.colorFrom, bytecode->u.lit.colorTo);

  return(errorCode);
};

byte parseCommandNext(
  char       *message,
  byte        length,
  byte       *position,
  command_t  *command,
  bytecode_t *bytecode) {

  byte errorCode = 0;
  bytecode->executer = command->executer;

  errorCode = parseRGB(message, length, position, & bytecode->u.lit.colorFrom);

  if (errorCode == 0) cubeNext(bytecode->u.lit.colorFrom);

  return(errorCode);
};

byte parseCommandCopyplane(
  char       *message,
  byte        length,
  byte       *position,
  command_t  *command,
  bytecode_t *bytecode) {

  byte axis;
  byte offset;
  byte destination;
  byte errorCode = 0;
  bytecode->executer = command->executer;

  errorCode = parseAxis(message, length, position, & axis);
  errorCode = parseOffset(message, length, position, & offset);
  errorCode = parseOffset(message, length, position, & destination);

  if (errorCode == 0) cubeCopyplane(axis, offset, destination);

  return(errorCode);
};

byte parseCommandMoveplane(
  char       *message,
  byte        length,
  byte       *position,
  command_t  *command,
  bytecode_t *bytecode) {

  byte axis;
  byte offset;
  byte destination;
  rgb_t rgb;
  byte errorCode = 0;
  bytecode->executer = command->executer;

  errorCode = parseAxis(message, length, position, & axis);
  errorCode = parseOffset(message, length, position, & offset);
  errorCode = parseOffset(message, length, position, & destination);
  errorCode = parseRGB(message, length, position, & rgb);

  if (errorCode == 0) cubeMoveplane(axis, offset, destination, rgb);

  return(errorCode);
};

byte parseCommandSetplane(
  char       *message,
  byte        length,
  byte       *position,
  command_t  *command,
  bytecode_t *bytecode) {

  byte axis;
  byte offset;
  byte errorCode = 0;
  bytecode->executer = command->executer;

  errorCode = parseAxis(message, length, position, & axis);
  errorCode = parseOffset(message, length, position, & offset);
  errorCode = parseRGB(message, length, position, & bytecode->u.lit.colorFrom);

  if (errorCode == 0) cubeSetplane(axis, offset, bytecode->u.lit.colorFrom);

  return(errorCode);
};

byte parseCommandHelp(
  char       *message,
  byte        length,
  byte       *position,
  command_t  *command,
  bytecode_t *bytecode) {

  byte errorCode = 0;
  bytecode->executer = command->executer;

  if (serial) {
    serial->println(F("  *** Available commands ***"));
    serial->println(F("Entire cube:"));
    serial->println(F("  all <colour>;                                        (eg: 'all RED;', or 'all ff0000;')"));
    serial->println(F("  shift <axis> <direction>;                            (eg: 'shift X +;', or 'shift Y -;')"));
    serial->println(F("Single LED:"));
    serial->println(F("  set <location> <colour>;                             (eg: 'set 112 GREEN;', or 'set 112 00ff00;')"));
    serial->println(F("  next <colour>;                                       (eg: 'next BLUE;', or 'next 0000ff;')"));
    serial->println(F("One axis:"));
    serial->println(F("  setplane <axis> <offset> <colour>;                   (eg: 'setplane X 2 BLUE;', or 'setplane Y 1 00ff00;')"));
    serial->println(F("  copyplane <axis> <from offset> <to offset>;          (eg: 'copyplane X 2 1;')"));
    serial->println(F("  moveplane <axis> <from offset> <to offset> <colour>; (eg: 'move Z 1 3 BLACK;', or 'move X 3 0 GREEN;')"));
    // Commented out due to taking up an additional 2% program storage space
    // serial->println(F("Graphics and shapes:"));
    // serial->println(F("  line <location1> <location2> <colour>;                     (eg: 'line 000 333 RED;', or 'line 000 333 ff0000;')"));
    // serial->println(F("  box <location1> <location2> <colour> (<style:0-4:solid/walls only/edges only/walls filled/edges filled>) (<fill>);  (eg: 'box 000 333 GREEN;', or 'box 000 333 00ff00 3 ffffff;')"));
    // serial->println(F("  sphere <location1> <location2> <colour> (<fill>);          (eg: 'sphere 000 333 BLUE;', or 'sphere 000 333 0000ff ffffff;')"));
    serial->println(F("Supported colour aliases:"));
    serial->println(F("  BLACK BLUE GREEN ORANGE PINK PURPLE RED WHITE YELLOW"));
    serial->println(F("  *** Please see www.freetronics.com/cube for more information ***"));
  }

  return(errorCode);
};

byte parseRGB(
  char  *message,
  byte   length,
  byte  *position,
  rgb_t *rgb) {

  byte digit;
  byte number;
  byte errorCode = 7;

  skipWhitespace(message, length, position);

  /* Temporary test of colour alias detection. Nasty, replace with a generalised parser
   *   - Only test the first and last character of the colour (as they are unique) to 
   *     minimise program storage space.
   */
  if (message[*position] == 'b'
      && message[*position + 4] == 'k') // BLACK
  {
    *rgb = BLACK;
    (*position) += 5;
    errorCode = 0;
    return(errorCode);
  }

  if (message[*position] == 'b'
      && message[*position + 3] == 'e') // BLUE
  {
    *rgb = BLUE;
    (*position) +=4;
    errorCode = 0;
    return(errorCode);
  }

  if (message[*position] == 'g'
      && message[*position + 4] == 'n') // GREEN
  {
    *rgb = GREEN;
    (*position) += 5;
    errorCode = 0;
    return(errorCode);
  }

  if (message[*position] == 'o'
      && message[*position + 5] == 'e') // ORANGE
  {
    *rgb = ORANGE;
    (*position) += 6;
    errorCode = 0;
    return(errorCode);
  }

  if (message[*position] == 'p'
      && message[*position + 3] == 'k') // PINK
  {
    *rgb = PINK;
    (*position) +=4;
    errorCode = 0;
    return(errorCode);
  }

  if (message[*position] == 'p'
      && message[*position + 5] == 'e') // PURPLE
  {
    *rgb = PURPLE;
    (*position) += 6;
    errorCode = 0;
    return(errorCode);
  }

  if (message[*position] == 'r'
      && message[*position + 2] == 'd') // RED
  {
    *rgb = RED;
    (*position) += 3;
    errorCode = 0;
    return(errorCode);
  }

  if (message[*position] == 'w'
      && message[*position + 4] == 'e') // WHITE
  {
    *rgb = WHITE;
    (*position) += 5;
    errorCode = 0;
    return(errorCode);
  }

  if (message[*position] == 'y'
      && message[*position + 5] == 'w') // YELLOW
  {
    *rgb = YELLOW;
    (*position) += 6;
    errorCode = 0;
    return(errorCode);
  }


  if (checkForHexadecimal(message, length, position, & digit)) {
    number = digit;
    (*position) ++;

    if (checkForHexadecimal(message, length, position, & digit)) {
      rgb->color[0] = number * 16 + digit;
      (*position) ++;

      if (checkForHexadecimal(message, length, position, & digit)) {
        number = digit;
        (*position) ++;

        if (checkForHexadecimal(message, length, position, & digit)) {
          rgb->color[1] = number * 16 + digit;
          (*position) ++;

          if (checkForHexadecimal(message, length, position, & digit)) {
            number = digit;
            (*position) ++;

            if (checkForHexadecimal(message, length, position, & digit)) {
              rgb->color[2] = number * 16 + digit;
              (*position) ++;
              errorCode = 0;
            }
          }
        }
      }
    }
  }

  return(errorCode);
};

byte parsePosition(
  char  *message,
  byte   length,
  byte  *position,
  byte  *positionX,
  byte  *positionY,
  byte  *positionZ) {

  byte digit;
  byte errorCode = 6;

  skipWhitespace(message, length, position);

  if (checkForOffset(message, length, position, & digit)) {
    *positionX = digit;
    (*position) ++;

    if (checkForOffset(message, length, position, & digit)) {
      *positionY = digit;
      (*position) ++;

      if (checkForOffset(message, length, position, & digit)) {
        *positionZ = digit;
        (*position) ++;
        errorCode = 0;
      }
    }
  }

  return(errorCode);
};

byte parseAxis(
  char  *message,
  byte   length,
  byte  *position,
  byte  *axis) {

  byte digit;
  byte errorCode = 10;

  skipWhitespace(message, length, position);

  if (checkForAxis(message, length, position, & digit)) {
    *axis = digit;
    (*position) ++;
    errorCode = 0;
  }

  return(errorCode);
};

byte parseDirection(
  char  *message,
  byte   length,
  byte  *position,
  byte  *direction) {

  byte digit;
  byte errorCode = 11;

  skipWhitespace(message, length, position);

  if (checkForDirection(message, length, position, & digit)) {
    *direction = digit;
    (*position) ++;
    errorCode = 0;
  }

  return(errorCode);
};

byte parseOffset(
  char  *message,
  byte   length,
  byte  *position,
  byte  *offset) {

  byte digit;
  byte errorCode = 6;

  skipWhitespace(message, length, position);

  if (checkForOffset(message, length, position, & digit)) {
    *offset = digit;
    (*position) ++;
    errorCode = 0;
  }

  return(errorCode);
};

byte checkForHexadecimal(
  char *message,
  byte  length,
  byte *position,
  byte *digit) {

  byte match = 0;

  if (*position < length) {
    if (message[*position] >= '0'  &&  message[*position] <= '9') {
      *digit = message[*position] - '0';
      match = 1;
    }

    if (message[*position] >= 'a'  &&  message[*position] <= 'f') {
      *digit = message[*position] - 'a' + 10;
      match = 1;
    }
  }

  return(match);
}

byte checkForDirection(
  char *message,
  byte  length,
  byte *position,
  byte *digit) {

  byte match = 0;

  if (*position < length) {
    if (message[*position] == '+' || message[*position] == '-') {
      *digit = message[*position];
      match = 1;
    }
  }

  return(match);
}

byte checkForAxis(
  char *message,
  byte  length,
  byte *position,
  byte *digit) {

  byte match = 0;

  if (*position < length) {
    if (message[*position] == 'x') {
      *digit = X;
      match = 1;
    }
    if (message[*position] == 'y') {
      *digit = Y;
      match = 1;
    }
    if (message[*position] == 'z') {
      *digit = Z;
      match = 1;
    }
  }

  return(match);
}

byte checkForOffset(
  char *message,
  byte  length,
  byte *position,
  byte *digit) {

  byte match = 0;

  if (*position < length) {
    if (message[*position] >= '0'  &&  message[*position] <= '9') {
      *digit = message[*position] - '0';
      match = 1;
    }

    if (message[*position] == 'h') {
      *digit = -1;
      match = 1;
    }
  }

  return(match);
}

void skipToken(
  char *message,
  byte  length,
  byte *position) {

  while (*position < length) {
    if (stringDelimiter(message[*position])) break;
    (*position) ++;
  }
}

void skipWhitespace(
  char *message,
  byte  length,
  byte *position) {

  while (*position < length  &&  message[*position] == SPACE) (*position) ++;
}

boolean stringCompare(
  char *source,
  char *target) {

  byte index = 0;
  byte match = 1;

  while (stringDelimiter(source[index]) == 0  ||
         stringDelimiter(target[index]) == 0) {

    if (source[index] != target[index]) {
      match = 0;
      break;
    }
    index ++;
  }

  return(match);
}

boolean stringDelimiter(
  char character) {

  return(character == NUL  ||  character == SPACE  ||  character == RBRAC);
}
#endif
