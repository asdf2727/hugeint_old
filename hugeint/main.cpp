#include <iostream>
#include <string>

#include "hugeint.h"

int main() {
	hugeint nr1 = (std::string)"100000000";
	hugeint nr2 = (std::string)"100000000";
	std::cout << (nr1 * nr2).to_string();
}