#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <algorithm>

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

int main()
{
    cout << "=== BigInt Class Test Program ===" << endl
         << endl;
    cout << "NOTE: All functions are currently empty." << endl;
    cout << "Your task is to implement ALL the functions above." << endl;
    cout << "The tests below will work once you implement them correctly." << endl
         << endl;

    // Test 1: Constructors and basic output
    cout << "1. Constructors and output:" << endl;
    BigInt a(12345);                          // Should create BigInt from integer
    BigInt b("-67890");                       // Should create BigInt from string
    BigInt c("0");                            // Should handle zero correctly
    BigInt d = a;                             // Should use copy constructor
    cout << "a (from int): " << a << endl;    // Should print "12345"
    cout << "b (from string): " << b << endl; // Should print "-67890"
    cout << "c (zero): " << c << endl;        // Should print "0"
    cout << "d (copy of a): " << d << endl
         << endl; // Should print "12345"

    // Test 2: Arithmetic operations
    cout << "2. Arithmetic operations:" << endl;
    cout << "a + b = " << a + b << endl; // Should calculate 12345 + (-67890)
    cout << "a - b = " << a - b << endl; // Should calculate 12345 - (-67890)
    cout << "a * b = " << a * b << endl; // Should calculate 12345 * (-67890)
    cout << "b / a = " << b / a << endl; // Should calculate (-67890) / 12345
    cout << "a % 100 = " << a % BigInt(100) << endl
         << endl; // Should calculate 12345 % 100

    // Test 3: Relational operators
    cout << "3. Relational operators:" << endl;
    cout << "a == d: " << (a == d) << endl; // Should be true (12345 == 12345)
    cout << "a != b: " << (a != b) << endl; // Should be true (12345 != -67890)
    cout << "a < b: " << (a < b) << endl;   // Should be false (12345 < -67890)
    cout << "a > b: " << (a > b) << endl;   // Should be true (12345 > -67890)
    cout << "c == 0: " << (c == BigInt(0)) << endl
         << endl; // Should be true (0 == 0)

    // Test 4: Unary operators and increments
    cout << "4. Unary operators and increments:" << endl;
    cout << "-a: " << -a << endl;   // Should print "-12345"
    cout << "++a: " << ++a << endl; // Should increment and print "12346"
    cout << "a--: " << a-- << endl; // Should print "12346" then decrement
    cout << "a after decrement: " << a << endl
         << endl; // Should print "12345"

    // Test 5: Large number operations
    cout << "5. Large number operations:" << endl;
    BigInt num1("12345678901234567890");
    BigInt num2("98765432109876543210");
    cout << "Very large addition: " << num1 + num2 << endl;
    cout << "Very large multiplication: " << num1 * num2 << endl
         << endl;

    // Test 6: Edge cases and error handling
    cout << "6. Edge cases:" << endl;
    BigInt zero(0);
    BigInt one(1);
    try
    {
        BigInt result = one / zero; // Should throw division by zero error
        cout << "Division by zero succeeded (unexpected)" << endl;
    }
    catch (const runtime_error &e)
    {
        cout << "Division by zero correctly threw error: " << e.what() << endl;
    }
    cout << "Multiplication by zero: " << one * zero << endl;              // Should be "0"
    cout << "Negative multiplication: " << BigInt(-5) * BigInt(3) << endl; // Should be "-15"
    cout << "Negative division: " << BigInt(-10) / BigInt(3) << endl;      // Should be "-3"
    cout << "Negative modulus: " << BigInt(-10) % BigInt(3) << endl;       // Should be "-1"

    //////////////Extra AI Generated Test Cases
    cout << "=== EXTRA TEST CASES ===" << endl;

    // 1. Zero edge cases
    cout << "0 + 0 = " << (BigInt(0) + BigInt(0)) << endl;         // 0
    cout << "0 - 5 = " << (BigInt(0) - BigInt(5)) << endl;         // -5
    cout << "0 * 12345 = " << (BigInt(0) * BigInt(12345)) << endl; // 0
    cout << "0 / 7 = " << (BigInt(0) / BigInt(7)) << endl;         // 0

    // 2. Carry-over addition
    cout << "999 + 1 = " << (BigInt("999") + BigInt("1")) << endl; // 1000

    // 3. Subtraction leading to negative
    cout << "5 - 10 = " << (BigInt(5) - BigInt(10)) << endl; // -5

    // 4. Division tests
    cout << "100 / 3 = " << (BigInt("100") / BigInt("3")) << endl; // 33
    cout << "100 % 3 = " << (BigInt("100") % BigInt("3")) << endl; // 1

    // 5. Negative divisions
    cout << "-10 / 3 = " << (BigInt(-10) / BigInt(3)) << endl; // -3
    cout << "-10 % 3 = " << (BigInt(-10) % BigInt(3)) << endl; // -1
    cout << "10 / -3 = " << (BigInt(10) / BigInt(-3)) << endl; // -3
    cout << "10 % -3 = " << (BigInt(10) % BigInt(-3)) << endl; // 1

    // 6. Very large number tests
    BigInt large1("999999999999999999999999999");
    BigInt large2("1");
    cout << "Large + 1 = " << (large1 + large2) << endl;    // 1000000000000000000000000000
    cout << "Large * 2 = " << (large1 * BigInt(2)) << endl; // 1999999999999999999999999998

    // 7. Equality and comparison
    cout << "(123 == 123): " << (BigInt(123) == BigInt(123)) << endl; // true
    cout << "(123 < 456): " << (BigInt(123) < BigInt(456)) << endl;   // true
    cout << "(456 > 123): " << (BigInt(456) > BigInt(123)) << endl;   // true

    cout << "\n=== Comprehensive Tests ===\n";

    // Test 1: Basic arithmetic with various signs
    cout << "1. Sign combinations:\n";
    cout << "5 + 3 = " << (BigInt(5) + BigInt(3)) << endl;       // 8
    cout << "-5 + 3 = " << (BigInt(-5) + BigInt(3)) << endl;     //-2
    cout << "5 + (-3) = " << (BigInt(5) + BigInt(-3)) << endl;   // 2
    cout << "-5 + (-3) = " << (BigInt(-5) + BigInt(-3)) << endl; //-8

    // Test 2: Subtraction edge cases
    cout << "\n2. Subtraction edge cases:\n";
    cout << "5 - 8 = " << (BigInt(5) - BigInt(8)) << endl; //-3
    cout << "0 - 5 = " << (BigInt(0) - BigInt(5)) << endl; //-5
    cout << "5 - 0 = " << (BigInt(5) - BigInt(0)) << endl; // 5
    cout << "0 - 0 = " << (BigInt(0) - BigInt(0)) << endl; // 0

    // Test 3: Multiplication edge cases
    cout << "\n3. Multiplication edge cases:\n";
    cout << "0 * 12345 = " << (BigInt(0) * BigInt(12345)) << endl;   // 0
    cout << "12345 * 0 = " << (BigInt(12345) * BigInt(0)) << endl;   // 0
    cout << "1 * 999999 = " << (BigInt(1) * BigInt(999999)) << endl; // 999999
    cout << "999999 * 1 = " << (BigInt(999999) * BigInt(1)) << endl; // 999999

    // Test 4: Division edge cases
    cout << "\n4. Division edge cases:\n";
    cout << "0 / 5 = " << (BigInt(0) / BigInt(5)) << endl; // 0
    cout << "5 / 1 = " << (BigInt(5) / BigInt(1)) << endl; // 5
    cout << "1 / 2 = " << (BigInt(1) / BigInt(2)) << endl; // 0

    // Test 5: Modulus edge cases
    cout << "\n5. Modulus edge cases:\n";
    cout << "5 % 3 = " << (BigInt(5) % BigInt(3)) << endl; // 2
    cout << "3 % 5 = " << (BigInt(3) % BigInt(5)) << endl; // 3
    cout << "0 % 5 = " << (BigInt(0) % BigInt(5)) << endl; // 0

    // Test 6: Large number operations
    cout << "\n6. Large number precision:\n";
    BigInt huge1("123456789012345678901234567890");
    BigInt huge2("987654321098765432109876543210");
    cout << "Huge1 + Huge2 = " << (huge1 + huge2) << endl; // 1111111110111111111011111111100
    cout << "Huge1 * 2 = " << (huge1 * BigInt(2)) << endl; // 246913578024691357802469135780

    // Test 7: Comparison operators
    cout << "\n7. Comparison operators:\n";
    cout << "5 == 5: " << (BigInt(5) == BigInt(5)) << endl;  // true
    cout << "5 != 3: " << (BigInt(5) != BigInt(3)) << endl;  // true
    cout << "-5 < 3: " << (BigInt(-5) < BigInt(3)) << endl;  // true
    cout << "3 > -5: " << (BigInt(3) > BigInt(-5)) << endl;  // true
    cout << "0 == -0: " << (BigInt(0) == BigInt(0)) << endl; // true

    // Test 8: Increment/Decrement
    cout << "\n8. Increment/Decrement:\n";
    BigInt x(5);
    cout << "x = " << x << ", x++ = " << x++ << ", after: " << x << endl; // x = 5, x++ = 5, after: 6
    x = BigInt(5);
    cout << "x = " << x << ", ++x = " << ++x << ", after: " << x << endl; // x = 5, ++x = 6, after: 6

    // Test 9: String constructor validation
    cout << "\n9. String validation:\n";
    try
    {
        BigInt invalid("abc123");
        cout << "ERROR: Should have thrown exception" << endl;
    }
    catch (const invalid_argument &e)
    {
        cout << "Invalid string correctly caught: " << e.what() << endl;
    }

    // Test 10: Very large division
    cout << "\n10. Large division:\n";
    BigInt dividend("1000000000000000000000000");
    BigInt divisor("1000000000000");
    cout << "Very large division: " << (dividend / divisor) << endl;

    cout << "\n=== Critical Edge Case Tests ===\n";

    // Test negative division and modulus (C++ behavior)
    cout << "1. Negative division and modulus:\n";
    cout << "-10 / 3 = " << (BigInt(-10) / BigInt(3)) << " (expected: -3)\n";
    cout << "-10 % 3 = " << (BigInt(-10) % BigInt(3)) << " (expected: -1)\n";

    // Test very large numbers with different signs
    cout << "\n2. Very large numbers with different signs:\n";
    BigInt huge_pos("999999999999999999999");
    BigInt huge_neg("-1000000000000000000000");
    cout << "999999999999999999999 + (-1000000000000000000000) = "
         << (huge_pos + huge_neg) << " (expected: -1)\n";

    // Test multiplication with different signs
    cout << "\n3. Multiplication with different signs:\n";
    BigInt num3("123456789");
    BigInt num4("-987654321");
    cout << "123456789 * (-987654321) = " << (num3 * num4)
         << " (expected: -121932631112635269)\n";

    // Test division by 1 and -1
    cout << "\n4. Division by 1 and -1:\n";
    BigInt very_large("12345678901234567890");
    cout << "12345678901234567890 / 1 = " << (very_large / BigInt(1))
         << " (expected: 12345678901234567890)\n";
    cout << "12345678901234567890 / (-1) = " << (very_large / BigInt(-1))
         << " (expected: -12345678901234567890)\n";

    // Test modulus with negative divisor
    cout << "\n5. Modulus with negative divisor:\n";
    cout << "10 % (-3) = " << (BigInt(10) % BigInt(-3)) << " (expected: 1)\n";
    cout << "-10 % 3 = " << (BigInt(-10) % BigInt(3)) << " (expected: -1)\n";
    cout << "-10 % (-3) = " << (BigInt(-10) % BigInt(-3)) << " (expected: -1)\n";
    cout << "10 % 3 = " << (BigInt(10) % BigInt(3)) << " (expected: 1)\n";

    // Test edge case: INT64_MIN
    cout << "\n6. INT64_MIN edge cases:\n";
    BigInt min_int64(INT64_MIN);
    cout << "INT64_MIN = " << min_int64 << " (expected: -9223372036854775808)\n";
    cout << "INT64_MIN + 1 = " << (min_int64 + BigInt(1))
         << " (expected: -9223372036854775807)\n";
    cout << "INT64_MIN - 1 = " << (min_int64 - BigInt(1))
         << " (expected: -9223372036854775809)\n";

    // Test division and modulus with INT64_MIN
    cout << "\n7. INT64_MIN division and modulus:\n";
    cout << "INT64_MIN / (-1) = " << (min_int64 / BigInt(-1))
         << " (expected: 9223372036854775808 - but this may overflow 64-bit)\n";
    cout << "INT64_MIN % 2 = " << (min_int64 % BigInt(2)) << " (expected: 0)\n";

    // Test operations with zero
    cout << "\n8. Operations with zero:\n";
    cout << "0 / (-5) = " << (BigInt(0) / BigInt(-5)) << " (expected: 0)\n";
    cout << "0 % (-5) = " << (BigInt(0) % BigInt(-5)) << " (expected: 0)\n";
    cout << "(-5) / 1 = " << (BigInt(-5) / BigInt(1)) << " (expected: -5)\n";
    cout << "(-5) % 1 = " << (BigInt(-5) % BigInt(1)) << " (expected: 0)\n";

    // Test very small numbers
    cout << "\n9. Very small number operations:\n";
    cout << "1 / 100000000000000000000 = " << (BigInt(1) / BigInt("100000000000000000000"))
         << " (expected: 0)\n";
    cout << "1 % 100000000000000000000 = " << (BigInt(1) % BigInt("100000000000000000000"))
         << " (expected: 1)\n";

    cout << "\n=== String Validation Tests ===\n";

    // Test valid strings
    try
    {
        BigInt valid1("123");
        cout << "Valid string '123': " << valid1 << endl;
    }
    catch (const exception &e)
    {
        cout << "ERROR: Valid string failed: " << e.what() << endl;
    }

    try
    {
        BigInt valid2("-456");
        cout << "Valid string '-456': " << valid2 << endl;
    }
    catch (const exception &e)
    {
        cout << " ERROR: Valid string failed: " << e.what() << endl;
    }

    try
    {
        BigInt valid3("+789");
        cout << "Valid string '+789': " << valid3 << endl;
    }
    catch (const exception &e)
    {
        cout << "ERROR: Valid string failed: " << e.what() << endl;
    }

    try
    {
        BigInt valid4("0000123");
        cout << "Valid string '0000123': " << valid4 << " (leading zeros removed)" << endl;
    }
    catch (const exception &e)
    {
        cout << "ERROR: Valid string failed: " << e.what() << endl;
    }

    // Test invalid strings - should throw exceptions
    vector<string> invalid_strings = {
        "abc123",
        "12a34",
        "12.34",
        "12-34",
        "12+34",
        " 123 ",
        "",
        "-",
        "+",
        "++123",
        "--456",
        "-+789",
        "+-123",
        "123abc",
        "0x123",
        "1e10"};

    for (const string &invalid_str : invalid_strings)
    {
        try
        {
            BigInt invalid(invalid_str);
            cout << " ERROR: Invalid string '" << invalid_str << "' should have thrown exception, but got: " << invalid << endl;
        }
        catch (const invalid_argument &e)
        {
            cout << " Invalid string '" << invalid_str << "' correctly caught: " << e.what() << endl;
        }
        catch (const exception &e)
        {
            cout << "ERROR: Unexpected exception for '" << invalid_str << "': " << e.what() << endl;
        }
    }

    // Test edge case: string with only zeros
    try
    {
        BigInt zeros("00000");
        cout << " String '00000' becomes: " << zeros << " (should be 0)" << endl;
    }
    catch (const exception &e)
    {
        cout << "ERROR: Zeros string failed: " << e.what() << endl;
    }

    // Test edge case: negative zero
    try
    {
        BigInt neg_zero("-0");
        cout << " String '-0' becomes: " << neg_zero << " (should be 0, not negative)" << endl;
    }
    catch (const exception &e)
    {
        cout << "ERROR: Negative zero failed: " << e.what() << endl;
    }

    return 0;
}
