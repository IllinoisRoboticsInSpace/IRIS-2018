#include <Servo.h>
#include "pwm_actuator.h"
#define SPEED 8
#define DIR 49
#define BRAKE 51
#define SPEED1 7
#define DIR1 48
#define BRAKE1 50
#define SPEED2 6
#define DIR2 44
#define BRAKE2 46
int actuatortest = 10;
int actuatorbrake = 11;
int actuatortest1 = 2;
int actuatorbrake1 = 3;
Servo myservo;
String data;
unsigned prevmillis = 0;
pwm_actuator actuator = pwm_actuator();
pwm_actuator actuatorl = pwm_actuator();
pwm_actuator actuatorr = pwm_actuator();
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);//Parse speed of serial string input
  Serial1.begin(9600);
  Serial2.begin(9600); //FOR DEBUG PURPOSES
  //Sets the mode of the motors to mode3 (check MD49 datasheet)
//  Serial1.write(0);
//  Serial1.write(52);
//  Serial1.write(1);
  myservo.attach(9);
  delay(15);
  myservo.write(0);
  pinMode(actuatortest, OUTPUT);
  pinMode(actuatorbrake, OUTPUT);
  pinMode(actuatortest1, OUTPUT);
  pinMode(actuatorbrake1, OUTPUT);
  digitalWrite(actuatorbrake1,HIGH);
  digitalWrite(actuatorbrake, HIGH);
  actuator.init(
        SPEED, DIR, BRAKE, //pins
        194, // extend_speed (forward)
        194, // retract_speedint (backwards)
        0 //brake_negate (enable negate)
  );
  actuatorl.init(
        SPEED1, DIR1, BRAKE1, //pins
        194, // extend_speed (forward)
        194, // retract_speedint (backwards)
        0 //brake_negate (enable negate)
  );
  actuatorr.init(
        SPEED2, DIR2, BRAKE2, //pins
        194, // extend_speed (forward)
        194, // retract_speedint (backwards)
        0 //brake_negate (enable negate)
  );
}

void loop() {

//   Serial.println("Looping.../");
  /*Takes in serial string input of the form: !command/value/servovalue#!
    command = go, stop, or turn
    value: [-128,127]
    servovalue: [0, 360]
  */
  bool check = false;
  //  if (Serial.available()) {
  //    Serial2.println(Serial.read());
  //    String s = Serial.readString();
  //    //checks for !s at the beginning and end and stops after 3s if not found
  //    /*if (!((s.charAt(0)==s.charAt(s.length()-3))&&(s.charAt(0)=='!'))){
  //      delay(3000);
  //      command = "!stop";
  //    }
  //    */
  //  }

  while (Serial.available() > 0)
  {
    data = "";
    int c = Serial.read(); // get next char
    while (c != '!')
    {
      if (c > 0) // check valid readf
        data += (char) c; // append to string

      c = Serial.read(); // get next char
    }

    // Check we got good data
    check = data.endsWith("#");
  }
  //  Command format:
  //  !motor1/motor2/act1/act2/act3/act4/act5/servo#!
  if (check) {
    Serial.println("We have data");
    Serial2.println(data.substring(0, data.length() - 1));
    //     0/0/1/1/1
    int index1 = data.indexOf("/");
    int index2 = data.indexOf("/",index1+1);
    int index3 = data.indexOf("/",index2+1);
    int index4 = data.indexOf("/",index3+1);
    int index5 = data.indexOf("/",index4+1);
    int index6 = data.indexOf("/",index5+1);
    int index7 = data.indexOf("/",index6+1);    
    int motor1 = data.substring(0, index1).toInt();
    int motor2 = data.substring(index1+1, index2).toInt();
    int actuator1 = data.substring(index2+1, index3).toInt();
    int actuator2 = data.substring(index3+1, index4).toInt();
    int actuator3 = data.substring(index4+1, index5).toInt();
    int actuator4 = data.substring(index5+1, index6).toInt();
    int actuator5 = data.substring(index6+1, index7).toInt();
    int servo = data.substring(index7+1).toInt()/2;
    Serial.println(data);
    
    Serial.print("motor1: ");
    Serial.println(motor1);
    Serial.print("motor2: ");
    Serial.print(motor2);

    motorControl(motor1,motor2,130);
    /*
    Serial1.write(0);
    Serial1.write(49);
    Serial1.write(motor1);
    Serial1.write(0);
    Serial1.write(50);
    Serial1.write(motor2);
    myservo.write(servo);
    */
    Serial.print("actuator1: ");
    Serial.println(actuator1);
    Serial.print("actuator2: ");
    Serial.println(actuator2);
    Serial.print("actuator3: ");
    Serial.println(actuator3);
    Serial.print("actuator4: ");
    Serial.println(actuator4);

    if (actuator1 == 0)
    {
      actuator.retract();
    }
    else if (actuator1 == 1)
    {
      actuator.stop();
    }
    else if (actuator1 == 2)
    {
      actuator.extend();
    }
    else{
      analogWrite(SPEED, 0);          //Stop actuator
      digitalWrite(BRAKE, HIGH != 0);
    }
    if (actuator2 == 0)
    {
      actuatorl.retract();
    }
    else if (actuator2 == 1)
    {
      actuatorl.stop();
    }
    else if (actuator2 == 2)
    {
      actuatorl.extend();
    }
    else{
      analogWrite(SPEED1, 0);          //Stop actuator
      digitalWrite(BRAKE1, HIGH != 0);
    }
    if (actuator3 == 0)
    {
      actuatorr.retract();
    }
    else if (actuator3 == 1)
    {
      actuatorr.stop();
    }
    else if (actuator3 == 2)
    {
      actuatorr.extend();
    }
    else{
      analogWrite(SPEED2, 0);          //Stop actuator
      digitalWrite(BRAKE2, HIGH != 0);
    }
    if(actuator4==2){
      digitalWrite(actuatorbrake,LOW);
      digitalWrite(actuatortest, HIGH);
    }
    else if(actuator4==0){
      digitalWrite(actuatorbrake,LOW);
      digitalWrite(actuatortest, LOW);
    }
    else{
      digitalWrite(actuatorbrake,HIGH);
    }
    if(actuator5==2){
      digitalWrite(actuatorbrake1,LOW);
      digitalWrite(actuatortest1, HIGH);
    }
    else if(actuator5==0){
      digitalWrite(actuatorbrake1,LOW);
      digitalWrite(actuatortest1, LOW);
    }
    else{
      digitalWrite(actuatorbrake1,HIGH);
    }
  }

}


int motorControl(double left, double right, int addr) {
 int command;
 int data;

 right = 0 - right;

 boolean leftForward = left > 0;
 boolean rightForward = right > 0;

 left = abs(left);
 left = left / 1000 * 127;
 right = abs(right);
 right = right / 1000 * 127;


 if (leftForward) {
   command = 0;
 } else {
   command = 1;
 }


 data = left;

 Serial.println("addr: " + String(addr) + "\n" + "command: " + String(command) + "\n" + "data: " + String(data) + "\n" + "All added and bitmasked with 0x7F: " + String(((addr + command + data) & 127)));
 Serial1.write((int)addr);
 Serial1.write((int)command);
 Serial1.write((int)data);
 Serial1.write((int)((addr + command + data) & 127));

 if (rightForward) {
   command = 4;
 } else {
   command = 5;
 }

 data = right;

 Serial.println(String(addr) + "\n" + String(command) + "\n" + String(data) + "\n" + String(((addr + command + data) & 127)));
 Serial1.write((int)addr);
 Serial1.write((int)command);
 Serial1.write((int)data);
 Serial1.write((int)((addr + command + data) & 127));
}
