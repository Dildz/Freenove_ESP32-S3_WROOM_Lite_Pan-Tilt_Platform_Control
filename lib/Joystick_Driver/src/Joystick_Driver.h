// Joystick_Driver.h


// INCLUDES
#include <Arduino.h>

// DEFINES
#ifndef JOYSTICK_DRIVER_H
#define JOYSTICK_DRIVER_H


// JOYSTICK CLASS
class Joystick {
  public:
    // Constructor: define the pins
    Joystick(uint8_t pinX, uint8_t pinY, uint8_t pinSW, int restingVal = 1900, int deadzone = 40);
    
    // Initialize the joystick - called in setup()
    void begin();
    
    // Read and process the raw values
    void read();
    
    // Get the scaled percentage values
    int getUp();
    int getDown();
    int getLeft();
    int getRight();
    bool isPressed();

    // Get the raw values for debugging
    int getRawX();
    int getRawY();

  private:
    uint8_t _pinX, _pinY, _pinSW;
    int _restingValue, _deadzoneValue;
    
    int _rawX = 0, _rawY = 0;
    bool _pressed = false;
    int _up = 0, _down = 0, _left = 0, _right = 0;

    // The scaling function, now a private method
    int scaleDirection(int value, bool isUpOrLeft);
};

#endif