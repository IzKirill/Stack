#include <stdio.h>
#include <locale.h>
#include <iostream>

int main() {
	
	 char *locale = setlocale(LC_ALL, "");
	int value = 0;

	unsigned char d = 0;

	scanf("%c", &d);

	std::cout << d;
	
}
