/*
  TunePlayer.cpp - Library for playing a tune through a speaker or buzzer

  Refer to TunePlayer.h for method signature and explanation
*/

#include "Arduino.h"
#include "../Speaker/TunePlayer.h"
#include "Notes.h"

TunePlayer::TunePlayer(const int speakerPin) {
  _speakerPin = speakerPin;
  pinMode(_speakerPin, OUTPUT);
}

void TunePlayer::setTune(const int * notes, const int * noteDurations, const int numberOfNotes) {
  _numberOfNotes = numberOfNotes;
  _notes = (int *) malloc(_numberOfNotes * sizeof(int));
  _noteDurations = (int *) malloc(_numberOfNotes * sizeof(int));

  memcpy(_notes, notes, _numberOfNotes * sizeof(int));
  memcpy(_noteDurations, noteDurations, _numberOfNotes * sizeof(int));
}

void TunePlayer::playTune() {
  for (int thisNote = 0; thisNote < _numberOfNotes; thisNote++) {
    int noteDuration = WHOLE_NOTE_DURATION / _noteDurations[thisNote];
    tone(_speakerPin, _notes[thisNote], noteDuration);

    int pauseBetweenNotes = _getPauseBetweenNotes(noteDuration);
    delay(pauseBetweenNotes);

    noTone(_speakerPin);
  }
}

int TunePlayer::_getPauseBetweenNotes(const int noteDuration) {
  return noteDuration * NOTE_SEPARATION_PERCENTAGE;
}
