#include <Servo.h>

Servo myservo;
String data;
unsigned prevmillis = 0;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);//Parse speed of serial string input
  Serial1.begin(9600);
  Serial2.begin(9600); //FOR DEBUG PURPOSES
  //Sets the mode of the motors to mode3 (check MD49 datasheet)
  Serial1.write(0);
  Serial1.write(52);
  Serial1.write(1);
  myservo.attach(9);
  delay(15);
  myservo.write(0);
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
    Serial2.println(data.substring(0, data.length() - 1));
    int index1 = data.indexOf("/");
    int index2 = data.indexOf("/",index1+1);
    int index3 = data.indexOf("/",index2+1);
    int index4 = data.indexOf("/",index3+1);
    int index5 = data.indexOf("/",index4+1);
    int motor1 = data.substring(0, index1).toInt();
    int motor2 = data.substring(index1+1, index2).toInt();
    int actuator1 = data.substring(index2+1, index2).toInt();
    int actuator2 = data.substring(index3+1, index3).toInt();
    int actuator3 = data.substring(index4+1, index5).toInt();
    int servo = data.substring(index5+1).toInt()/2;
    Serial.print(data);
    Serial.print(motor1);
    Serial.print(motor2);
    Serial1.write(0);
    Serial1.write(49);
    Serial1.write(motor1);
    Serial1.write(0);
    Serial1.write(50);
    Serial1.write(motor2);
    myservo.write(servo);
    //Serial2.println( "Command: " + command + "Value: " + value + "Svalue: " + svalue); //FOR DEBUG PURPOSES: WRITES VALUE READ TO SERIAL 2
    //Serial2.println(data); //FOR DEBUG PURPOSES: WRITES VALUE READ TO SERIAL 2
    //Serial2.println(data); //FOR DEBUG PURPOSES: WRITES VALUE READ TO SERIAL 2
    //speed = value (127 = max forward, -128 = max backwards)
    ///Serial.print(command);
  }
  //Serial2.println(command);

}
