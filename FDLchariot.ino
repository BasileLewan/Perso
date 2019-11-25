int const PIN1_MOTEUR1 = 8; //pin de commande moteur 1
int const PIN2_MOTEUR1 = 7; // pin de commande moteur 1
int const PWN_MOTEUR_1 = 6; // pin PWM moteur 1
int const PIN1_MOTEUR2 = 10; // pin de commande moteur 2
int const PIN2_MOTEUR2 = 9; // pin de commande moteur 2
int const PWN_MOTEUR_2 = 11; // pin PWM moteur 2

void setup() {
  delay(2000);
  Serial.begin(9600); //initialise la communication série
  pinMode(PIN1_MOTEUR1, OUTPUT);
  pinMode(PIN2_MOTEUR1, OUTPUT);
  pinMode(PWN_MOTEUR_1, OUTPUT);
  pinMode(PIN1_MOTEUR2, OUTPUT);
  pinMode(PIN2_MOTEUR2, OUTPUT);
  pinMode(PWN_MOTEUR_2, OUTPUT);

}

void loop() {
  actionMoteur(1, 1, 255); //moteur 1 100% puissance sens 1
  actionMoteur(2, 1, 255); //moteur 2 100% puissance sens 1
  delay(1000); //attente de 5 secondes
  actionMoteur(1, -1, 128); //moteur 1 50% puissance sens -1
  actionMoteur(2, -1, 128); //moteur 2 50% puissance sens -1
  delay(1000);
  actionMoteur(1, 0, 0); //arrêt moteur 1
  actionMoteur(2, 0, 0); //arrêt moteur 2
  delay(1000);
  


}
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
