// Joystick_Driver.cpp


// INCLUDES
#include "Joystick_Driver.h"

// DEFINES
#define MAX_VALUE  4095  // Maximum ADC value
#define MIN_VALUE  0     // Minimum ADC value

// CONSTRUCTOR
Joystick::Joystick(uint8_t pinX, uint8_t pinY, uint8_t pinSW, int restingVal, int deadzone) 
  : _pinX(pinX), _pinY(pinY), _pinSW(pinSW), _restingValue(restingVal), _deadzoneValue(deadzone) {
}


// Initialize the joystick pins
void Joystick::begin() {
  pinMode(_pinX, INPUT);
  pinMode(_pinY, INPUT);
  pinMode(_pinSW, INPUT_PULLUP);
}

// Read and process the raw values
void Joystick::read() {
    _rawX = analogRead(_pinX);
    _rawY = analogRead(_pinY);
    _pressed = (digitalRead(_pinSW) == LOW); // Active LOW

    // Scale values to percentages
    _up = scaleDirection(_rawX, true);
    _down = scaleDirection(_rawX, false);
    _left = scaleDirection(_rawY, false);
    _right = scaleDirection(_rawY, true);
}

// Getters for the scaled values and button state
int Joystick::getUp() { return _up; }
int Joystick::getDown() { return _down; }
int Joystick::getLeft() { return _left; }
int Joystick::getRight() { return _right; }
bool Joystick::isPressed() { return _pressed; }
int Joystick::getRawX() { return _rawX; }
int Joystick::getRawY() { return _rawY; }

// Scaling function
int Joystick::scaleDirection(int value, bool isUpOrLeft) {
  // Apply deadzone - if value is within deadzone range, return 0
  if (abs(value - _restingValue) <= _deadzoneValue) {
    return 0;
  }

  // For UP and LEFT directions
  if (isUpOrLeft) {
    if (value >= (_restingValue + _deadzoneValue)) {
      // Scale from (RESTING_VALUE + DEADZONE) to MAX_VALUE to 0 to 100
      return map(value, _restingValue + _deadzoneValue, MAX_VALUE, 0, 100);
    }
    else {
      return 0;  // Not moving in this direction
    }
  }

  // For DOWN and RIGHT directions
  else {
    if (value <= (_restingValue - _deadzoneValue)) {
      // Scale from (RESTING_VALUE - DEADZONE) to 0 to 0 to 100 (inverted)
      return map(value, MIN_VALUE, _restingValue - _deadzoneValue, 100, 0);
    }
    else {
      return 0;  // Not moving in this direction
    }
  }
}