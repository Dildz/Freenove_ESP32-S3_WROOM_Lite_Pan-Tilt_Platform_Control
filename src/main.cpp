/****************************************************************************
  Filename   : Pan/Tilt_Platform_Control

  Controller : Freenove ESP32-S3-WROOM lite (FNK0099)
 
  Description: - Controls two MG90S continuous rotation servos.
               - Reads analog directional values from a KY-023 joystick module.
               - Displays directional percentages and distance measurements
                 on a 0.91 inch OLED display.
               - Joystick button turns on the on-board LED with button press
                 and changes screens on button release.

  Notes : - Uses the Freenove Breakout Board for ESP32 (FNK0091).
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
****************************************************************************/


// INCLUDES
#include "Display_SSD1306.h"
#include "Joystick_Driver.h"
#include "Servo_Driver.h"
#include "UltraSonic_Driver.h"

// DEFINES
#define ONBOARD_LED  2  // Built-in blue LED pin

// CLASS OBJECTS
Display display(128, 32, &Wire, -1);  // WIDTH, HEIGHT, Wire object, RESET pin
Joystick joystick(10, 11, 12);        // X pin, Y pin, Switch pin
UltraSonic ultrasonic(47, 48);        // Trig pin, Echo pin
ServoControl servos(17, 18);          // Pan pin, Tilt pin

// ENUMS
enum DisplayMode {
  DISPLAY_JOYSTICK_INFO,
  DISPLAY_DISTANCE_INFO
};
DisplayMode currentDisplayMode = DISPLAY_JOYSTICK_INFO;

// GLOBAL VARIABLES
bool lastButtonState = false;


// SETUP
void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("\nStarting Pan/Tilt Platform Control\n");

  // Initialize onboard LED
  Serial.println("Initializing on-board LED...");
  pinMode(ONBOARD_LED, OUTPUT);
  delay(100);

  // Initialize joystick
  Serial.println("Initializing joystick module...");
  joystick.begin();
  delay(100);

  // Initialize ultrasonic sensor
  Serial.println("Initializing ultrasonic module...");
  ultrasonic.begin();
  delay(100);
  
  // Initialize the display
  Serial.println("Initializing display...");
  if (!display.begin()) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Don't proceed, loop forever
  }
  Serial.println("OLED found at 0x3C!");
  delay(100);
  
  // Initialize servos
  Serial.println("Initializing servos...");
  servos.begin();
  delay(100);

  Serial.println("\nSetup completed! Starting main loop.\n");
  delay(1000);
}

// MAIN LOOP
void loop() {
  static unsigned long previousMillis = 0;
  const unsigned long updateInterval = 20;  // 20ms

  // Only execute the main code at the specified interval
  if (millis() - previousMillis >= updateInterval) {
    previousMillis = millis();

    // Measure distance
    long objectDistance = ultrasonic.measureDistanceCM();
    
    // Read and process the joystick values
    joystick.read();

    // Check for button release to toggle mode
    bool currentButtonState = joystick.isPressed();

    // If button was pressed and is now released, toggle display mode
    if (lastButtonState && !currentButtonState) {
      // Toggle display mode
      currentDisplayMode = (currentDisplayMode == DISPLAY_JOYSTICK_INFO) ? DISPLAY_DISTANCE_INFO : DISPLAY_JOYSTICK_INFO;
      
      Serial.print("Display mode changed to: ");
      Serial.println(currentDisplayMode == DISPLAY_JOYSTICK_INFO ? "SERVO INFO" : "DISTANCE INFO");
    }
    
    lastButtonState = currentButtonState;

    // Toggle onboard LED based on mode (ON in auto mode, OFF in manual)
    digitalWrite(ONBOARD_LED, currentButtonState ? HIGH : LOW);

    // Move servos based on joystick input values
    servos.moveManual(
      joystick.getUp(),
      joystick.getDown(), 
      joystick.getLeft(),
      joystick.getRight()
    );
    
    // Update display based on current mode
    if (currentDisplayMode == DISPLAY_JOYSTICK_INFO) {  // Show servo control information      
      display.updateServoInfo(
        joystick.getUp(),
        joystick.getDown(), 
        joystick.getLeft(),
        joystick.getRight(),
        joystick.isPressed()
      );
    }
    else {  // Show distance information      
      display.updateDistanceInfo(objectDistance);
    }

    // Print to serial for debugging - include distance in both modes
    const char* displayModeText = (currentDisplayMode == DISPLAY_JOYSTICK_INFO) ? "Servo Info" : "Distance Info";
        
    Serial.printf(
      "Raw X: %d, Raw Y: %d | UP: %d%%, DOWN: %d%%, LEFT: %d%%, RIGHT: %d%% | Button: %s | Display Mode: %s | Distance: %ldcm\n", 
      joystick.getRawX(),
      joystick.getRawY(), 
      joystick.getUp(),
      joystick.getDown(),
      joystick.getLeft(),
      joystick.getRight(),
      joystick.isPressed() ? "Pressed" : "Not Pressed",
      displayModeText,
      objectDistance
    );
  }
}
