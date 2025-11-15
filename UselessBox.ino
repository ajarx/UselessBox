#include <ESP32Servo.h>
#include "esp_system.h"

#define LID_SWRVO_PIN 0 //盒盖舵机引脚
#define HAND_SERVO_PIN 1       // 开关舵机信号线
#define LID_ANGLE 30
#define LID_CLOSE_ANGLE 172
#define HAND_ANGLE 93
#define HAND_STOP_ANGLE 20
#define SWITCH_PIN 10     // 开关输入引脚

Servo lidServo;
Servo handServo;
uint8_t lidCurrentAngle; 
uint8_t handCurrentAngle;

bool lastSwitchState = HIGH; // 保存上次开关状态（默认未按下）

void setup() {
  Serial.begin(115200);

  // 初始化舵机
  lidServo.attach(LID_SWRVO_PIN, 500, 2500);  // 脉宽范围 0.5ms～2.5ms
  handServo.attach(HAND_SERVO_PIN, 500, 2500);  // 脉宽范围 0.5ms～2.5ms

  // 初始化开关输入
  pinMode(SWITCH_PIN, INPUT_PULLUP);  // 使用内部上拉，开关闭合时接地为 LOW

  // 舵机复位
  lidServo.write(LID_CLOSE_ANGLE);
  lidCurrentAngle = LID_CLOSE_ANGLE;
  handServo.write(HAND_STOP_ANGLE);
  handCurrentAngle = HAND_STOP_ANGLE;
}

void loop() {
  bool switchState = digitalRead(SWITCH_PIN);

  // 检测开关按下
  if (switchState == LOW) {
    Serial.println("Switch pressed, triggering servo...");
    int modeNo = (esp_random() % 7) + 1;
    switch(modeNo)
    {
      case 1:
        mode1();
        break;
      case 2:
        mode2();
        break;
      case 3:
        mode3();
        break;
      case 4:
        mode4();
        break;
      case 5:
        mode5();
      case 6:
        mode6();
      case 7:
        mode7();
      default:
        break;
    }
  }

  delay(10); // 简单防抖
}

void mode1()
{
    delay(500);
    if(checkAndReset())
    {
      return;
    }
    openLid(0);
    delay(500);
    if(checkAndReset())
    {
      return;
    }
    toggleSwitch(0);
    closeLid(0);
}

void mode2()
{
    delay(500);
    openLid(80);
    if(checkAndReset())
    {
      return;
    }
    toggleSwitch(0);
    closeLid(0);
}

void mode3()
{
    delay(500);
    openLid(0);
    delay(200);
    if(checkAndReset())
    {
      return;
    }
    closeLid(0);
    delay(200);
    if(checkAndReset())
    {
      return;
    }
    openLid(0);
    delay(800);
    if(checkAndReset())
    {
      return;
    }
    closeLid(0);
    delay(200);
    if(checkAndReset())
    {
      return;
    }
    openLid(0);
    delay(100);
    if(checkAndReset())
    {
      return;
    }
    closeLid(0);
    delay(100);
    if(checkAndReset())
    {
      return;
    }
    openLid(0);
    toggleSwitch(0);
    closeLid(0);
}

void mode4()
{
    delay(500);
    openLid(0);
    delay(200);
    if(checkAndReset())
    {
      return;
    }
    closeLid(0);
    delay(200);
    if(checkAndReset())
    {
      return;
    }
    openLid(0);
    delay(800);
    if(checkAndReset())
    {
      return;
    }
    closeLid(0);
    delay(200);
    if(checkAndReset())
    {
      return;
    }
    openLid(0);
    delay(100);
    if(checkAndReset())
    {
      return;
    }
    closeLid(0);
    delay(1500);
    if(checkAndReset())
    {
      return;
    }
    openLid(0);
    toggleSwitch(0);
    closeLid(0);
}

void mode5()
{
    delay(200);
    if(checkAndReset())
    {
      return;
    }
    openLid(0);
    if(checkAndReset())
    {
      return;
    }
    toggleSwitch(0);
    closeLid(0);
}

void mode6()
{
    delay(1500);
    if(checkAndReset())
    {
      return;
    }
    openLid(0);
    closeLid(0);
    delay(1000);
    if(checkAndReset())
    {
      return;
    }
    openLid(0);
    setHandAngle(HAND_ANGLE - 30, 80);
    setHandAngle(HAND_STOP_ANGLE, 0);
    closeLid(0);
    delay(800);
    if(checkAndReset())
    {
      return;
    }
    openLid(30);
    setHandAngle(HAND_ANGLE - 25, 80);
    setHandAngle(HAND_STOP_ANGLE, 0);
    closeLid(0);
    delay(600);
    if(checkAndReset())
    {
      return;
    }
    openLid(50);
    setHandAngle(HAND_ANGLE, 0);
    delay(200);
    setHandAngle(HAND_STOP_ANGLE, 0);
    closeLid(0);
    delay(200);
}

void mode7()
{
  delay(300);
  openLid(0);
  delay(2500);
  if(checkAndReset())
  {
    return;
  }
  setHandAngle(HAND_ANGLE, 0);
  delay(200);
  setHandAngle(HAND_STOP_ANGLE, 0);
  closeLid(0);
}

bool checkAndReset()
{
  bool switchState = digitalRead(SWITCH_PIN);
  if (switchState == HIGH)
  {
    //Reset hand
    setHand(HAND_STOP_ANGLE);
    closeLid(0);
    return true;
  }
  return false;
}

void setLid(uint8_t angle)
{
  lidServo.write(angle);
  lidCurrentAngle = angle;
}

void openLid(uint8_t delayMs) {
  uint8_t stopAng = LID_CLOSE_ANGLE - LID_ANGLE;
  while(lidCurrentAngle > stopAng)
  {
    setLid(lidCurrentAngle - 1);
    delay(delayMs);
  }
}

void closeLid(uint8_t delayMs)
{
  uint8_t stopAng = LID_CLOSE_ANGLE;
  while(lidCurrentAngle < stopAng)
  {
    setLid(lidCurrentAngle + 1);
    delay(delayMs);
  }
  delay(100);
}

void setHand(uint8_t angle)
{
  handServo.write(angle);
  handCurrentAngle = angle;
}
void toggleSwitch(uint8_t delayMs) {
  // 从0°转到90°
  while(handCurrentAngle < HAND_ANGLE)
  {
    setHand(handCurrentAngle + 1);
    delay(delayMs);
  }
  delay(300);  // 停顿0.5秒
  while(handCurrentAngle > HAND_STOP_ANGLE)
  {
    setHand(handCurrentAngle - 1);
    delay(delayMs);
  }
  delay(200);
}

void setHandAngle(uint8_t angle, uint8_t delayMs)
{
  if(handCurrentAngle < angle)
  {
    while(handCurrentAngle < angle)
    {
      setHand(handCurrentAngle + 1);
      delay(delayMs);
    }
  }
  else
  {
    while(handCurrentAngle > angle)
    {
      setHand(handCurrentAngle - 1);
      delay(delayMs);
    }
  }
}
