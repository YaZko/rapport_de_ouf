#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void evil_code() {
	printf("Argh, we got hacked!\n");
}

void foo(char* input){
	char buf[1];
	printf("\nStack before:\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n\n");
	strcpy(buf, input);
	printf("\nStack after :\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n\n");
}

int main (int argc, char* argv[]) {
	void (*a)(void) = evil_code;
	printf("Address of evil_code = %#010x\n", evil_code);
	foo(argv[1]);
	//foo("aaaaaaaaa\x4b\x84\x04\x08");
	//foo("aaaaaaaaa\x3b\x84\x04\x08");
	//0x804844b
	return 0;
}


