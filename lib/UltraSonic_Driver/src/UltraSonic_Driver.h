// UltraSonic_Driver.h

// INCLUDES
#include <Arduino.h>

// DEFINES
#ifndef ULTRASONIC_DRIVER_H
#define ULTRASONIC_DRIVER_H


// ULTRASONIC CLASS
class UltraSonic {
  public:
    // Constructor: define the pins
    UltraSonic(uint8_t trigPin, uint8_t echoPin);
    
    // Initialize the ultrasonic sensor - called in setup()
    void begin();
    
    // Measure distance in cm
    long measureDistanceCM();

  private:
    uint8_t _trigPin, _echoPin;
};

#endif