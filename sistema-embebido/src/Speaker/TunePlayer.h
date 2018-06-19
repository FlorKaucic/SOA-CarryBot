/*
  TunePlayer.h - Library for playing a tune through a speaker or buzzer

  Initialize your object indicating the output pin for the speaker/buzzer.

  You need to set your tune at least once, before playing anything.
  You are able to do this by calling setTune() with your array of notes, array of note durations and number of notes.
  If you need to change your tune, you can do this anytime by calling the same method.
 
  Then, call playTune() whenever you want to play your tune.

  There's a headers file with all the notes you'll need already defined. 
  In order to use it to make your melody, it has to be included first.
*/

/*
  Example of speaker library

  #include "src/Speaker/TunePlayer.h"
  #include "src/Speaker/Notes.h"


  const int SPEAKER_PIN = 53;

  TunePlayer tunePlayer(SPEAKER_PIN);

  const int numberOfNotes = 11;
  const int myNotes[] = {
    NOTE_G2, 0, NOTE_D2, NOTE_G2, 0, NOTE_D2, NOTE_G2, NOTE_D2, NOTE_G2, NOTE_B2, NOTE_D3
  };
  const int myNoteDurations[] = {
    4, 4, 8, 4, 8, 8, 8, 8, 8, 8, 4
  };

  During setup:
  tunePlayer.setTune(myNotes, myNoteDurations, numberOfNotes);

  Whenever you want to make it play the tune:
  tunePlayer.playTune();
*/


#ifndef TunePlayer_h 
#define TunePlayer_h

#include "Arduino.h"

class TunePlayer 
{
  public:
    TunePlayer(const int speakerPin);
    void setTune(const int * notes, const int * noteDurations, const int numberOfNotes);
    void playTune(void);

  private:
    int _speakerPin;
    int _numberOfNotes;
    int * _notes;
    int * _noteDurations;
    int _getPauseBetweenNotes(const int noteDuration);
    const int WHOLE_NOTE_DURATION = 1000;
    const double NOTE_SEPARATION_PERCENTAGE = 1.30;
};

#endif
