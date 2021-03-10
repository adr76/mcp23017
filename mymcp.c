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

#define INT_PIN 7 // OrangePi INT pin
#define KEYS 11
#define LEDS 5

int led[LEDS] = {9, 12, 15, 5, 7};
int key[KEYS] = {8, 10, 11, 0, 1, 2, 3, 4, 13, 14, 6};

void keyPress()
{
    printf("Key press!!!\r\n");
    // printf("last interrupt Pin: %d\r\n",mcp_getLastInterruptPin());
    // printf("tlast interrupt Val: %d\r\n",mcp_getLastInterruptPinValue());
    // printf("_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\r\n");
}

void mcp_readRegisters() {
for(int addr = 0; addr< 22; addr++) {
        printf("addr:0x%02x,value:0x%02x\r\n",addr,mcp_readRegister(addr));
    }
    printf("------------------------------------------\r\n");   
}

void mcp_readPorts(){
    printf("MCP PORTS:  ");
	for (int i = 0 ; i < 16 ; ++i)
		printf("%d ", mcp_digitalRead(i));
	printf("\n");
}

void ledTest(void)
{
    for(int i = 0; i<LEDS; i++)
    {
        mcp_digitalWrite(led[i],1);   
        delay(1000);
        mcp_digitalWrite(led[i],0);
    }
}

int main(void)
{
    wiringPiSetup();
    pinMode(INT_PIN, INPUT);
    pullUpDnControl(INT_PIN, PUD_UP);
    wiringPiISR(INT_PIN, INT_EDGE_FALLING, &keyPress);
    // printf("INT_PIN: %d\n", digitalRead(INT_PIN));

    fd = wiringPiI2CSetupInterface("/dev/i2c-0", 0x20);
    
    // init MCP23017
    // mcp_writeGPIOAB(0x6d5f); // Set Def ports state 
    mcp_initReg(); 
    mcp_readPorts();

    // Init Leds
    for(int i = 0; i < LEDS; i++)
        mcp_pinMode(led[i], 0);
     
    // Init Keys
    mcp_setupInterrupts(1,0,0);

    for(int i = 0; i < KEYS; i++)
    {
        mcp_pinMode(key[i], 1);
        mcp_pullUp(key[i], 1);
        mcp_setupInterruptPin(key[i], 1);
    }

    // printf("GPIOAB:0x%02x\r\n",mcp_readGPIOAB());
    //mcp_readPorts();
    
    

	while(1)
    {
        delay(500);
        
        // NO Interrupt Key TEST
        // ------------------------------------------------------------------
        // for(int i = 0 ; i < KEYS ; i++)
        //     if(mcp_digitalRead(key[i]) != 1) printf("Key Press: %d\n", i);
        // ------------------------------------------------------------------

        // Interrupt Key TEST
        // ------------------------------------------------------------------
        printf("INT_PIN: %d\n", digitalRead(INT_PIN));

        // mcp_setupInterrupts(1,0,0);
        
        printf("INT Pin:%d Val:%d \n",mcp_getLastInterruptPin(), mcp_getLastInterruptPinValue());
    }
	return 0;
}
