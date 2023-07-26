#include <Arduino.h>

#define clockPin  6
#define latchPin  5
#define dataPin  7

uint8_t ledNumberTable[] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0,0xFE , 0xF6, 0x00};
uint8_t posotionLedTable[] = {0x00, 0x80, 0x40, 0x20, 0x10};

unsigned long start_time;
unsigned long end_time;
unsigned long time_per_cycle;

void CLK(void){
  digitalWrite(clockPin, 1);
  delayMicroseconds(100);
  digitalWrite(clockPin, 0);
}

// set number value and position for segmnent led

void numberPosition4SegmentLed(uint8_t number, uint8_t position)
{
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(dataPin, (posotionLedTable[position] >> i) & 0x01 );
    CLK();
  }

  for (int i = 0; i < 8; i++)
  {
    digitalWrite(dataPin, (ledNumberTable[number] >> i) & 0x01 );
    CLK();
  }

  delay(1);
  digitalWrite(latchPin, 1);
  delayMicroseconds(100);
  digitalWrite(latchPin, 0);
}

void setup() {
  pinMode(clockPin,OUTPUT);
  pinMode(latchPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
 
}

void loop() {
  if (start_time == 0)
  {
    start_time = millis();
  }

  for ( int num = 0; num <10000; num++)
  {
    for (int i = 0; i < 45; i++)
    {
      numberPosition4SegmentLed(num/1000, 1);
      delayMicroseconds(2);
      numberPosition4SegmentLed(10, 1);

      numberPosition4SegmentLed(num/100%10, 2);
      delayMicroseconds(2);
      numberPosition4SegmentLed(10, 2);

      numberPosition4SegmentLed(num/10%10, 3);
      delayMicroseconds(2);
      numberPosition4SegmentLed(10, 3);

      numberPosition4SegmentLed(num%10, 4);
      delayMicroseconds(2);
      numberPosition4SegmentLed(10, 4);
    }
  }
  end_time = millis();

  // Calculate time count a cycle
  time_per_cycle = (end_time - start_time);

  Serial.print("Time per cycle: ");
  Serial.print(time_per_cycle);
  Serial.println(" ms");

  // Reset time
  start_time = 0;
  
}

