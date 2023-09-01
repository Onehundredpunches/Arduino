#include <Arduino.h>
#include <TimerOne.h>

#define clockPin 6
#define latchPin 5
#define dataPin 7

#define Clock_Pin 2
#define Load_Pin 3
#define DataIn_Pin 4
#define ledPin A0

uint8_t ledNumberTable[] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6, 0x00};
uint8_t posotionLedTable[] = {0x00, 0x80, 0x40, 0x20, 0x10};
uint8_t buttonValue[8]; // Mảng lưu trữ dữ liệu từ 8 chân vào
uint8_t count = 0;
bool buttonD0Pressed = false; // Biến kiểm tra trạng thái nút D0
// bool buttonD7Pressed = false;
bool ledUpdated = false; // Biến kiểm tra trạng thái LED đã được cập nhật



void CLK(void)
{
  digitalWrite(clockPin, 1);
  delayMicroseconds(100);
  digitalWrite(clockPin, 0);
}

void LATCH(void)
{
  digitalWrite(latchPin, 1);
  delayMicroseconds(100);
  digitalWrite(latchPin, 0);
}

void in_CLK(void)
{
  digitalWrite(Clock_Pin, 1);
  delayMicroseconds(100);
  digitalWrite(Clock_Pin, 0);
}

void in_LOAD(void)
{
  digitalWrite(Load_Pin, 0); // Đưa IC về chế độ SHIFT MODE
  delayMicroseconds(100);
  digitalWrite(Load_Pin, 1);
}

void numberPosition4SegmentLed(uint8_t number, uint8_t position)
{
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(dataPin, (posotionLedTable[position] >> i) & 0x01);
    CLK();
  }

  for (int i = 0; i < 8; i++)
  {
    digitalWrite(dataPin, (ledNumberTable[number] >> i) & 0x01);
    CLK();
  }

  delay(1);
  LATCH();
}
void increase_Number(void)
{
  for (int i = 0; i < 15; i++)
  {
    // Tách các chữ số
    int digit1 = count / 1000;
    int digit2 = (count / 100) % 10;
    int digit3 = (count / 10) % 10;
    int digit4 = count % 10;

    // Cập nhật dữ liệu cho LED 7 đoạn
    numberPosition4SegmentLed(digit1, 1);
    numberPosition4SegmentLed(10, 1);
    numberPosition4SegmentLed(digit2, 2);
    numberPosition4SegmentLed(10, 2);
    numberPosition4SegmentLed(digit3, 3);
    numberPosition4SegmentLed(10, 3);
    numberPosition4SegmentLed(digit4, 4);
    numberPosition4SegmentLed(10, 4);
  }
}

void update_SegmentLed()
{
  in_CLK();
  in_LOAD();

  // Cách đọc giá trị từ IC thứ 2:
  for (int i = 0; i < 8; i++)
  {
    int bitValue = digitalRead(DataIn_Pin);
    buttonValue[i] = bitValue;

    in_CLK();
  }

  in_LOAD();

  // Cách gán giá trị đọc được vào biên thứ 2
  bool newButtonD0State = buttonValue[0] == LOW;
  // bool newButtonD7State = buttonValue[7] == LOW;

  // Kiểm tra nút D0 đã được nhấn
  if (newButtonD0State && !buttonD0Pressed) // Kiểm tra trạng thái nút nhấn đã nhấn (newButtonD0State = true) và trước đó nhấn (buttonD0Pressed = true)
  {
    buttonD0Pressed = true;
    increase_Number();
    Serial.print("Button at D0 presssed: ");
    Serial.print(count);
    digitalWrite(ledPin, 1);
    delay(500);
    digitalWrite(ledPin, 0);
    count++;
    ledUpdated = true;
  }
  else if (!newButtonD0State && buttonD0Pressed)
  {
    buttonD0Pressed = false;
    Serial.println("\nButton at D0 no no no presssed");
    ledUpdated = false;
  }
/*
if (newButtonD7State && !buttonD7Pressed) 
  {
    buttonD7Pressed = true;
    increase_Number();
    Serial.println("Button at D7 presssed");
    digitalWrite(ledPin, 1);
    delay(300);
    digitalWrite(ledPin, 0);
    count--;
    ledUpdated = true;
  }
  else if (!newButtonD7State && buttonD7Pressed)
  {
    buttonD7Pressed = false;
    Serial.println("Button at D0 no no no presssed");
    ledUpdated = false;
  }
  */
  
  if (ledUpdated == false) {
    increase_Number();
  }
  
  // increase_Number();
  delay(100);
}

void setup()
{
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(Clock_Pin, OUTPUT);
  pinMode(Load_Pin, OUTPUT);
  pinMode(DataIn_Pin, INPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(Load_Pin, LOW); // Đưa IC về chế độ LOAD MODE ban đầu
  digitalWrite(Clock_Pin, LOW);
  digitalWrite(Load_Pin, HIGH);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(update_SegmentLed);
  Serial.begin(9600);
}

void loop()
{
  
}
