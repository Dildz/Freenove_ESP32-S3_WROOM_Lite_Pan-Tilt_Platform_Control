// Display_SSD1306.cpp


// INCLUDES
#include "Display_SSD1306.h"

// DEFINES
#define I2C_SDA_PIN  5
#define I2C_SCL_PIN  4
#define I2C_CLOCK_SPEED  400000  // 400kHz

// CONSTRUCTOR
Display::Display(int width, int height, TwoWire *wire, int resetPin, uint8_t i2cAddress)
  : _display(width, height, wire, resetPin), _i2cAddress(i2cAddress), _scrollOffset(0) {
    // The member initializer list already handles the construction,
    // this line is redundant and could cause issues.
    //_display = Adafruit_SSD1306(width, height, wire, resetPin);
}

// Set a consistent character width (6 pixels for Adafruit font size 1)
const int characterWidth = 6;

// Initialize the display
bool Display::begin() {
  // Initialize I2C with defined pins & clock
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.setClock(I2C_CLOCK_SPEED);

  // Set rotation first if using 180° otherwise logo is at default
  _display.setRotation(2);  // 0=0°, 1=90°, 2=180°, 3=270°

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  bool success = _display.begin(SSD1306_SWITCHCAPVCC, _i2cAddress);  // Member variable _i2cAddress
  if (success) {
    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    _display.display();
    delay(2000);  // Pause for 2 seconds

    /* 
    You MUST call display() after drawing commands
    to make them visible on screen!
    */

    // Show a simple startup message
    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setTextColor(SSD1306_WHITE);
    _display.setCursor(0,0);
    _display.println("Freenove ESP32-S3");
    _display.setCursor(0,10);
    _display.println("Tilt/Pan Ctrl Ready!");
    _display.display();
    delay(2000);
  }
  return success;
}

// Draw scrolling text without wrapping - updated to support multiple instances
void Display::drawScrollingText(const String& text, int y, int offset) {
  _display.setTextWrap(false);  // Prevent automatic wrapping
  int textPixelWidth = text.length() * characterWidth;  // Calculate total pixel width of the string
  int startCharIndex = max(0, -(_scrollOffset + offset) / characterWidth);  // Calculate the start and end indices of the visible part of the string
  int endCharIndex = min((int)text.length(), (_display.width() - (_scrollOffset + offset)) / characterWidth + 1);  // Cast text.length() to int
  
  // Draw the visible part of the string
  if (startCharIndex < endCharIndex) {
    String visibleText = text.substring(startCharIndex, endCharIndex);
    _display.setCursor(_scrollOffset + offset + startCharIndex * characterWidth, y);
    _display.print(visibleText);
  }
}

// Update the display with joystick data (manual mode)
void Display::updateServoInfo(int up, int down, int left, int right, bool buttonPressed) {
  _display.clearDisplay();
  _display.setTextSize(1);
  _display.setTextColor(SSD1306_WHITE);
  
  // Create a scolling string
  String scrollingLine = "Button: ";
  scrollingLine += (buttonPressed ? "PRESSED" : "RELEASED");
  scrollingLine += " | Manual";

  int textPixelWidth = scrollingLine.length() * characterWidth;
  int secondInstanceOffset = textPixelWidth + (5 * characterWidth); // 5 characters spacing - adjust as needed

  // Update the scroll offset
  if (!buttonPressed) {
    _scrollOffset--;

    /* Continuously scroll by wrapping the offset -
       when it goes too far negative, wrap it around to create seamless scrolling */
    if (_scrollOffset < -textPixelWidth) {
      _scrollOffset += textPixelWidth + (4 * characterWidth);  // Wrap around
    }
  }
  else {
    // Snap to position 0 when button is pressed
    _scrollOffset = 0;
  }
  
  // Draw multiple instances to create continuous scrolling effect across the screen
  int totalWidthNeeded = _display.width() + textPixelWidth + (4 * characterWidth);
  int numInstances = ceil((float)totalWidthNeeded / (textPixelWidth + (4 * characterWidth))) + 1;
  
  for (int i = 0; i < numInstances; i++) {
    int instanceOffset = i * (textPixelWidth + (4 * characterWidth));
    drawScrollingText(scrollingLine, 0, instanceOffset);
  }
  
  // Display UP/DOWN values
  _display.setCursor(0, 12);
  _display.print("UP:  ");
  _display.print(up);
  _display.print("%");
  
  _display.setCursor(64, 12);
  _display.print("DWN: ");
  _display.print(down);
  _display.print("%");
  
  // Display LEFT/RIGHT values
  _display.setCursor(0, 24);
  _display.print("LFT: ");
  _display.print(left);
  _display.print("%");
  
  _display.setCursor(64, 24);
  _display.print("RHT: ");
  _display.print(right);
  _display.print("%");

  _display.display();
}

// Update the display with ultrasonic data (auto mode)
void Display::updateDistanceInfo(long distance) {
  _display.clearDisplay();
  _display.setTextSize(1);
  _display.setTextColor(SSD1306_WHITE);

  // Display distance value on line 1
  _display.setCursor(0, 0);
  _display.print("Dist: ");
  _display.print(distance);
  _display.print("cm");

  // Auto Mode text on right side of line 1
  _display.setCursor(82, 0);
  _display.print("|");
  _display.setCursor(104, 0);
  _display.print("Auto");
  
  // Draw a bar graph using lines 2 and 3
  int barWidth = map(distance, 0, 400, 0, 128);  // Map distance (0-400cm) to display width
  if (barWidth > 128) barWidth = 128;  // Cap at maximum width
  
  // Draw bar outline
  _display.drawRect(0, 12, 128, 10, SSD1306_WHITE);
  
  // Fill the bar proportional to distance
  if (barWidth > 2) {  // Only fill if bar has visible width
    _display.fillRect(1, 13, barWidth - 2, 8, SSD1306_WHITE);
  }
  
  // Add scale labels: 0, 200, 400
  _display.setTextSize(1);
  _display.setCursor(0, 24);    // Left
  _display.print("0");
  
  _display.setCursor(52, 24);   // Middle
  _display.print("200");
  
  _display.setCursor(110, 24);  // Right
  _display.print("400");
  
  // Update the display
  _display.display();
}