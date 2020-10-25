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

byte colPins[COLS] = { 38, 40, 42 }; //Definition der Pins für die 3 Spalten
byte rowPins[ROWS] = { 44, 46, 48, 50 };//Definition der Pins für die 4 Zeilen
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
const int RELAIS5 = 30; //Arduino Pin 30
const int RELAIS6 = 32; //Arduino Pin 32
byte relaisPins[RELAIS_COUNT] = { RELAIS1, RELAIS2, RELAIS3, RELAIS4 , RELAIS5 , RELAIS6 };//Definition der Pins für die Relais
void relais(int x, int dauer);



//------------- IR Sensoren -----------
// Pin 35 Oben, Pin 37 Unten
#define IR_SENSOR_OBEN   35
#define IR_SENSOR_UNTEN  37

#define GLAS_GROSS  2 
#define GLAS_KLEIN  1
#define GLAS_FEHLER 0

int teste_glas();



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

  //Initialsierung IR Sensoren
  pinMode(IR_SENSOR_OBEN, INPUT);
  pinMode(IR_SENSOR_UNTEN, INPUT);


}

int phase = 0;


void erstelle_getraenk(int glas,int ziel,int relaisnr,int fuellzeit);

void loop()
{
  static unsigned long timer = millis();

  Taste = Tastenfeld.getKey(); //pressedKey entspricht der gedrückten Taste
  if (Taste) { //Wenn eine Taste gedrückt wurde

    //Ausgabe welche Taste gedrückt wurde
    Serial.print("Die Taste ");
    Serial.print(Taste);
    Serial.print(" wurde gedrueckt");
    Serial.println(); 

    if(Taste == '#')
    {
      Serial.println(F("Reset, Springe zu Phase 1."));
      phase = 0;
    }
    
    if(Taste == '*')
    {
       Serial.println(F("Testphase."));
       phase = 99;
    }

  }

  
    switch(phase)
    {
      case 0:
        //Begrüßung
        myDFPlayer.play(11);  //Begruessung
        Serial.println(F("Schnapsmaschine bereit. Bitte Taste druecken!"));

        /* FALLTHROUGH */
      case 1: //betriebsphase
        if(Taste == '1')
        {
          erstelle_getraenk(GLAS_KLEIN,3 /* position */,2,1500);
          

        }
        break;
      case 99:
        //Testphase
        if(Taste == '1')
        {
          //Test-MP3-Player
          Serial.println(F("Teste MP3-Player."));
          myDFPlayer.play(2);  //Play MP3 2
        }
        if(Taste == '2')
        {
           //Test-Schrittmotor
           Serial.println(F("Teste Schrittmotor nach links."));
           motor(LOW,1170); //1170 viertelUmderhung,2340 Halbe Umdrehung 
        }
        if(Taste == '3')
        {
           //Test-Schrittmotor
           Serial.println(F("Teste Schrittmotor nach rechts."));
           motor(HIGH,1170); //1170 viertelUmderhung,2340 Halbe Umdrehung 
        }
        if(Taste == '4')
        {
          //Test-Relais
          Serial.println(F("Teste Relais."));
          for (int x = 0; x <RELAIS_COUNT; x++)
          {
            relais(x,1500);
          }
        }
        if(Taste == '5')
        {
            switch(teste_glas())
            {
              case GLAS_KLEIN:
                 myDFPlayer.play(20);  //Play MP3
                 break;
              case GLAS_GROSS:
                 myDFPlayer.play(21);  //Play MP3
                 break;
               default:
                 myDFPlayer.play(22);  //Play MP3
                 break;
            }
        }
        break;
    }

  

    
      
  //Beobachtung
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}

//                0 <-> 1 <-> 2 <-> 3 <-> 4 <-> 5 
int schritte[6] = { 1170, 1170, 1170, 1170, 1170 };

int fahre_zu_position(int aktuell,int ziel)
{
    
   Serial.println(F("Reset, Springe zu Phase 1."));

   while(aktuell != ziel)
   {
      if(aktuell < ziel)
      {
         //nach rechts fahren z.B. von 2 (Mitte) nach 3 (rechts)
          Serial.println(F("Schrittmotor nach rechts."));
          if(aktuell >= 5) //Position 5 - Endposition
          {
             Serial.println(F("Nach rechts nicht mehr möglich"));
             return -1;
          }
          motor(LOW,schritte[aktuell]); //1170 viertelUmderhung,2340 Halbe Umdrehung 
          aktuell = aktuell + 1;

      }
      else {
        //nach links fahren von 2 nach 1
        Serial.println(F("Schrittmotor nach links."));
        if(aktuell <= 0) //position 0 - Endposition
        {
           Serial.println(F("Nach links nicht mehr möglich"));
           return -1;
        }
        motor(LOW,schritte[aktuell-1]); //1170 viertelUmderhung,2340 Halbe Umdrehung 
        aktuell = aktuell - 1;
      }
      
    
   }


   return ziel;
  
}


//----- Getraenk zubereiten
void erstelle_getraenk(int glas,int ziel,int relaisnr,int fuellzeit)
{
    /*
     * Glas prüfen
     * 
     */
    int glas_erkannt = teste_glas();
    if(glas != glas_erkannt)
    {
        if(glas_erkannt == GLAS_FEHLER)
        {
           if(glas == GLAS_GROSS)
           {
              myDFPlayer.play(23);  //Play MP3
           }
           else //Schnapsglas
           {
              myDFPlayer.play(24);  //Play MP3
           }
        }
        else
        {
           if(glas == GLAS_GROSS)
           {
              myDFPlayer.play(25);  //Play MP3
           }
           else //Schnapsglas
           {
              myDFPlayer.play(26);  //Play MP3
           }
        }
        return;      
    }
    
  

    /*
     * Position anfahren
     */
     int start = 2; //(Mitte)
    int aktuell = fahre_zu_position(start,ziel);
    if(aktuell < 0)
    {
       myDFPlayer.play(30);  //Play MP3
       return;
    }

    /*
     * füllen
     */
      relais(relaisnr,fuellzeit);


     /*
      * eventuell. 2 Füllstation
      */
      /*
      * zu Start fahren
      */
    
    aktuell = fahre_zu_position(aktuell,start);

  
}




//---- IR sensoren -----
int teste_glas()
{
  boolean oben = digitalRead(IR_SENSOR_OBEN);
  boolean unten = digitalRead(IR_SENSOR_UNTEN);

  if(oben == LOW && unten == LOW)
  {
     return GLAS_GROSS;
  }
  if(oben == HIGH && unten == LOW)
  {
     return GLAS_KLEIN;
  }
  return GLAS_FEHLER;
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
