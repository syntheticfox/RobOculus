#include <Servo.h>

Servo horPlane; //servo in the horizontal plane
Servo vertPlane; //servo in the vertPlane

char pos[2];
int horPos = 90, vertPos = 90;

void setup(){
  horPlane.attach(11);
  vertPlane.attach(10);
  Serial.begin(9600);
}

void loop(){ 
  if(Serial.available() > 0){
    Serial.readBytes(pos, 2); 
    horPos = (int)pos[0] * 2;//input is half of actual degree
    vertPos = (int)pos[1] * 2;
    
    if(vertPos < 61) vertPos = 61;//servo cannot go lower than this physically
    
    horPlane.write(horPos);//move servo to specific degree
    vertPlane.write(vertPos);
    delay(15);    
  }  
}
