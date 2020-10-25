/***************************************************
  Schnapsmaschine
  Entwickelt von Wolfgang
    mit Unterstuetzung bei der Softwareentwicklung von Dirk

        .
        .
       . .
        ...
      \~~~~~/
       \   /
        \ /
         V
         |
         |
        ---
 ****************************************************/

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"


#include <Keypad.h>


//-------------- MP3-Player --------------
SoftwareSerial mySoftwareSerial(6, 7); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

//-------------- Tastenfeld --------------
//Groesse des Keypads 
const byte COLS = 3; //3 Spalten
const byte ROWS = 4; //4 Zeilen
//Die Ziffern/Zeichen:
char hexaKeys[ROWS][COLS]={
  {'#','0','*'},
  {'9','8','7'},
  {'6','5','4'},
  {'3','2','1'}
};

byte colPins[COLS] = { 34, 36, 38 }; //Definition der Pins für die 3 Spalten
byte rowPins[ROWS] = { 40, 42, 44, 46 };//Definition der Pins für die 4 Zeilen
char Taste = 0; //pressedKey entspricht in Zukunft den gedrückten Tasten
Keypad Tastenfeld = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

//-------------- Schrittmotor --------------
const int dir = 9; // RICHTUNGSPIN
const int Step = 8; // SCHRITT PIN
void motor(int richtung, int steps);


//-------------- Relais --------------
const byte RELAIS_COUNT = 6; //6 Relais
const int RELAIS1 = 22; //Arduino Pin 22
const int RELAIS2 = 24; //Arduino Pin 24
const int RELAIS3 = 26; //Arduino Pin 26
const int RELAIS4 = 28; //Arduino Pin 28
const int RELAIS5 = 23; //Arduino Pin 30
const int RELAIS6 = 25; //Arduino Pin 32
byte relaisPins[RELAIS_COUNT] = { RELAIS1, RELAIS2, RELAIS3, RELAIS4 , RELAIS5 , RELAIS6 };//Definition der Pins für die Relais
void relais(int x, int dauer);

void setup()
{
  Serial.begin(9600);
  
  Serial.println();
  Serial.println(F("Schnapsmaschine Version 1.0"));
  
  //Initialisierung MP3-Player
  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) 
  {  
    Serial.println(F("MP3-Player nicht gefunden. Bitte pruefen."));
  }
  else
  {
    Serial.println(F("MP3-Player erfolgreich initialisiert."));
    myDFPlayer.volume(30);  //Set volume value. From 0 to 30
    //myDFPlayer.play(2);  //Play Startnachricht
  }

  //Initialsierung Schrittmotor
  pinMode (dir, OUTPUT); // RICHTUNG ALS AUSGABE
  pinMode (Step, OUTPUT); // SCHRITT ALS AUSGANG

  //Initialsierung Relais
  for (int x = 0; x <RELAIS_COUNT; x++)
  {
    pinMode(relaisPins[x], OUTPUT);
    //Defaultzustand = AUS
    digitalWrite(relaisPins[x], HIGH);
  }

}

void loop()
{
  static unsigned long timer = millis();

  Taste = Tastenfeld.getKey(); //pressedKey entspricht der gedrückten Taste
  if (Taste) { //Wenn eine Taste gedrückt wurde
    Serial.print("Die Taste ");
    Serial.print(Taste);
    Serial.print(" wurde gedrueckt");
    Serial.println(); //Teile uns am Serial Monitor die gedrückte Taste mit

    if(Taste == '0')
    {
      //Test-MP3-Player
      Serial.println(F("Teste MP3-Player."));
      myDFPlayer.play(1);  //Play MP3 1
    }
    
    if(Taste == '*')
    {
      //Test-Schrittmotor
      Serial.println(F("Teste Schrittmotor."));
      motor(LOW,1170); //1170 viertelUmderhung,2340 Halbe Umdrehung 
      delay (1000); 
      motor(HIGH,1170); //1170 viertelUmderhung,2340 Halbe Umdrehung 
      delay (1000); 
    }
    
    if(Taste == '#')
    {
      //Test-Relais
      Serial.println(F("Teste Relais."));
      for (int x = 0; x <RELAIS_COUNT; x++)
      {
        relais(x,1000);
      }
    }
   

  
  }

 
  if (millis() - timer > 3000) {
    timer = millis();
    Serial.println(F("Schnapsmaschine bereit. Bitte Taste druecken!"));
  }
  
  
  //Beobachtung
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}


//Schrittmotor bewegen
void motor(int richtung, int steps)
{

  digitalWrite (dir, richtung); // richtung LOW = vorwaerts/links , HIGH = rueckwaerts/rechts
  for (int x = 0; x <1170; x ++) // 1170 viertelUmderhung,2340 Halbe Umdrehung 
  {
    digitalWrite (Step, HIGH); // SCHRITT HOCH
    delayMicroseconds(1500); // WARTEN
    digitalWrite (Step, LOW); // SCHRITT NIEDRIG
    delayMicroseconds(1500); // WARTEN
  }
}

//Relais schalten
void relais(int x, int dauer)
{
    digitalWrite(relaisPins[x], LOW); //Einschalten
    delay(dauer); //WARTEN
    digitalWrite(relaisPins[x], HIGH);//Ausschalten

}


void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  
}
