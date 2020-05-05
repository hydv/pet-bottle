#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  350
#define SERVOMAX  400

int servoNum[12] =  {0,  1,  2,   4,  5,  6,   8,  9,  10,  12, 13, 14};
int servoInit[12] = {355,395,325, 375,400,335, 375,390,330, 360,340,360};
int servoDefOld[12] = {0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0};
int servoDef[12] = {0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0};

uint8_t servonum = 0;

const int BUTTON = 7;
const int LED = 13;
const int vol_pin = 1;
int vol_value = 0;
int old_vol_value_1 = 800;
int old_vol_value_2 = 800;

int val = 0;
int old_val = 0;
int state = 0;

int i = 0;
int j = 0;


void setup() {
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");

  pwm.begin();
  
  pwm.setPWMFreq(60);

  pinMode(LED,OUTPUT);
  pinMode(BUTTON, INPUT);
  
  delay(10);
}


void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;
  pulselength /= 60;
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000000;
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}


void loop() {

  val = digitalRead(BUTTON);

  if((val==1)&&(old_val==0)){
    state = 1 - state;
    delay(100);
  }
 
  old_val = val;
  
  vol_value = analogRead( vol_pin );
  if(state == 1){
    setu();
    digitalWrite(LED,HIGH);
  }else if((vol_value + old_vol_value_1 + old_vol_value_2)/3 < 801) {
    
    digitalWrite(LED,LOW);
    if(i < 5){walk(); i++;}
    if(i > 4 && i < 8){migisen(); i++;}
    if(i > 7 && i < 12){walk(); i++;}
    if(i > 11 && i < 15){hidarisen(); i++;}
    if(i > 14){hidarisen(); i = 0;}
    j = 0;

    Serial.print("1 : ");
    old_vol_value_2 = old_vol_value_1;
    old_vol_value_1 = vol_value;
    
  }else if((vol_value + old_vol_value_1 + old_vol_value_2)/3 >800 && (vol_value + old_vol_value_1 + old_vol_value_2)/3 <901){
    digitalWrite(LED,LOW);
    
    if(i < 5){walk2(); i++;}
    if(i > 4 && i < 8){migisen2(); i++;}
    if(i > 7 && i < 12){walk2(); i++;}
    if(i > 11 && i < 15){hidarisen2(); i++;}
    if(i > 14){hidarisen2(); i = 0;}
    j = 0;
    old_vol_value_2 = old_vol_value_1;
    old_vol_value_1 = vol_value;
    Serial.print("2 : ");

  }else if((vol_value + old_vol_value_1 + old_vol_value_2)/3 > 900){
    digitalWrite(LED,LOW);
    if(j < 4){
      walk3(); 
      j++;}
    if(j > 3 && j < 5) {dead(); j++;}
    old_vol_value_2 = old_vol_value_1;
    old_vol_value_1 = vol_value;
    
    Serial.print("3 : ");
  }

  
  Serial.println((vol_value + old_vol_value_1 + old_vol_value_2)/3);
}


void defSet(int d_0, int d_1, int d_2, int d_4, int d_5, int d_6, 
  int d_8, int d_9, int d_10, int d_12, int d_13, int d_14){
  int i = 0;
  servoDef[i++] = d_0;
  servoDef[i++] = d_1;
  servoDef[i++] = d_2;
  servoDef[i++] = d_4;
  servoDef[i++] = d_5;
  servoDef[i++] = d_6;
  servoDef[i++] = d_8;
  servoDef[i++] = d_9;
  servoDef[i++] = d_10;
  servoDef[i++] = d_12;
  servoDef[i++] = d_13;
  servoDef[i++] = d_14;
}


void servoSet(int d_0, int d_1, int d_2, int d_4, int d_5, int d_6, 
  int d_8, int d_9, int d_10, int d_12, int d_13, int d_14){
    
  defSet(d_0, d_1, d_2, d_4, d_5, d_6, d_8, d_9, d_10, d_12, d_13, d_14);
  
  for (int i = 0; i < 12; i++){
    pwm.setPWM(servoNum[i], 0, servoInit[i]+servoDef[i]);
  }
}


void servoMove(int d_0, int d_1, int d_2, int d_4, int d_5, int d_6, 
  int d_8, int d_9, int d_10, int d_12, int d_13, int d_14, int smot){     
  defSet(d_0, d_1, d_2, d_4, d_5, d_6, d_8, d_9, d_10, d_12, d_13, d_14);

  smot = constrain(smot, 10, 1000);
  for(int j = 0; j < smot; j++){
    for (int i = 0; i < 12; i++){
      pwm.setPWM(servoNum[i], 0, servoInit[i]+servoDefOld[i]+(servoDef[i]-servoDefOld[i])*j/smot);     
    }
    delay(5);
    //Serial.println(servoDefOld[1]);
    
  }
  for(int i = 0; i < 12; i++){
    servoDefOld[i] = servoDef[i];
  }
}


void setu(){
  servoMove(0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0, 50);
  delay(1000);
}


void bata(){
  servoMove(20, 20, 20,  20, 20, 20,  20, 20, 20,  20, 20, 20, 200 );
  delay(1000);
  servoMove(-20, -20, -20,  -20, -20, -20,  -20, -20, -20,  -20, -20, -20, 200);
  delay(1000);
}


void walk(){
  servoMove(120, 0,  0,  -120,-30,-20,    0, 0, 0,      0,-40,-20,   30);
  servoMove(120, 0,  0,  -120, 0,  0,     0, 0, 0,      0, 0,  0,    10);
  servoMove(120,-40,-20, -120, 0,  0,     0,-40,-20,   0, 0,  0,    10);
  servoMove(  0,-40,-20,    0, 0,  0,   120,-40,-20, -120, 0,  0,    30);
  servoMove(  0, 0,  0,     0, 0,  0,   120, 0, 0,   -120, 0,  0,    10);
  servoMove(  0, 0,  0,     0,-30,-20,  120, 0, 0,   -120,-40,-20,   10);
}


void walk2(){
  servoMove(120, 0,  0,  -120,-30,-20,    0, 0, 0,      0,-40,-20,   60);
  servoMove(120, 0,  0,  -120, 0,  0,     0, 0, 0,      0, 0,  0,    20);
  servoMove(120,-40,-20, -120, 0,  0,     0,-40,-20,   0, 0,  0,    20);
  servoMove(  0,-40,-20,    0, 0,  0,   120,-40,-20, -120, 0,  0,    60);
  servoMove(  0, 0,  0,     0, 0,  0,   120, 0, 0,   -120, 0,  0,    20);
  servoMove(  0, 0,  0,     0,-30,-20,  120, 0, 0,   -120,-40,-20,   20);
}


void walk3(){
  servoMove(120, 0,  0,  -120,-30,-20,    0, 0, 0,      0,-40,-20,   120);
  servoMove(120, 0,  0,  -120, 0,  0,     0, 0, 0,      0, 0,  0,    40);
  servoMove(120,-40,-20, -120, 0,  0,     0,-40,-20,   0, 0,  0,     40);
  servoMove(  0,-40,-20,    0, 0,  0,   120,-40,-20, -120, 0,  0,    120);
  servoMove(  0, 0,  0,     0, 0,  0,   120, 0, 0,   -120, 0,  0,    40);
  servoMove(  0, 0,  0,     0,-30,-20,  120, 0, 0,   -120,-40,-20,   40);
}


void migisen(){
  servoMove( 40,-40,-20,  -40, 0,  0,    40,-40,-20,  -40, 0,   0,   10);
  servoMove(-40,-40,-20,   40, 0,  0,   -40,-40,-20,   40, 0,   0,   30);
  servoMove(-40, 0,  0,    40, 0,  0,   -40, 0,  0,    40, 0,   0,   10);
  servoMove(-40, 0,  0,    40,-40,-20,  -40, 0,  0,    40,-40, -20,  10); 
  servoMove( 40, 0,  0,   -40,-40,-20,   40, 0,  0,   -40,-40, -20,  30);
  servoMove( 40, 0,  0,   -40, 0,  0,    40, 0,  0,   -40, 0,   0,   10);
}


void migisen2(){
  servoMove( 40,-40,-20,  -40, 0,  0,    40,-40,-20,  -40, 0,   0,   20);
  servoMove(-40,-40,-20,   40, 0,  0,   -40,-40,-20,   40, 0,   0,   60);
  servoMove(-40, 0,  0,    40, 0,  0,   -40, 0,  0,    40, 0,   0,   20);
  servoMove(-40, 0,  0,    40,-40,-20,  -40, 0,  0,    40,-40, -20,  20); 
  servoMove( 40, 0,  0,   -40,-40,-20,   40, 0,  0,   -40,-40, -20,  60);
  servoMove( 40, 0,  0,   -40, 0,  0,    40, 0,  0,   -40, 0,   0,   20);
}


void hidarisen(){
  servoMove(-40,-40,-20,   40, 0,  0,   -40,-40,-20,   40, 0,   0,   10);
  servoMove( 40,-40,-20,  -40, 0,  0,    40,-40,-20,  -40, 0,   0,   30);
  servoMove( 40, 0,  0,   -40, 0,  0,    40, 0,  0,   -40, 0,   0,   10);
  servoMove( 40, 0,  0,   -40,-40,-20,   40, 0,  0,   -40,-40, -20,  10); 
  servoMove(-40, 0,  0,    40,-40,-20,  -40, 0,  0,    40,-40, -20,  30);
  servoMove(-40, 0,  0,    40, 0,  0,   -40, 0,  0,    40, 0,   0,   10);
}


void hidarisen2(){
  servoMove(-40,-40,-20,   40, 0,  0,   -40,-40,-20,   40, 0,   0,   20);
  servoMove( 40,-40,-20,  -40, 0,  0,    40,-40,-20,  -40, 0,   0,   60);
  servoMove( 40, 0,  0,   -40, 0,  0,    40, 0,  0,   -40, 0,   0,   20);
  servoMove( 40, 0,  0,   -40,-40,-20,   40, 0,  0,   -40,-40, -20,  20); 
  servoMove(-40, 0,  0,    40,-40,-20,  -40, 0,  0,    40,-40, -20,  60);
  servoMove(-40, 0,  0,    40, 0,  0,   -40, 0,  0,    40, 0,   0,   20);
}


void deadwalk(){
  servoMove(- 50, 60, 80,  0, -70, -70,  70, -20, -50,  0, -70, -70,  200);
  servoMove(70, 0, -30,  -70, -70, -70,  -50, 30, 60,  30, -20, 50,  200);
  servoMove(0, -70, -70,  -70, -20, -50,  0, -70, -70,  50, 60, 80,  200);
  servoMove(-30, -20, 50,  50, 30, 60,  70, -70, -70,  -70, 0, -30,  200);
}


void dead(){
  servoMove(120, 0,  0,  -120,-30,-50,    0, 0, 0,      0,-40,-50,   300);
  servoMove(120, 0,  0,  -120, 20,  100,     0, 0, 0,      0, 0,  0,    200);
  servoMove(120, 10,  40, -120, 20,  100,     0,-40, 0,   0, 20,  10,     200);
  servoMove(100, 20,  50,    -60, 20,  100,   60,-40, 0, -60, 0,  100,    400);
  servoMove(90, 55,  90,      10, 45,  90,   60,-40, 0,  30, 30,  90,    200);
}
