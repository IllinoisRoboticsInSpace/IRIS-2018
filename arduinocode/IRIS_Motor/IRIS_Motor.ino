#include <Servo.h>

Servo myservo;
String command;
String data;
unsigned prevmillis = 0;
int value;
int svalue = 0;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);//Parse speed of serial string input
  Serial1.begin(9600);
  Serial2.begin(9600); //FOR DEBUG PURPOSES
  //Sets the mode of the motors to mode3 (check MD49 datasheet)
  Serial1.write(0);
  Serial1.write(52);
  Serial1.write(3);
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
    int index = data.indexOf("/");
    int index2 = data.indexOf("/", index + 1);
    int index3 = data.indexOf("/", index2 + 1);
    command = data.substring(0, index);
    value = data.substring(index + 1, index2).toInt();
    svalue = data.substring(index2 + 1).toInt() / 2;
    myservo.write(svalue);
    //Serial2.println( "Command: " + command + "Value: " + value + "Svalue: " + svalue); //FOR DEBUG PURPOSES: WRITES VALUE READ TO SERIAL 2
    //Serial2.println(data); //FOR DEBUG PURPOSES: WRITES VALUE READ TO SERIAL 2
    //Serial2.println(data); //FOR DEBUG PURPOSES: WRITES VALUE READ TO SERIAL 2
    //speed = value (127 = max forward, -128 = max backwards)
    ///Serial.print(command);
    if (command == "go") {
      //Serial2.println(command);
      Serial1.write(0);
      Serial1.write(49);
      Serial1.write(value);
      Serial1.write(0);
      Serial1.write(50);
      Serial1.write(0);
    }
    //stops speed and turn
    if (command == "stop") {
      //Serial2.println(command);
      Serial1.write(0);
      Serial1.write(49);
      Serial1.write(0);
      Serial1.write(0);
      Serial1.write(50);
      Serial1.write(0);
    }
    /*speed = speed + value for motor1
      speed = speed - value for motor2
      positve value = turn right, vice versa (-128 to 127)
    */
    if (command == "turn") {
      //Serial2.println(command);
      Serial1.write(0);
      Serial1.write(50);
      Serial1.write(value);
    }
  }
  //Serial2.println(command);

}
