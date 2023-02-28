
#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

HX711 scale; 

float Calibration_Factor= -3.7; // this calibration factor is adjusted according to the load sensor
float units;
float ounces;
bool calibrate = true;

void setup() {
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");
  Serial.println("Press . or c to end calibration and to recalibrate");

  scale.set_scale();
  scale.tare(); // Reset scale to 0
  long zero_factor = scale.read_average(); // Get basic reading
  Serial.print("Zero factor: "); // This can be used to eliminate the need to tare the scale. Useful in constant scale projects.
  Serial.println(zero_factor);
  delay(2000);
}

void loop() {
// for calibration
if (calibrate) {

  scale.set_scale(Calibration_Factor); // Adjust this calibration factor
  
  units = scale.get_units();
  /*if (units < 0)
  {
    units = 0.00;
  }*/
  ounces = units * 0.035274;
  Serial.print("Unit: ");
  Serial.print(units);
  Serial.print("  Reading: ");
  Serial.print(ounces);
  Serial.print(" grams");
  Serial.print("  Calibration_Factor: ");
  Serial.print(Calibration_Factor);
  long zero_factor = scale.read_average(); // Get basic reading
  Serial.print("  Zero factor: "); // This can be used to eliminate the need to tare the scale. Useful in constant scale projects.
  Serial.println(zero_factor);
  delay(1000);
  if (Serial.available())
  {
    char temp = Serial.read();
    if (temp == '+' || temp == 'a')
      Calibration_Factor += 1;
    else if (temp == '-' || temp == 'z')
      Calibration_Factor -= 1;
    else if (temp == '.' || temp == 'c') {
      calibrate = false;
      scale.set_scale();
      scale.tare(); // reset the sensor to 0
      scale.set_scale(Calibration_Factor); // apply calibration
      Serial.println(" End calibration ");
    }
      
  }
  delay(200);
}
else{
  
   for(int i=0; i<10; i++) units =+ scale.get_units(), 10; // make measurements 10 times
   units / 10; // divide values by 10
   ounces = units * 0.035274; // convert values into grams
   Serial.print("Reading: ");
   Serial.print(ounces);
   Serial.print(" grams");  
   Serial.println();
   if (Serial.available())
  {
    char temp = Serial.read();
    if (temp == '.' || temp == 'c')
      calibrate = true;
  }
}
delay(200);
}