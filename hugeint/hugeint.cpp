#include <iostream>
#include <deque>
#include <string>
#include <cmath>
#include <vector>
#include <array>

#include "hugeint.h"

typedef unsigned long long int ullint;
typedef long long int llint;
typedef unsigned int uint;
typedef unsigned short int usint;
typedef short int sint;

/*
Add sign (a bool)
add bits op.

TODO:
iterate with unsigned int / unsigned long long int for bigger numbers
erase the resize for += -= for faster operations
faster *
read infint for ideas for generalization
standardize x and x= operator dependance (x= comes before x unles its faster to make x a separare operator)

TEST:
ALL
*/

hugeint::hugeint() {
	bits.clear();
}
hugeint::hugeint(uint nri) {
	bits.clear();
	if (nri) {
		bits.push_back(nri);
	}
}
hugeint::hugeint(ullint nri) {
	bits.clear();
	if (nri) {
		bits.push_back(nri & 0x00000000ffffffff);
	}
	if (nri & 0xffffffff00000000) {
		bits.push_back((nri & 0xffffffff00000000) >> 32);
	}
}
hugeint::hugeint(std::string nri) {
	bits.clear();
	for (char digit : nri) {
		this->operator*=(10);
		this->operator+=(digit - '0');
	}
}

std::string hugeint::to_string() const {
	std::vector <char> calc;
	bool sav, last, isnull;
	calc.push_back(0);
	isnull = true;
	for (int index = bits.size() - 1; index >= 0; index--) {
		for (uint pos = 0x80000000; pos > 0; pos >>= 1) {
			if (bits[index] & pos) {
				for (int index2 = 0; index2 < (int)calc.size(); index2++) {
					if (calc[index2] == 9) {
						calc[index2] = 0;
						if (index2 == (int)calc.size() - 1) {
							calc.push_back(1);
							break;
						}
					}
					else {
						calc[index2]++;
						break;
					}
				}
				isnull = false;
			}
			if ((index || pos != 1) && !isnull) {
				sav = false;
				for (int index2 = 0; index2 < (int)calc.size(); index2++) {
					last = sav;
					if (calc[index2] > 4) {
						sav = true;
						calc[index2] = (calc[index2] << 1) - 10;
					}
					else {
						sav = false;
						calc[index2] <<= 1;
					}
					if (last) {
						calc[index2]++;
					}
				}
				if (sav) {
					calc.push_back(1);
				}
			}
		}
	}
	std::string ans = "";
	for (int index = calc.size() - 1; index >= 0; index--) {
		ans.push_back(calc[index] + '0');
	}
	return ans;
}
ullint hugeint::to_long() const {
	return ((bits.size() > 1 ? (ullint)bits[1] << 32 : 0ull)) + (bits.empty() ? 0 : bits[0]);
}
uint hugeint::to_int() const {
	return (bits.empty() ? 0 : bits[0]);
}

hugeint hugeint::operator=(hugeint nri) {
	bits = nri.bits;
	return *this;
}
hugeint hugeint::operator=(uint nri) {
	bits.clear();
	if (nri) {
		bits.push_back(nri);
	}
	return *this;
}
hugeint hugeint::operator=(ullint nri) {
	bits.clear();
	if (nri) {
		bits.push_back(nri & 0x00000000ffffffff);
	}
	if (nri & 0xffffffff00000000) {
		bits.push_back((nri & 0xffffffff00000000) >> 32);
	}
	return *this;
}
hugeint hugeint::operator=(std::string nri) {
	return *this = *(new hugeint(nri));
}

hugeint hugeint::operator~() const {
	hugeint ans;
	ullint sav = 1;
	if (!bits.empty()) {
		ans.bits.resize(bits.size());
		for (int index = 0; index < (int)bits.size() - 1; index++) {
			ans.bits[index] = ~bits[index];
		}
		for (int index = 16; index > 0; index >>= 1) {
			if (bits.back() >= sav * (1ull << index)) {
				sav *= (1ull << index);
			}
		}
		sav <<= 1;
		ans.bits.back() = (uint)(sav - 1 - bits.back());
	}
	return ans;
}

hugeint hugeint::operator&=(hugeint to_and) {
	bits.resize(std::min(bits.size(), to_and.bits.size()));
	for (int index = 0; index < (int)bits.size(); index++) {
		bits[index] &= to_and.bits[index];
	}
	while (!bits.empty() && bits.back() == 0) {
		bits.pop_back();
	}
	return (*this);
}
hugeint hugeint::operator&(hugeint to_and) const {
	hugeint ans = *this;
	ans &= to_and;
	return ans;
}

hugeint hugeint::operator|=(hugeint to_or) {
	if (bits.size() < to_or.bits.size()) {
		bits.resize(to_or.bits.size());
	}
	for (int index = 0; index < (int)to_or.bits.size(); index++) {
		bits[index] |= to_or.bits[index];
	}
	return (*this);
}
hugeint hugeint::operator|(hugeint to_or) const {
	hugeint ans = (*this);
	ans |= to_or;
	return ans;
}

hugeint hugeint::operator^=(hugeint to_xor) {
	if (bits.size() < to_xor.bits.size()) {
		bits.resize(to_xor.bits.size());
	}
	for (int index = 0; index < (int)to_xor.bits.size(); index++) {
		bits[index] ^= to_xor.bits[index];
	}
	while (!bits.empty() && bits.back() == 0) {
		bits.pop_back();
	}
	return (*this);
}
hugeint hugeint::operator^(hugeint to_xor) const {
	hugeint ans = (*this);
	ans ^= to_xor;
	return ans;
}

hugeint hugeint::operator<<=(llint val) {
	if (val > 0) {
		if (!bits.empty()) {
			ullint digadd = val >> 5;
			int bitshift = val % 32;
			if (bitshift) {
				uint part1 = (1ull << (32 - bitshift)) - 1;
				uint part2 = 0xffffffff ^ part1;
				bits.push_back(0);
				for (int index = bits.size() - 1; index > 0; index--) {
					bits[index] = ((bits[index] & part1) << bitshift) + ((bits[index - 1] & part2) >> (32 - bitshift));
				}
				bits[0] = (bits[0] & part1) << bitshift;
				if (bits.back() == 0) {
					bits.pop_back();
				}
			}
			for (int index = 0; index < digadd; bits.push_front(0), index++);
		}
	}
	else if (val < 0) {
		this->operator>>=(-val);
	}
	return *this;
}
hugeint hugeint::operator<<(llint val) const {
	hugeint calc = *this;
	calc <<= val;
	return calc;
}
hugeint hugeint::operator>>=(llint val) {
	if (val > 0) {
		ullint digdel = val >> 5;
		int bitshift = val % 32;
		for (int index = 0; !bits.empty() && index < digdel; bits.pop_front(), index++);
		if (!bits.empty() && bitshift) {
			uint part1 = (1ull << (32 - bitshift)) - 1;
			uint part2 = 0xffffffff ^ part1;
			for (int index = 0; index < (int)bits.size() - 1; index++) {
				bits[index] = ((bits[index] & part2) >> bitshift) + ((bits[index + 1] & part1) << (32 - bitshift));
			}
			bits.back() = (bits.back() & part2) >> bitshift;
			if (bits.back() == 0) {
				bits.pop_back();
			}
		}
	}
	else if (val < 0) {
		this->operator<<=(-val);
	}
	return *this;
}
hugeint hugeint::operator>>(llint val) const {
	hugeint calc = *this;
	calc >>= val;
	return calc;
}

bool hugeint::operator<=(hugeint to_comp) const {
	if (bits.size() < to_comp.bits.size()) {
		return true;
	}
	else if (bits.size() > to_comp.bits.size()) {
		return false;
	}
	else {
		for (int index = bits.size() - 1; index >= 0; index--) {
			if (bits[index] != to_comp.bits[index]) {
				return bits[index] < to_comp.bits[index];
			}
		}
	}
	return true;
}
bool hugeint::operator<=(uint to_comp) const {
	return *this <= (hugeint)to_comp;
}
bool hugeint::operator<(hugeint to_comp) const {
	if (bits.size() < to_comp.bits.size()) {
		return true;
	}
	else if (bits.size() > to_comp.bits.size()) {
		return false;
	}
	else {
		for (int index = bits.size() - 1; index >= 0; index--) {
			if (bits[index] != to_comp.bits[index]) {
				return bits[index] < to_comp.bits[index];
			}
		}
	}
	return false;
}
bool hugeint::operator<(uint to_comp) const {
	return *this < (hugeint)to_comp;
}
bool hugeint::operator>=(hugeint to_comp) const {
	return to_comp <= *this;
}
bool hugeint::operator>=(uint to_comp) const {
	return (hugeint)to_comp <= *this;
}
bool hugeint::operator>(hugeint to_comp) const {
	return to_comp < *this;
}
bool hugeint::operator>(uint to_comp) const {
	return (hugeint)to_comp < *this;
}

bool hugeint::operator==(hugeint to_comp) const {
	return bits == to_comp.bits;
}
bool hugeint::operator==(uint to_comp) const {
	return this->operator==((hugeint)to_comp);
}
bool hugeint::operator!=(hugeint to_comp) const {
	return !(bits == to_comp.bits);
}
bool hugeint::operator!=(uint to_comp) const {
	return this->operator!=((hugeint)to_comp);
}

bool hugeint::operator&&(hugeint to_comp) const {
	return (bits.size() && to_comp.bits.size());
}
bool hugeint::operator&&(uint to_comp) const {
	return (bits.size() && to_comp);
}
bool hugeint::operator||(hugeint to_comp) const {
	return (bits.size() || to_comp.bits.size());
}
bool hugeint::operator||(uint to_comp) const {
	return (bits.size() || to_comp);
}

hugeint hugeint::operator++() {
	bool mod = false;
	for (int index = 0; index < (int)bits.size(); index++) {
		if (bits[index] == 0xffffffff) {
			bits[index] = 0;
		}
		else {
			bits[index]++;
			mod = true;
			break;
		}
	}
	if (!mod) {
		bits.push_back(0);
	}
	return *this;
}
hugeint hugeint::operator--() {
	for (int index = 0; index < (int)bits.size(); index++) {
		if (bits[index] == 0) {
			bits[index] = 0xffffffff;
		}
		else {
			bits[index]--;
			if (!bits[index] && index == bits.size() - 1) {
				bits.pop_back();
			}
			break;
		}
	}
	return *this;
}
hugeint hugeint::operator++(int) {
	hugeint sav = *this;
	this->operator++();
	return sav;
}
hugeint hugeint::operator--(int) {
	hugeint sav = *this;
	this->operator--();
	return sav;
}

hugeint hugeint::operator+=(hugeint to_add) {
	if (to_add.bits.size() < bits.size()) {
		to_add.bits.resize(bits.size());
	}
	if (to_add.bits.size() > bits.size()) {
		bits.resize(to_add.bits.size());
	}
	bool last, sav = false;
	for (int index = 0; index < (int)bits.size(); index++) {
		last = sav;
		sav = ((last ? 1ull : 0ull) + bits[index] + to_add.bits[index]) & 0x0000000100000000;
		bits[index] = (uint)((last ? 1ull : 0ull) + bits[index] + to_add.bits[index]);
	}
	if (sav) {
		bits.push_back(1);
	}
	return *this;
}
hugeint hugeint::operator+=(uint to_add) {
	this->operator+=((hugeint)to_add);
	return *this;
}
hugeint hugeint::operator+(hugeint to_add) {
	hugeint calc = *this;
	calc += to_add;
	return calc;
}
hugeint hugeint::operator+(uint to_add) {
	hugeint calc = *this;
	calc += (hugeint)to_add;
	return calc;
}

hugeint hugeint::operator-=(hugeint to_sub) {
	if (to_sub.bits.size() < bits.size()) {
		to_sub.bits.resize(bits.size());
	}
	if (to_sub.bits.size() > bits.size()) {
		bits.resize(to_sub.bits.size());
	}
	bool last, sav = false;
	for (int index = 0; index < (int)bits.size(); index++) {
		last = sav;
		sav = 0ll + bits[index] - to_sub.bits[index] - (last ? 1ll : 0ll) < 0ll;
		bits[index] = (uint)((sav ? 0x0000000100000000ll : 0ll) + bits[index] - to_sub.bits[index] + (last ? 1ll : 0ll));
	}
	while (!bits.empty() && bits.back() == false) {
		bits.pop_back();
	}
	return *this;
}
hugeint hugeint::operator-=(uint to_sub) {
	return this->operator-=((hugeint)to_sub);
}
hugeint hugeint::operator-(hugeint to_sub) {
	hugeint calc = *this;
	calc -= to_sub;
	return calc;
}
hugeint hugeint::operator-(uint to_sub) {
	hugeint calc = *this;
	calc -= (hugeint)to_sub;
	return calc;
}

static int test = 5;

#define MULT_SIZ 32
#define MULT_SAV 18
#define MULT_MOD (ullint)(1ull + (1ull << MULT_SIZ) * MULT_SAV) //equals 77309411329, is prime

constexpr void MultMod(ullint& num1, ullint num2) {
	ullint part11 = (uint)num1, part21 = num1 >> 32, part12 = (uint)num2, part22 = num2 >> 32;
	ullint prec = (part11 * part22) % MULT_MOD + (part12 * part21) % MULT_MOD;
	num1 = ((part11 * part12) % MULT_MOD + (prec << 32) % MULT_MOD + ((113346 * (part21 * part22 + (prec >> 32))) % MULT_MOD) * 149465) % MULT_MOD;
	/*
	Functiones like so:
	ab * cd = (a * c) << 2S + (a * d + b * c) << S + b * d
	where S = 32 (the size for uint)

	the first: b * d + ((a * d + b * c) << S)
	the second: ((a * c) + (a * d + b * c) >> S) * ((1 << 2S) % MOD)
	here ((1 << 2S) % MOD) = 113346 * 149465, and is multiplied in two steps to avoid overflow
	*/
}

class ConstexprSolutionsArray {
public:
	ullint sol[MULT_SIZ];
	constexpr ConstexprSolutionsArray() : sol{} {
		ullint nrf = 1, cnt = 2;
		bool done = true;
		while (1) {
			nrf = 1;
			for (int index = 0; index < MULT_SAV; index++) {
				MultMod(nrf, cnt);
			}
			sol[0] = nrf;
			for (int index = 1; index < MULT_SIZ; index++) {
				MultMod(nrf, nrf);
				sol[index] = nrf;
				if (nrf == 1) {
					done = false;
					break;
				}
			}
			if (done) {
				break;
			}
			done = true;
			cnt++;
		}
	}
	ullint operator[](size_t index) { return sol[index]; }
};
ConstexprSolutionsArray MULT_SOL;

void MultiplicationDFT(std::vector <uint>& coef, int log_size, bool inverse = false) {
	if (!log_size) {
		return;
	}
	std::vector <uint> num[2];
	for (int index = 0; index < coef.size(); index++) {
		num[index & 1].push_back(coef[index]);
	}
	MultiplicationDFT(num[0], log_size - 1, inverse);
	MultiplicationDFT(num[1], log_size - 1, inverse);
	ullint first_sol = MULT_SOL[MULT_SIZ - log_size];
	ullint now_sol = 1, val;
	for (int index = 0; index < coef.size(); index++) {
		val = now_sol;
		//MultMod(val, num[1][index & ((coef.size() >> 1) - 1)]);
		//val += num[0][index & ((coef.size() >> 1) - 1)];
		MultMod(val, num[1][index % (coef.size() / 2)]);
		val += num[0][index % (coef.size() / 2)];
		val %= MULT_MOD;
		if (inverse) {
			MultMod(val, 38654705665);
		}
		coef[index] = (uint)val;
		MultMod(now_sol, first_sol);
		now_sol %= MULT_MOD;
	}
}

hugeint hugeint::operator*(hugeint to_mult) {
	uint max_size = bits.size() + to_mult.bits.size() + 1;
	int log_size = 0;
	for (int index = 16; index > 0; index >>= 1) {
		if (max_size >  1 << (log_size + index)) {
			log_size += index;
		}
	}
	log_size += 2;
	max_size = 1 << log_size;
	std::vector <uint> num1(max_size), num2(max_size);
	for (int index = 0; index < bits.size(); index += 2) {
		num1[index] = bits[index] & 0x0000ffff;
		num1[index + 1] = bits[index] >> 16;
	}
	for (int index = 0; index < to_mult.bits.size(); index += 2) {
		num2[index] = to_mult.bits[index] & 0x0000ffff;
		num2[index + 1] = to_mult.bits[index] >> 16;
	}
	MultiplicationDFT(num1, log_size);
	MultiplicationDFT(num2, log_size);
	for (int index = 0; index < max_size; index++) {
		num1[index] = ((ullint)num1[index] * num2[index]) % MULT_MOD;
	}
	MultiplicationDFT(num1, log_size, true);
	hugeint ans;
	ans.bits.resize(num1.size() >> 1);
	ullint last = 0;
	for (int index = 0; index < num1.size(); index += 2) {
		ans.bits[index >> 1] = (last + num1[index] + (num1[index + 1] << 16));
		last = ((last + num1[index]) >> 16 + num1[index + 1]) >> 16;
	}
	while (!ans.bits.empty() && ans.bits.back() == 0) {
		ans.bits.pop_back();
	}
	return ans;
}
hugeint hugeint::operator*(uint to_mult) {
	hugeint calc = *this;
	calc *= to_mult;
	return calc;
}
hugeint hugeint::operator*=(hugeint to_mult) {
	return *this = this->operator*(to_mult);
}
hugeint hugeint::operator*=(uint to_mult) {
	ullint form = 0;
	for (int index = 0; index < (int)bits.size(); index++) {
		form += (ullint)bits[index] * to_mult;
		bits[index] = form & 0x00000000ffffffff;
		form >>= 32;
	}
	if (form) {
		bits.push_back((uint)form);
	}
	return *this;
}

uint DivBinSearch(hugeint& rest, hugeint to_div) {
	uint calc = 0;
	hugeint form, to_add = (to_div << 31);
	for (uint index = 0x80000000; index > 0; index >>= 1) {
		if (form + to_add <= rest) {
			form += to_add;
			calc += index;
		}
		to_add >>= 1;
	}
	rest -= form;
	return calc;
}

hugeint hugeint::operator/(hugeint to_div) {
	hugeint calc, rest;
	for (int index = bits.size() - 1; index >= 0; index--) {
		rest <<= 32;
		rest += bits[index];
		calc <<= 32;
		if (to_div <= rest) {
			calc += DivBinSearch(rest, to_div);
		}
	}
	return calc;
}
hugeint hugeint::operator/(uint to_div) {
	return *this / (hugeint)to_div;
}
hugeint hugeint::operator/=(hugeint to_div) {
	return *this = *this / to_div;
}
hugeint hugeint::operator/=(uint to_div) {
	return *this = *this / (hugeint)to_div;
}

hugeint hugeint::operator%(hugeint to_div) {
	hugeint rest;
	for (int index = bits.size() - 1; index >= 0; index--) {
		rest <<= 32;
		rest += bits[index];
		if (to_div <= rest) {
			DivBinSearch(rest, to_div);
		}
	}
	return rest;
}
hugeint hugeint::operator%(uint to_div) {
	return *this % (hugeint)to_div;
}
hugeint hugeint::operator%=(hugeint to_div) {
	return *this = *this % to_div;
}
hugeint hugeint::operator%=(uint to_div) {
	return *this = *this % (hugeint)to_div;
}