/* sampleCodeModule.c */
#include <userio.h>

void analizeBuffer();

int main() {
	printColor("ben@SO: $> ", 0x4e9a06);

	int count = 0;	
	char buffer[1024] = {0};
	while(1) {
		char c = getChar();
		if (c == '\n') {
			analizeBuffer(buffer);
			printColor("\nben@SO: $> ", 0x4e9a06);
			count = 0;
		} else if (c == '\b') {
			if (count > 0) {
				printChar(c);
				count--;
			}
		} else if (c != 0xFF && c > 0) {
			printColorChar(c, 0x4e9a06);
			buffer[count++] = c;
			buffer[count] = 0;
		}
	}

	return 0;
}

int strcmp(char * str1, char * str2) {
	int i = 0;
	while (str1[i] != 0 && str2[i] != 0) {
		if (str1[i] != str2[i]) {
			return 0;
		}
		i++;
	}
	return str1[i] == str2[i];
}

void analizeBuffer(char * buffer) {
	if (strcmp(buffer, "help") || strcmp(buffer, "HELP")) {
		printColor("\nComandos disponibles:\n", 0x4e9a06);
	}
}