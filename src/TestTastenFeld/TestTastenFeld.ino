#include <Keypad.h>
//Hier wird die größe des Keypads definiert
const byte COLS = 3; //3 Spalten
const byte ROWS = 4; //4 Zeilen
//Die Ziffern/Zeichen:
char hexaKeys[ROWS][COLS]={
{'#','0','*'},
{'9','8','7'},
{'6','5','4'},
{'3','2','1'}
};
byte colPins[COLS] = { 38, 40, 42 }; //Definition der Pins für die 3 Spalten
byte rowPins[ROWS] = { 44, 46, 48, 50 };//Definition der Pins für die 4 Zeilen
char Taste; //pressedKey entspricht in Zukunft den gedrückten Tasten
Keypad Tastenfeld = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); //Das Keypad kann absofort mit myKeypad angesprochen werden
void setup() {
Serial.begin(9600);
}

void loop() {
Taste = Tastenfeld.getKey(); //pressedKey entspricht der gedrückten Taste
if (Taste) { //Wenn eine Taste gedrückt wurde
Serial.print("Die Taste ");
Serial.print(Taste);
Serial.print("wurde gedrueckt");
Serial.println(); //Teile uns am Serial Monitor die gedrückte Taste mit
}
}
