#include <Servo.h> // On utilise des servo
#include <NewPing.h> // librairie à installer, permet de simplifier les appels au capteur ultrasons

Servo servo1;  // servo qui gère l'axe vertical
Servo servo2;  //servo qui gère l'axe horizontal
int const PIN1_MOTEUR1 = 8; //pin de commande moteur 1
int const PIN2_MOTEUR1 = 7; // pin de commande moteur 1
int const PWN_MOTEUR_1 = 6; // pin PWM moteur 1
int const PIN1_MOTEUR2 = 10; // pin de commande moteur 2
int const PIN2_MOTEUR2 = 9; // pin de commande moteur 2
int const PWN_MOTEUR_2 = 11; // pin PWM moteur 2
int const PIN_SERVO_1 = 12; // pin servo haut/bas
int const PIN_SERVO_2 = 13; // pin servo gauche/droite
int const PIN_LED = 2; // pin Lampe

#define TRIGGER_PIN 4
#define ECHO_PIN 5
#define MAX_DISTANCE 60 // toute mesure au dela de cette distance (en cm) est consideree comme nulle

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  delay(2000);
  Serial.begin(9600); //initialise la communication série
  pinMode(PIN1_MOTEUR1, OUTPUT);
  pinMode(PIN2_MOTEUR1, OUTPUT);
  pinMode(PWN_MOTEUR_1, OUTPUT);
  pinMode(PIN1_MOTEUR2, OUTPUT);
  pinMode(PIN2_MOTEUR2, OUTPUT);
  pinMode(PWN_MOTEUR_2, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  servo1.attach(PIN_SERVO_1); //ratachement à la sortie 12. Ce servo va de 0 à 45°(de bas en haut)
  servo2.attach(PIN_SERVO_2); //ratachement à la sortie 13. Ce servo va de 0 à 360° (gauche à droite)
}

void loop() {
  int nbPoteau = 1; 
  
  actionMoteur(1, 1, 80);
  actionMoteur(2, 1, 80);

  while (nbPoteau <= 7) {
    unsigned int d = sonar.ping_cm();
    Serial.println("d = " + String(d));


    if (d > 10) {
      delay(250);
      continue;
    } else {
      switch (nbPoteau) {
        case 1 :
          Serial.println("Poteau 1");
          actionMoteur(1, 1, 80);
          actionMoteur(2, 1, 80);

        case 2 :
            Serial.println("Poteau 2");

            actionMoteur(1, 1, 180);
            actionMoteur(2, 1, 180);
          
        case 3 :
          Serial.println("Poteau 3");

          actionMoteur(1, 1, 80);
          actionMoteur(2, 1, 80);

        case 4 :
          Serial.println("Poteau 4");

          actionMoteur(1, 1, 180);
          actionMoteur(2, 1, 180);

        case 5 :
          Serial.print("Poteau 5");

          actionMoteur(1, 1, 80);
          actionMoteur(2, 1, 80);

        case 6 :
          Serial.println("Poteau 6");

          actionMoteur(1, 1, 180);
          actionMoteur(2, 1, 180);
          delay(2000);
          actionMoteur(1, 1, 50);
          actionMoteur(2, 1, 50);

        case 7 :
          Serial.println("Poteau 7");

          actionMoteur(1, 1, 80);
          actionMoteur(2, 1, 80);
          delay(3000);

      }
      nbPoteau ++;
    }
  }
  Serial.println("yey");
}



//partie  Théophile (pan/tilt)

//fonction de base arret.
void attente(int temps)
{
  servo1.write(0);
  servo2.write(0);
  delay(temps);
}


void mouv_servo_1_2(int X1, int X2, int Y1, int Y2, int tps)
{ float pas;
  if (abs(X1 - X2) > abs(Y1 - Y2)) {
    pas = abs(X1 - X2);
  } else {
    pas = abs(Y1 - Y2);
  }
  float i = X1;
  float j = Y1;

  int temps = tps / pas;


  if (X1 <= X2 && Y1 <= Y2) {
    while (i <= X2 && j <= Y2) {

      servo1.write((int)i);
      servo2.write((int)j);

      i += abs(X2 - X1) / pas;
      j += abs(Y2 - Y1) / pas;
      delay(temps);
    }
  }

  if (X1 > X2 && Y1 > Y2) {
    while (i >= X2 && j >= Y2) {

      servo1.write((int)i);
      servo2.write((int)j);

      i -= abs(X2 - X1) / pas;
      j -= abs(Y2 - Y1) / pas;
      delay(temps);
    }
  }

  if (X1 <= X2 && Y1 >= Y2) {
    while (i <= X2 && j >= Y2) {

      servo1.write((int)i);
      servo2.write((int)j);

      i += abs(X2 - X1) / pas;
      j -= abs(Y2 - Y1) / pas;
      delay(temps);
    }
  }
  if (X1 >= X2 && Y1 <= Y2) {
    while (i >= X2 && j <= Y2) {

      servo1.write((int)i);
      servo2.write((int)j);

      i -= abs(X2 - X1) / pas;
      j += abs(Y2 - Y1) / pas;
      delay(temps);
    }
  }



}


//partie Basile (moteur)

void actionMoteur(int moteur, int sens, int pwr) {
  int pin1, etat1, pin2, etat2, pinP, puissance; //variable de la fonction
  //test numéro du moteur
  if (moteur == 1) {
    pin1 = PIN1_MOTEUR1;
    pin2 = PIN2_MOTEUR1;
    pinP = PWN_MOTEUR_1;
  }
  else {
    pin1 = PIN1_MOTEUR2;
    pin2 = PIN2_MOTEUR2;
    pinP = PWN_MOTEUR_2;
  }
  //test sens du moteur 1,-1 (sens contraire) ou tout autre valeur (stoppe le moteur)
  if (sens == 1) {
    etat1 = 1;
    etat2 = 0;
  }
  else if (sens == -1) {
    etat1 = 0;
    etat2 = 1;
  }
  else {
    etat1 = 0;
    etat2 = 0;
  }
  analogWrite(pinP, pwr);
  digitalWrite(pin1, etat1);
  digitalWrite(pin2, etat2);
  //affichage sur le moniteur série (facultatif)
  Serial.print("Moteur : ");
  Serial.print(moteur);
  if (sens == -1 || sens == 1) {
    Serial.print(" sens : ");
    Serial.print(sens);
  }
  else {
    Serial.print(" ! stop ! ");
  }
  Serial.print(" puissance : ");
  Serial.println(pwr);
}
