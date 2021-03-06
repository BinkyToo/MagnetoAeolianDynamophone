/* This programme runs on a slave controller in an automated musical instrument.
 *  Command strings are off the format address:note,duration
 *  In this particualr case, duration isn't used
 */

int drumpin = 13;
const int drumdelay = 45;
const int dingDelay = 80;
const int drumrelaxdelay = 5;
const String address = "drum";
String addressstring = "";
String pitchstring = "";
String timestring = "";

void setup() {
  pinMode(drumpin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  addressstring = "";
  pitchstring = "";
  timestring = "";
  if (Serial.available()) {
    addressstring = Serial.readStringUntil(':');
    pitchstring = Serial.readStringUntil(',');
    timestring = Serial.readStringUntil('\n');
    if (addressstring == address) {
      if (pitchstring == "*") {
        thumpdrum();
      }
     }
  }
}

void thumpdrum() {
  digitalWrite(drumpin, HIGH);
  delay(drumdelay);
  digitalWrite(drumpin, LOW);
  delay(drumrelaxdelay);
}


