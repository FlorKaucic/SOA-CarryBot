/*
  TunePlayer.cpp - Library for playing a tune through a speaker or buzzer

  Refer to TunePlayer.h for method signature and explanation
*/

#include "Arduino.h"
#include "TunePlayer.h"
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

void TunePlayer::setTune1(){
	int number = 5;
	int notes[] = {NOTE_F4, NOTE_C4,NOTE_C4,NOTE_D4,NOTE_C4};
	int times[] = {4,8,8,4,4};
	TunePlayer::setTune(notes,times,number);
}

void TunePlayer::setTune2(){
	int number = 4;
	int notes[] = {NOTE_E6, NOTE_E6,NOTE_E6,NOTE_E6};
	int times[] = {2,1,2,1};
	TunePlayer::setTune(notes,times,number);
}

void TunePlayer::setTune3(){
	int number = 2;
	int notes[] = {NOTE_E5, NOTE_A4};
	int times[] = {4,4};
	TunePlayer::setTune(notes,times,number);
}

void TunePlayer::setTune4(){
	int number = 3;
	int notes[] = {NOTE_A5, NOTE_C6,NOTE_E6};
	int times[] = {16,16,16};
	TunePlayer::setTune(notes,times,number);
}

void TunePlayer::setTune5(){
	int number = 3;
	int notes[] = {NOTE_E6, NOTE_C6,NOTE_A5};
	int times[] = {16,16,16};
	TunePlayer::setTune(notes,times,number);
}

int TunePlayer::_getPauseBetweenNotes(const int noteDuration) {
  return noteDuration * NOTE_SEPARATION_PERCENTAGE;
}
