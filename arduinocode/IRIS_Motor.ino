String command;
int value;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial1.write(0);
  Serial1.write(52);
  Serial1.write(3);
}

void loop() {
  if (Serial.available()) { 
    String s = Serial.readString();
    int index = s.indexOf("/");
    command = s.substring(0,index);
    value = s.substring(index+1).toInt();
  }

  Serial.print(command);
  
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
