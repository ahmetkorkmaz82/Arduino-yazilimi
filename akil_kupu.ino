#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);
#include <LedControl.h>

int DIN = 11;
int CS =  12;
int CLK = 13;


LedControl lc=LedControl(DIN,CLK,CS,0);

int color = 0;
// steppers
float distance = 100;
float degree = 90;
float pi = 3.14159;
float wheel_dia =  33; //66.25;   // mm (increase = spiral out)
int wheel_base = 86;    // mm (increase = spiral in) 
int wheel_bpi = wheel_base * pi;
int steps_rev = 4076; //bir turdaki adım sayısı
float steps_dist = steps_rev / (wheel_dia * pi);

#define MAX_SPEED 1500
int frontsensorpin = A0;
int rightsensorpin = A1;
int leftsensorpin = A2;
int engelpin = 10;
AccelStepper rightMotor(AccelStepper::HALF4WIRE, 2,4,3,5, true);
AccelStepper leftMotor(AccelStepper::HALF4WIRE, 6,8,7,9, true);
MultiStepper motors;
long steps[2];

//
// motorOff: turn off stepper motors
//
  byte fwd[8]=  {0x18, 0x30, 0x60, 0xFF, 0xFF, 0x60, 0x30, 0x18};
  byte bwd[8] = {0x18, 0x0C, 0x06, 0xFF, 0xFF, 0x06, 0x0C, 0x18};
  byte rgt[8] = {0x1F, 0x1F, 0x18, 0x99, 0xDB, 0x7E, 0x3C, 0x18};
  byte lft[8] = {0x18, 0x3C, 0x7E, 0xDB, 0x99, 0x18, 0x18, 0x1F};
  byte i[5]={B00000000,B00100010,B10111110,B00000010,B00000000};
  byte n[5]={B00111110,B00010000,B00100000,B00100000,B00011110};
  byte o[5]={B00011100,B00100010,B00100010,B00100010,B00011100};

void setup() {
   lc.shutdown(0,false);       
 lc.setIntensity(0,15);      
 lc.clearDisplay(0);    
  pinMode(frontsensorpin, INPUT); 
  pinMode(engelpin, INPUT); 
  pinMode(rightsensorpin, INPUT); 
  pinMode(leftsensorpin, INPUT); 
  delay(1000);
  tcs.begin();
  Serial.begin(9600);
  leftMotor.setMaxSpeed(MAX_SPEED);
  rightMotor.setMaxSpeed(MAX_SPEED);
  motors.addStepper(leftMotor);
  motors.addStepper(rightMotor);
}


void forward() {
  printByte(fwd);
  int s = int(distance*steps_dist);
  steps[0] = -s;
  steps[1] = s;
  motors.moveTo(steps);
  motors.runSpeedToPosition();
  leftMotor.setCurrentPosition(0);
  rightMotor.setCurrentPosition(0);
  Serial.println("f");
  lc.clearDisplay(0);
}

void backward() {
  printByte(bwd);
  int s = int(distance*steps_dist);
  steps[0] = s;
  steps[1] = -s;
  motors.moveTo(steps);
  motors.runSpeedToPosition();
  leftMotor.setCurrentPosition(0);
  rightMotor.setCurrentPosition(0);
  Serial.println("b");
  lc.clearDisplay(0);
}
void back() {
  float turnDistance = wheel_bpi * ((degree*2)/360.0);
  int s = int(turnDistance*steps_dist);
  steps[0] = -s;
  steps[1] = -s;
  motors.moveTo(steps);
  motors.runSpeedToPosition();
  leftMotor.setCurrentPosition(0);
  rightMotor.setCurrentPosition(0);
  Serial.println("a");
}
void left() {
  printByte(lft);
  float leftDistance = wheel_bpi * (degree/360.0);
  int s = int(leftDistance*steps_dist);
  steps[0] = -s;
  steps[1] = -s;
  motors.moveTo(steps);
  motors.runSpeedToPosition();
  leftMotor.setCurrentPosition(0);
  rightMotor.setCurrentPosition(0);
  Serial.println("l");
  lc.clearDisplay(0);
}

void right() {
  printByte(rgt);
  float rightDistance = wheel_bpi * (degree/360.0);
  int s = int(rightDistance*steps_dist);
  steps[0] = s;
  steps[1] = s;
  motors.moveTo(steps);
  motors.runSpeedToPosition();
  leftMotor.setCurrentPosition(0);
  rightMotor.setCurrentPosition(0);
  Serial.println("r");
  lc.clearDisplay(0);
}
void frontsensor(){
if (digitalRead(frontsensorpin)==HIGH)
  Serial.println("0");
  else
  Serial.println("1");
}
void engel(){
if (digitalRead(engelpin)==HIGH)
  Serial.println("1");
  else
  Serial.println("0");
}
void rightsensor(){
if (digitalRead(rightsensorpin)==HIGH)
  Serial.println("0");
  else
  Serial.println("1");
}
void leftsensor(){
if (digitalRead(leftsensorpin)==HIGH)
  Serial.println("0");
  else
  Serial.println("1");
}
void printByte(byte character [])
{
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc.setRow(0,i,character[i]);
  }
}
void read_red() {
  float average, r, g, b;
  uint16_t clearcol, red, green, blue;
  tcs.getRawData(&red, &green, &blue, &clearcol);
   average = (red+green+blue)/3;
 r = red/average;
 g = green/average;
 b = blue/average;
 if ((r > 1.4) && (g < 0.9) && (b < 0.9)) {
    color=1;
    }
    else{
      color=0;
      }
    Serial.println(color);
}
void read_green() {
  float average, r, g, b;
  uint16_t clearcol, red, green, blue;
  tcs.getRawData(&red, &green, &blue, &clearcol);
   average = (red+green+blue)/3;
 r = red/average;
 g = green/average;
 b = blue/average;
if ((r < 0.95) && (g > 1.4) && (b < 0.9)) {
    color=1;
    }
    else{
      color=0;
      }
    Serial.println(color);
}
void read_blue() {
  float average, r, g, b;
  uint16_t clearcol, red, green, blue;
  tcs.getRawData(&red, &green, &blue, &clearcol);
   average = (red+green+blue)/3;
 r = red/average;
 g = green/average;
 b = blue/average;
if ((r < 0.8) && (g < 1.2) && (b > 1.2)) {
    color=1;
    }
    else{
      color=0;
      }
    Serial.println(color);
}
  void siyahadon(){
  Serial.println("t");
  }
  void yesiledon(){
  Serial.println("t");
  }
  void kirmiziyadon(){
  Serial.println("t");
  }
  void maviyedon(){
  Serial.println("t");
  }
  void beyazadon(){
  Serial.println("t");
  } 

void read_all() {
  uint16_t clearcol, red, green, blue;
  tcs.getRawData(&red, &green, &blue, &clearcol);
 Serial.print("Kirizi: ");
    Serial.print(red);
    Serial.print("Yesil: ");
     Serial.print(green);
     Serial.print("Mavi: ");
      Serial.print(blue);
      Serial.println("");
}

void read_color2() {
  uint16_t clearcol, red, green, blue;
  float average, r, g, b;
  tcs.getRawData(&red, &green, &blue, &clearcol);
  average = (red+green+blue)/3;
  r = red/average;
  g = green/average;
  b = blue/average;
  delay(50);
  if((r < 1.74)&& (r > 1.58) && (g > 0.76) && (g < 0.9)&& (b < 0.56&& (b > 0.45))){
    color = 1;
  }
  if((r < 1.05)&& (r > 0.75) && (g > 1.38) && (g < 1.5)&& (b < 1&& (b > 0.54))){
    color = 2;
  }
  if((r < 1.32)&& (r > 1.19) && (g > 1.24) && (g < 1.34)&& (b < 0.52&& (b > 0.42))){
    color = 3;
  }
  if((r < 1.5)&& (r > 1.35) && (g > 0.74) && (g < 0.85)&& (b < 0.9)&& (b > 0.7)){
    color = 4;
  }
  if((r < 2.2)&& (r > 1.8) && (g > 0.5) && (g < 0.65)&& (b < 0.55)&& (b > 0.4)){
    color = 5;
  }
  if((r < 1)&& (r > 0.48) && (g > 1.24) && (g < 1.34)&& (b < 1.24)&& (b > 1.14)){
    color = 6;
  }
  Serial.println(color);
}
void loop() {
  if(Serial.available() > 0)
  {
    char data;
    data = Serial.read();
    switch (data)
    {
    case 'F': 
    forward();
    break;
    case 'D': 
    delay(900);
    Serial.println("t");
    break;
    case 'B': 
    backward();
    break;
    case 'R':
    right();
    break;
    case 'L': 
    left();
    break;
    case 'K': 
    read_red(); 
    break;
    case 'Y': 
    read_green(); 
    break;
    case 'M': 
    read_blue(); 
    break;
    case 'Q': 
    read_all(); 
    break;
    case 'O': 
    frontsensor(); 
    break;
    case 'S': 
    rightsensor(); 
    break;
    case 'P': 
    leftsensor(); 
    break;
    case 'E': 
    engel(); 
    break;
    case 'y': 
    yesiledon(); 
    break;
    case 'k': 
    kirmiziyadon(); 
    break;
    case 'm': 
    maviyedon(); 
    break;
    case 'w': 
    beyazadon(); 
    break;
    case 'z': 
    siyahadon(); 
    break;
    
}
  
  }
}



