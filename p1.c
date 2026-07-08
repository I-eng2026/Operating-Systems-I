#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char *argv[]) 
{
    // Program expects a character as a command line argument
    if (argc != 2) 
    {
        // If argument is not provided, print message to 
        // stderr and exit the program
        fprintf(stderr, "Usage: %s <character>\n", argv[0]);     
        exit(1);
    }
    // Args are strings, get first char of first arg.
    char ch = argv[1][0];
		
    // Loop will print the character 60 times, pausing for 1s
    // each time (1 minute run-time total).
    for (int i = 1; i <= 60; i++) 
    {
        sleep(1);
        fprintf(stdout, "%c", ch);
        // stdout is line-buffered by default, so we flush to 
        // make sure each character is printed after 1s and not
        // all at once after 60s.
        fflush(stdout); 
    }
	
    return 0;    
}
