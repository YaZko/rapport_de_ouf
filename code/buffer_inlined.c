#include <stdio.h>

char * strcopy(char *s1, const char *s2) {
	char *s = s1;
	while ((*s++ = *s2++) != 0)
		;
	return s1;
}

void evil_code() {
	printf("Argh, we got hacked!\n");
}

void foo(char* input){
	char buf[1];
	printf("\nStack before:\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n\n");
	strcopy(buf, input);
	printf("\nStack after :\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n%#010x\n\n");
}

int main (int argc, char* argv[]) {
	void (*a)(void) = evil_code;
	printf("Address of evil_code = %#010x\n", evil_code);
	if (argc < 2) {
		printf("Need an argument\n");
	} else {
		foo(argv[1]);
	}
	return 0;
}


