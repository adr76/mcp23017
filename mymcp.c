/*
OLED 1.3 I2C 0x3C
MCP23017 i2C 0x20
PORT   A0  A1  A2  A3  A4  A5  A6   A7  |  B0  B1  B2  B3  B4  B5  B6   B7
FUNC   K4  K5  K6  K7  K8  L4  K11  L5  |  K1  L1  K2  K3  L2  K9  K10  L3
OUT    0   0   0   0   0   1   0    1   |  0   1   0   0   1   0   0    1
PIN    0   1   2   3   4   5   6    7      8   9   10  11  12  13  14   15
*/

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "mcp23017.h"

#define debug 0
#define outputTest 0
#define inputTest 0
#define interruptTest 0

#define INT_PIN 7 // OrangePi INT pin
#define KEYS 11
#define LEDS 5

// #define key(num) 

int key[KEYS] = {8, 10, 11, 0, 1, 2, 3, 4, 13, 14, 6};
int led[LEDS] = {9, 12, 15, 5, 7};

void myInterruptService()
{
    printf("_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\r\n");
    printf("the key has been pressed!!!\r\n");
    printf("the last interrupt Pin is: %d\r\n",mcp_getLastInterruptPin());
    printf("the last interrupt value is: %d\r\n",mcp_getLastInterruptPinValue());
    printf("_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\r\n");
}

void readReg() {
for(int addr = 0; addr< 22; addr++) {
        printf("addr:0x%02x,value:0x%02x\r\n",addr,mcp_readRegister(addr));
    }
    printf("------------------------------------------\r\n");   
}

void readPort(){
	for (int i = 0 ; i < 16 ; ++i)
		printf("%d ", mcp_digitalRead(i));
	printf("\n");
}

int main(void)
{
    wiringPiSetup();
    fd = wiringPiI2CSetupInterface("/dev/i2c-0", 0x20);
    
    //init the MCP23017 Register
    mcp_initReg();

    // Init Keys
    // mcp_setupInterrupts(1,0,0);
    // for(int port = 0; port<KEYS; port++)
    // {
    //     mcp_pinMode(key[port], 1);
    //     mcp_pullUp(key[port],1);
    //     mcp_setupInterruptPin(key[port],1);
    // }
     // Init Leds
    for(int port = 0; port<LEDS; port++)
    {
        mcp_pinMode(led[port], 0);
        //test
        // mcp_digitalWrite(led[port],1);   
        // delay(1000);
        // mcp_digitalWrite(led[port],0);
    }

    printf("GPIOAB:0x%02x\r\n",mcp_readGPIOAB());
    readPort();
    
    //Configure OrangePi INT_PIN as an interrupt function
    pinMode(INT_PIN,INPUT);
    pullUpDnControl(INT_PIN,PUD_UP);
    wiringPiISR(INT_PIN,INT_EDGE_FALLING,&myInterruptService);

	while(1)
    {
        delay(500);

        //mirroring, don't openDrain, INT output pin will be low when interrupt occur
        mcp_setupInterrupts(1,0,0);
        
        //input
        mcp_pinMode(8,1);
        //pullup
        mcp_pullUp(8,1);
        //1 means compare against given value,and 1 means when differ from 1 occur the interrupt
        mcp_setupInterruptPin(8,1);
        
        //input
        mcp_pinMode(10,1);
        //pullup
        mcp_pullUp(10,1);
        //1 means compare against given value,and 1 means when differ from 1 occur the interrupt
        mcp_setupInterruptPin(10,1);
    }
	return 0;
}
