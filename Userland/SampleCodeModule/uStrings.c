// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <stdint.h>
#include <stddef.h>
#include <uStrings.h>

void strcpy(char * dest, char * src) {
	int i = 0;
	while (src[i] != 0) {
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;
}

int strcmp(const char * str1, const char * str2) {
    int i = 0;
    while (str1[i] != 0 && str2[i] != 0 && str1[i] == str2[i]) {
        i++;
    }
    return str1[i] - str2[i];
}

int strncmp(char * str1, char * str2, int length) {
	int i = 0;
	while (i < length && str1[i] != 0 && str2[i] != 0 && str1[i] == str2[i]) {
		i++;
	}
	return i == length;
}

uint64_t atoi(char * str) {
    uint64_t num = 0;
    int i = 0;
    while (str[i] && (str[i] >= '0' && str[i] <= '9')) {
        num = num * 10 + (str[i] - '0');
        i++;
    }
    return num;
}

int strtoi(char * buffer, int * i) {
	char str[18];
	int size = 0;
	while(buffer[*i] != ' ' && buffer[*i] != '\n' && buffer[*i] != 0) {
		str[size++] = buffer[*i];
		(*i)++;
	}
	uint64_t out = atoi(str);
	return out;
}

void intToStr(int num, char* str) {
    int i = 0;
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    if (num < 0) {
        str[i] = '-';
        i++;
        num = -num;
    }
    int aux = num;
    while (aux > 0) {
        aux /= 10;
        i++;
    }
    str[i] = 0;
    i--;
    while (num > 0) {
        str[i] = num % 10 + '0';
        num /= 10;
        i--;
    }
}

// Convert 64 bit integer to hex string
void intToHex(uint64_t num, char* hex) {
    int i = 0;
    for (i = 15; i >= 0; i--) {
        int aux = num & 0xF;
        if (aux < 10) {
            hex[i] = aux + '0';
        } else {
            hex[i] = aux - 10 + 'A';
        }
        num >>= 4;
    }
    hex[16] = 0;
}

unsigned int strlen(char* str) {
    unsigned int i = 0;
    while (str[i] != 0) {
        i++;
    }
    return i;
}

char *strchr(const char *str, int c) {
    while (*str) {
        if (*str == (char)c)
            return (char *)str;  // Cast away const to match standard strchr signature
        str++;
    }
    return NULL;
}

static char *next = NULL;
char *strtok(char *str, const char *delim) {
   if (str != 0)
        next = str;
    else if (next == 0)
        return 0;

    char *start = next;
    while (*start && strchr(delim, *start))
        start++;

    if (*start == '\0') {
        next = 0;
        return 0;
    }

    char *end = start;
    while (*end && !strchr(delim, *end))
        end++;

    if (*end) {
        *end = '\0';
        next = end + 1;
    } else {
        next = 0;
    }

    return start;
}

void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
}


void appendString(char* dest, const char* src) {
    while (*dest) {
        dest++;
    }
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = 0;
}

void copyString(char *dest, const char *src) {
    while (*src) {
        *dest++ = *src++;
    }
    *dest = 0;
}

int isDigit(char c) {
    return c >= '0' && c <= '9';
}

int isVowel(char c) {
    char lower = (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
    return lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u';
}

int isNumber(const char *str) {
    if (str == NULL || *str == 0) {
        return 0;
    }
    int i = 0;
    while (str[i]) {
        if (!isDigit(str[i])) {
            return 0;
        }
        i++;
    }
    return 1;
}