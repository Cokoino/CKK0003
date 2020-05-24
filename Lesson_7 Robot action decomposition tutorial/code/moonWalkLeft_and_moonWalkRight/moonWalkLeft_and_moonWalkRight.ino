#include <Servo.h>
#include <Oscillator.h>
#include <EEPROM.h>

#define N_SERVOS 4
//-- First step: Configure the pins where the servos are attached
/*
         --------------- 
        |     O   O     |
        |---------------|
YR 6==> |               | <== YL 11
         --------------- 
            ||     ||
            ||     ||
RR 7==>   -----   ------  <== RL 10
         |-----   ------|
*/
#define EEPROM_TRIM false 
// Activate to take callibration data from internal memory
#define TRIM_RR 0
#define TRIM_RL 0
#define TRIM_YR 0
#define TRIM_YL 0

#define PIN_RR 7
#define PIN_RL 10
#define PIN_YR 6
#define PIN_YL 11

Oscillator servo[N_SERVOS];

void moonWalkLeft(int steps, int T=1000);
void moonWalkRight(int steps, int T=1000);

char state_rgb_led=1;
int t=495;
///////////////////////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);

  pinMode(9,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  
  servo[0].attach(PIN_RR);
  servo[1].attach(PIN_RL);
  servo[2].attach(PIN_YR);
  servo[3].attach(PIN_YL);
  
  //EEPROM.write(0,TRIM_RR);
  //EEPROM.write(1,TRIM_RL);
  //EEPROM.write(2,TRIM_YR);
  //EEPROM.write(3,TRIM_YL);
  
  int trim;  
  if(EEPROM_TRIM){
    for(int x=0;x<4;x++){
      trim=EEPROM.read(x);
      if(trim>128)trim=trim-256;
      Serial.print("TRIM ");
      Serial.print(x);
      Serial.print(" en ");
      Serial.println(trim);
      servo[x].SetTrim(trim);
    }
  }
  else{
    servo[0].SetTrim(TRIM_RR);
    servo[1].SetTrim(TRIM_RL);
    servo[2].SetTrim(TRIM_YR);
    servo[3].SetTrim(TRIM_YL);
  } 
  for(int i=0;i<4;i++) servo[i].SetPosition(90);
}
///////////////////////////////////////////////////////////////////////////////////
void loop()
{
 moonWalkLeft(4, t*2); 
 moonWalkRight(4, t*2);
}
//////////////////////////////////CONTROL FUNCIONES//////////////////////////////////
void oscillate(int A[N_SERVOS], int O[N_SERVOS], int T, double phase_diff[N_SERVOS]){
  for (int i=0; i<4; i++) {
    servo[i].SetO(O[i]);
    servo[i].SetA(A[i]);
    servo[i].SetT(T);
    servo[i].SetPh(phase_diff[i]);
  }
  double ref=millis();
   for (double x=ref; x<T+ref; x=millis()){
     for (int i=0; i<4; i++){
        servo[i].refresh();
     }
  }
}

void moonWalkRight(int steps, int T){
    int A[4]= {25, 25, 0, 0};
    int O[4] = {-15 ,15, 0, 0};
    double phase_diff[4] = {DEG2RAD(0), DEG2RAD(180 + 120), DEG2RAD(90), DEG2RAD(90)}; 
    
    for(int i=0;i<steps;i++)oscillate(A,O, T, phase_diff);
}

void moonWalkLeft(int steps, int T){
    int A[4]= {25, 25, 0, 0};
    int O[4] = {-15, 15, 0, 0};
    double phase_diff[4] = {DEG2RAD(0), DEG2RAD(180 - 120), DEG2RAD(90), DEG2RAD(90)}; 
    
    for(int i=0;i<steps;i++)oscillate(A,O, T, phase_diff);
}
