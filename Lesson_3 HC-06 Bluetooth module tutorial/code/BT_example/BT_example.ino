char val; 
int ledpin=13;           //Define the pin 13 of ledpin
void setup(){            //Set parameter function 
Serial.begin(9600);     //Set the baud rate of serial port for 9600
 pinMode(ledpin,OUTPUT); //Define 13pin to output mode
} 
void loop(){                //The main loop function
val=Serial.read();         //Read a serial port is stored into the val
if(val=='U'){           //Judge whether the value of the val FF01F 
  digitalWrite(ledpin,HIGH);  //Ledpin output high level
  delay(250);                 //delay 250ms
  digitalWrite(ledpin,LOW);   //Ledpin output low level
  delay(250);                 //delay 250ms
  Serial.println(val);       //IED serial output val values
  }
}
