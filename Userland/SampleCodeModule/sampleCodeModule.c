/* sampleCodeModule.c */
#include <userio.h>

int main() {
	printColor("ben@SO: $> ", 0x4e9a06);

	while(1) {
		char c = getChar();
		if (c != 0xFF && c > 0)
			printColorChar(c, 0x4e9a06);
	}

	return 0;
}