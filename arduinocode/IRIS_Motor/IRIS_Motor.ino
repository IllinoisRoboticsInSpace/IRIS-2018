#include <Servo.h>

Servo myservo;
String command;
int value;
int svalue = 0;

void setup() {
  Serial.begin(115200);
  //Serial.setTimeout(50);//Parse speed of serial string input
  Serial1.begin(9600);
  //Sets the mode of the motors to mode3 (check MD49 datasheet)
  Serial1.write(0);
  Serial1.write(52);
  Serial1.write(3);
  myservo.attach(9);
}

void loop() {
  /*Takes in serial string input of the form: !command/value/servovalue!
   *command = go, stop, or turn 
   *value: [-128,127]
   *servovalue: [0, 360]
   */
  if (Serial.available()) { 
    String s = Serial.readString();
    int index = s.indexOf("/");
    int index2 = s.indexOf("/",index+1);
    int index3 = s.indexOf("/",index2+1);
    command = s.substring(0,index);
    value = s.substring(index+1, index2).toInt();
    svalue = s.substring(index2+1).toInt()/2;
    myservo.write(svalue);
    //checks for !s at the beginning and end and stops after 3s if not found
    if (!((s.charAt(0)==s.charAt(s.length()-3))&&(s.charAt(0)=='!'))){
      delay(3000);
      command = "!stop";
    }
  }
  //speed = value (127 = max forward, -128 = max backwards)
  Serial.print(command);
  if (command == "!go"){
    Serial1.write(0);
    Serial1.write(49);
    Serial1.write(value);
    Serial1.write(0);
    Serial1.write(50);
    Serial1.write(0);
  }
  //stops speed and turn
  if (command == "!stop"){
    Serial1.write(0);
    Serial1.write(49);
    Serial1.write(0);
    Serial1.write(0);
    Serial1.write(50);
    Serial1.write(0);
  }
  /*speed = speed + value for motor1
   *speed = speed - value for motor2
   *positve value = turn right, vice versa (-128 to 127)
   */
  if (command == "!turn"){
    Serial1.write(0);
    Serial1.write(50);
    Serial1.write(value);
  }
}
