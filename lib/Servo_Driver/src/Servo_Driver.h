// Servo_Driver.h


// INCLUDES
#include <ESP32Servo.h>

// DEFINES
#ifndef SERVO_DRIVER_H
#define SERVO_DRIVER_H

/*     
 * FWD & REV values are different because they seem to move at different speeds even if the same value is used.
 * My best attempt to match speeds:
 * STOP = 90 | FWD = 103 (180 max) | REV = 86 (0 max)
 */

// SERVO CONTROL CLASS
class ServoControl {
  public:
    // Constructor: define the pins and movement values
    ServoControl(uint8_t panPin, uint8_t tiltPin, int stopVal = 90, int fwdVal = 103, int revVal = 86);
    
    // Attach the servos - called in setup()
    void begin();
    
    // Control the servos based on Manual or Auto mode
    void moveManual(int up, int down, int left, int right);
    void moveAuto(bool up, bool down, bool left, bool right);

  private:
    Servo _servoLR; // Pan (Left/Right)
    Servo _servoUD; // Tilt (Up/Down)
    uint8_t _panPin, _tiltPin;
    int _stop, _fwd, _rev;
};

#endif