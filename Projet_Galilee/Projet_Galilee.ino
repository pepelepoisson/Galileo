#include <Wire.h>  // Comes with Arduino IDE
#include <LiquidCrystal_I2C.h>

#define PIN_STATUS 13  // LED sur le board Arduino - eventuelement utilise

// Definitions pour buzzer piezo
#define bHP 3 // Piezo buzzer
#define HPOn   bitSet  (PORTD,bHP) 
#define HPOff  bitSet  (PORTD,bHP)
#define HPToggle  PORTD ^= (1<<bHP)

// Definitions pour les entrees reliees aux phototransistors
#define Digital_In_1 6
#define Digital_In_2 7
#define Digital_In_3 8
#define Digital_In_4 9

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

// Definition des variables d'etat
bool capteur_1=true, capteur_2=true, capteur_3=true, capteur_4=true;  // Etat courant de chaque phototransistor. True=emmeteur visible. False=obstacle detecte
bool previous_capteur_1=true, previous_capteur_2=true, previous_capteur_3=true, previous_capteur_4=true;  // Etat precedent de chaque phototransistor

// Definition des variables de chronometrage
long capteur_1_debut=0, capteur_1_fin=0;
long capteur_2_debut=0, capteur_2_fin=0;
long capteur_3_debut=0, capteur_3_fin=0;
long capteur_4_debut=0, capteur_4_fin=0;
int nombre_mesures=1;

// Sous-routines
void Beep (long loud_duration, long loud_frequency)  {   
  // Activation du buzzer piezo - duree en millisecondes, frequence en Hz
  long nb_half_periods=loud_duration*2*loud_frequency/1000;
  long loud_half_period=1000000/(2*loud_frequency);  // microseconds
  for (int i; i<nb_half_periods; i++) { 
       HPToggle;
       delayMicroseconds (loud_half_period); // microseconds
     }
}

void Resultats ()  {
  long capteur_1_milieu=(capteur_1_debut+capteur_1_fin)/2;
  long capteur_2_milieu=(capteur_2_debut+capteur_2_fin)/2;
  long capteur_3_milieu=(capteur_3_debut+capteur_3_fin)/2;
  long capteur_4_milieu=(capteur_4_debut+capteur_4_fin)/2;

  // Affichage des resultats sur port serie
  /*
  Serial.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
  Serial.print("capteur_1_debut: "); Serial.print(capteur_1_debut); Serial.println(" microsecondes");
  Serial.print("capteur_1_milieu: "); Serial.print(capteur_1_milieu); Serial.println(" microsecondes");
  Serial.print("capteur_1_fin: "); Serial.print(capteur_1_fin); Serial.println(" microsecondes");
  Serial.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
  Serial.print("capteur_2_debut: "); Serial.print(capteur_2_debut); Serial.println(" microsecondes");
  Serial.print("capteur_2_milieu: "); Serial.print(capteur_2_milieu); Serial.println(" microsecondes");
  Serial.print("capteur_2_fin: "); Serial.print(capteur_2_fin); Serial.println(" microsecondes");
  Serial.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
  Serial.print("capteur_3_debut: "); Serial.print(capteur_3_debut); Serial.println(" microsecondes");
  Serial.print("capteur_3_milieu: "); Serial.print(capteur_3_milieu); Serial.println(" microsecondes");
  Serial.print("capteur_3_fin: "); Serial.print(capteur_3_fin); Serial.println(" microsecondes");  
  Serial.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
  Serial.print("capteur_4_debut: "); Serial.print(capteur_4_debut); Serial.println(" microsecondes");
  Serial.print("capteur_4_milieu: "); Serial.print(capteur_4_milieu); Serial.println(" microsecondes");
  Serial.print("capteur_4_fin: "); Serial.print(capteur_4_fin); Serial.println(" microsecondes");
  Serial.println("===========================================================================================");
*/
  // Tests de validation des donnees
  if (capteur_1_debut==0 || capteur_1_fin==0 || capteur_2_debut==0 || capteur_2_fin==0 || capteur_3_debut==0 || capteur_3_fin==0){
    Serial.print("Mesure: ");
    Serial.println(nombre_mesures);
    Serial.println("XXXXXXXXXXXXXXXXXXXXXXX");
    Serial.println("Oups! Mesure manquante.");
    Serial.println("XXXXXXXXXXXXXXXXXXXXXXX");
    
    lcd.setCursor(0,0);lcd.print("Mesure: ");
    lcd.setCursor(8,0);lcd.print(nombre_mesures);
    lcd.setCursor(0,1);lcd.print("XXXXXXXXXXXXXXXXXXXX"); 
    lcd.setCursor(0,2);lcd.print("Oups! Pb de Mesure  ");
    lcd.setCursor(0,3);lcd.print("XXXXXXXXXXXXXXXXXXXX"); 
  }
  else {

  //Serial.print("intervalle 1-2:"); Serial.print(capteur_2_milieu - capteur_1_milieu); Serial.println(" microsecondes");
  //Serial.print("intervalle 2-3:"); Serial.print(capteur_3_milieu - capteur_2_milieu); Serial.println(" microsecondes");
  //Serial.print("intervalle 3-4:"); Serial.print(capteur_4_milieu - capteur_3_milieu); Serial.println(" microsecondes");
  Serial.print("Mesure: ");
  Serial.println(nombre_mesures);
  Serial.print("intervalle 1-2: "); Serial.print(float((capteur_2_debut - capteur_1_debut)/1000)); Serial.println(" millisecondes");
  Serial.print("intervalle 2-3: "); Serial.print(float((capteur_3_debut - capteur_2_debut)/1000)); Serial.println(" millisecondes");
  Serial.print("intervalle 3-4: "); Serial.print(float((capteur_4_debut - capteur_3_debut)/1000)); Serial.println(" millisecondes");
  // Affichage des resultats sur LCD
  lcd.clear();
  lcd.setCursor(0,0);lcd.print("Mesure: ");
  lcd.setCursor(8,0);lcd.print(nombre_mesures);
  lcd.setCursor(0,1);lcd.print("t 1-2:"); 
  lcd.setCursor(10,1);lcd.print(float((capteur_2_debut - capteur_1_debut)/1000));
  lcd.setCursor(18,1);lcd.print("ms");  
  lcd.setCursor(0,2);lcd.print("t 2-3:"); 
  lcd.setCursor(10,2);lcd.print(float((capteur_3_debut - capteur_2_debut)/1000));
  lcd.setCursor(18,2);lcd.print("ms");   
  lcd.setCursor(0,3);lcd.print("t 3-4:"); 
  lcd.setCursor(10,3);lcd.print(float((capteur_4_debut - capteur_3_debut)/1000));
  lcd.setCursor(18,3);lcd.print("ms");
  }
  
  // Reinitialise les variables de chronometrage pour prochaine utilisation
  capteur_1_debut=0;
  capteur_1_fin=0;
  capteur_2_debut=0;
  capteur_2_fin=0;
  capteur_3_debut=0;
  capteur_3_fin=0;
  capteur_4_debut=0;
  capteur_4_fin=0;

  nombre_mesures++;
  
  // Signal de fin d'affichage
  for (int i=4; i>=0;i--){Beep(100,220*(i+1));}
}

void setup(void) {
  // Communication serie
  Serial.begin(115200); 
  
  lcd.begin(20,4);   // initialize the lcd for 16 chars 2 lines, turn on backlight
  //lcd.backlight();
  //lcd.noBacklight();

  lcd.setCursor(0,0); //Start at character 4 on line 0
  lcd.print("Experience Galilee");
  lcd.setCursor(0,1);
  lcd.print("Made in ze Laketanou");
  lcd.setCursor(0,2);
  lcd.print("papasinventeurs.org");
  lcd.setCursor(0,3);
  lcd.print("* V1 Decembre 2017 *");
  
  // Initialisation des sorties
  pinMode(PIN_STATUS, OUTPUT);
  pinMode(bHP,OUTPUT);

  // Initialisation des entrees
  pinMode(Digital_In_1,INPUT);
  pinMode(Digital_In_2,INPUT);
  pinMode(Digital_In_3,INPUT);
  pinMode(Digital_In_4,INPUT);  

  // Test du buzzer piezo et fin d'initialisation
  for (int i=0; i<5;i++){Beep(100,220*(i+1));}
}
void loop(void) {
  capteur_1=digitalRead(Digital_In_1);
  capteur_2=digitalRead(Digital_In_2);
  capteur_3=digitalRead(Digital_In_3);
  capteur_4=digitalRead(Digital_In_4);


  if (previous_capteur_1 && !capteur_1 && capteur_1_debut==0){capteur_1_debut=micros();Beep(50,1000);}
  if (!previous_capteur_1 && capteur_1){capteur_1_fin=micros();}
  if (previous_capteur_2 && !capteur_2 && capteur_2_debut==0){capteur_2_debut=micros();Beep(50,1000);}
  if (!previous_capteur_2 && capteur_2){capteur_2_fin=micros();}  
  if (previous_capteur_3 && !capteur_3 && capteur_3_debut==0){capteur_3_debut=micros();Beep(50,1000);}
  if (!previous_capteur_3 && capteur_3){capteur_3_fin=micros();}
  if (previous_capteur_4 && !capteur_4 && capteur_4_debut==0){capteur_4_debut=micros();Beep(50,1000);}
  if (!previous_capteur_4 && capteur_4){
    capteur_4_fin=micros();
    Resultats();
    //Beep(100,440);
    }  

  //if (!capteur_1 || !capteur_2 || !capteur_3 || !capteur_4){Beep(100,1500);}
  
  previous_capteur_1=capteur_1;
  previous_capteur_2=capteur_2;
  previous_capteur_3=capteur_3;
  previous_capteur_4=capteur_4;

  /*Serial.print(millis()); Serial.print(","); 
  //Serial.print(micros()); Serial.print(","); 
  Serial.print(capteur_1); Serial.print(",");
  Serial.print(capteur_2); Serial.print(",");
  Serial.print(capteur_3); Serial.print(",");
  Serial.println(capteur_4);
  */
}
