#ifndef BIGINT_HPP
# define BIGINT_HPP

# include <iostream>
# include <string>
# include <algorithm>
# include <sstream> // pour remplacer to_string et stoull

class bigint {
private:
	std::string _data;

	// supprime les zéros inutiles
	void trim() {
		while (_data.size() > 1 && _data[0] == '0')
			_data.erase(0, 1);
	}

	// addition de deux strings
	static std::string add_str(const std::string &a, const std::string &b) {
		std::string res;
		int carry = 0;
		int i = a.size() - 1, j = b.size() - 1;
		while (i >= 0 || j >= 0 || carry) {
			int sum = carry;
			if (i >= 0) sum += a[i--] - '0';
			if (j >= 0) sum += b[j--] - '0';
			carry = sum / 10;
			res.push_back((sum % 10) + '0');
		}
		std::reverse(res.begin(), res.end());
		return res;
	}

	// convert string -> size_t (remplace stoull)
	static size_t to_size_t(const std::string &s) {
		std::istringstream iss(s);
		size_t val = 0;
		iss >> val;
		return val;
	}

	// convert size_t -> string (remplace to_string)
	static std::string from_size_t(size_t n) {
		std::ostringstream oss;
		oss << n;
		return oss.str();
	}

public:
	// constructeurs
	bigint() : _data("0") {}
	bigint(size_t n) : _data(from_size_t(n)) {}
	bigint(const std::string &s) : _data(s) { trim(); }
	bigint(const bigint &other) : _data(other._data) {}

	// opérateurs de comparaison
	bool operator<(const bigint &o) const {
		if (_data.size() != o._data.size())
			return _data.size() < o._data.size();
		return _data < o._data;
	}
	bool operator>(const bigint &o) const { return o < *this; }
	bool operator<=(const bigint &o) const { return !(o < *this); }
	bool operator>=(const bigint &o) const { return !(*this < o); }
	bool operator==(const bigint &o) const { return _data == o._data; }
	bool operator!=(const bigint &o) const { return _data != o._data; }

	// addition
	bigint operator+(const bigint &o) const {
		return bigint(add_str(_data, o._data));
	}
	bigint &operator+=(const bigint &o) {
		_data = add_str(_data, o._data);
		return *this;
	}
	bigint &operator++() { return *this += bigint(1); }
	bigint operator++(int) { bigint tmp(*this); ++(*this); return tmp; }

	// digitshift gauche et droite
	bigint operator<<(size_t n) const { return bigint(_data + std::string(n, '0')); }
	bigint operator>>(size_t n) const {
		if (n >= _data.size()) return bigint(0);
		return bigint(_data.substr(0, _data.size() - n));
	}
	bigint &operator<<=(size_t n) { _data += std::string(n, '0'); return *this; }
	bigint &operator>>=(size_t n) {
		if (n >= _data.size()) _data = "0";
		else _data.erase(_data.size() - n);
		trim();
		return *this;
	}

	// digitshift avec bigint
	bigint operator<<(const bigint &o) const { return *this << to_size_t(o._data); }
	bigint operator>>(const bigint &o) const { return *this >> to_size_t(o._data); }
	bigint &operator<<=(const bigint &o) { return *this <<= to_size_t(o._data); }
	bigint &operator>>=(const bigint &o) { return *this >>= to_size_t(o._data); }

	// affichage
	friend std::ostream &operator<<(std::ostream &os, const bigint &b) {
		return os << b._data;
	}
};

#endif
