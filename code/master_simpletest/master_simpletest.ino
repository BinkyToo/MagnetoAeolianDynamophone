void setup() {
  // put your setup code here, to run once:
  Serial2.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial2.print("orgn0:A,800\n");
  delay(1600);
}
