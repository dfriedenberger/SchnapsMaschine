// Pin 37 Oben, Pin 35 Unten
#define DIGITAL_PIN 37
boolean sensorOut = HIGH;
void setup() {
 Serial.begin(9600);
 pinMode(DIGITAL_PIN, INPUT);
}
void loop() {
 sensorOut = digitalRead(DIGITAL_PIN);
 if (sensorOut == LOW)
 {
 Serial.println("Obstacle detected!");
 }
 else
 {
 Serial.println("No Obstacle");
 } 
 delay(1000);
}
