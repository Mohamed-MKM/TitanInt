#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <limits>

using namespace std;

class BigInt;
BigInt operator+(BigInt lhs, const BigInt &rhs);
BigInt operator-(BigInt lhs, const BigInt &rhs);
BigInt operator*(BigInt lhs, const BigInt &rhs);
BigInt operator/(BigInt lhs, const BigInt &rhs);
BigInt operator%(BigInt lhs, const BigInt &rhs);

class BigInt
{
    string number;   // Stores the number as a string
    bool isNegative; // True if number is negative

public:
    // Moved function (removeLeadingZeros,compareMagnitude) to public for declaration and external access
    // Remove unnecessary leading zeros from the number string
    void removeLeadingZeros()
    {
        int i = 0;
        while (i < number.length() && number[i] == '0')
        {
            ++i;
        }
        number.erase(0, i);
        if (number.empty())
        {
            number = "0";
            isNegative = false;
        }
    }

    // Compare absolute values of two BigInts (ignore signs)
    // Returns: 1 if |this| > |other|, 0 if equal, -1 if |this| < |other|
    int compareMagnitude(const BigInt &other) const
    {
        if (number.size() > other.number.size())
        {
            return 1;
        }
        else if (number.size() < other.number.size())
        {
            return -1;
        }
        else
        {
            for (int i = 0; i < number.size(); i++)
            {
                if (number[i] > other.number[i])
                    return 1;
                else if (number[i] < other.number[i])
                    return -1;
            }
            return 0;
        }
    }

    // Default constructor - initialize to zero
    BigInt()
    {
        number = "0";
        isNegative = false;
    }

    // Constructor from 64-bit integer
    BigInt(int64_t value)
    {
        if (value > 0)
        {
            isNegative = false;
            number = to_string(value);
        }
        else if (value < 0)
        {
            isNegative = true;
            if (value == INT64_MIN) // cause |MIN| - |MAX| = 1 , so I will cause overflow
            {
                number = "9223372036854775808";
            }
            else
            {
                number = to_string(-value);
            }
        }
        else
        {
            isNegative = false;
            number = "0";
        }

        removeLeadingZeros();
    }

    // Constructor from string representation
    BigInt(const string &str)
    {
        if (str.empty())
        {
            throw invalid_argument("Invalid empty string for BigInt");
        }
        string s = str;
        if (s[0] == '-')
        {
            isNegative = true;
            s.erase(0, 1);
        }
        else if (s[0] == '+')
        {
            isNegative = false;
            s.erase(0, 1);
        }
        else
        {
            isNegative = false;
        }
        if (s.empty())
        {
            throw invalid_argument("Invalid input string: " + str + " (only sign character)");
        }

        for (char c : s)
        {
            if (c < '0' || c > '9')
            {
                throw invalid_argument("Invalid input string: " + str + " (contains non-digit characters)");
            }
        }

        number = s;
        removeLeadingZeros();
        if (number == "0")
        {
            isNegative = false;
        }
    }

    // Copy constructor
    BigInt(const BigInt &other)
    {
        number = other.number;
        isNegative = other.isNegative;
    }

    // Destructor
    ~BigInt()
    {
        // no destructor nedeed
    }

    // Assignment operator
    BigInt &operator=(const BigInt &other)
    {
        if (this != &other)
        {
            number = other.number;
            isNegative = other.isNegative;
        }
        return *this;
    }

    // Unary negation operator (-x)
    BigInt operator-() const
    {
        BigInt result(*this);
        if (result.number != "0")
        {
            result.isNegative = !isNegative;
        }
        else
        {
            result.isNegative = false;
        }
        return result;
    }

    // Unary plus operator (+x)
    BigInt operator+() const
    {
        return *this; // return the copy
    }

    // Addition assignment operator (x += y)
    BigInt &operator+=(const BigInt &other)
    {
        BigInt temp = *this;
        *this = temp + other;
        return *this;
    }

    // Subtraction assignment operator (x -= y)
    BigInt &operator-=(const BigInt &other)
    {
        BigInt temp = *this;
        *this = temp - other;
        return *this;
    }

    // Multiplication assignment operator (x *= y)
    BigInt &operator*=(const BigInt &other)
    {
        int n = number.length(), m = other.number.length();
        if (n == 0 || m == 0)
        {
            number = "0";
            isNegative = false;
            return *this;
        }

        vector<int> result(n + m, 0);

        for (int i = n - 1; i >= 0; i--)
        {
            for (int j = m - 1; j >= 0; j--)
            {
                int mul = (number[i] - '0') * (other.number[j] - '0');
                int sum = result[i + j + 1] + mul;

                result[i + j + 1] = sum % 10;
                result[i + j] += sum / 10;
            }
        }

        string str = "";
        for (int num : result)
        {
            if (!(str.empty() && num == 0))
            {
                str += to_string(num);
            }
        }

        number = str.empty() ? "0" : str;
        isNegative = (isNegative != other.isNegative) && (number != "0");
        removeLeadingZeros();
        return *this;
    }

    // Division assignment operator (x /= y)
    BigInt &operator/=(const BigInt &other)
    {
        *this = *this / other;
        return *this;
    }

    // Modulus assignment operator (x %= y)
    BigInt &operator%=(const BigInt &other)
    {
        *this = *this % other;
        return *this;
    }

    // Pre-increment operator (++x)
    BigInt &operator++()
    {
        *this += BigInt(1);
        return *this;
    }

    // Post-increment operator (x++)
    BigInt operator++(int)
    {
        BigInt temp = *this;
        *this += BigInt(1);
        return temp;
    }

    // Pre-decrement operator (--x)
    BigInt &operator--()
    {
        // TODO: Implement this operator
        *this -= BigInt(1);
        return *this;
    }

    // Post-decrement operator (x--)
    BigInt operator--(int)
    {
        BigInt temp(*this);
        *this -= BigInt(1);
        return temp;
    }

    // Convert BigInt to string representation
    string toString() const
    {
        if (isNegative && number != "0")
        {
            return "-" + number; // return the number with the sign if negative
        }

        return number; // return the number if zero or positive
    }

    // Output stream operator (for printing)
    friend ostream &operator<<(ostream &os, const BigInt &num)
    {
        os << num.toString();
        return os;
    }

    // Input stream operator (for reading from input)
    friend istream &operator>>(istream &is, BigInt &num)
    {

        string str;
        is >> str;
        num = BigInt(str);
        return is;
    }

    // Friend declarations for comparison operators
    friend bool operator==(const BigInt &lhs, const BigInt &rhs);
    friend bool operator<(const BigInt &lhs, const BigInt &rhs);

    // using getters for decleraition number and isNegative in binary opertators
    string getNumber() const
    {
        return number;
    }
    bool getIsNegative() const
    {
        return isNegative;
    }
    void setNumber(const string &num)
    {
        number = num;
    }
    void setIsNegative(bool neg)
    {
        isNegative = neg;
    }
};

// Binary addition operator (x + y)

BigInt operator+(BigInt lhs, const BigInt &rhs)
{
    if (lhs.getIsNegative() == rhs.getIsNegative())
    {
        string a = lhs.getNumber(), b = rhs.getNumber();
        if (a.size() < b.size())
            swap(a, b);
        int carry = 0;
        int n = a.size(), m = b.size();
        string result = "";
        for (int i = 0; i < n; i++)
        {
            int da = a[n - 1 - i] - '0';
            int db = (i < m) ? (b[m - 1 - i] - '0') : 0;
            int sum = da + db + carry;
            carry = sum / 10;
            result.push_back((sum % 10) + '0');
        }
        if (carry)
            result.push_back(carry + '0');
        reverse(result.begin(), result.end());
        BigInt res;
        res.setNumber(result);
        res.setIsNegative(lhs.getIsNegative());
        return res;
    }
    else
    {
        return lhs - (-rhs);
    }
}

// Binary subtraction operator (x - y)

BigInt operator-(BigInt lhs, const BigInt &rhs)
{
    if (lhs.getIsNegative() != rhs.getIsNegative())
    {
        return lhs + (-rhs);
    }
    int cmp = lhs.compareMagnitude(rhs);
    if (cmp == 0)
        return BigInt(0);

    bool resultNegative = false;
    string a = lhs.getNumber(), b = rhs.getNumber();
    if (cmp < 0)
    {
        swap(a, b);
        resultNegative = true;
    }
    int n = a.size(), m = b.size();
    string result = "";
    int borrow = 0;
    for (int i = 0; i < n; i++)
    {
        int da = a[n - 1 - i] - '0' - borrow;
        int db = (i < m) ? (b[m - 1 - i] - '0') : 0;
        if (da < db)
        {
            da += 10;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }
        result.push_back((da - db) + '0');
    }
    while (result.size() > 1 && result.back() == '0')
        result.pop_back();
    reverse(result.begin(), result.end());

    BigInt res;
    res.setNumber(result);
    res.setIsNegative(lhs.getIsNegative() ? !resultNegative : resultNegative);
    if (res.getNumber() == "0")
        res.setIsNegative(false);
    return res;
}

// Binary multiplication operator (x * y)

BigInt operator*(BigInt lhs, const BigInt &rhs)
{
    string a = lhs.getNumber(), b = rhs.getNumber();
    int n = a.size(), m = b.size();
    vector<int> result(n + m, 0);
    for (int i = n - 1; i >= 0; i--)
    {
        for (int j = m - 1; j >= 0; j--)
        {
            int mul = (a[i] - '0') * (b[j] - '0');
            int sum = mul + result[i + j + 1];
            result[i + j + 1] = sum % 10;
            result[i + j] += sum / 10;
        }
    }
    string resStr;
    bool leading = true;
    for (int num : result)
    {
        if (leading && num == 0)
            continue;
        leading = false;
        resStr.push_back(num + '0');
    }
    if (resStr.empty())
        resStr = "0";
    BigInt res;
    res.setNumber(resStr);
    res.setIsNegative(lhs.getIsNegative() != rhs.getIsNegative() && resStr != "0");
    return res;
}

// Binary division operator (x / y)
BigInt operator/(BigInt lhs, const BigInt &rhs)
{
    if (rhs.getNumber() == "0")
    {
        throw runtime_error("Division by zero");
    }

    if (lhs.compareMagnitude(rhs) < 0)
    {
        return BigInt(0);
    }

    BigInt dividend = lhs;
    BigInt divisor = rhs;
    dividend.setIsNegative(false);
    divisor.setIsNegative(false);

    string quotientStr;
    BigInt current(0);

    for (size_t i = 0; i < dividend.getNumber().size(); i++)
    {

        current = current * BigInt(10);
        int digit = dividend.getNumber()[i] - '0';
        current = current + BigInt(digit);

        int count = 0;
        while (current.compareMagnitude(divisor) >= 0)
        {
            current = current - divisor;
            count++;
        }
        quotientStr += to_string(count);
    }

    BigInt result(quotientStr);
    result.removeLeadingZeros();
    if (result.getNumber() == "0")
    {
        result.setIsNegative(false);
    }
    else
    {
        result.setIsNegative(lhs.getIsNegative() != rhs.getIsNegative());
    }

    return result;
}

// Binary modulus operator (x % y)
BigInt operator%(BigInt lhs, const BigInt &rhs)
{
    if (rhs.getNumber() == "0")
    {
        throw runtime_error("modulus by zero");
    }
    if (lhs.compareMagnitude(rhs) < 0)
    {
        return lhs;
    }
    BigInt result = lhs - (lhs / rhs) * rhs;
    if (result.getNumber() == "0")
    {
        result.setIsNegative(false);
    }
    else
    {
        result.setIsNegative(lhs.getIsNegative());
    }
    return result;
}

// Equality comparison operator (x == y)
bool operator==(const BigInt &lhs, const BigInt &rhs)
{
    return ((lhs.getIsNegative() == rhs.getIsNegative()) && (lhs.getNumber() == rhs.getNumber()));
}
// Inequality comparison operator (x != y)
bool operator!=(const BigInt &lhs, const BigInt &rhs)
{
    return !(lhs == rhs);
}

// Less-than comparison operator (x < y)
bool operator<(const BigInt &lhs, const BigInt &rhs)
{
    if (lhs.getIsNegative() && !rhs.getIsNegative())
    {
        return true;
    }
    if (!lhs.getIsNegative() && rhs.getIsNegative())
    {
        return false;
    }

    int magnitudeComparison = lhs.compareMagnitude(rhs);

    if (!lhs.getIsNegative())
    {

        return magnitudeComparison < 0;
    }
    else
    {
        return magnitudeComparison > 0;
    }

    return false;
}

// Less-than-or-equal comparison operator (x <= y)
bool operator<=(const BigInt &lhs, const BigInt &rhs)
{
    return (lhs < rhs) || (lhs == rhs);
}

// Greater-than comparison operator (x > y)
bool operator>(const BigInt &lhs, const BigInt &rhs)
{
    return !(lhs <= rhs);
}

// Greater-than-or-equal comparison operator (x >= y)
bool operator>=(const BigInt &lhs, const BigInt &rhs)
{
    return (lhs > rhs) || (lhs == rhs);
}

// ==================== TEST FUNCTIONS ====================

void runBasicTests()
{
    cout << "\n=== BASIC CONSTRUCTOR AND OUTPUT TESTS ===" << endl;

    BigInt a(12345);
    BigInt b("-67890");
    BigInt c("0");
    BigInt d = a;

    cout << "a (from int): " << a << endl;
    cout << "b (from string): " << b << endl;
    cout << "c (zero): " << c << endl;
    cout << "d (copy of a): " << d << endl;
}

void runArithmeticTests()
{
    cout << "\n=== ARITHMETIC OPERATIONS TESTS ===" << endl;

    BigInt a(12345);
    BigInt b("-67890");

    cout << "a = " << a << ", b = " << b << endl;
    cout << "a + b = " << a + b << endl;
    cout << "a - b = " << a - b << endl;
    cout << "a * b = " << a * b << endl;
    cout << "b / a = " << b / a << endl;
    cout << "a % 100 = " << a % BigInt(100) << endl;

    cout << "\nLarge number operations:" << endl;
    BigInt num1("12345678901234567890");
    BigInt num2("98765432109876543210");
    cout << "Large addition: " << num1 + num2 << endl;
    cout << "Large multiplication: " << num1 * num2 << endl;
}

void runComparisonTests()
{
    cout << "\n=== COMPARISON OPERATORS TESTS ===" << endl;

    BigInt a(12345);
    BigInt b("-67890");
    BigInt c(12345);
    BigInt zero(0);

    cout << "a = " << a << ", b = " << b << ", c = " << c << ", zero = " << zero << endl;
    cout << "a == c: " << (a == c) << " (expected: 1)" << endl;
    cout << "a != b: " << (a != b) << " (expected: 1)" << endl;
    cout << "a < b: " << (a < b) << " (expected: 0)" << endl;
    cout << "a > b: " << (a > b) << " (expected: 1)" << endl;
    cout << "a <= c: " << (a <= c) << " (expected: 1)" << endl;
    cout << "a >= c: " << (a >= c) << " (expected: 1)" << endl;
    cout << "zero == 0: " << (zero == BigInt(0)) << " (expected: 1)" << endl;
}

void runUnaryAndIncrementTests()
{
    cout << "\n=== UNARY OPERATORS AND INCREMENT/DECREMENT TESTS ===" << endl;

    BigInt a(12345);
    cout << "Original a: " << a << endl;
    cout << "-a: " << -a << endl;
    cout << "+a: " << +a << endl;

    cout << "\nIncrement/Decrement tests:" << endl;
    cout << "++a: " << ++a << " (a is now " << a << ")" << endl;

    a = BigInt(12345);
    cout << "a++: " << a++ << " (a is now " << a << ")" << endl;

    a = BigInt(12345);
    cout << "--a: " << --a << " (a is now " << a << ")" << endl;

    a = BigInt(12345);
    cout << "a--: " << a-- << " (a is now " << a << ")" << endl;
}

void runNegativeNumberTests()
{
    cout << "\n=== NEGATIVE NUMBER OPERATIONS TESTS ===" << endl;

    cout << "Addition with negatives:" << endl;
    cout << "-5 + -10 = " << (BigInt(-5) + BigInt(-10)) << " (expected: -15)" << endl;
    cout << "-5 + 10 = " << (BigInt(-5) + BigInt(10)) << " (expected: 5)" << endl;
    cout << "5 + -10 = " << (BigInt(5) + BigInt(-10)) << " (expected: -5)" << endl;

    cout << "\nSubtraction with negatives:" << endl;
    cout << "5 - (-10) = " << (BigInt(5) - BigInt(-10)) << " (expected: 15)" << endl;
    cout << "-5 - (-10) = " << (BigInt(-5) - BigInt(-10)) << " (expected: 5)" << endl;
    cout << "-5 - 10 = " << (BigInt(-5) - BigInt(10)) << " (expected: -15)" << endl;

    cout << "\nMultiplication with negatives:" << endl;
    cout << "-5 * 10 = " << (BigInt(-5) * BigInt(10)) << " (expected: -50)" << endl;
    cout << "-5 * -10 = " << (BigInt(-5) * BigInt(-10)) << " (expected: 50)" << endl;
    cout << "5 * -10 = " << (BigInt(5) * BigInt(-10)) << " (expected: -50)" << endl;

    cout << "\nDivision with negatives:" << endl;
    cout << "-10 / 2 = " << (BigInt(-10) / BigInt(2)) << " (expected: -5)" << endl;
    cout << "-10 / -2 = " << (BigInt(-10) / BigInt(-2)) << " (expected: 5)" << endl;
    cout << "10 / -2 = " << (BigInt(10) / BigInt(-2)) << " (expected: -5)" << endl;

    cout << "\nModulus with negatives:" << endl;
    cout << "10 % (-3) = " << (BigInt(10) % BigInt(-3)) << " (expected: 1)" << endl;
    cout << "(-10) % (-3) = " << (BigInt(-10) % BigInt(-3)) << " (expected: -1)" << endl;
    cout << "(-10) % 3 = " << (BigInt(-10) % BigInt(3)) << " (expected: -1)" << endl;
}

void runEdgeCaseTests()
{
    cout << "\n=== EDGE CASE TESTS ===" << endl;

    cout << "Zero operations:" << endl;
    cout << "0 + 0 = " << (BigInt(0) + BigInt(0)) << endl;
    cout << "0 - 5 = " << (BigInt(0) - BigInt(5)) << endl;
    cout << "0 * 12345 = " << (BigInt(0) * BigInt(12345)) << endl;
    cout << "0 / 7 = " << (BigInt(0) / BigInt(7)) << endl;

    cout << "\nCarry-over tests:" << endl;
    cout << "999 + 1 = " << (BigInt("999") + BigInt("1")) << endl;
    cout << "1000 - 1 = " << (BigInt("1000") - BigInt("1")) << endl;

    cout << "\nINT64_MIN edge cases:" << endl;
    BigInt min_int64(INT64_MIN);
    cout << "INT64_MIN = " << min_int64 << endl;
    cout << "INT64_MIN + 1 = " << (min_int64 + BigInt(1)) << endl;
    cout << "INT64_MIN - 1 = " << (min_int64 - BigInt(1)) << endl;

    cout << "\nDivision by zero tests:" << endl;
    try
    {
        BigInt result = BigInt(1) / BigInt(0);
        cout << "ERROR: Division by zero should have thrown exception" << endl;
    }
    catch (const runtime_error &e)
    {
        cout << "Division by zero correctly caught: " << e.what() << endl;
    }

    try
    {
        BigInt result = BigInt(1) % BigInt(0);
        cout << "ERROR: Modulus by zero should have thrown exception" << endl;
    }
    catch (const runtime_error &e)
    {
        cout << "Modulus by zero correctly caught: " << e.what() << endl;
    }

    cout << "\nVery large numbers:" << endl;
    BigInt huge1("999999999999999999999999999");
    BigInt huge2("1");
    cout << "999999999999999999999999999 + 1 = " << (huge1 + huge2) << endl;
    cout << "999999999999999999999999999 * 2 = " << (huge1 * BigInt(2)) << endl;
}

void runStringValidationTests()
{
    cout << "\n=== STRING VALIDATION TESTS ===" << endl;

    cout << "Valid strings:" << endl;
    try
    {
        BigInt valid1("123");
        cout << "Valid string '123': " << valid1 << endl;

        BigInt valid2("-456");
        cout << "Valid string '-456': " << valid2 << endl;

        BigInt valid3("+789");
        cout << "Valid string '+789': " << valid3 << endl;

        BigInt valid4("0000123");
        cout << "Valid string '0000123': " << valid4 << " (leading zeros removed)" << endl;

        BigInt valid5("-0");
        cout << "Valid string '-0': " << valid5 << " (should be 0, not negative)" << endl;
    }
    catch (const exception &e)
    {
        cout << "ERROR: Valid string failed: " << e.what() << endl;
    }

    cout << "\nInvalid strings (should throw exceptions):" << endl;
    vector<string> invalid_strings = {
        "abc123", "12a34", "12.34", "12-34", "12+34",
        " 123 ", "", "-", "+", "++123", "--456",
        "-+789", "+-123", "123abc", "0x123", "1e10"};

    for (const string &invalid_str : invalid_strings)
    {
        try
        {
            BigInt invalid(invalid_str);
            cout << "ERROR: Invalid string '" << invalid_str << "' should have thrown exception, but got: " << invalid << endl;
        }
        catch (const invalid_argument &e)
        {
            cout << "Invalid string '" << invalid_str << "' correctly caught" << endl;
        }
        catch (const exception &e)
        {
            cout << "ERROR: Unexpected exception for '" << invalid_str << "': " << e.what() << endl;
        }
    }
}

void runAllTests()
{
    cout << "\n=== RUNNING ALL COMPREHENSIVE TESTS ===" << endl;

    runBasicTests();
    runArithmeticTests();
    runComparisonTests();
    runUnaryAndIncrementTests();
    runNegativeNumberTests();
    runEdgeCaseTests();
    runStringValidationTests();

    cout << "\n All tests completed!" << endl;
    cout << "\nPress Enter to return to main menu...";
    cin.ignore();
    cin.get();
}

void performArithmeticOperation()
{
    cout << "\n=== ARITHMETIC OPERATIONS CALCULATOR ===" << endl;
    cout << "Enter two BigInt numbers to perform all arithmetic operations:" << endl;

    string num1_str, num2_str;

    cout << "Enter first number: ";
    cin >> num1_str;
    cout << "Enter second number: ";
    cin >> num2_str;

    try
    {
        BigInt num1(num1_str);
        BigInt num2(num2_str);

        cout << "\n=== ARITHMETIC OPERATIONS RESULTS ===" << endl;
        cout << "Numbers: " << num1 << " and " << num2 << endl
             << endl;

        cout << "ADDITION:" << endl;
        cout << num1 << " + " << num2 << " = " << (num1 + num2) << endl;

        cout << "\nSUBTRACTION:" << endl;
        cout << num1 << " - " << num2 << " = " << (num1 - num2) << endl;
        cout << num2 << " - " << num1 << " = " << (num2 - num1) << endl;

        cout << "\nMULTIPLICATION:" << endl;
        cout << num1 << " * " << num2 << " = " << (num1 * num2) << endl;

        cout << "\nDIVISION:" << endl;
        if (num2 != BigInt(0))
        {
            cout << num1 << " / " << num2 << " = " << (num1 / num2) << endl;
        }
        else
        {
            cout << num1 << " / " << num2 << " = Cannot divide by zero" << endl;
        }

        if (num1 != BigInt(0))
        {
            cout << num2 << " / " << num1 << " = " << (num2 / num1) << endl;
        }
        else
        {
            cout << num2 << " / " << num1 << " = Cannot divide by zero" << endl;
        }

        cout << "\nMODULUS:" << endl;
        if (num2 != BigInt(0))
        {
            cout << num1 << " % " << num2 << " = " << (num1 % num2) << endl;
        }
        else
        {
            cout << num1 << " % " << num2 << " = Cannot modulus by zero" << endl;
        }

        if (num1 != BigInt(0))
        {
            cout << num2 << " % " << num1 << " = " << (num2 % num1) << endl;
        }
        else
        {
            cout << num2 << " % " << num1 << " = Cannot modulus by zero" << endl;
        }

        cout << "\nCOMPOUND OPERATIONS:" << endl;
        cout << "(" << num1 << " + " << num2 << ") * 2 = " << ((num1 + num2) * BigInt(2)) << endl;
        cout << "(" << num1 << " * " << num2 << ") + 10 = " << ((num1 * num2) + BigInt(10)) << endl;
    }
    catch (const runtime_error &e)
    {
        cout << "Math Error: " << e.what() << endl;
    }
    catch (const invalid_argument &e)
    {
        cout << "Input Error: " << e.what() << endl;
    }
    catch (const exception &e)
    {
        cout << "Error: " << e.what() << endl;
    }

    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void performComparisonOperation()
{
    cout << "\n=== COMPARISON OPERATIONS CALCULATOR ===" << endl;
    cout << "Enter two BigInt numbers to compare:" << endl;

    string num1_str, num2_str;
    cout << "Enter first number: ";
    cin >> num1_str;
    cout << "Enter second number: ";
    cin >> num2_str;

    try
    {
        BigInt num1(num1_str);
        BigInt num2(num2_str);

        cout << "\nComparison results:" << endl;
        cout << num1 << " == " << num2 << " : " << (num1 == num2) << endl;
        cout << num1 << " != " << num2 << " : " << (num1 != num2) << endl;
        cout << num1 << " < " << num2 << " : " << (num1 < num2) << endl;
        cout << num1 << " > " << num2 << " : " << (num1 > num2) << endl;
        cout << num1 << " <= " << num2 << " : " << (num1 <= num2) << endl;
        cout << num1 << " >= " << num2 << " : " << (num1 >= num2) << endl;
    }
    catch (const exception &e)
    {
        cout << "Error: " << e.what() << endl;
    }

    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

void performUnaryOperation()
{
    cout << "\n=== UNARY OPERATIONS CALCULATOR ===" << endl;
    cout << "Enter a BigInt number:" << endl;

    string num_str;
    cout << "Enter number: ";
    cin >> num_str;

    try
    {
        BigInt num(num_str);
        BigInt original = num;

        cout << "\nUnary operations on " << original << ":" << endl;
        cout << "Unary plus (+num): " << (+num) << endl;
        cout << "Unary minus (-num): " << (-num) << endl;

        cout << "\nIncrement/Decrement operations:" << endl;
        num = original; // Reset
        cout << "Pre-increment (++num): " << (++num) << " (num is now " << num << ")" << endl;

        num = original; // Reset
        cout << "Post-increment (num++): " << (num++) << " (num is now " << num << ")" << endl;

        num = original; // Reset
        cout << "Pre-decrement (--num): " << (--num) << " (num is now " << num << ")" << endl;

        num = original; // Reset
        cout << "Post-decrement (num--): " << (num--) << " (num is now " << num << ")" << endl;
    }
    catch (const exception &e)
    {
        cout << "Error: " << e.what() << endl;
    }

    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

void performAssignmentOperation()
{
    cout << "\n=== ASSIGNMENT OPERATIONS CALCULATOR ===" << endl;
    cout << "Enter two BigInt numbers and test assignment operations:" << endl;

    string num1_str, num2_str;
    cout << "Enter first number: ";
    cin >> num1_str;
    cout << "Enter second number: ";
    cin >> num2_str;

    try
    {
        BigInt num1(num1_str);
        BigInt num2(num2_str);
        BigInt original1 = num1;

        cout << "\nAssignment operations with " << original1 << " and " << num2 << ":" << endl;

        num1 = original1;
        num1 += num2;
        cout << original1 << " += " << num2 << " = " << num1 << endl;

        num1 = original1;
        num1 -= num2;
        cout << original1 << " -= " << num2 << " = " << num1 << endl;

        num1 = original1;
        num1 *= num2;
        cout << original1 << " *= " << num2 << " = " << num1 << endl;

        if (num2 != BigInt(0))
        {
            num1 = original1;
            num1 /= num2;
            cout << original1 << " /= " << num2 << " = " << num1 << endl;

            num1 = original1;
            num1 %= num2;
            cout << original1 << " %= " << num2 << " = " << num1 << endl;
        }
        else
        {
            cout << "Skipping division and modulus operations (division by zero)" << endl;
        }
    }
    catch (const exception &e)
    {
        cout << "Error: " << e.what() << endl;
    }

    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

void displayMenu()
{
    cout << "\n========================================" << endl;
    cout << "    WELCOME TO BIGINT CLASS TESTER" << endl;
    cout << "========================================" << endl;
    cout << "Choose the operation you want to perform:" << endl;
    cout << "1. Run All Test Cases" << endl;
    cout << "2. Basic Constructor & Output Tests" << endl;
    cout << "3. Arithmetic Operations Tests" << endl;
    cout << "4. Comparison Operators Tests" << endl;
    cout << "5. Unary & Increment/Decrement Tests" << endl;
    cout << "6. Negative Number Operations Tests" << endl;
    cout << "7. Edge Case Tests" << endl;
    cout << "8. String Validation Tests" << endl;
    cout << "9. Interactive Arithmetic Calculator" << endl;
    cout << "10. Interactive Comparison Calculator" << endl;
    cout << "11. Interactive Unary Operations" << endl;
    cout << "12. Interactive Assignment Operations" << endl;
    cout << "13. Exit" << endl;
    cout << "========================================" << endl;
    cout << "Enter your choice (1-13): ";
}

int main()
{
    int choice;

    cout << "BigInt Class - Menu-Driven Testing Interface" << endl;
    cout << "This program allows you to test all BigInt operations interactively." << endl;

    do
    {
        displayMenu();

        if (!(cin >> choice))
        {
            cout << "\nInvalid input! Please enter a number between 1-13." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
            runAllTests();
            break;
        case 2:
            runBasicTests();
            break;
        case 3:
            runArithmeticTests();
            break;
        case 4:
            runComparisonTests();
            break;
        case 5:
            runUnaryAndIncrementTests();
            break;
        case 6:
            runNegativeNumberTests();
            break;
        case 7:
            runEdgeCaseTests();
            break;
        case 8:
            runStringValidationTests();
            break;
        case 9:
            performArithmeticOperation();
            break;
        case 10:
            performComparisonOperation();
            break;
        case 11:
            performUnaryOperation();
            break;
        case 12:
            performAssignmentOperation();
            break;
        case 13:
            cout << "\nThank you for using BigInt Class Tester!" << endl;
            cout << "Goodbye!" << endl;
            break;
        default:
            cout << "\nInvalid choice! Please enter a number between 1-13." << endl;
            break;
        }

        if (choice != 13)
        {
            cout << "\nPress Enter to return to main menu...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

    } while (choice != 13);
    return 0;
}
