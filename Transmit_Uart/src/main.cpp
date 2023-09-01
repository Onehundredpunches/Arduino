#include <Arduino.h>

#define ledButton 13

void setup()
{
  Serial.begin(9600);
  pinMode(ledButton, INPUT);
}

void loop()
{
  int buttonState = digitalRead(ledButton);
  if (buttonState == HIGH)
  {
    Serial.write('L'); 
    Serial.write('E');
    Serial.write('D');
    Serial.write(' ');
    Serial.write('O');
    Serial.write('N');
  }
  else if (buttonState == LOW)
  {
    Serial.write('L');
    Serial.write('E');  
    Serial.write('D');
    Serial.write(' ');
    Serial.write('O');
    Serial.write('F');
    Serial.write('F');
  }
  delay(200);
}
