//const int EN = 2; // PIN AKTIVIEREN
const int dir = 9; // RICHTUNGSPIN
const int Step = 8; // SCHRITT PIN


void setup () 
{
  //pinMode (EN, OUTPUT); // ALS AUSGABE AKTIVIEREN
  pinMode (dir, OUTPUT); // RICHTUNG ALS AUSGABE
  pinMode (Step, OUTPUT); // SCHRITT ALS AUSGANG
 // digitalWrite (EN, LOW); // SET AKTIVIEREN AUF NIEDRIG
}


void loop () 
{
  digitalWrite (dir, LOW); // RICHTUNG FÜR VORWÄRTSDREHUNG NIEDRIG EINSTELLEN
  for (int x = 0; x <1600; x ++) // 1600 viertelUmderhung,3200 Halbe Umdrehung 
  {
    digitalWrite (Step, HIGH); // SCHRITT HOCH
    delayMicroseconds(950); // WARTEN
    digitalWrite (Step, LOW); // SCHRITT NIEDRIG
    delayMicroseconds(950); // WARTEN
  }
  delay (1000); // VERZÖGERUNG VOR DER SCHALTERRICHTUNG
  digitalWrite (dir, HIGH); // RICHTUNG FÜR RÜCKWÄRTSDREHUNG HOCH STELLEN
  for (int x = 0; x <1600; x ++) // 1600 viertelUmderhung,3200 Halbe Umdrehung
  {
    digitalWrite (Step, HIGH); // SCHRITT HOCH
    delayMicroseconds(950);; // WARTEN
    digitalWrite (Step, LOW); // SCHRITT NIEDRIG
    delayMicroseconds(950);; // WARTEN
  }
  delay (3000); // VERZÖGERUNG VOR DER SCHALTERRICHTUNG
}
