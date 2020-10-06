#include <Arduino.h>
#include <libRobus.h>
#define GAUCHE 0
#define DROIT 1

int i;
const int p = 1500;
int sumErr_G, sumErr_D;
float kp_gauche;
float kp_droit;
float ki_gauche;
float ki_droit;
const float v_wanted = 0.5;
/**
 * Code pour le robot 12A
 */


/**
 * Ne sert qu'à comparer les valeurs des encodeurs
 */
void showEncoderValues()
{
  Serial.print(ENCODER_Read(GAUCHE));
  Serial.print("\t\t");
  Serial.println(ENCODER_Read(DROIT));
}
/**
 * la fonction avec le pid
 * 
 * errkp = l'erreur entre la pondération voulu multiplié par le facteur de pondération
 * errki = l'intégrale des erreur multiplié par le facteur d'intégration
 * la vitesse est la somme de ces erreurs plus la vitesse que nous voulons.
 * 
 * Note : changer l'une de ces variable demande de changer toutes les autres
 */
float setSpeed(int error, float kp, float ki, int sumErr, float speedWanted)
{
  float speed;
  float errKp = error * kp;
  float errKi = sumErr * ki;

  speed = errKp + errKi + speedWanted;
  return speed;

}


//--------------------------------
void setup() {
  BoardInit();
  //i = 0; le nombre d'itération du calcul de vitesse
  sumErr_G = 0;
  sumErr_D = 0;
  kp_gauche = 0.0001;
  kp_droit = 0.0000985;
  ki_gauche = 0.00002;
  ki_droit = 0.00002;
  delay(1500);
}

void go_straight(float length)
{
  int mean_Pulse = 0;
  int nb_Pulse = 0;
  int error_D = 0;
  int error_G = 0;
  while(mean_Pulse/1437.33 <= length)
  {
    //Fonction pour aller droit

   //  showEncoderValues();
    mean_Pulse += (ENCODER_Read(GAUCHE)+ENCODER_Read(DROIT))/2; //Moyenne pour déterminer la distance parcouru
    error_D = p - ENCODER_ReadReset(DROIT);
    error_G = p - ENCODER_ReadReset(GAUCHE);
    sumErr_D += error_D;
    sumErr_G += error_G;
    while(i < 3)
    {
      MOTOR_SetSpeed(GAUCHE, setSpeed(error_G, kp_gauche,ki_gauche,sumErr_G,v_wanted));
      MOTOR_SetSpeed(DROIT, setSpeed(error_D, kp_droit,ki_droit,sumErr_D,v_wanted-0.3));
      i++;
    }
    MOTOR_SetSpeed(GAUCHE, setSpeed(error_G, kp_gauche,ki_gauche,sumErr_G,v_wanted));
    MOTOR_SetSpeed(DROIT, setSpeed(error_D, kp_droit,ki_droit,sumErr_D,v_wanted));
  }
  delay(250);
}

void loop() 
{
  go_straight(50);
  while(true)
  {
    MOTOR_SetSpeed(DROIT, 0);
    MOTOR_SetSpeed(GAUCHE, 0);

  }


   //showEncoderValues();


  

}