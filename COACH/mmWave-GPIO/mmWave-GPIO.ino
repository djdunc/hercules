#define PresencePin  5

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(PresencePin, INPUT);  

}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.print("Presence State: ");
  Serial.println(digitalRead(PresencePin));

  delay(1000);
}
