# MCP23017 demo by Waveshare
# make mcp-demo
# sudo ./mcp-demo
#
# My MCP23017 test for OledCotroller
# make mymcp
# sudo ./mymcp

mymcp:mymcp.o mcp23017.o
	gcc -Wall -o mymcp mymcp.o mcp23017.o -lwiringPi -std=gnu99
mcp-demo:mcp-demo.o mcp23017.o
	gcc -Wall -o mcp-demo mcp-demo.o mcp23017.o -lwiringPi -std=gnu99
mymcp.o: mymcp.c mcp23017.h
	gcc -Wall -c mymcp.c -lwiringPi -std=gnu99	
mcp-demo.o: mcp-demo.c mcp23017.h
	gcc -Wall -c mcp-demo.c -lwiringPi -std=gnu99
mcp23017.o: mcp23017.c mcp23017.h
	gcc -Wall -c  mcp23017.c -lwiringPi -std=gnu99
clean:
	rm -f mcp-demo.o mymcp.o mcp23017.o mcp-demo mymcp