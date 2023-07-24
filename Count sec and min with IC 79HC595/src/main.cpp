#include <Arduino.h>
#include <TimerOne.h>

#define clockPin  6
#define latchPin  5
#define dataPin  7

unsigned long start_time;
unsigned long end_time;
unsigned long time_per_cycle;

uint8_t dataValue = 0;
uint8_t dataTable[] = {0,0,0,0,0,0,0,0};
uint8_t ledNumberTable[] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0,0xFE , 0xF6, 0x00};
uint8_t ledNumberTable_Dot[] = {0xFD, 0x61, 0xDB, 0xF3, 0x67, 0xB7, 0xBF, 0xE1,0xFF , 0xF7, 0x00};
uint8_t posotionLedTable[] = {0x00, 0x80, 0x40, 0x20, 0x10};

uint32_t second = 0;
uint32_t minute = 0;
uint32_t count = 0;
uint32_t positionLed7 = 4;


void CLK(void){
  digitalWrite(clockPin, 1);
  delayMicroseconds(100);
  digitalWrite(clockPin, 0);
}

// set number value and position for segmnent led without DOT

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

// set number value and position for segmnent led with DOT

void segLedDot(uint8_t num, uint8_t pos)
{
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(dataPin, (posotionLedTable[pos] >> i) & 0x01 );
    CLK();
  }

  for (int i = 0; i < 8; i++)
  {
    digitalWrite(dataPin, (ledNumberTable_Dot[num] >> i) & 0x01 );
    CLK();
  }

  delay(1);
  digitalWrite(latchPin, 1);
  delayMicroseconds(100);
  digitalWrite(latchPin, 0);
}

/*
// set number value for segmnent led
void ledSegmentNumber(uint8_t num)
{
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(dataPin, (ledNumberTable[num] >> i) & 0x01 );
    CLK();
  }
  digitalWrite(latchPin, 1);
  delayMicroseconds(100);
  digitalWrite(latchPin, 0);
}


// set signal for output
void shiftLedBit(uint8_t pos, uint8_t bitValue)
{
  dataTable[pos] = bitValue;
  dataValue = dataValue | dataTable[0];
  for (int i = 1; i < 8; i++)
  {
    dataValue = (dataValue << 1) | dataTable[i];
  }

  for (int i = 0; i < 8; i++)
  {
    digitalWrite(dataPin, (dataValue >> i) & 0x01);
    CLK(); 
  }
  
  digitalWrite(latchPin, 1);
  delayMicroseconds(100);
  digitalWrite(latchPin, 0);
}
*/


void countTime(void)
{ 
  if( positionLed7 == 4)
  {
    numberPosition4SegmentLed(second%10, positionLed7);
    positionLed7--;
  }
  else if( positionLed7 == 3)
  {
    numberPosition4SegmentLed(second/10, positionLed7);
    positionLed7--;
  }
  else if( positionLed7 == 2)
  {
    segLedDot(minute%10, positionLed7);
    positionLed7--;
  }
  else if( positionLed7 == 1)
  {
    numberPosition4SegmentLed(minute/10, positionLed7);
    positionLed7--;
  }
  else if( positionLed7 == 0)
  {
    positionLed7 = 4;
  }

  
  if (count == 557)
  {
    if (second == 60)
        {
          second = 0;
          minute ++;
          if (minute == 60)
          {
            minute = 0;
          }
        }
      second ++;
      count = 0;
  }
  count ++;
  
  
}

void setup() {
  pinMode(clockPin,OUTPUT);
  pinMode(latchPin,OUTPUT);
  pinMode(dataPin,OUTPUT);  

  Timer1.initialize(1000); 
  Timer1.attachInterrupt(countTime);
  Serial.begin(9600);

}


void loop() {

  
}


/*for (int i=1; i<=4; i++)
    {
        numberPosition4SegmentLed(random(10), i);
        delay(400);
    }*/
