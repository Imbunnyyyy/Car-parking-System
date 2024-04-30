
// Include the library files
#define BLYNK_TEMPLATE_ID "TMPL3gSx9JmZd"
#define BLYNK_TEMPLATE_NAME "Car Parking System"
#define BLYNK_AUTH_TOKEN "P7tartN-vQwXF1mKLb66gCO3RE22UsX9"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
Servo servo;

// Create an object for the BMP180 sensor
int IR1 = D1; // choose input pin (for Infrared sensor)
int IR2 = D2; // choose input pin (for Infrared sensor)
int IR3 = D7;
int IR4 = D5; // choose input pin (for Infrared sensor)

char auth[] = "P7tartN-vQwXF1mKLb66gCO3RE22UsX9"; // Enter your Auth token
char ssid[] = "Bunny";                           // Enter your WIFI name
char pass[] = "12345678";                         // Enter your WIFI password

BlynkTimer timer;

void setup() {
  Serial.begin(115200);

  Wire.begin(D6, D8); // Use predefined PINS consts
  lcd.begin(20, 4);   // The begin call takes the width and height.
  lcd.backlight();    // Turn on the backlight.
  lcd.setCursor(4, 1); // Move the cursor at origin
  lcd.print("CAR PARKING");
  lcd.setCursor(6, 2);
  lcd.print("SYSTEM");
  delay(4000);
  lcd.clear();

  pinMode(IR1, INPUT); // declare Infrared sensor as input
  pinMode(IR2, INPUT); // declare Infrared sensor as input
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);

  servo.attach(D4); // D4
  servo.write(0);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  // Call the functions
  timer.setInterval(100L, Slot1);
  timer.setInterval(100L, Slot2);
  timer.setInterval(100L, Slot3);
}

void Slot1() {
  int value1 = digitalRead(IR1);
  if (value1 == 0) {
    WidgetLED LED(V0);
    LED.off();
  } else {
    WidgetLED LED(V0);
    LED.on();
  }
}

void Slot2() {
  int value2 = digitalRead(IR2);
  if (value2 == 0) {
    WidgetLED LED(V1);
    LED.off();
  } else {
    WidgetLED LED(V1);
    LED.on();
  }
}

void Slot3() {
  int value3 = digitalRead(IR3);
  if (value3 == 0) {
    WidgetLED LED(V2);
    LED.off();
  } else {
    WidgetLED LED(V2);
    LED.on();
  }
}

void loop() {
  Blynk.run(); // Run the Blynk library
  timer.run(); // Run the Blynk timer

  int slotsAvailable = 0;

  if (digitalRead(IR1) == HIGH) {
    slotsAvailable++;
  }
  if (digitalRead(IR2) == HIGH) {
    slotsAvailable++;
  }
  if (digitalRead(IR3) == HIGH) {
    slotsAvailable++;
  }

  if (slotsAvailable == 0) {
    lcd.clear();
    lcd.setCursor(8, 1);
    lcd.print("Sorry ):");
    lcd.setCursor(5, 1);
    lcd.print("Parking Is Full");
    delay(5000);
    // Stop the servo motor
    servo.write(0);
  } else {
    // Check if entry is blocked
    if (digitalRead(IR4) == LOW) {
      servo.write(180);
      delay(3000);
      servo.write(0);
      
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Available Slots:");
    lcd.setCursor(0, 1);
    lcd.print("Slot 1: ");
    lcd.print(digitalRead(IR1) == HIGH ? "Empty" : "Occupied");
    lcd.setCursor(0, 2);
    lcd.print("Slot 2: ");
    lcd.print(digitalRead(IR2) == HIGH ? "Empty" : "Occupied");
    lcd.setCursor(0, 3);
    lcd.print("Slot 3: ");
    lcd.print(digitalRead(IR3) == HIGH ? "Empty" : "Occupied");

    delay(1000);
  }
}