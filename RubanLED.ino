/* ATTENTION MONTAGE
    Ajouter une résistance entre 300 et 500 Ohm entre la bande LED et la PIN Arduino
    Si alimentation de basse qualité, peu stabilisée, compenser avec une capacité de 1000 uF
*/


#include <Adafruit_NeoPixel.h>
//#include <ArduinoSTL.h>    // std::find

#define PIN 6 // Variable PIN créée et prend la valeur n
#define NbPixel 144 // Variable nombre de LED


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NbPixel, PIN, NEO_GRB + NEO_KHZ800);
/*  nb de LED, numéro de la pin Arduino, paramètres de la bande LED à choisir entre :
  NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
  NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
  NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
  NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
*/

// Constantes :

int const ACC = 60; // Acceleration
int const SKIP[] = {70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86}; // Index des led à sauter
int const STOP[] = {40, 80, 120}; // Index des led d'arrêt
int const WAIT = 300; // Delai d'arrêt du stop en ms
int const TAILLE = 6; // Taille du vers, en leds
int LedCh[NbPixel][3];

void setup() {
  delay(2000);
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  Serial.begin(9600);
  pixels.begin();
  pixels.show(); // Toutes les LED à 0, remise à zéro du programme, pour partir sur de bonnes bases


  for (int i = 1; i < NbPixel; i++) {
    if (in(i, SKIP, sizeof(SKIP))) {
      LedCh[i][0] = 1;
    } else {
      LedCh[i][0] = 0;
    }
    if (in(i, STOP, sizeof(STOP))) {
      LedCh[i][1] = 0;
      LedCh[i][2] = 0;
      continue;
    } else {
      LedCh[i][1] = LedCh[i - 1][1] + 1;
    }
    // LedCh[i][2] = WAIT - ACC*pow(LedCh[i][1], 2) -  LedCh[i][1] ;
    LedCh[i][2] = WAIT/ ((pow(LedCh[i][1], 2)/ACC) + 1);
    if (LedCh[i][2] < 1) {
      LedCh[i][2] = 1;
    }
  }
  for (int i = 0; i < NbPixel; i++) {
    toString(LedCh[i]);
    Serial.println();
  }
}

void loop() {


  for (int i = 0; i < NbPixel; i++) {
    Serial.println("pos " + String(i));
    pixels.clear();
    if (LedCh[i][0] == 1) {
      Serial.println("Skipping");
      continue;
    }
    Serial.println("yey");
    int j = i;
    int lux = 64;
    //TODO : buggué de ouf
    /*
      do {
      int lux = 128 - exp(i - j);
      Serial.println("px = " + String(j) + " lux = " + String(lux));
      pixels.setPixelColor(j, pixels.Color(lux, lux, lux));
      j--;
      Serial.println(String(j > 0 && lux > 0));
      } while (j > 0 && lux > 0);
    */
    while (j > (i - TAILLE)) {
      lux -= pow((i-j), 2);
      pixels.setPixelColor(j, pixels.Color(lux, lux, lux));
      j--;
    }
    pixels.show();
    // Serial.println("delay = " + String(LedCh[i][2]));
    delay(LedCh[i][2]);
  }
}

// TODO : opti
boolean in(int val, int tab[], int size) {
  for (byte i = 0; i <= size; i++) {
    if (val == tab[i]) {
      return true;
    }
  }
  return false;
}

// TODO : refaire
void toString(int tab[]) {
  for (int i = 0; i <= sizeof(tab); i++) {
    Serial.print(String(tab[i]) + " ");
  }
}
