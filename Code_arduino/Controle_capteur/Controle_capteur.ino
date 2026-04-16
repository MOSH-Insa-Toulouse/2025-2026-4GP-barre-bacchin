
// Packages
#include <SoftwareSerial.h>  //Bluetooth
#include <SPI.h>

// --------------------------------- Definitions  ----------------------------------
//Bluetooth
#define rxPin 8
#define txPin 7
#define baudrate 9600
SoftwareSerial mySerial (rxPin, txPin);

//Digit pot
#define MCP_NOP 0b00000000
#define MCP_WRITE 0b00010001
#define MCP_SHTDWN 0b00100001
const int ssMCPin = 10; // Define the slave select for the digital pot

//Flex Sensor
const int flexPin = A1;      // Pin connected to voltage divider output

// --------------------------------- Variables  ----------------------------------

//Digit potard
float tension_capteur = 0;
int indice_potard ;
float r1 = 100000.0;
float r3 = 100000.0;
float r5 =10000.0;
float vcc = 5.0;
float r2 = 100.0;
float rc ;
float rc_mohm ;
float r0 = 1000;
float varrel_r;

//Flex sensor
const float R_DIV = 47000.0;  // resistor used to create a voltage divider
const float flatResistance = 25000.0; // resistance when flat
const float bendResistance = 100000.0;  // resistance at 90 deg
float varrel_flex;
char data[50];





// --------------------------------- Fonctions ----------------------------------

// Fonction write Potard
void SPIWrite(uint8_t cmd, uint8_t data, uint8_t ssPin) // SPI write the command and data to the MCP IC connected to the ssPin
{
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0)); //https://www.arduino.cc/en/Reference/SPISettings
  
  digitalWrite(ssPin, LOW); // SS pin low to select chip
  
  SPI.transfer(cmd);        // Send command code
  SPI.transfer(data);       // Send associated value
  
  digitalWrite(ssPin, HIGH);// SS pin high to de-select chip
  SPI.endTransaction();
}

//Fonction centrage de la tension autour de 2.5V en ajustant la valeur de la résistance du potard
void centrage() {
  // Renvoie l'indice de potard tel que la tension est la plus proche de 2.5V 
  // Initialise le potard avec cet indice
  int potard = 256;  // valeur par défaut = erreur
  float ecart_min = 1000;  // grande valeur initiale pour comparer
  int potard_proche = -1;


  for (int i = 0; i < 256; i++) {
    SPIWrite(MCP_WRITE, i, ssMCPin);
    delay(200);

    tension_capteur = analogRead(A0) * (5.0 / 1023.0);
    Serial.println(tension_capteur);

    float ecart = fabs(tension_capteur - 2.5);

    // On garde en mémoire le plus proche
    if (ecart < ecart_min) {
      ecart_min = ecart;
      potard_proche = i;
    }

    // Si on est dans la plage "acceptable", on arrête la boucle
    if (ecart < 0.4) {
      potard = i;
      break;
    }
  }

  // Si aucune valeur dans l'intervalle n'a été trouvée
  if (potard == 256) {
    potard = potard_proche;  // on prend le plus proche
    Serial.println("Aucune valeur exacte, on prend la plus proche");
    Serial.print("potard = ");
    Serial.println(potard);
    SPIWrite(MCP_WRITE, potard, ssMCPin);
  } else {
    SPIWrite(MCP_WRITE, potard, ssMCPin);
    Serial.println("Centrage exact trouvé :");
    Serial.print("potard = ");
    Serial.println(potard);
  }
  delay(1000) ; 
  r2 = 58 + potard*37 ;
  tension_capteur = analogRead(A0) * (5.0 / 1023.0)  ;
  r0 = ((r1/r2)*(r2 + r3)*(vcc/tension_capteur) - r1 -r5) ;
  Serial.println(r0);
  
}
//Fonction de mesure de la variation relative du flex.
float mesure_flex(){
   // Read the ADC, and calculate voltage and resistance from it
  int ADCflex = analogRead(flexPin);
  float Vflex = ADCflex * vcc / 1023.0;
  float Rflex = R_DIV * (vcc / Vflex - 1.0);
  float var_flex_res ;
  //Serial.println("Resistance: " + String(Rflex) + " ohms");

  // Use the calculated resistance to estimate the sensor's bend angle:
  float angle = map(Rflex, flatResistance, bendResistance, 0, 90.0); 
  //Serial.println("Bend: " + String(angle) + " degrees");
  //Serial.println();
  var_flex_res = ( (Rflex - flatResistance)/flatResistance);
  return var_flex_res ;
}

// --------------------------------- Setup ----------------------------------
void setup() {
  // put your setup code here, to run once:
  pinMode (rxPin,INPUT);
  pinMode (txPin,OUTPUT);
  pinMode (A0,INPUT) ; //Output du capteur amplifiée
  pinMode(flexPin, INPUT);

  mySerial.begin(baudrate);
  Serial.begin(9600);

  SPI.begin();
  centrage();
}



// --------------------------------- Main loop  ----------------------------------
void loop() {
// Obtention des mesures
  varrel_flex = mesure_flex();
  tension_capteur = analogRead(A0) * (5.0 / 1023.0)  ;

// Calcul des Resistances
  Serial.print("Resistance r2 : ") ;
  Serial.println(r2);
  rc = ((r1/r2)*(r2 + r3)*(vcc/tension_capteur) - r1 -r5) ;
  varrel_r= (rc - r0)/r0;
  rc_mohm = rc*1/1000000 ;
  Serial.print("Résistance capteur : ") ;
  Serial.print(rc_mohm) ;
  Serial.println(" MOhm") ;
  Serial.print("Resistance R0 : ") ;
  Serial.print(r0*1/1000000) ;
  Serial.println(" MOhm") ;

  //Envoie bluetooth
  String data = String(varrel_r) + ";" + String(varrel_flex)+ ";" + String(r0*1/1000000) +";" + String(r2) ;
  Serial.println(data);
  mySerial.println(data);

  // Affichage Tension et angle capteur et flex
  Serial.print("tension capteur :");
  Serial.println(tension_capteur);
  Serial.println("variation relative : " + String(varrel_r) + ";" );
  Serial.print("variation relativ flex :");
  Serial.println(varrel_flex);

  delay(300) ; 
}

