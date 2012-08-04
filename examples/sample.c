#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// gcc -g -O2 sample.c -o sample
int main(int argc, char *argv[]) {
	char *buffer = NULL;
	size_t length = 0;
	
	srand(time(0));
	length = rand() % 4096 + 20;
	
	printf ("Start of the sample application...\n");
	printf ("Allocate %ld\n", length);
	
	buffer = (char *)malloc(length);
	if (buffer) {
		strcpy(buffer, "Hello World!\0");
		
		printf("Buffer contains: %s\n", buffer);

		printf("Free buffer\n");
		free(buffer);
		buffer = NULL;
	}
	return 0;
}