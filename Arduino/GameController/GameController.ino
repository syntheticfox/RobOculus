int topleft = 3;//Wheels
int topright = 11;
int botleft = 5;
int botright = 9;

//inputs for motor drivers
int left_in1 = 7, left_in2 = 6, left_in3 = 4, left_in4 = 2;
int right_in1 = 13, right_in2 = 12, right_in3 = 10, right_in4 = 8;

char mode; //forward, backward
int left;
int right;

char data[3];//first byte: mode, second bypte: left wheel values, third byte: right wheel values

void setup(){
  Serial.begin(9600);
  
   pinMode(topleft,OUTPUT);//set motors speed as output
   pinMode(topright,OUTPUT);
   pinMode(botleft,OUTPUT);
   pinMode(botright,OUTPUT);
  
 
   pinMode(left_in1, OUTPUT);
   pinMode(left_in2, OUTPUT);
   pinMode(left_in3, OUTPUT);
   pinMode(left_in4, OUTPUT);
   
   pinMode(right_in1, OUTPUT);
   pinMode(right_in2, OUTPUT);
   pinMode(right_in3, OUTPUT);
   pinMode(right_in4, OUTPUT);
  
}
void loop(){
  if(Serial.available() > 0){    
    Serial.readBytes(data, 3);
    mode = data[0];
    left = 2 * (int)data[1];
    right = 2 * (int)data[2];
           
    //the following code should be cleaned up of redundancies      
    if(mode == 'f'){//move forward
       analogWrite(botleft, left);
       analogWrite(botright, right);
       analogWrite(topleft, left);
       analogWrite(topright, right);
        
       digitalWrite(left_in1, HIGH);//forward
       digitalWrite(left_in2, LOW);
       digitalWrite(left_in3, HIGH);
       digitalWrite(left_in4, LOW);
       
       digitalWrite(right_in1, HIGH);//forward
       digitalWrite(right_in2, LOW);
       digitalWrite(right_in3, HIGH);
       digitalWrite(right_in4, LOW);
    }
    else if(mode == 'b'){//move backward (values are neg)
       analogWrite(topleft, left);
       analogWrite(topright, right);
       analogWrite(botleft, left);
       analogWrite(botright, right);
      
       digitalWrite(left_in1, LOW);//backward
       digitalWrite(left_in2, HIGH);
       digitalWrite(left_in3, LOW);
       digitalWrite(left_in4, HIGH);
       
       digitalWrite(right_in1, LOW);//backward
       digitalWrite(right_in2, HIGH);
       digitalWrite(right_in3, LOW);
       digitalWrite(right_in4, HIGH); 
    }
}
