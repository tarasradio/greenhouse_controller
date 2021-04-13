#include <Arduino.h>
#include <Encoder.h>

extern long lastEncoderTime;

enum EncoderStates {
  Nothing,
  TurnLeft,
  TurnRight
};

void encoderUpdate(Encoder &encoder);
EncoderStates encoderReadState();