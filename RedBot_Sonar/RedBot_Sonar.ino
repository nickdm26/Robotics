int trigPin = A3;
int echoPin = A2;


void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  float duration = pulseIn(echoPin, HIGH);

  float distance = duration / 58;
  Serial.println(distance);


}
