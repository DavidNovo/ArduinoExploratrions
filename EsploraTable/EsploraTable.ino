
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
 the full left position, it will long, if put full right the wait will be short.
 
 Steps for development:
   1. DONE Switch 1 changes the color of the LED from blue to green.
   Note that the state of the swithc will have to be polled because
   the switch will not transmit information.
   
   2. DONE   Make connection to host computer and send data to computer.  
   Make sure the rows fo data can added to a spreadsheet easily.
   
   3. DONE   When printing data, change LED from green to red.  When done
   printing change LED from red to green.
   
   4. DONE implement slider to change rate of data accquisition.
   
   5. Add headers to the output stream.
 */
   
boolean isLoggingActive = false;

/*
Adding time as data written to the host computer.
This holds the ms value when logging was activated.
Outputted in the time column.
*/
unsigned long startedAt = 0;

/*
This variable holds tha last polled state of the start button.  
If the code sees a change between this value and a new value,
it means the button was pressed.
*/
boolean lastStartBtnState = HIGH;

/*
This variable is used to hold the time until the next sample is taken.
This variable is checked against the startedAt variable to 
see when the next sample should be taken.
*/
unsigned long nextSampleInThisManySeconds = 0;

/*
These variables are used when read the potentiometer.
*/
unsigned long lowValueOfPotentiometer = 0;
unsigned long highValuesOfPotentiometer = 1023;
unsigned long smallestSampleRateInterval = 10; // in seconds
unsigned long largestSampleRateInterval = 290; // in seconds

void setup() {
  // initalization code esstablish connection to host computer
  Keyboard.begin();
  Serial.begin(9600);
}



void loop() {
  activeDelay(50);
  
  if (isLoggingActive) {
    setLEDLoggingActive();
    if (nextSampleInThisManySeconds < millis() ) {
    // check if it is time to sample
    // the first time through this loop should always create a sample
    // then read the slider to determine the time for next sample
    int slider = Esplora.readSlider();
    // this is for testing only
    Serial.println(slider);
    
    // set the value from the slider to the variable use map function
    int sampleInterval = map(slider, lowValueOfPotentiometer, highValuesOfPotentiometer, 
                              smallestSampleRateInterval, largestSampleRateInterval);
    nextSampleInThisManySeconds = millis() + sampleInterval * 1000;
    // gather data and print to host computer
    logDataAndPrint();
    }  
  } else {
    setLEDLoggingInactive();
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
    // remember the button needs to be pressed, not held down
    // this next check is important, pressing the button means the state becomes LOW
    // for a short period of time.
    // without this check the state does not stay changed
    if (startBtn == HIGH) {  //the button is released
     isLoggingActive = !isLoggingActive;
     startedAt = millis();
    } 
    // else assign new state of button to the last polled state of the button 
   lastStartBtnState = startBtn;
  } 
}

void setLEDLoggingActive() {
  Esplora.writeGreen(100);
  Esplora.writeBlue(0);
}
  
void setLEDLoggingInactive() {
  Esplora.writeBlue(100);
  Esplora.writeGreen(0);    
}

/*
this method will gater desired data and print
on host computer.  The formating of the
data will happen in this methods
*/
void logDataAndPrint() {
  //set LED to red to show data gathering and printing is occuring
  Esplora.writeRed(200);
  Esplora.writeGreen(0);
  
  // time reading was taken
  unsigned long timeSecs = (millis() - startedAt) / 1000;
  
  //gather all desirec data
  int xAxis = Esplora.readAccelerometer(X_AXIS);
  int yAxis = Esplora.readAccelerometer(Y_AXIS);
  int zAxis = Esplora.readAccelerometer(Z_AXIS);
  
  // ambient temperature
  int celsius = Esplora.readTemperature(DEGREES_C);
  int fahrenheit = Esplora.readTemperature(DEGREES_F);
  
  // ambient noise
  int sound = Esplora.readMicrophone();

  // ambient light level
  int light = Esplora.readLightSensor();
    
    
  //print data to host computer include tabs so data is readable
  // in a spreadsheet application
  Keyboard.print("{");
  Keyboard.print("time");
  Keyboard.print(":"); 
  Keyboard.print(timeSecs);
  Keyboard.write(KEY_TAB);
  activeDelay(300);
  
  activeDelay(300);
  Keyboard.print("x-axis");
  Keyboard.print(":"); 
  Keyboard.print(xAxis);
  Keyboard.write(KEY_TAB);
  
  activeDelay(300);
  Keyboard.print("y-axis");
  Keyboard.print(":"); 
  Keyboard.print(yAxis);
  Keyboard.write(KEY_TAB);
  
  activeDelay(300);
  Keyboard.print("z-axis");
  Keyboard.print(":"); 
  Keyboard.print(zAxis);
  Keyboard.write(KEY_TAB);
  
  activeDelay(300);
  Keyboard.print("celsius");
  Keyboard.print(":"); 
  Keyboard.print(celsius);
  Keyboard.write(KEY_TAB);

  activeDelay(300);  
  Keyboard.print("fahrenheit");
  Keyboard.print(":");  
  Keyboard.print(fahrenheit);
  Keyboard.write(KEY_TAB);
  
  activeDelay(300);
  Keyboard.print("sound_level");
  Keyboard.print(":");
  Keyboard.print(sound);
  Keyboard.write(KEY_TAB);
  
  activeDelay(300);
  Keyboard.print("light_level");
  Keyboard.print(":");  
  Keyboard.print(light);
  Keyboard.print("}");
  Keyboard.write(KEY_TAB);
  
  activeDelay(300); 
  Keyboard.println();
  
  activeDelay(300);

  /*
  This is an important correction. the print() function
  was sending an ASCII code, of 210 for the KEY_HOME constant.
  The write() function sends a key stroke.
  */
  //Keyboard.print(KEY_HOME);


  // turn off red LED
  Esplora.writeRed(0);
  setLEDLoggingActive();
}

/*
There is a problems with speed.  The spreadsheet program does not keep up
with the generation of data.  Also pressing the switch does not
turn off logging in a timely fashion.

This function will take care of both issues.  The number 
passed in will determine the number of times the switch is polled
for information.  Doing this will also create a needed delay.
*/
void activeDelay(unsigned long amount) {
  unsigned long at = millis() + amount;
  while (millis() < at) {
    checkSwitchPress();
  }
}

   


  
  
