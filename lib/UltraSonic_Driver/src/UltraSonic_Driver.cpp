// UltraSonic_Driver.cpp

// INCLUDES
#include "UltraSonic_Driver.h"

// CONSTRUCTOR
UltraSonic::UltraSonic(uint8_t trigPin, uint8_t echoPin)
  : _trigPin(trigPin), _echoPin(echoPin) {
}

// Initialize the ultrasonic sensor
void UltraSonic::begin() {
  pinMode(_trigPin, OUTPUT);
  pinMode(_echoPin, INPUT);
}

// Measure distance in cm
long UltraSonic::measureDistanceCM() {
  long duration;
  float distance;

  // Trigger the sensor
  digitalWrite(_trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(_trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(_trigPin, LOW);

  // Read the Echo pin
  duration = pulseIn(_echoPin, HIGH);

  // Calculate distance
  distance = duration * 0.034 / 2;

  // Cap the distance values
  if (distance < 2.0) {         // Less than 2cm reads as 0
    distance = 0.0;
  }
  else if (distance > 400.0) {  // Greater than 400cm reads as 400
    distance = 400.0;
  }

  return (long)distance;
}