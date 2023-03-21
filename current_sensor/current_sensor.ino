#include "EmonLib.h"  
#include <Wire.h> 
#include <SoftwareSerial.h>

EnergyMonitor emon1;

SoftwareSerial mySerial(2, 3); // RX, TX for the new serial port

double IrmsNock = 0;
double total_power = 0;
unsigned long t1= 0;

int loopCounter = 0;

void setup() 
{
  Serial.begin(9600); // Starts the serial communication
  mySerial.begin(115200); // initialize the new serial port
  
  emon1.current(0, 30);

  for(int i = 0;i<5;i++){
    IrmsNock += emon1.calcIrms(1480);
  }
  IrmsNock = IrmsNock / 5; //IrmsNock /= 5

  t1 = millis();
}


void loop() {

    //calculations of current
    double Irms = emon1.calcIrms(1480)-IrmsNock;  // Calculate Irms only
    if(Irms < 0) Irms = 0;
  
    unsigned int t2 = millis();
    int nocktime = t2 - t1;
    t1 = t2;
    double power = Irms*220.0;
    
    total_power += power * nocktime/3600; 
    Serial.print(nocktime);
    Serial.print(" ");
    Serial.print(total_power/1000,4);
    Serial.print(" ");
    Serial.print(power,4);
    Serial.println(Irms*220.0); // Apparent Power
    Serial.print(" ");
    Serial.println(Irms,4);   

    if(loopCounter >= 15){
          mySerial.println(String(total_power/1000,2) + "," + String(power,2) + "," + String(Irms*220.0) + "," + String(Irms,2) + "%");
          loopCounter = 0;
    }


  loopCounter++;
  //delay(1000);
}
