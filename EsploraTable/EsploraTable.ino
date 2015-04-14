
#include <Esplora.h>

/*
Remote sensing station

This will change the Explora into a remote sensing station.  The following information will
be sent back to the host computer:
  location - the x, y , z coordinates after logging is turned on
  temperature - in Celcius and Farhenite
  ambient light - use light sensor
  ambinent light - use light sensor
  time - time since logging started
  
logging/transmission turned on/off using switch 1

RBG led shows state of the board:
  blue - idle, no logging
  green - active, will print eventually
  red - printing/sending data
 
 goal is to have data send with headers.  The first row will be the headers, the
 next rows will have the detail data listed above.
 
 The rate of data accquisition will be derermined by the slider; if put to 
 the full left position, it will be 5 min between data accquisition, if put 
 full right the wait will be 10 seconds.
 
 Steps for development:
   1. Switch 1 changes the color of the LED from blue to green.
   Note that the state of the swithc will have to be polled because
   the switch will not transmit information.
   
   2. Make connection to host computer and send data to computer.  
   Make sure the rows fo data can added to a spreadsheet easily.
   
   3. When printing data, change LED from green to red.  When done
   printing change LED from red to green.
   
   4. implement slider to change rate of data accquisition.
   
   5. Add headers to the output stream.
 */
   
boolean isLoggingActive = false;

/*
This variable holds tha last polled state of the start button.  
If the code sees a change between this value and a new value,
it means the button was pressed.
*/
boolean lastStartBtnState = HIGH;


void setup() {
  
}



void loop() {
  checkSwitchPress();
  
  if (isLoggingActive) {
    Esplora.writeGreen(100);
  } else {
    Esplora.writeBlue(100);
  }  
}

void checkSwitchPress() {
  // read switch 1
  boolean startBtn = Esplora.readButton(SWITCH_DOWN);
  
  // if switch is down set the LED to blue, set logging to active
  // but how do I know if it has been pressed? create a starting
  // state for the button
  
  // if state of start button has changed
  if (startBtn != lastStartBtnState) {
  // then if the start button is high it has just been relased
    // change the state of logging   
   isLoggingActive = !isLoggingActive; 
  }
  // else assign new state of button to the last polled state of the button 
  lastStartBtnState = startBtn;
  
  
}

   


  
  
