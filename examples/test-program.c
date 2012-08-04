#include <stdio.h>
#include <unistd.h>

int myfunction(int a_int, char a_char, short a_short, long a_long, long long a_long_long)
{
	printf("in myfunction\n");
	return 1;
}

void myfunction2(char *message)
{
	printf("c program: the address of message in call is %p\n", message);
	printf("c program: message = %s\n", message);
}

int main(int argc, char **argv)
{
	char message[100];
	memset(message, 0, 100);
	strncpy(message, "koko lala one two", 99);

	int ret;
    ret = sleep(1);
	printf("c program: sleep returned %d\n", ret);
	printf("c program: message address before call is %p\n", message);
	myfunction2(message);
    sleep(2);
	printf("c program: sleep returned %d\n", ret);
    sleep(3);
	printf("c program: sleep returned %d\n", ret);
    return 0;
}

