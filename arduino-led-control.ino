/*
   Control LED strip over serial port \w save data to EEPROM.
   See `usageText` (`printUsage` fn) for more detail.
   (based on Keith Lord example sketch)

   @author: qbbr
*/

#include <EEPROM.h>

#define REDUCED_MODES // sketch too big for Arduino Leonardo flash, so invoke reduced modes
#include <WS2812FX.h>

#define LED_COUNT 30
#define LED_PIN 12
#define MAX_NUM_CHARS 16 // maximum number of characters read from the Serial Monitor

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

char cmd[MAX_NUM_CHARS];      // char[] to store incoming serial commands
boolean cmdComplete = false;  // whether the command string is complete

// EEPROM addresses
#define ADDRESS_COLOR_RED 0
#define ADDRESS_COLOR_GREEN 1
#define ADDRESS_COLOR_BLUE 2
#define ADDRESS_MODE 3
#define ADDRESS_BRIGHTNESS 4
#define ADDRESS_SPEED 5

byte currentRedColor; // 0 - 255
byte currentGreenColor;
byte currentBlueColor;
byte currentMode;
byte currentBrightness;
int currentSpeed; // 10 - 65535

void setup() {
  Serial.begin(115200);

  //printAllEEPROM();

  // color
  EEPROM.get(ADDRESS_COLOR_RED, currentRedColor);
  EEPROM.get(ADDRESS_COLOR_GREEN, currentGreenColor);
  EEPROM.get(ADDRESS_COLOR_BLUE, currentBlueColor);

  // mode
  currentMode = EEPROM.read(ADDRESS_MODE);

  if (255 == currentMode) {
    currentMode = FX_MODE_STATIC; // by default
  }

  // brightness
  currentBrightness = EEPROM.read(ADDRESS_BRIGHTNESS);

  // speed
  EEPROM.get(ADDRESS_SPEED, currentSpeed);

  // init led
  ws2812fx.init();
  ws2812fx.setBrightness(currentBrightness);
  ws2812fx.setSpeed(currentSpeed);
  ws2812fx.setColor(currentRedColor, currentGreenColor, currentBlueColor);
  ws2812fx.setMode(currentMode);
  ws2812fx.start();

  //printModes();
  //printUsage();
  printVars();
}

void loop() {
  ws2812fx.service();

  recvChar(); // read serial cmd

  if (cmdComplete) {
    processCommand();
  }
}

/*
   Checks received command and calls corresponding functions.
*/
void processCommand() {
  if (strncmp(cmd, "h", 1) == 0) {
    printUsage();
  } else if (strncmp(cmd, "v", 1) == 0) {
    printVars();
  } else if (strncmp(cmd, "c ", 2) == 0) {
    String c = String(cmd).substring(2);
    currentRedColor = getValue(c, '.', 0).toInt();
    currentGreenColor = getValue(c, '.', 1).toInt();
    currentBlueColor = getValue(c, '.', 2).toInt();
    EEPROM.update(ADDRESS_COLOR_RED, currentRedColor);
    EEPROM.update(ADDRESS_COLOR_GREEN, currentGreenColor);
    EEPROM.update(ADDRESS_COLOR_BLUE, currentBlueColor);
    ws2812fx.setColor(currentRedColor, currentGreenColor, currentBlueColor);
    //Serial.print(F("Set color to: 0x"));
    //Serial.println(ws2812fx.getColor(), HEX);
  } else if (strncmp(cmd, "m ", 2) == 0) {
    currentMode = atoi(cmd + 2);
    EEPROM.update(ADDRESS_MODE, currentMode);
    ws2812fx.setMode(currentMode);
    //Serial.print(F("Set mode to: "));
    //Serial.print(ws2812fx.getMode());
    //Serial.print(" - ");
    //Serial.println(ws2812fx.getModeName(ws2812fx.getMode()));
  } else if (strncmp(cmd, "b ", 2) == 0) {
    currentBrightness = atoi(cmd + 2);
    Serial.println(currentBrightness);
    EEPROM.update(ADDRESS_BRIGHTNESS, currentBrightness);
    ws2812fx.setBrightness(currentBrightness);
    //Serial.print(F("Set brightness to: "));
    //Serial.println(ws2812fx.getBrightness());
  } else if (strncmp(cmd, "s ", 2) == 0) {
    currentSpeed = atoi(cmd + 2);
    EEPROM.put(ADDRESS_SPEED, currentSpeed);
    ws2812fx.setSpeed(currentSpeed);
    //Serial.print(F("Set speed to: "));
    //Serial.println(ws2812fx.getSpeed());
  }

  cmd[0] = '\0';       // reset the commandstring
  cmdComplete = false; // reset command complete
}

/*
   Print usage.
*/
const char usageText[] PROGMEM = R"=====(
usage:
 h         - display this help
 v         - print vars (@var: value)
 c <R.G.B> - set color
 m <n>     - set mode
 b <n>     - set brightness
 s <n>     - set speed
)=====";

void printUsage() {
  Serial.println((const __FlashStringHelper *)usageText);
}


/*
   Print all available modes.
*/
//void printModes() {
//  Serial.println(F("Supporting the following modes: "));
//  Serial.println();
//  for (int i = 0; i < ws2812fx.getModeCount(); i++) {
//    Serial.print(i);
//    Serial.print(F("\t"));
//    Serial.println(ws2812fx.getModeName(i));
//  }
//  Serial.println();
//}


/*
   Reads new input from serial to cmd string. Command is completed on \n.
*/
void recvChar(void) {
  static byte index = 0;
  while (Serial.available() > 0 && cmdComplete == false) {
    char rc = Serial.read();
    if (rc != '\n'/* && rc != '\r'*/) {
      if (index < MAX_NUM_CHARS) cmd[index++] = rc;
    } else {
      cmd[index] = '\0'; // terminate the string
      index = 0;
      cmdComplete = true;
      Serial.print("< received '");
      Serial.print(cmd);
      Serial.println("'");
    }
  }
}

/*
   Read all EEPROM data.
 */
//void printAllEEPROM() {
//    int address = 0;
//    byte value;
//
//    while (address < EEPROM.length()) {
//        value = EEPROM.read(address);
//        Serial.print("Address: ");
//        Serial.print(String(address));
//        Serial.print(", value: ");
//        Serial.println(String(value));
//        address++;
//    }
//}

/*
   Print vars for parse them in python.
 */
void printVars() {
  Serial.print(F("@COLOR_RED: "));
  Serial.println(currentRedColor);
  Serial.print(F("@COLOR_GREEN: "));
  Serial.println(currentGreenColor);
  Serial.print(F("@COLOR_BLUE: "));
  Serial.println(currentBlueColor);

  Serial.print(F("@MODE: "));
  Serial.println(currentMode);

  Serial.print(F("@BRIGHTNESS: "));
  Serial.println(currentBrightness);

  Serial.print(F("@SPEED: "));
  Serial.println(currentSpeed);
}

/*
   For splitting RGB color from python.
 */
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
