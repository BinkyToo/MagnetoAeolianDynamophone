void setup() {
  // put your setup code here, to run once:
  Serial2.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial2.print("orgn1:F, 80\n");
  delay(140);
}
