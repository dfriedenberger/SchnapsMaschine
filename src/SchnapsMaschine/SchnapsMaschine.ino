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



#define INIT 1 //Die Schnapsmaschine wurde erfolgreich gestartet.
#define BEREIT 2 //Hallo! Bitte wähle ein Getränk.
#define GETRAENK_START 3 //Eine ausgezeichnete Wahl. Ich beginne jetzt mit dem Zubereiten.
#define GETRAENK_FERIG 4 //Dein Getränk ist fertig. Prost!
#define TEST_GLAS_KLEIN 5 // Du hast ein kleines Glas hingestellt. Du willst wohl einen Schnaps? 
#define TEST_GLAS_GROSS 6 // Du hast ein großes Glas hingestellt. Du willst wohl ein Mixgetränk? 
#define TEST_KEIN_GLAS 7 // Es steht kein Glas da. 
#define GLAS_KEIN_GROSS 8  //Du hast kein Glas hingestellt. Bitte stelle ein großes Glas hin. 
#define GLAS_KEIN_KLEIN 9 //Du hast kein Glas hingestellt. Bitte stelle ein Schnapsglas hin.
#define GLAS_KLEIN_GROSS 10 //Du hast ein kleines Glas hingestellt. Bitte stelle ein großes Glas hin.
#define GLAS_GROSS_KLEIN 11 //Du hast ein großes Glas hingestellt. Bitte stelle ein kleines Glas hin.
#define FEHLER 12 //Es ist ein Fehler aufgetreten. Abbruch.
#define TEST_AUSGABE 13 // test test test
#define JOKE_FIRST        14
// ...
#define JOKE_LAST         24
#define PROGRAMMMENUE     25 // Bitte wähle ein Programm! 0 - Hauptprogramm, 1 - Testprogramm, 2 - Spülprogramm
#define TESTPROGRAMM      26 // Testprogramm
#define SPUELPROGRAMM     27 // Spülprogramm


boolean playsong = false;
void warteaufsongende();

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
const int aktiv = 10; //SLEEP PIN
const int dir = 9; // RICHTUNGS PIN
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
#define IR_SENSOR_OBEN   11
#define IR_SENSOR_UNTEN  35

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
  {
    Serial.println(F("MP3-Player erfolgreich initialisiert."));
    myDFPlayer.volume(28);  //Set volume value. From 0 to 30
  }

  //Initialsierung Schrittmotor
  pinMode (dir, OUTPUT); // RICHTUNG ALS AUSGABE
  pinMode (Step, OUTPUT); // SCHRITT ALS AUSGANG
  //pinMode (aktiv, OUTPUT); // SLEEP

  //digitalWrite (aktiv, HIGH); // Motor aus

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


  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
}


//-----------  Commands
void command();

int phase = 0;


void erstelle_getraenk(int glas,int ziel,int relaisnr,int fuellzeit);

void loop()
{
  static unsigned long lastkey = millis();

  command();

  Taste = Tastenfeld.getKey(); //pressedKey entspricht der gedrückten Taste


 
  if (Taste) { //Wenn eine Taste gedrückt wurde
    lastkey = millis();

    //Ausgabe welche Taste gedrückt wurde
    Serial.print(F("Die Taste "));
    Serial.print(Taste);
    Serial.print(F(" wurde gedrueckt"));
    Serial.println(); 

    if(Taste == '#')
    {
      Serial.println(F("Reset, Springe zu Phase 1."));
      phase = 0;
    }
    
    if(Taste == '*')
    {
       Serial.println(F("Waehle Programm."));
       myDFPlayer.play(PROGRAMMMENUE);  
       phase = 2;
    }
  }

  
    switch(phase)
    {
      case 0:
        //Begruessung
        myDFPlayer.play(BEREIT);  //Begruessung
        Serial.println(F("Schnapsmaschine bereit. Bitte Taste druecken!"));
        /* FALLTHROUGH */
        phase = 1;
      case 1: //betriebsphase
        if(Taste == '1')
        {
          erstelle_getraenk(GLAS_GROSS,1 /* position */,RELAIS1 /* Relais */,1600);
        }
        if(Taste == '2')
        {
          erstelle_getraenk(GLAS_GROSS,2 /* position */,RELAIS2 /* Relais */,1600);
        }
        if(Taste == '3')
        {
          erstelle_getraenk(GLAS_GROSS,3 /* position */,RELAIS3 /* Relais */,1600);
        }
        if(Taste == '4')
        {
          erstelle_getraenk(GLAS_GROSS,4 /* position */,RELAIS4 /* Relais */,1600);
        }
        if(Taste == '5')
        {
          erstelle_getraenk(GLAS_GROSS,5 /* position */,RELAIS5 /* Relais */,1600);
        }
        if(Taste == '6')
        {
          erstelle_getraenk(GLAS_GROSS,6 /* position */,RELAIS6 /* Relais */,1600);
        }
        
        if((millis() - lastkey) > 60000)
        {
          int song = random(JOKE_FIRST , JOKE_LAST + 1);
          Serial.println(F("Timeout einige Minuten gewartet"));
          myDFPlayer.play(song);
          //simulate key press
          lastkey = millis();
        }
       
        break;
      case 2:
        if(Taste == '0')
        {
          //Hauptprogramm
          phase = 0;
          break;
        }
        if(Taste == '1')
        {
          //Testprogramm
          phase = 99;
          Serial.println(F("Testprogramm"));
          myDFPlayer.play(TESTPROGRAMM);  
          break;
        }
        if(Taste == '2')
        {
          //Spülprogramm
          phase = 98;
          Serial.println(F("Spuelprogramm"));
          myDFPlayer.play(SPUELPROGRAMM);  
          break;
        }
        break;
      case 98: 
        if(Taste == '1') relais(RELAIS1, 3000); //Einschalten
        if(Taste == '2') relais(RELAIS2, 3000); //Einschalten
        if(Taste == '3') relais(RELAIS3, 3000); //Einschalten
        if(Taste == '4') relais(RELAIS4, 3000); //Einschalten
        if(Taste == '5') relais(RELAIS5, 3000); //Einschalten
        if(Taste == '6') relais(RELAIS6, 3000); //Einschalten

     
        break;
      case 99:
        //Testphase
        if(Taste == '1')
        {
          //Test-MP3-Player
          Serial.println(F("Teste MP3-Player."));
          myDFPlayer.play(TEST_AUSGABE);  //Play MP3
        }
        
        if(Taste == '2')
        {
           //Test-Schrittmotor
           Serial.println(F("Teste Schrittmotor nach links."));
           motor(LOW,1600); //1600 viertelUmderhung,3200 Halbe Umdrehung 
        }
        if(Taste == '3')
        {
           //Test-Schrittmotor
           Serial.println(F("Teste Schrittmotor nach rechts."));
           motor(HIGH,1600); //1600 viertelUmdrehung, 3200 halbe Umdrehung 
        }

        if(Taste == '4')
        {
          //Test-Relais
          Serial.println(F("Teste Relais."));
          for (int x = 0; x <RELAIS_COUNT; x++)
          {
            int pin = relaisPins[x];
            relais(pin,1500);
          }
        }
        if(Taste == '5')
        {
            switch(teste_glas())
            {
              case GLAS_KLEIN:
                 myDFPlayer.play(TEST_GLAS_KLEIN);  //Play MP3
                 break;
              case GLAS_GROSS:
                 myDFPlayer.play(TEST_GLAS_GROSS);  //Play MP3
                 break;
               default:
                 myDFPlayer.play(TEST_KEIN_GLAS);  //Play MP3
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

//-----------  Commands
String inData = "";
void command() {
   if (Serial.available() > 0)
    {
      
        char recieved = Serial.read();
        inData += recieved; 
 
        // Process message when new line character is recieved
        if (recieved == '\n')
        {
            Serial.print("Arduino Received: ");
            Serial.print(inData);
 
            inData.trim();
            inData.toLowerCase();
            if(inData == "ping"){ 
              Serial.println("pong");
            }

            if(inData.startsWith("play ")) { 
              int song = inData.substring(5).toInt();
              Serial.print("play song ");
              Serial.println(song);
              myDFPlayer.play(song);  //Play the first mp3
            }
            inData = ""; // Clear recieved buffer
        }
    }
}


//  5 , 4 , 3 , 2 , 1
//                1 <-> 2 <-> 3 <-> 4 <-> 5 <-> 6
int schritte[6] = { 1600 , 2600, 2400, 1300 , 400 };

int fahre_zu_position(int aktuell,int ziel)
{
  
   


   if(aktuell < ziel)
     for(int i = aktuell + 1;i <= ziel;i++)
     {
         Serial.print(F("Fahre nach links zur Position "));
         Serial.println(i);
         // Pos 1 -> Pos 2 => i = 2 schritte0
         motor(LOW,schritte[i-2]); //1600 viertelUmdrehung, 3200 halbe Umdrehung 
     }

   if(aktuell > ziel)
     for(int i = aktuell - 1;i >= ziel;i--)
     {
         Serial.print(F("Fahre nach rechts zur Position "));
         Serial.println(i);
         // Pos 2 -> Pos 1 => i = 1 schritte0
         motor(HIGH,schritte[i-1]); //1600 viertelUmdrehung, 3200 halbe Umdrehung 
     }

 
   return ziel;
  
}


//----- Getraenk zubereiten
void erstelle_getraenk(int glas,int ziel,int relaispin,int fuellzeit)
{
    /*
     * Glas prüfen
     */
    int glas_erkannt = teste_glas();
    if(glas != glas_erkannt)
    {
        if(glas_erkannt == GLAS_FEHLER)
        {
           if(glas == GLAS_GROSS)
           {
              myDFPlayer.play(GLAS_KEIN_GROSS);  //Play MP3
           }
           else //Schnapsglas
           {
              myDFPlayer.play(GLAS_KEIN_KLEIN);  //Play MP3
           }
        }
        else
        {
           if(glas == GLAS_GROSS)
           {
              myDFPlayer.play(GLAS_KLEIN_GROSS);  //Play MP3
           }
           else //Schnapsglas
           {
              myDFPlayer.play(GLAS_GROSS_KLEIN);  //Play MP3
           }
        }
        return;      
    }
    
  
    myDFPlayer.play(GETRAENK_START);  //Play MP3
    playsong = true; //Marker setzen, dass Song gestartet wird

    //digitalWrite (aktiv, LOW); // Motor ein
    

    /*
     * Position anfahren
     */
    int aktuell = fahre_zu_position(3 /* start */,ziel);
    if(aktuell < 0)
    {
       myDFPlayer.play(FEHLER);  //Play MP3
       return;
    }


     delay(1000);


     warteaufsongende();

    /*
     * füllen
     */
     relais(relaispin,fuellzeit);

     delay(1000);
     

     /*
      * eventuell. 2 Füllstation
      */
      /*
      * zu Start fahren
      */
    
    aktuell = fahre_zu_position(aktuell,3 /* start */);
    if(aktuell < 0)
    {
       myDFPlayer.play(FEHLER);  //Play MP3
       return;
    }


    //fertig
    myDFPlayer.play(GETRAENK_FERIG);  //Play MP3

   delay(1000);
   //digitalWrite (aktiv, HIGH); // Motor aus

}




//---- IR sensoren -----
int teste_glas()
{
  boolean oben = digitalRead(IR_SENSOR_OBEN);
  boolean unten = digitalRead(IR_SENSOR_UNTEN);

  if(oben == LOW)
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
  for (int x = 0; x <steps; x++) // 1600 viertelUmderhung,3200 Halbe Umdrehung 
  {
    digitalWrite (Step, HIGH); // SCHRITT HOCH
    delayMicroseconds(600); // WARTEN
    digitalWrite (Step, LOW); // SCHRITT NIEDRIG
    delayMicroseconds(600); // WARTEN
  }
  
   //delay(100);
}

//Relais schalten
void relais(int pin, int dauer)
{
    digitalWrite(pin, LOW); //Einschalten
    delay(dauer); //WARTEN
    digitalWrite(pin, HIGH);//Ausschalten

}



void warteaufsongende()
{
  for(int i = 0;i< 20;i++)
  {
    if(!playsong) break;
    delay(100); 
  }
  
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
      playsong = false;
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
