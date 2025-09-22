Filename   : Pan/Tilt_Platform_Control


Controller : Freenove ESP32-S3-WROOM lite (FNK0099)


Description:
- Controls two MG90S continuous rotation servos.
- Reads analog values from a KY-023 joystick module.
- Displays directional percentages on a 0.91 inch OLED display.
- Toggles the onboard LED with the joystick's button.
- Switches between manual and auto mode with button release.


Notes :
- Uses the Freenove Breakout Board for ESP32 (FNK0091).
- Pan/Tilt platform used: https://www.aliexpress.com/item/1005008114555165.html
- 3D printed case for joystick: https://www.thingiverse.com/thing:700346
- 3D printed case for HC-SR04: https://www.thingiverse.com/thing:1349633
- 3D printed case for 0.91' OLED display: https://www.thingiverse.com/thing:3514326
- Connect the KY-023 module to the breakout board:
  - VCC to 3.3V
  - VRx to GPIO 10
  - VRy to GPIO 11
  - SW  to GPIO 12
- Connect the HC-SR04 ultrasonic sensor to the breakout board:
  - VCC  to 5V
  - Trig to GPIO 47
  - Echo to GPIO 48
  - GND  to GND
- Connect MG90S servos to the breakout board:
  - Left/Right servo to GPIO 17
  - Up/Down servo to GPIO 18
- Connect the 0.91 inch OLED display to the breakout board:
  - VCC to 3.3V
  - GND to GND
  - SCL to GPIO 4
  - SDA to GPIO 5
- Supply breakout board power via barrel plug (12V 2A).
