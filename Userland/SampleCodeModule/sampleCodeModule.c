/* sampleCodeModule.c */
#include <userio.h>

int main() {
	print("Hello WorldHello World!Hello World!Hello World!Hello World!Hello World!Hello World!Hello World!Hello World!!");

	while(1) {
		char c = getChar();
		if (c != 0xFF && c > 0)
			printChar(c);
	}

	return 0;
}