#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#define Password_Length 8
char Data[Password_Length];
char Master[Password_Length] = "123A456";
int lockOutput = 13;
byte data_count = 0;
char customKey;
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2);
int val;
int tempPin = A0;
int ledt=10;
int ledl=11;
void setup()
{
Serial.begin(9600);  
lcd.backlight();
lcd.init();  
pinMode(lockOutput, OUTPUT);
pinMode(ledt,OUTPUT);
pinMode(ledl,OUTPUT);
digitalWrite(ledt,LOW);
}

void clearData() {
  // Go through array and clear data
  while (data_count != 0) {
    Data[data_count--] = 0;
  }
  return;
}
void loop()
{
  //task 1 temperature
val = analogRead(tempPin);
float mv = ( val/1024.0)*5000; 
float cel = mv/10;
if(cel>30)
{
  digitalWrite(ledt,HIGH);
}
else
{
  digitalWrite(ledt,LOW);
}

   //task 2 light
 float sensorValue = analogRead(A1);
  // Reads the analog input ranging from 0 to 1023
  Serial.println(sensorValue);
  if(sensorValue<=400)
  {
    digitalWrite(ledl,HIGH);
  }
  else
  {
  digitalWrite(ledl,LOW);
  }

  //task 3 lock system

  lcd.setCursor(0, 0);
  lcd.print("Enter Password:");
 
  // Look for keypress
  customKey = customKeypad.getKey();
  if (customKey) {
    // Enter keypress into array and increment counter
    Data[data_count] = customKey;
    lcd.setCursor(data_count, 1);
    lcd.print(Data[data_count]);
    data_count++;
  }
 
  // See if we have reached the password length
  if (data_count == Password_Length - 1) {
    lcd.clear();
 
    if (!strcmp(Data, Master)) {
      // Password is correct
      lcd.print("Correct");
      // Turn on relay for 5 seconds
      digitalWrite(lockOutput, HIGH);
      delay(5000);
      digitalWrite(lockOutput, LOW);
    }
    else {
      // Password is incorrect
      lcd.print("Incorrect");
      delay(1000);
    }
 
    // Clear data and LCD display
    lcd.clear();
    clearData();
  }
}  
 
/*void clearData() {
  // Go through array and clear data
  while (data_count != 0) {
    Data[data_count--] = 0;
  }
  return;
}
}*/
  
