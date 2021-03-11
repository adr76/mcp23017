/* 
gcc -Wall -o mcp23017_wiringPi mcp23017_wiringPi.c -lwiringPi -lpthread
sudo ./mcp23017_wiringPi

MCP23017
PORT   A0  A1  A2  A3  A4  A5  A6   A7  |  B0  B1  B2  B3  B4  B5  B6   B7
FUNC   K4  K5  K6  K7  K8  L4  K11  L5  |  K1  L1  K2  K3  L2  K9  K10  L3
OUT    0   0   0   0   0   1   0    1   |  0   1   0   0   1   0   0    1
PORT   0   1   2   3   4   5   6    7      8   9   10  11  12  13  14   15
*/

#include <stdio.h>
//#include <string.h>
//#include <errno.h>
//#include <stdlib.h>
#include <wiringPi.h>
//#include <wiringPiI2C.h>
#include <mcp23017.h>

#define	INT_PIN	7
#define LEDS 5
#define KEYS 11

#define key(num) (100 + key[num])
#define led(num) (100 + led[num])


int led[LEDS] = {9, 12, 15, 5, 7};
int key[KEYS] = {8, 10, 11, 0, 1, 2, 3, 4, 13, 14, 6};

int main(void)
{
	wiringPiSetup();
	// pinMode(INT_PIN, INPUT);
	// pullUpDnControl(INT_PIN, PUD_UP);
	
	// MCP Init
	mcp23017Setup(100, 0x20);
	
	// MCP Ports init
	for (int i = 0 ; i < LEDS ; ++i)
		pinMode (led(i), OUTPUT);

	for (int i = 0 ; i < KEYS ; ++i)
	{
		pinMode 		(key(i), INPUT);
	 	pullUpDnControl (key(i), PUD_UP);	
	}

	
	while(1)
    {
        delay(500);
		
		if (digitalRead (key(0)) == 0)
			digitalWrite (led(0), 1);
		else 
			digitalWrite (led(0), 0);
		
		for (int i = 0 ; i < KEYS ; ++i)
			if (digitalRead (key(i)) == 0) printf("Key:%d \n" ,i);
	}
	
	return 0;
}
