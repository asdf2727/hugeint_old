#pragma once

#include <string>
#include <deque>

class hugeint {
private:
	std::deque <unsigned int> bits;

	class ModData {
		int put = 30, mult = 3;
	};
public:
	hugeint();
	hugeint(unsigned int nri);
	hugeint(unsigned long long int nri);
	hugeint(std::string nri);

	std::string to_string() const;
	unsigned long long int to_long() const;
	unsigned int to_int() const;

	hugeint operator=(hugeint nri);
	hugeint operator=(unsigned int nri);
	hugeint operator=(unsigned long long int nri);
	hugeint operator=(std::string nri);

	hugeint operator~() const;

	hugeint operator&(hugeint to_and) const;
	hugeint operator&=(hugeint to_and);

	hugeint operator|(hugeint to_or) const;
	hugeint operator|=(hugeint to_or);

	hugeint operator^(hugeint to_xor) const;
	hugeint operator^=(hugeint to_xor);

	hugeint operator<<=(long long int val);
	hugeint operator<<(long long int val) const;
	hugeint operator>>=(long long int val);
	hugeint operator>>(long long int val) const;

	bool operator<=(hugeint to_comp) const;
	bool operator<=(unsigned int to_comp) const;
	bool operator<(hugeint to_comp) const;
	bool operator<(unsigned int to_comp) const;
	bool operator>=(hugeint to_comp) const;
	bool operator>=(unsigned int to_comp) const;
	bool operator>(hugeint to_comp) const;
	bool operator>(unsigned int to_comp) const;

	bool operator==(hugeint to_comp) const;
	bool operator==(unsigned int to_comp) const;
	bool operator!=(hugeint to_comp) const;
	bool operator!=(unsigned int to_comp) const;

	bool operator&&(hugeint to_comp) const;
	bool operator&&(unsigned int to_comp) const;
	bool operator||(hugeint to_comp) const;
	bool operator||(unsigned int to_comp) const;

	hugeint operator++();
	hugeint operator--();

	hugeint operator++(int);
	hugeint operator--(int);

	hugeint operator+=(hugeint to_add);
	hugeint operator+=(unsigned int to_add);
	hugeint operator+(hugeint to_add);
	hugeint operator+(unsigned int to_add);

	hugeint operator-=(hugeint to_sub);
	hugeint operator-=(unsigned int to_sub);
	hugeint operator-(hugeint to_sub);
	hugeint operator-(unsigned int to_sub);

	hugeint operator*(hugeint to_mult);
	hugeint operator*(unsigned int to_mult);
	hugeint operator*=(hugeint to_mult);
	hugeint operator*=(unsigned int to_mult);

	hugeint operator/(hugeint to_div);
	hugeint operator/(unsigned int to_div);
	hugeint operator/=(hugeint to_div);
	hugeint operator/=(unsigned int to_div);

	hugeint operator%(hugeint to_div);
	hugeint operator%(unsigned int to_div);
	hugeint operator%=(hugeint to_div);
	hugeint operator%=(unsigned int to_div);
};
