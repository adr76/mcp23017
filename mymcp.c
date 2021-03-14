/* OLED Controller
MCP23017 PORTS i2C.ADDR 0x20 
--------------------------------------------------------------------------
PORT   A0  A1  A2  A3  A4  A5  A6   A7  |  B0  B1  B2  B3  B4  B5  B6   B7
FUNC   K4  K5  K6  K7  K8  L4  K11  L5  |  K1  L1  K2  K3  L2  K9  K10  L3
OUT    0   0   0   0   0   1   0    1   |  0   1   0   0   1   0   0    1
PIN    0   1   2   3   4   5   6    7      8   9   10  11  12  13  14   15
--------------------------------------------------------------------------
FUNC    LED1 LED2 LED3 LED4 LED5
PORT    9    12   15   5    7
--------------------------------------------------------------------------
FUNC    KEY1 KEY2 KEY3 KEY4 KEY5 KEY6 KEY7 KEY8 KEY9 KEY10 KEY11
PORT    8    10   11   0    1    2    3    4    13   14    6
*/

/* WiringPi I2C Function
int wiringPiI2CRead           (int fd) ;
int wiringPiI2CReadReg8       (int fd, int reg) ;
int wiringPiI2CReadReg16      (int fd, int reg) ;
int wiringPiI2CWrite          (int fd, int data) ;
int wiringPiI2CWriteReg8      (int fd, int reg, int data) ;
int wiringPiI2CWriteReg16     (int fd, int reg, int data) ;
int wiringPiI2CSetupInterface (const char *device, int devId) ;
int wiringPiI2CSetup          (const int devId) ;
*/


#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "mcp23017.h"

#define I2C_BUS  "/dev/i2c-0"
#define MCP_ADDR 0x20

#define INT_PIN 7 // OrangePi wPi:7
#define KEYS 11
#define LEDS 5

int led[LEDS] = {9, 12, 15, 5, 7};
int key[KEYS] =     {8, 10, 11, 0, 1, 2, 3, 4, 13, 14, 6};
int key_map[KEYS] = {8, 10, 11, 0, 1, 2, 3, 4, 13, 14, 6};

void event(void)
{
    printf("Key press!!!\r\n");
    printf("last interrupt Pin: %d\r\n",mcp_getLastInterruptPin());
    printf("tlast interrupt Val: %d\r\n",mcp_getLastInterruptPinValue());
    printf("_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\r\n");
}

void mcp_readRegs(void)
{
for(int addr = 0; addr< 22; addr++) {
        printf("addr:0x%02x,value:0x%02x\r\n",addr,mcp_readRegister(addr));
    }
    printf("------------------------------------------\r\n");   
}

void mcp_readPorts(void)
{
    printf("MCP PORTS:  ");
	for (int i = 0 ; i < 16 ; ++i)
		printf("%d ", mcp_digitalRead(i));
	printf("\n");
}

void mcp_init(void) 
{
    // Init Leds
    // int i;
    // for(i = 0; i < LEDS; i++)
    //     mcp_pinMode(led[i], 0);
    // // Init Keys
    // for(i = 0; i < KEYS; i++)
    // {
    //     mcp_pinMode(key[i], 1);
    //     mcp_pullUp(key[i], 1);
    //     mcp_setupInterruptPin(key[i], 1);
    // }

    // Mode & PolUp & Int
    mcp_writeRegister(MCP23017_IODIRA, 0x5f);
    mcp_writeRegister(MCP23017_IODIRB, 0x6d);
    mcp_writeRegister(MCP23017_GPPUA, 0x5f);
    mcp_writeRegister(MCP23017_GPPUB, 0x6d);
    mcp_writeRegister(MCP23017_GPINTENA, 0x5f);
    mcp_writeRegister(MCP23017_GPINTENB, 0x6d);
    // mcp_writeGPIOAB(0x6d5f); // Set Def ports state 
    // printf("IODIRA:0x%02x\r\n", mcp_readRegister(MCP23017_IODIRA));
    // printf("GPIOAB:0x%02x\r\n", mcp_readGPIOAB());
    
    // Init Interrupts
    // mcp_setupInterrupts(1,0,0); 
    mcp_writeRegister(MCP23017_IOCONA, 0x40); // Mirror
    mcp_writeRegister(MCP23017_IOCONB, 0x40); // Mirror
    // mcp_writeRegister(MCP23017_IOCONA, 0x42); // Mirror and PolUp
    // Set INT DefVal
    // mcp_writeRegister(MCP23017_DEFVALA, 0x5f);
    // mcp_writeRegister(MCP23017_DEFVALB, 0x6d);


    // printf("IOCON:0x%02x\r\n", mcp_readRegister(MCP23017_IOCONA));
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

    // Interrupt pin 
    pinMode(INT_PIN, INPUT);
    //pullUpDnControl(INT_PIN, PUD_UP); // HW 10k=>3v3
    // wiringPiISR(INT_PIN, INT_EDGE_BOTH, &keyPress);
    // wiringPiISR(INT_PIN, INT_EDGE_RISING, &keyPress); 
    wiringPiISR(INT_PIN, INT_EDGE_FALLING, &event);

    // init MCP23017
    fd = wiringPiI2CSetupInterface(I2C_BUS, MCP_ADDR);
    mcp_resetRegs();
    mcp_init();

	while(1)
    {
        delay(500);
        
        // NO Interrupt Key TEST
        // ------------------------------------------------------------------
        // for(int i = 0 ; i < KEYS ; i++)
        //         if(mcp_digitalRead(key[i]) != 1)
        //             printf("Key Press: %d\n", i);
        // ------------------------------------------------------------------

        // Interrupt Key TEST
        // ------------------------------------------------------------------
        // printf("INT_PIN: %d\n", digitalRead(INT_PIN));

        //int key_relise = mcp_getLastInterruptPinValue();
        int key_num = mcp_getLastInterruptPin();

        if(!digitalRead(INT_PIN))
        {
            // mcp_setupInterrupts(1,0,0);   
            // printf("INT Pin:%d Val:%d \n",mcp_getLastInterruptPin(), mcp_getLastInterruptPinValue());
            printf("Key Press: %d\n", key_num);
        }
    }
	return 0;
}
