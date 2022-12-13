/**
 * @file main.cpp
 * @author Alexandre Desgagné (alex.desgagne@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <mbed.h>
#include "melody.h"

// Init GPIOs
DigitalOut myLed(PC_13);
PwmOut buzzer(PA_8);
AnalogIn thermistor(PA_0);

// Declarations
float readTemperature();

float r1 = 465.0f;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

int main()
{
  // Change this to make the song slower or faster.
  int tempo{ 114 };
  float tempLimit{ 45.00f }; // degrees

  // sizeof() gives the number of bytes, each int value is composed of two bytes (16 bits)
  // there are two values per note (pitch and duration), so for each note there are four bytes
  int notes = sizeof(melody) / sizeof(melody[0]) / 2;

  // This calculates the duration of a whole note in ms.
  int wholenote = (60000 * 4) / tempo;
  int divider{}, noteDuration{};

  float inputValue{};

  // Boot LED sequence.
  for(int i = 0; i < 5; ++i){
    myLed = 0;
    wait_ms(250);
    myLed = 1;
    wait_ms(250);
  }
  
  while(true){
    inputValue = readTemperature();
    if(inputValue > tempLimit){
      myLed = 0; // Open LED

      for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2){
        inputValue = readTemperature();
        if(inputValue < tempLimit){
          myLed = 1;
          break; // Stop the sound when temperature lower back.
        }
        // Calculates the duration of each note.
        divider = melody[thisNote + 1];
        if (divider > 0){
          noteDuration = (wholenote) / divider; // Regular note, just proceed.
        } 
        else if (divider < 0) {
          // Dotted notes are represented with negative durations.
          noteDuration = (wholenote) / abs(divider);
          noteDuration *= 1.5; // Increases the duration in half for dotted notes.
        }

        // We only play the note for 90% of the duration, leaving 10% as a pause
        buzzer.period(1.0f / melody[thisNote]); // Set period
        buzzer.write(0.9f); // Set duty cycle
        wait_ms(noteDuration);
        buzzer.write(0.0f); // Stop the waveform generation before the next note.
      }
      wait(0.5f);
    }
    else{
      myLed = 1;
    }
  }
  return 0;
}

float readTemperature()
{
  float inputValue{ thermistor.read() };
  float r2 = r1 * (1.0f/(inputValue) - 1);
  float logR2 = log(r2);
  float T = (1.0f / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T = T - 273.15f;
  //T = (T * 9.0)/ 5.0 + 32.0; 
  return T;
}
