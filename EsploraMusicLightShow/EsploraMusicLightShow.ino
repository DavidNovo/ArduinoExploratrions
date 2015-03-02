/*
  Esplora Music Light Show

 Created on 2 March 2015
 by D. Novogrodsky
 
 This example is in the public domain.
 */

#include <Esplora.h>

// here is the list of notes for the instrument
// these are the frequencies for the notes from middle C
// to one octave above middle C:
const int note[] = {
262, // C
277, // C#
294, // D
311, // D#
330, // E
349, // F
370, // F#
392, // G
415, // G#
440, // A
466, // A#
494, // B
523  // C next octave
};

const int color[] = {
262, // C
277, // C#
294, // D
311, // D#
330, // E
349, // F
370, // F#
392, // G
415, // G#
440, // A
466, // A#
494, // B
523  // C next octave
};
void setup() {
  // There's nothing to set up for this sketch
}

void loop() {
  // if the button labeled SWITCH_UP.  If it is
  // down play a note and flash some color
  // depending on the note
    if (Esplora.readButton(SWITCH_UP) == LOW) {
    int slider = Esplora.readSlider();
    
    
    // use map() to map the slider's range to the 
    // range of notes you have:
    byte thisNote = map(slider, 0, 1023, 0, 13);
    // play the note corresponding to the slider's position:
    Esplora.tone(note[thisNote]);   
   // reading the slider for a note, in this care blue 
    byte blue = slider/4;
    
    Esplora.writeRGB(0,0,blue);
  }
  else {
    // if the button isn't pressed, turn the note off:
    Esplora.noTone();
    Esplora.writeRGB(0,0,0);
  }
        

}


