#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);


const int alcoholSensorPin = A0;


const int alcoholThreshold = 500;

// Relay pin
const int relayPin = 8;

// Buzzer pin
const int buzzerPin = 9;


SoftwareSerial sim800(5, 6); 

void sendSMS() {
  sim800.println("AT+CMGF=1"); 
  delay(1000);
  sim800.println("AT+CMGS=\"9067558597\""); 
  sim800.println("Alcohol detected! Please take action."); 
  delay(1000);
  sim800.println((char)26); 
  delay(5000);
}

void setup() {
  
  lcd.init();
  lcd.backlight();
  
  
  lcd.setCursor(0, 0);
  lcd.print("Alcohol Detector");
  delay(2000); 
  
  
  lcd.clear();
  
  Serial.begin(9600);
  Serial.println("Alcohol Detection System Ready!");

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); 

  
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW); 

  sim800.begin(9600);
  delay(2000);
}

void loop() {
  int sensorValue = analogRead(alcoholSensorPin);
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);

  lcd.setCursor(0, 0);
  if (sensorValue > alcoholThreshold) {
    lcd.print("Alcohol Detected!");
    lcd.setCursor(0, 1);
    lcd.print("Level: ");
    lcd.print(sensorValue);

    digitalWrite(relayPin, LOW);

    digitalWrite(buzzerPin, HIGH);

    // Send SMS alert
    sendSMS();
  } else {
    lcd.print("No Alcohol Found ");
    lcd.setCursor(0, 1);
    lcd.print("                "); 

  
    digitalWrite(relayPin, HIGH);

    digitalWrite(buzzerPin, LOW);
  }

  delay(500);
}
