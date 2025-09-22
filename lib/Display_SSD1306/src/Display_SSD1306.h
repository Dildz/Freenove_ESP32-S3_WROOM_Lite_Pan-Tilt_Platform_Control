// Display_SSD1306.h


// INCLUDES
#include <Adafruit_SSD1306.h>
#include <Wire.h>

// DEFINES
#ifndef DISPLAY_SSD1306_H
#define DISPLAY_SSD1306_H


// DISPLAY CLASS
class Display {
  public:
    // Constructor
    Display(int width, int height, TwoWire *wire, int resetPin, uint8_t i2cAddress = 0x3C);  // < See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
    
    // Initialize the display - call in setup()
    bool begin();

    // Update the display with joystick data (manual mode)
    void updateServoInfo(int up, int down, int left, int right, bool buttonPressed);
    
    // Update the display with ultrasonic data (auto mode)
    void updateDistanceInfo(long distance);

  private:
    // Private member variables
    Adafruit_SSD1306 _display; 
    uint8_t _i2cAddress;       
    int _scrollOffset = 0;  // Member to track scroll position
    void drawScrollingText(const String& text, int y, int offset = 0);  // Member to draw scrolling text
};

#endif