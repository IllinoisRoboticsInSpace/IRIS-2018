//Original code in IRIS_Motor.ino.txt
//This code is modified to work for Cassie at RMC 2019

//#include <Servo.h> //We no longer have a servo motor
#include "pwm_actuator.h"
#define SPEED0 11 //Actuator 0 is the turntable motor 
#define DIR0 50
#define BRAKE0 4
#define SPEED1 8 //Actuator 1 is the Upper arm
#define DIR1 53
#define BRAKE1 3
#define SPEED2 9 //Actuator 2 is the forearm
#define DIR2 52
#define BRAKE2 2
#define SPEED3 10 //Actuator 3 is wrist
#define DIR3 51
#define BRAKE3 6

//Limit switch input pins for the turntable
#define LIMITLEFT 30
#define LIMITRIGHT 25

String data;
unsigned prevmillis = 0;

pwm_actuator turntable = pwm_actuator();
pwm_actuator actuatorarm = pwm_actuator();
pwm_actuator actuatorforearm = pwm_actuator();
pwm_actuator actuatorwrist = pwm_actuator();

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);//Parse speed of serial string input
  Serial1.begin(9600);
  Serial2.begin(9600); //FOR DEBUG PURPOSES
  delay(15);

  //Limit switch pins to make sure the turntable doesent go too far
  pinMode(LIMITLEFT, INPUT);
  pinMode(LIMITRIGHT, INPUT);
  

  turntable.init(
        SPEED0, DIR0, BRAKE0, //pins
        100, // extend_speed (forward)
        100, // retract_speedint (backwards)
        0 //brake_negate (enable negate)
  );
  actuatorarm.init(
        SPEED1, DIR1, BRAKE1, //pins
        255, // extend_speed (forward)
        255, // retract_speedint (backwards)
        0 //brake_negate (enable negate)
  );
  actuatorforearm.init(
        SPEED2, DIR2, BRAKE2, //pins
        194, // extend_speed (forward)
        194, // retract_speedint (backwards)
        0 //brake_negate (enable negate)
  );
  actuatorwrist.init(
        SPEED3, DIR3, BRAKE3,
        194,
        194,
        0
  );
}

void loop() {
  /*Takes in serial string input of the form: !command/value/servovalue!
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


  if (check) {
    // Serial2.println(data.substring(0, data.length() - 1));
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
    int actuator1 = data.substring(index2+1, index3).toInt(); //A/B
    int actuator2 = data.substring(index3+1, index4).toInt(); //Start/Select
    int actuator3 = data.substring(index4+1, index5).toInt(); //X/Y
    int actuator4 = data.substring(index5+1, index6).toInt(); //Left/Right
    int actuator5 = data.substring(index6+1, index7).toInt(); //Up/Down
    int servo = data.substring(index7+1).toInt()/2;
    Serial.println(data);
    
    Serial.print(motor1);
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
    Serial.println(actuator1);
    Serial.println(actuator2);
    Serial.println(actuator3);
    Serial.println(actuator4);
    Serial.println(actuator5);

    //A/B will be the forearm
    if (actuator1 == 0)
    {
      actuatorforearm.retract();
    }
    else if (actuator1 == 1)
    {
      actuatorforearm.stop();
    }
    else if (actuator1 == 2)
    {
      actuatorforearm.extend();
    }
    else{
      actuatorforearm.stop();
    }

    //Next actuator is the big arm, mapped to start/select
    if (actuator2 == 0)
    {
      actuatorarm.extend();
    }
    else if (actuator2 == 1)
    {
      actuatorarm.stop();
    }
    else if (actuator2 == 2)
    {
      actuatorarm.retract();
    }
    else{
      actuatorarm.stop();
    }

    //Next actuator is X/Y mapped to the wrist
    if (actuator3 == 0)
    {
      actuatorwrist.retract();
    }
    else if (actuator3 == 1)
    {
      actuatorwrist.stop();
    }
    else if (actuator3 == 2)
    {
      actuatorwrist.extend();
    }
    else{
      actuatorwrist.stop();
    }
    
    //Next is the turntable (spinny thing), mapped to left and right
    //We put in some extra tests to make sure that the thing won't go too far and mess things up
    if (actuator4 == 0 && digitalRead(LIMITLEFT) == HIGH)  
    {
      turntable.retract();
    }
    else if (actuator4 == 1)
    {
      turntable.stop();
    }
    else if (actuator4 == 2 && digitalRead(LIMITRIGHT) == LOW) //Christian wired this weirdly so we want it to be low on this one
    {
      turntable.extend();
    }
    else {
      turntable.stop(); 
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

 Serial.println(String(addr) + "\n" + String(command) + "\n" + String(data) + "\n" + String(((addr + command + data) & 127)));
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
