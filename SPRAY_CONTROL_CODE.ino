#define SPRAY_PIN 7

void setup() {
  pinMode(SPRAY_PIN, OUTPUT);
  digitalWrite(SPRAY_PIN, LOW);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();

    if (cmd == 'W') {
      digitalWrite(SPRAY_PIN, HIGH); // Spray ON
    }
    else if (cmd == 'S') {
      digitalWrite(SPRAY_PIN, LOW);  // Spray OFF
    }
  }
}
