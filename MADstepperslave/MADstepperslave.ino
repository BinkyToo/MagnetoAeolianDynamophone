int step1 = 2;            // The pins used to control the stepper motor
int step2 = 3;
int step3 = 4;
int step4 = 5;

const String address = "stepper";   // Address used for recognising incoming commands
String addressstring = "";          // Initialise strings which will be populated by processing incomign commands
String pitchstring = "";
String timestring = "";

int bpm = 180;                      // Playback speed FIXME should be used in slave device
long cycles = 0;                    // Counting how many complete stepper rotations have been done
long cyclesDone = 0;
const int highTonedurations[] = {3822, 3405, 3034, 2703, 2551, 2273, 2025};   // Wave period in microseconds
const int lowTonedurations[] = {7644, 6810, 6060, 5406, 5102, 4546, 4050};
long duration = 30000000 / bpm;  //calculates the duration in microseconds for each quaver    // Placeholder initialisation
boolean forwards = 0;               // Which way is the stepper turning? we keep reversing it.

void setup() {
  Serial.begin(9600);
  Serial.println("\n\n=== Magneto-Aeolian Dynamophone: Stepper-motor module ===");
  pinMode(step1, OUTPUT);
  pinMode(step2, OUTPUT);
  pinMode(step3, OUTPUT);
  pinMode(step4, OUTPUT);
  Serial.println("Finished setup");
}

void loop() {
  if (Serial.available()) {
    Serial.println("Found incoming data");
    addressstring = Serial.readStringUntil(':');
    pitchstring = Serial.readStringUntil(',');
    timestring = Serial.readStringUntil('\n');
    if (addressstring == address) {
      Serial.println("\tData is addressed to this module");
      if (timestring.length() == 1) {         // FIXME should be able to handle multi-digit numbers
        int foo = 1*timestring.charAt(0)-48;  // debug stuff to be removed
        Serial.println(foo);
        Serial.println(bpm);
        duration = (30000000 / bpm) * (foo);  // How long the note is played for
        Serial.print("Setting duration to "); Serial.println(duration);
      }
      if (pitchstring.length() == 1) {
        Serial.println("Seems to have a valid single-character pitch string");
        byte ch = pitchstring.charAt(0);     // This bit can almost certainly be simplified (FIXME)
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
          Serial.println("Changing direction to forwards");
          step1 = 2;
          step2 = 3;
          step3 = 4;
          step4 = 5;
          forwards = 1;
        }
        else if (forwards == 1) {
          Serial.println("Changing direction to backwards");
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
  Serial.print("Playing note with time period "); Serial.println(timePeriod);
  cycles = duration / timePeriod;
  cycles = cycles / 4;
  Serial.print(cycles); Serial.println(" cycles");
  timePeriod = timePeriod - 20;    //it takes 20 us to do the lines.

  digitalWrite(step1, HIGH);       // why the extra cycle? DNRY seems aplicable here FIXME
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
