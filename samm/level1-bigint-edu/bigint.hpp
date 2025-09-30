#ifndef BIGINT_HPP
# define BIGINT_HPP
# include <algorithm>
# include <iostream>
# include <sstream>
# include <string>

class bigint
{
  private:
	std::string _data; // Stores the digits of the number as a string

	// Converts a size_t number to string
	std::string to_string(size_t number)
	{
		std::ostringstream oss;
		oss << number;
		return (oss.str()); // Recordar el oss.str()!!!!
	}

	// Pads a number with leading zeros to match the length of another number
	std::string n_padding(std::string numberToPad,
		std::string numberToCompare) const
	{
		while (numberToPad.length() < numberToCompare.length())
			numberToPad = "0" + numberToPad;
		return (numberToPad);
	}

	// Performs addition of two string numbers
	std::string sum(std::string bi1, std::string bi2) const
	{
		std::string result;
		int carry = 0;
		std::string num1 = n_padding(bi1, bi2);
		std::string num2 = n_padding(bi2, bi1);

		for (int i = num1.length() - 1; i >= 0; i--)
		{
			int sum_digit = (num1[i] - '0') + (num2[i] - '0') + carry;
			carry = sum_digit / 10;
			result = static_cast<char>(sum_digit % 10 + '0') + result;
		}
		if (carry)
			result = '1' + result;
		// Remove leading zeros
		while (result.length() > 1 && result[0] == '0')
			result.erase(0, 1);

		return (result);
	}

	// Shifts digits to the left (multiplies by 10^shift)
	std::string shift_left(std::string numberToShift, size_t shift) const
	{
		if (numberToShift == "0")
			return ("0");

		while (shift > 0)
		{
			numberToShift += "0";
			shift--;
		}
		return (numberToShift);
	}

	// Shifts digits to the right (divides by 10^shift)
	std::string shift_right(std::string numberToShift, size_t shift) const
	{
		if (numberToShift == "0")
			return ("0");
		if (shift >= numberToShift.length())
			return ("0");
		std::string result = numberToShift.substr(0, numberToShift.length() - shift);
		if (result.empty())
			return ("0");
		while (result.length() > 1 && result[0] == '0')
			result.erase(0, 1);
		return (result);
	}

	// Converts a bigint to size_t for shift operations
	size_t bigint_to_size_t(const bigint& b) const
	{
		size_t result = 0;
		const std::string &str = b._data;

		for (size_t i = 0; i < str.length(); i++) //is a atoi for size_t
		{
			result = result * 10 + (str[i] - '0');
		}
		return (result);
	}

  public:
	// Default constructor - initializes to "0"
	bigint(void) : _data("0")
	{
	}

	// Constructor from size_t
	bigint(size_t numberToCast)
	{
		_data = to_string(numberToCast);
	}

	// Copy constructor
	bigint(const bigint &other) : _data(other._data)
	{
	}

	// Constructor from string (useful for large numbers)
	bigint(const std::string &str) : _data(str)
	{
		// Remove leading zeros
		while (_data.length() > 1 && _data[0] == '0')
			_data.erase(0, 1);
	}

	// Comparison operators - using padding approach
	// For comparison, we pad both numbers to same length then compare lexicographically
	bool operator<(const bigint &other) const
	{
		std::string num1 = n_padding(_data, other._data);
		std::string num2 = n_padding(other._data, _data);
		return (num1 < num2);
	}

	// Less than or equal - uses padding comparison logic
	bool operator<=(const bigint &other) const
	{
		std::string num1 = n_padding(_data, other._data);
		std::string num2 = n_padding(other._data, _data);
		return (num1 <= num2);
	}

	// Greater than - uses padding comparison logic
	bool operator>(const bigint &other) const
	{
		std::string num1 = n_padding(_data, other._data);
		std::string num2 = n_padding(other._data, _data);
		return (num1 > num2);
	}

	// Greater than or equal - uses padding comparison logic
	bool operator>=(const bigint &other) const
	{
		std::string num1 = n_padding(_data, other._data);
		std::string num2 = n_padding(other._data, _data);
		return (num1 >= num2);
	}

	// Equality comparison - uses padding comparison logic
	bool operator==(const bigint &other) const
	{
		std::string num1 = n_padding(_data, other._data);
		std::string num2 = n_padding(other._data, _data);
		return (num1 == num2);
	}

	// Inequality comparison - uses padding comparison logic
	bool operator!=(const bigint &other) const
	{
		std::string num1 = n_padding(_data, other._data);
		std::string num2 = n_padding(other._data, _data);
		return (num1 != num2);
	}

	// Addition operator
	bigint operator+(const bigint &other) const
	{
		bigint result;
		result._data = sum(_data, other._data);
		return result;
	}

	// Addition assignment operator
	bigint &operator+=(const bigint &other)
	{
		_data = sum(_data, other._data);
		return *this;
	}

	// Pre-increment operator
	bigint &operator++()
	{
		*this += bigint(1);
		return *this;
	}

	// Post-increment operator
	bigint operator++(int)
	{
		bigint temp(*this);
		*this += bigint(1);
		return temp;
	}

	// Left shift operators with size_t (digit shift left)
	bigint operator<<(size_t shift) const
	{
		bigint result;
		result._data = shift_left(_data, shift);
		return result;
	}

	// Right shift with size_t (digit shift right)
	bigint operator>>(size_t shift) const
	{
		bigint result;
		result._data = shift_right(_data, shift);
		return result;
	}

	// Left shift assignment with size_t
	bigint &operator<<=(size_t shift)
	{
		_data = shift_left(_data, shift);
		return *this;
	}

	// Right shift assignment with size_t
	bigint &operator>>=(size_t shift)
	{
		_data = shift_right(_data, shift);
		return *this;
	}

	// MISSING OPERATORS: shift with bigint
	// Left shift with bigint parameter
	bigint operator<<(const bigint &shift) const
	{
		size_t shift_val = bigint_to_size_t(shift);
		return *this << shift_val;
	}

	// Right shift with bigint parameter
	bigint operator>>(const bigint &shift) const
	{
		size_t shift_val = bigint_to_size_t(shift);
		return *this >> shift_val;
	}

	// Left shift assignment with bigint parameter
	bigint &operator<<=(const bigint &shift)
	{
		size_t shift_val = bigint_to_size_t(shift);
		return *this <<= shift_val;
	}

	// Right shift assignment with bigint parameter
	bigint &operator>>=(const bigint &shift)
	{
		size_t shift_val = bigint_to_size_t(shift);
		return *this >>= shift_val;
	}

	// Extra: subtraction
	// I understood there's a subtraction result, *bigint(c) - bigint(c)
    // So, we created a "dummy" answer and push it. ;)

/* 	bigint &operator-(const bigint &other)
	{
		bigint result;
		return result;
	}
 */
	// Stream output operator for printing
	friend std::ostream& operator<<(std::ostream &os, const bigint &b)
	{
		return os << b._data;
	}
};
#endif
