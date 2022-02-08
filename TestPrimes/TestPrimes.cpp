#include <iostream>
#include <vector>
#include <cmath>

int PUT = 16;
#define DIV ((unsigned long long int)1 << PUT)

std::vector <bool> iscomp;
std::vector <unsigned long long int> prim;

void DoPrims(int maxp) {
	int pos = 2, last = 2;
	if (pos < maxp) {
		iscomp.resize(maxp + 1);
		for (; pos * pos <= maxp; pos++) {
			if (!iscomp[pos]) {
				for (unsigned long long int index2 = (unsigned long long int)pos * pos; index2 <= maxp; index2 += pos) {
					iscomp[pos] = true;
				}
			}
		}
		for (; last <= maxp; last++) {
			if (!iscomp[last]) {
				prim.push_back(last);
			}
		}
	}
}

int cnt = 1;

bool IsPrime(unsigned long long int num) {
	std::cout << "1 + 2^" << PUT << " * " << cnt << " = " << num << " is ";
	DoPrims((int)(std::sqrt(num) + 10));
	for (int index = 0; index < (int)prim.size(); index++) {
		if (num % prim[index] == 0) {
			std::cout << "divisible by " << prim[index] << '\n';
			return false;
		}
	}
	std::cout << "prime\n";
	while (!(cnt & 1)) {
		PUT++;
		cnt >>= 1;
	}
	return true;
}

int main() {
	unsigned long long int num = (1ull << PUT);
	while (1) {
		IsPrime(num + 1);
		num += (1ull << PUT);
		cnt++;
	}
}