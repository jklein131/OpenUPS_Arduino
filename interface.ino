
 
#include <Wire.h>
 
 
void setup()
{
  Wire.begin();
 
  Serial.begin(9600);
  while (!Serial);             // Leonardo: wait for serial monitor
  Serial.println("\nSMBUS ARDUINO MKR GSM WIRE LIBRARY FOR OPENUPS");
  	pinMode(10, OUTPUT);
	digitalWrite(10, HIGH);
}
int commandSMBUS(int addr,int cmd){
  Wire.beginTransmission(addr);
      Wire.write(cmd);
      Wire.endTransmission();
      Wire.requestFrom(addr, 2);
      return returnword();
}
char commandSMBUS_byte(int addr,int cmd){
  Wire.beginTransmission(addr);
      Wire.write(cmd);
      Wire.endTransmission();
      Wire.requestFrom(addr, 1);
      return returnbyte();
}
int returnword(){
  char c1 = Wire.read();
  char c2 = Wire.read();
  return (int)c1|((( int)c2)<<8);
}
char returnbyte(){
  return Wire.read();
}
void loop()
{
  byte error, address;
  int nDevices;
 
    address = 56;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      //if (address<16)
      //  Serial.print("0");
      Serial.print(address);
      Serial.println("  !");
      //ask this device for something 
      
        Serial.print(address);
        Serial.print("\t");
        Serial.print("0x");
         if (address<16)
        Serial.print("0");
      
         Serial.print("\t");
        //delay(200);
      Serial.println();
      Serial.print("batt voltage:");
      Serial.print(commandSMBUS(address,9)/1000);
      Serial.println(); 
      Serial.print("batt %:");
      Serial.print((int)commandSMBUS_byte(address,13));
      Serial.println(); 
      Serial.print("batt %:");
      Serial.print((int)commandSMBUS_byte(address,14));
      Serial.println(); 
      Serial.print("batt %:");
      Serial.print((int)commandSMBUS_byte(address,15));
      Serial.println(); 
      Serial.print("hours to empty:");
      int timetoempty = commandSMBUS(address,17)/60;
      if( timetoempty > 1024) {
        //battery is plugged in
        Serial.print("plugged in");
      } else {
        //battery is not plugged in
        Serial.print(timetoempty);
      }
      
      Serial.println(); 
      Serial.print("battery power or time to charge:");
      int timetoofull = commandSMBUS(address,19);
      if(timetoofull > (65535- 50)  ) {
        Serial.print("on battery");
      } else {
        Serial.print("time to charge:");
        Serial.print(timetoofull/60);
      }
      //Serial.print();
      Serial.println(); 
      
     //battery percentage
     delay(2000);
      //repeat the first address
      address--;
      
      nDevices++;
    } else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    } 
 
  delay(5000);           // wait 5 seconds for next scan
}
