int step1 = 2;
int step2 = 3;
int step3 = 4;
int step4 = 5;

const String address = "stepper";
String addressstring = "";
String pitchstring = "";
String timestring = "";

long bpm = 180;
int pause = 0;
long cycles = 0;
long cyclesDone = 0;
const int highTonedurations[] = {3822, 3405, 3034, 2703, 2551, 2273, 2025};
const int lowTonedurations[] = {7644, 6810, 6060, 5406, 5102, 4546, 4050};
int duration = 30000000 / bpm;  //calculates the duration in microseconds for each quaver
boolean forwards = 0;

void setup() {
  Serial.begin(9600);
  pinMode(step1, OUTPUT);
  pinMode(step2, OUTPUT);
  pinMode(step3, OUTPUT);
  pinMode(step4, OUTPUT);

}

void loop() {
  if (Serial.available()) {
    addressstring = Serial.readStringUntil(':');
    pitchstring = Serial.readStringUntil(',');
    timestring = Serial.readStringUntil('\n');
    if (addressstring == address) {
      if (timestring.length() == 1) {
        duration = (30000000 / bpm) * timestring.charAt(1);
      }
      if (pitchstring.length() == 1) {
        char ch = pitchstring.charAt(1);
        if (ch == 'z') {
          delay(pause);
        }
        if (ch >= 'C' and ch <= 'G') {
          playNote(lowTonedurations[ch - 'C']);
        }
        if (ch >= 'c' and ch <= 'g') {
          playNote(highTonedurations[ch - 'c']);
        }
        if ( ch >= 'A' and ch <= 'B') {
          playNote(lowTonedurations[ch - 60]);
        }
        if (ch >= 'a' and ch < 'b') {
          playNote(highTonedurations[ch - 92]);
        }
        if (forwards == 0) {
          step1 = 2;
          step2 = 3;
          step3 = 4;
          step4 = 5;
          forwards = 1;
        }
        else if (forwards == 1) {
          step1 = 5;
          step2 = 4;
          step3 = 3;
          step4 = 2;
          forwards = 0;
        }
      }
    }
  }
}

  void playNote(long timePeriod) {
    cycles = duration / timePeriod;
    cycles = cycles / 4;
    timePeriod = timePeriod - 20;    //it takes 20 us to do the lines.

    digitalWrite(step1, HIGH);
    delayMicroseconds(timePeriod);
    digitalWrite(step1, LOW);

    digitalWrite(step2, HIGH);
    delayMicroseconds(timePeriod);
    digitalWrite(step2, LOW);

    digitalWrite(step3, HIGH);
    delayMicroseconds(timePeriod);
    digitalWrite(step3, LOW);

    digitalWrite(step4, HIGH);
    delayMicroseconds(timePeriod);
    digitalWrite(step4, LOW);

    while (cyclesDone < cycles) {
      digitalWrite(step1, HIGH);
      delayMicroseconds(timePeriod);
      digitalWrite(step1, LOW);

      digitalWrite(step2, HIGH);
      delayMicroseconds(timePeriod);
      digitalWrite(step2, LOW);

      digitalWrite(step3, HIGH);
      delayMicroseconds(timePeriod);
      digitalWrite(step3, LOW);

      digitalWrite(step4, HIGH);
      delayMicroseconds(timePeriod);
      digitalWrite(step4, LOW);

      cyclesDone++;
    }
    cyclesDone = 0;
  }
