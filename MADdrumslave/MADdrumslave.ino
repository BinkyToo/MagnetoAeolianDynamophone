/* This programme runs on a slave controller in an automated musical instrument.
 *  Command strings are off the format address:note,duration
 *  In this particualr case, duration isn't used
 */

int drumpin = 13;
int bell2 = 11;
int bell3 = 12;
const int drumdelay = 45;
const int dingDelay = 80;
const int drumrelaxdelay = 5;
const String address = "percussion";
String addressstring = "";
String pitchstring = "";
String timestring = "";

void setup() {
  pinMode(drumpin, OUTPUT);
  pinMode(bell2, OUTPUT);
  pinMode(bell3, OUTPUT);
  digitalWrite(bell2,HIGH);
  digitalWrite(bell3,HIGH);
  Serial.begin(9600);
}

void loop() {
  String addressstring = "";
  String pitchstring = "";
  String timestring = "";
  if (Serial.available()) {
    addressstring = Serial.readStringUntil(':');
    pitchstring = Serial.readStringUntil(',');
    timestring = Serial.readStringUntil('.');
    if (addressstring == address) {
      if (pitchstring == "d") 
      {
        thumpdrum();
      }
      else if (pitchstring == "2")
      {
        ding2();
      }
      else if (pitchstring == "3")
      {
        ding3();
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

void ding2()
{
  digitalWrite(bell2, LOW);
  delay(dingDelay);
  digitalWrite(bell2,HIGH);
}

void ding3()
{
  digitalWrite(bell3, LOW);
  delay(dingDelay);
  digitalWrite(bell3,HIGH);
}

