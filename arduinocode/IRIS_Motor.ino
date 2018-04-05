#include <Servo.h>

Servo myservo;
String command;
int value;
int pos = 0;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(50);
  Serial1.begin(115200);
  Serial1.write(0);
  Serial1.write(52);
  Serial1.write(3);
  myservo.attach(9);
}

void loop() {
  if (Serial.available()) { 
    String s = Serial.readString();
    int index = s.indexOf("/");
    int index2 = s.indexOf("/",index+1);
    command = s.substring(0,index);
    value = s.substring(index+1, index2).toInt();
    pos = s.substring(index2+1).toInt()/2;
    myservo.write(pos);
  }
  
  if (command == "go"){
    Serial1.write(0);
    Serial1.write(49);
    Serial1.write(value);
    Serial1.write(0);
    Serial1.write(50);
    Serial1.write(0);
  }

  if (command == "stop"){
    Serial1.write(0);
    Serial1.write(49);
    Serial1.write(0);
    Serial1.write(0);
    Serial1.write(50);
    Serial1.write(0);
  }
  
  if (command == "turn"){
    Serial1.write(0);
    Serial1.write(50);
    Serial1.write(value);
  }
}
