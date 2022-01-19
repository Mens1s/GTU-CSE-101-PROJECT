#include <Servo.h>
Servo sg90;
int temp = 0;
int counter = 0;

void setup() {
  Serial.begin(9600);
  sg90.attach(6);
  sg90.write(45);
  pinMode(5, OUTPUT);
  pinMode(9, OUTPUT);
}

void loop() {
  
  if(Serial.available()){
    
    char input=(char)Serial.read();
      // C communication
    if(input=='2'){ 
      //making the right lighting according to the human condition
      digitalWrite(5,HIGH);
      digitalWrite(9,LOW);
      //giving the mask commands
      delay(2500);
      sg90.write(temp);
      
      counter += 1;
      if(counter ==1){
        temp+=90;
      }
      else if(counter == 2){
        temp += 90;
        }
      else{
        temp = 45;
        counter = 0;
      }
    }
    
    else if(input=='1'){
      //making the right lighting according to the human condition
      digitalWrite(9,HIGH);
      digitalWrite(5,LOW);
      delay(4000);
    }
    
  }
  
}
