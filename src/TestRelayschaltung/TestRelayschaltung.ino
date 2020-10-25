const int RELAIS1 = 22; //Arduino Pin 22
const int RELAIS2 = 24; //Arduino Pin 24
const int RELAIS3 = 26; //Arduino Pin 26
const int RELAIS4 = 28; //Arduino Pin 28
const int RELAIS5 = 23; //Arduino Pin 30
const int RELAIS6 = 25; //Arduino Pin 32
//const int RELAIS7 = 27; //Arduino Pin 30
//const int RELAIS8 = 29; //Arduino Pin 32
void setup() {
pinMode(RELAIS1, OUTPUT);
pinMode(RELAIS2, OUTPUT);
pinMode(RELAIS3, OUTPUT);
pinMode(RELAIS4, OUTPUT);
pinMode(RELAIS5, OUTPUT);
pinMode(RELAIS6, OUTPUT);
//pinMode(RELAIS7, OUTPUT);
//pinMode(RELAIS8, OUTPUT);
}
void loop() {
digitalWrite(RELAIS1, LOW);
digitalWrite(RELAIS2, HIGH);
digitalWrite(RELAIS3, HIGH);
digitalWrite(RELAIS4, HIGH);
digitalWrite(RELAIS5, HIGH);
digitalWrite(RELAIS6, HIGH);
delay(1000);
digitalWrite(RELAIS1, HIGH);
digitalWrite(RELAIS2, LOW);
digitalWrite(RELAIS3, HIGH);
digitalWrite(RELAIS4, HIGH);
digitalWrite(RELAIS5, HIGH);
digitalWrite(RELAIS6, HIGH);
delay(1000);
digitalWrite(RELAIS1, HIGH);
digitalWrite(RELAIS2, HIGH);
digitalWrite(RELAIS3, LOW);
digitalWrite(RELAIS4, HIGH);
digitalWrite(RELAIS5, HIGH);
digitalWrite(RELAIS6, HIGH);
delay(1000);
digitalWrite(RELAIS1, HIGH);
digitalWrite(RELAIS2, HIGH);
digitalWrite(RELAIS3, HIGH);
digitalWrite(RELAIS4, LOW);
digitalWrite(RELAIS5, HIGH);
digitalWrite(RELAIS6, HIGH);
delay(1000);
digitalWrite(RELAIS1, HIGH);
digitalWrite(RELAIS2, HIGH);
digitalWrite(RELAIS3, HIGH);
digitalWrite(RELAIS4, HIGH);
digitalWrite(RELAIS5, LOW);
digitalWrite(RELAIS6, HIGH);
delay(1000);
digitalWrite(RELAIS1, HIGH);
digitalWrite(RELAIS2, HIGH);
digitalWrite(RELAIS3, HIGH);
digitalWrite(RELAIS4, HIGH);
digitalWrite(RELAIS5, HIGH);
digitalWrite(RELAIS6, LOW);
delay(1000);
}
