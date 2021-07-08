#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// #include <Bounce2.h>
#include <IRremote.h>
#include <RTClib.h>

// #define RECV_PIN 2
#define RECV_PIN 36

#define yamaha1 0xFE808A75
#define yamaha2 0xFE804AB5
#define yamaha3 0xFE80CA35
#define yamaha4 0xFE802AD5
#define yamaha5 0xFE80AA55
#define yamaha6 0xFE806A95
#define yamaha7 0xFE80EA15
#define yamaha8 0xFE801AE5
#define yamaha9 0xFE809A65
#define yamaha0 0xFE805AA5
#define yamahaPlus10 0xFE80DA25
#define yamahaEnt 0xFE803AC5

#define samsungRed 0xE0E036C9
#define samsungTtx 0xE0E034CB
#define samsungEmode 0xE0E029D6

#define samsungGreen 0xE0E028D7
#define samsungYellow 0xE0E0A857
#define samsungBlue 0xE0E06897

// Bounce debouncer = Bounce();
IRrecv irrecv(RECV_PIN);
decode_results results;

#define NUMBER_OF_EVETROVALVONEZ 4

// int elettrovalvolezPins[][2] = {{4, 5}, {6, 7} , {8, 9}, {10, 11}};
int elettrovalvolezPins[][2] = {{16, 17}, {18, 19} , {25, 26}, {32, 33}};
boolean elettrovalvolezAreOpen[] = {false, false, false, false};

RTC_DS3231 rtc;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306* display;

// ESP32
// Wire.begin(21, 22);

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
// Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  for (int i = 0; i < NUMBER_OF_EVETROVALVONEZ; i++) {
    pinMode(elettrovalvolezPins[i][0], OUTPUT);
    pinMode(elettrovalvolezPins[i][1], OUTPUT);
  }

    Serial.begin(115200);
  // debouncer.attach(BUTTON, INPUT_PULLUP);
  // debouncer.interval(25);

  
  Wire.begin(21, 22);
  display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

  irrecv.enableIRIn(); // Start the receiver

  // Wire.begin(); // i2c discover adress
  
  if(!display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Display allocation failed"));
  }

/*  if(!display2.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) {
    Serial.println(F("Display 2 allocation failed"));
  }*/

  // Clear the buffer
  display->clearDisplay();
  // display2.clearDisplay();

  // if (! rtc.begin()) {
  //   Serial.println("Couldn't find RTC");
  //   Serial.flush();
  //   abort();
  // }

  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

}

void elettrovalvolaz(int number) {
  Serial.println(number);
  if (elettrovalvolezAreOpen[number]) {
    // Serial.println("closing elettrovalvolaz " + number);
    digitalWrite(elettrovalvolezPins[number][0], LOW);
    digitalWrite(elettrovalvolezPins[number][1], HIGH);
  } else {
    // Serial.println("opening elettrovalvolaz " + number);
    digitalWrite(elettrovalvolezPins[number][0], HIGH);
    digitalWrite(elettrovalvolezPins[number][1], LOW);
  }
  delay(100);
  digitalWrite(elettrovalvolezPins[number][0], LOW);
  digitalWrite(elettrovalvolezPins[number][1], LOW);
  // Serial.println("done");
  // Serial.println(elettrovalvolezPins[number][0]);
  // Serial.println(elettrovalvolezPins[number][1]);
  elettrovalvolezAreOpen[number] = !elettrovalvolezAreOpen[number];
}


// void scanI2c()
// {
//   byte error, address;
//   int nDevices;
//   Serial.println("Scanning...");
//   nDevices = 0;
//   for(address = 1; address < 127; address++ ) 
//   {
//     // The i2c_scanner uses the return value of
//     // the Write.endTransmisstion to see if
//     // a device did acknowledge to the address.
//     Wire.beginTransmission(address);
//     error = Wire.endTransmission();
//     if (error == 0)
//     {
//       Serial.print("I2C device found at address 0x");
//       if (address<16) 
//         Serial.print("0");
//       Serial.print(address,HEX);
//       Serial.println("  !");
//       nDevices++;
//     }
//     else if (error==4) 
//     {
//       Serial.print("Unknown error at address 0x");
//       if (address<16) 
//         Serial.print("0");
//       Serial.println(address,HEX);
//     }    
//   }
//   if (nDevices == 0)
//     Serial.println("No I2C devices found\n");
//   else
//     Serial.println("done\n");
//   delay(5000);           // wait 5 seconds for next scan
// }

void render(DateTime now, float temperature) {
  display->clearDisplay();
  // display->setTextSize(2); // Draw 2X-scale text
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(0, 0);

  display->println(F("Elettrovalvolaz"));

  display->print(now.day());
  display->print("/");
  display->print(now.month());
  display->print("/");
  display->print(now.year());
  display->print(" ");
  display->print(now.hour());
  display->print(":");
  display->print(now.minute());
  display->print(":");
  display->println(now.second());
  display->print("T: ");
  display->println(temperature);

  for (int i = 0; i < NUMBER_OF_EVETROVALVONEZ; i++) {
    display->print("i");
    display->print(i);
    display->print(" ");
    display->println(elettrovalvolezAreOpen[i]);
  }
  display->display(); // Show

  // display2.clearDisplay();
  // display2.setTextColor(SSD1306_WHITE);
  // display2.setCursor(0, 0);
  // display2.print("T: ");
  // display2.println(temperature);
  // display2.display(); // Show
}

void loop() {
  // debouncer.update();
  // if ( debouncer.fell() ) { 
  //   elettrovalvolaz(0);
  // }
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    if (results.value == yamaha1 || results.value == samsungRed) {
      elettrovalvolaz(0);
    } else if (results.value == yamaha2 || results.value == samsungGreen) {
      elettrovalvolaz(1);
    } else if (results.value == yamaha3 || results.value == samsungYellow) {
      elettrovalvolaz(2);
    } else if (results.value == yamaha4 || results.value == samsungBlue) {
      elettrovalvolaz(3);
    } else if (results.value == yamaha5) {
      elettrovalvolezAreOpen[0] = false;
      elettrovalvolaz(0);
    } else if (results.value == yamaha6) {
      elettrovalvolezAreOpen[1] = false;
      elettrovalvolaz(1);
    } else if (results.value == yamaha7) {
      elettrovalvolezAreOpen[2] = false;
      elettrovalvolaz(2);
    } else if (results.value == yamaha8) {
      elettrovalvolezAreOpen[3] = false;
      elettrovalvolaz(3);
    } else if (results.value == yamaha9) {
      elettrovalvolezAreOpen[0] = true;
      elettrovalvolaz(0);
    } else if (results.value == yamaha0) {
      elettrovalvolezAreOpen[1] = true;
      elettrovalvolaz(1);
    } else if (results.value == yamahaPlus10) {
      elettrovalvolezAreOpen[2] = true;
      elettrovalvolaz(2);
    } else if (results.value == yamahaEnt) {
      elettrovalvolezAreOpen[3] = true;
      elettrovalvolaz(3);
    }
    irrecv.resume(); // Receive the next value
  }

  // scanI2c();
  
  DateTime now = rtc.now();
  float temperature = rtc.getTemperature();
  
  render(now, temperature);

  delay(100);
}
 