/******************************************************
MCP23017
PORT   A0  A1  A2  A3  A4  A5  A6   A7  |  B0  B1  B2  B3  B4  B5  B6   B7
FUNC   K4  K5  K6  K7  K8  L4  K11  L5  |  K1  L1  K2  K3  L2  K9  K10  L3
OUT    0   0   0   0   0   1   0    1   |  0   1   0   0   1   0   0    1

// Set Port Mode  (inverted Byte)
setPortMode(B00100000, A);  // 0 - INPUT
setPortMode(B10010010, B);  // 1 - OUTPUT

*******************************************************/

#include <Arduino.h>
#include <Wire.h>
// #include <MCP23017.h>

#define LED 13 // WittyCloud Blue LED

// MCP23017 myMCP(0x20); // alternative option not using the reset pin

// int interruptPin = 3;
// volatile bool event = false;
// byte intCapReg; 

// void eventHappened();

void setup(){
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  Wire.begin();
    while (!Serial);
    Serial.println("\nI2C Scanner");
} 

void loop(){
    byte error, address;
    int nDevices;
 
    Serial.println("Scanning...");
 
    nDevices = 0;
    for(address = 8; address < 127; address++ ){
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
 
        if (error == 0){
            Serial.print("I2C device found at address 0x");
            if (address<16)
                Serial.print("0");
            Serial.print(address,HEX);
            Serial.println(" !");
 
            nDevices++;
        }
        else if (error==4) {
            Serial.print("Unknow error at address 0x");
            if (address<16)
                Serial.print("0");
            Serial.println(address,HEX);
        } 
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");
 
    delay(5000);
}
