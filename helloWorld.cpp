#include <iostream>
#include <stdlib.h>
#include <time.h>

int main() {
	std::cout << "hello" << std::endl;	
	// srand(time(NULL));
	srand(1);
	std::cout << rand() << std::endl;
	srand(2);
	std::cout << rand() << std::endl;
	srand(3);
	std::cout << rand() << std::endl;
	srand(1);
	std::cout << rand() << std::endl;
	srand(2);
	std::cout << rand() << std::endl;
	srand(3);
	std::cout << rand() << std::endl;
}

