#include <Arduino.h>

#define ledPin 13

char readState;
char ledOn[] = {'L', 'E', 'D', ' ', 'O', 'N'};
char ledOff[] = {'L', 'E', 'D', ' ', 'O', 'F', 'F'};
int index = 0;

void setup()
{
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  if (Serial.available() > 0)
  {
    readState = Serial.read();
    if (readState == ledOn[index])
    {
      index++;
      if (index == 6)
      {
        digitalWrite(ledPin, HIGH);
        index = 0;
      }
    }
    else if (readState == ledOff[index])
    {
      index++;
      if (index == 7)
      {
        digitalWrite(ledPin, LOW);
        index = 0;
      }
    }
    else
    {
      index = 0;
    }
  }
}
