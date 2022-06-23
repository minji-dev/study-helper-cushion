int a;
const int motorPin=9;

void setup() {
  Serial.begin(9600);
  pinMode(motorPin, OUTPUT);
}

void loop() {
  if(Serial.available()) {
    a=(int)Serial.read();
    if(a =='1') { //존다
      analogWrite(motorPin, 255);
    }
    else if(a =='0') {
      analogWrite(motorPin, 0);
    }
  }
}
