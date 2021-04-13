#include <enc.hpp>

long oldPosition  = 0;

uint8_t encoderState = EncoderStates::Nothing;

long lastEncoderTime = millis();

void encoderUpdate(Encoder &encoder)
{
  long newPosition = encoder.read();

  if (newPosition != oldPosition) 
  {
    encoderState = (newPosition > oldPosition) ? EncoderStates::TurnRight : EncoderStates::TurnLeft;
    oldPosition = newPosition;
  } else {
    encoderState = EncoderStates::Nothing;
  }
}

EncoderStates encoderReadState()
{
  EncoderStates state = EncoderStates(encoderState);
  encoderState = EncoderStates::Nothing;
  return state;
}