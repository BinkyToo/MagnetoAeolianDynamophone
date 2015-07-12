int drumpin = 13;
const int drumdelay = 45;

void setup() {
  pinMode(drumpin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()){
    delay(1);
    String addressstring = Serial.readStringUntil(':');
    String pitchstring = Serial.readStringUntil(',');
    String timestring = Serial.readStringUntil('\n');
    if (addressstring=="drums"){
     if (pitchstring=="*"){
      thumpdrum();
     }
    }
  }
}

void thumpdrum() {
  digitalWrite(drumpin, HIGH);
  delay(drumdelay);
  digitalWrite(drumpin, LOW);
  Serial.print("thumpped drum\n");
}

