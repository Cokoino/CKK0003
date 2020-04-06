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

void kickLeft(int tempo);
void kickRight(int tempo);
void walk(int steps, int T=1000);
void backyard(int steps, int T=3000);
void turnLeft(int steps, int T=3000);
void turnRight(int steps, int T=3000);
void moonWalkLeft(int steps, int T=1000);
void moonWalkRight(int steps, int T=1000);
void swing(int steps, int T=1000);
void buzzer_rings(void);
void RGB_LED(void);

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
  char BT_data;    //Defines a variable to receive data read by bluetooth
  if(Serial.available())     //Determine whether data is received
  {
    BT_data = Serial.read();       //Read the received data and assign it to val
  }
  switch(BT_data)    //Determine the data and then execute the corresponding function
  {
    case 'U':  walk(4, t*1.5);       break;    //advance
    case 'D':  backyard(4, t*2);     break;    //reverse
    case 'L':  turnLeft(4, t*2);     break;    //turn left
    case 'R':  turnRight(4, t*2);    break;    //turn right
    case 'S':  swing(4, t*2);        break;    //stop
    case '1':  moonWalkLeft(4, t*2); break;
    case '2':  kickLeft(1000);       break;
    case '3':  kickRight(1000);      break;
    case '4':  moonWalkRight(4, t*2);break;
    case '6':  buzzer_rings();       break;
    case '7':  RGB_LED();            break;
    default:break;
  }   
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

void kickLeft(int tempo){
  for(int i=0;i<4;i++) servo[i].SetPosition(90);
  delay(tempo);
  servo[0].SetPosition(50); //pie derecho
  servo[1].SetPosition(70); //pie izquiero
  delay(tempo);
  servo[0].SetPosition(80); //pie derecho
  servo[1].SetPosition(70); //pie izquiero
  delay(tempo/4);
  servo[0].SetPosition(30); //pie derecho
  servo[1].SetPosition(70); //pie izquiero
  delay(tempo/4);
  servo[0].SetPosition(80); //pie derecho
  servo[1].SetPosition(70); //pie izquiero
  delay(tempo/4);
  servo[0].SetPosition(30); //pie derecho
  servo[1].SetPosition(70); //pie izquiero
  delay(tempo/4);
  servo[0].SetPosition(80); //pie derecho
  servo[1].SetPosition(70); //pie izquiero
  delay(tempo);
}

void kickRight(int tempo){
for(int i=0;i<4;i++) servo[i].SetPosition(90);
  delay(tempo);
  servo[0].SetPosition(110); //pie derecho
  servo[1].SetPosition(130); //pie izquiero
  delay(tempo);
  servo[0].SetPosition(110); //pie derecho
  servo[1].SetPosition(100); //pie izquiero
  delay(tempo/4);
  servo[0].SetPosition(110); //pie derecho
  servo[1].SetPosition(150); //pie izquiero
  delay(tempo/4);
  servo[0].SetPosition(110); //pie derecho
  servo[1].SetPosition(80); //pie izquiero
  delay(tempo/4);
  servo[0].SetPosition(110); //pie derecho
  servo[1].SetPosition(150); //pie izquiero
  delay(tempo/4);
  servo[0].SetPosition(110); //pie derecho
  servo[1].SetPosition(100); //pie izquiero
  delay(tempo);
}

void walk(int steps, int T){
    int A[4]= {15, 15, 30, 30};
    int O[4] = {0, 0, 0, 0};
    double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(90), DEG2RAD(90)};
    
    for(int i=0;i<steps;i++)oscillate(A,O, T, phase_diff);
}

void backyard(int steps, int T){
    int A[4]= {15, 15, 30, 30};
    int O[4] = {0, 0, 0, 0};
    double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(-90), DEG2RAD(-90)}; 
    
    for(int i=0;i<steps;i++)oscillate(A,O, T, phase_diff);
}

void turnLeft(int steps, int T){
    int A[4]= {20, 20, 10, 30};
    int O[4] = {0, 0, 0, 0};
    double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(90), DEG2RAD(90)}; 
    
    for(int i=0;i<steps;i++)oscillate(A,O, T, phase_diff);
}

void turnRight(int steps, int T){
    int A[4]= {20, 20, 30, 10};
    int O[4] = {0, 0, 0, 0};
    double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(90), DEG2RAD(90)}; 
    
    for(int i=0;i<steps;i++)oscillate(A,O, T, phase_diff);
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

void swing(int steps, int T){
    int A[4]= {25, 25, 0, 0};
    int O[4] = {-15, 15, 0, 0};
    double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(90), DEG2RAD(90)};
    
    for(int i=0;i<steps;i++)oscillate(A,O, T, phase_diff);
}

void buzzer_rings(void){
  for(char i=0;i<100;i++){
    digitalWrite(9,HIGH);
    delayMicroseconds(500);
    digitalWrite(9,LOW);
    delayMicroseconds(500);
    digitalWrite(9,HIGH);
    delayMicroseconds(200);
    digitalWrite(9,LOW);
    delayMicroseconds(800);
    digitalWrite(9,HIGH);
    delayMicroseconds(800);
    digitalWrite(9,LOW);
    delayMicroseconds(200);
    digitalWrite(9,HIGH);
    delayMicroseconds(700);
    digitalWrite(9,LOW);
    delayMicroseconds(300);
    digitalWrite(9,HIGH);
    delayMicroseconds(600);
    digitalWrite(9,LOW);
    delayMicroseconds(400);
    }
}

void RGB_LED(void){
  if(state_rgb_led==0){
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);    
    }
  if(state_rgb_led==1){
    digitalWrite(12,HIGH);
    digitalWrite(13,HIGH);    
    }
  state_rgb_led++;
  if(state_rgb_led>1)
    state_rgb_led=0;
}

