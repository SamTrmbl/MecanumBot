#include <Arduino.h>
#include <Encoder.h> //Installer la librairie Encoder de Paul Stoffregen v1.4.1 dans platformio
#include <Ultrasonic.h>


/////TODO IL FAUT RENOMMER CE FICHIER Qbot.h

/////TODO Faire une fonction ultrason en adaptant ce lien https://create.arduino.cc/projecthub/abdularbi17/ultrasonic-sensor-hc-sr04-with-arduino-tutorial-327ff6  FAIT

//TODO faire une fonction limit switch qui retourne un bool

//Pour savoir les # de pins pour Ultrasonic et Sensor, mettre un chiffre bidon, je vais m'occuper de les trouver

int vitesse;
bool direction;
int distanceLowLevelVariable;

int ultrason(){
  Ultrasonic ultrason(1,1);
  int distanceUltraSonic;

  distanceUltraSonic= ultrason.read();
  return distanceUltraSonic;
}

///////Définition des moteurs de chaque roues
void vitesseRoueLowLevel(int pinA, int pinB, int pinPWM, float vitesse){
  //TODO ajouter une deadband
  vitesse = constrain(vitesse,-100,100);// accepte des valeurs de -100 à 100
  vitesse = map(vitesse,-100,100,-255,255); //transfert ces valeurs sur la bonne plage PWM
  bool direction = vitesse >= 0; //avancer = True
  digitalWrite(pinA,!direction); //Pour avancer, la pin A est à 0 et la pin B est à 1
  digitalWrite(pinB,direction);
  analogWrite(pinPWM, abs(vitesse));
}

void vitesseAvG(int vitesse){
  vitesseRoueLowLevel(34,35,12,vitesse);
}

void vitesseAvD(int vitesse){
  vitesseRoueLowLevel(36,37,8,vitesse);//inverse les pins A et B car le moteur de droite doit tourner à l'envers
}

void vitesseArG(int vitesse){
  vitesseRoueLowLevel(43,42,9,vitesse);
}

void vitesseArD(int vitesse){
  vitesseRoueLowLevel(A5,A4,5,vitesse);//inverse les pins A et B car le moteur de droite tourne à l'envers
}

////////////Drive modes

void stop(){
  vitesseAvD(0);
  vitesseAvG(0);
  vitesseArD(0);
  vitesseArG(0);
} 

void simpleDrive(int vitesse){
  
  vitesseAvG(vitesse);
  vitesseAvD(vitesse);
  vitesseArG(vitesse);
  vitesseArD(vitesse);
}

void arcadeDrive(int vx, int vz){
 
 int vg = vx+vz;
 int vd = vx-vz;

int vMax = max(abs(vg),abs(vd));

vg=vg/vMax;
vd=vd/vMax;

 vitesseAvG(vg);
 vitesseAvD(vd);
 vitesseArG(vg);
 vitesseArD(vd);
}

void mecanumDrive(int vx, int vy, int vz){

 int vAvG = vx+vz+vy;
 int vAvD = vx-vz+vy;
 int vArG = vx+vz-vy;
 int vArD = vx-vz-vy;


int vMax = max(max(abs(vAvG),abs(vAvD))
          ,max(abs(vArG),abs(vArD))
          );

vAvG= vAvG/vMax;
vAvD= vAvD/vMax;
vArG= vArG/vMax;
vArD= vArD/vMax;

 vitesseAvG(vAvG);
 vitesseAvD(vAvD);
 vitesseArG(vArG);
 vitesseArD(vArD);
}


/////Encodeurs

Encoder encodeurAvG(18,31);
Encoder encodeurAvD(19,38);
Encoder encodeurArG(3,49);
Encoder encodeurArD(2,A1);

long distanceLowLevel(Encoder encodeur){//long pour éviter l'overflow
  distanceLowLevelVariable=encodeur.read()/1000;//diviser par 1000 car c'est environ un tour de shaft
  return distanceLowLevelVariable;
}

void resetEncoder(){
  encodeurAvG.write(0);
  encodeurAvD.write(0);
  encodeurArG.write(0);
  encodeurArD.write(0);
}

long distanceAvG(){
  return -distanceLowLevel(encodeurAvG);
}

long distanceAvD(){
  return distanceLowLevel(encodeurAvD);
}

long distanceArG(){
  return -distanceLowLevel(encodeurArG);
}

long distanceArD(){
  return distanceLowLevel(encodeurArD);
}

long distance(){
  return (distanceAvD() + distanceAvG() + distanceArG() + distanceArD())/4;
}

long distanceDroite(){
  return(distanceAvD() + distanceArD())/2;
}

long distanceGauche(){
  return (distanceAvG() + distanceArG())/2;
}


