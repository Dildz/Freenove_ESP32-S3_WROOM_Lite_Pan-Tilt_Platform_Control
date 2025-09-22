// Servo_Driver.cpp


// INCLUDES
#include "Servo_Driver.h"

// CONSTRUCTOR
ServoControl::ServoControl(uint8_t panPin, uint8_t tiltPin, int stopVal, int fwdVal, int revVal)
  : _panPin(panPin), _tiltPin(tiltPin), _stop(stopVal), _fwd(fwdVal), _rev(revVal) {
}


// Attach the servos to their pins
void ServoControl::begin() {
  _servoLR.attach(_panPin, 500, 2400);  // GPIO 17, min 500µs, max 2400µs pulse
  _servoUD.attach(_tiltPin, 500, 2400); // GPIO 18, min 500µs, max 2400µs pulse
    
  // Start with servos stopped
  _servoLR.write(_stop);
  _servoUD.write(_stop);
}

// Control the servos in Manual Mode based on direction percentages
void ServoControl::moveManual(int up, int down, int left, int right) {
  // Up/Down servo control (Tilt)
  if (up > 20) {         // Up - move forward
    _servoUD.write(_fwd);
  }
  else if (down > 20) {  // Down - move reverse
    _servoUD.write(_rev);
  }
  else {                 // Stop
    _servoUD.write(_stop);
  }

  // Left/Right servo control (Pan)
  if (left > 20) {        // Left - move reverse
    _servoLR.write(_rev);
  }
  else if (right > 20) {  // Right - move forward
    _servoLR.write(_fwd);
  }
  else {                  // Stop
    _servoLR.write(_stop);
  }
}

// Control the servos in Auto Mode
void ServoControl::moveAuto(bool up, bool down, bool left, bool right) {
  // Up/Down servo control (Tilt)
  if (up) {         // Up - move forward
    _servoUD.write(_fwd);
  }
  else if (down) {  // Down - move reverse
    _servoUD.write(_rev);
  }
  else {            // Stop
    _servoUD.write(_stop);
  }

  // Left/Right servo control (Pan)
  if (left) {       // Left - move reverse
    _servoLR.write(_rev);
  }
  else if (right) {  // Right - move forward
    _servoLR.write(_fwd);
  }
  else {             // Stop
    _servoLR.write(_stop);
  }
}
