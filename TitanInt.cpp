#include <iostream>
#include <string>
#include <cstdint>

using namespace std;

class BigInt
{
    string number;   // Stores the number as a string
    bool isNegative; // True if number is negative

public:
    // Moved function (removeLeadingZeros,compareMagnitude) to public for declaration and external access
    // Remove unnecessary leading zeros from the number string
    void removeLeadingZeros()
    {
        // TODO: Implement this function
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
        // TODO: Implement this function
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
        // TODO: Implement this constructor
        number = "0";
        isNegative = false;
    }

    // Constructor from 64-bit integer
    BigInt(int64_t value)
    {
        // TODO: Implement this constructor
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
        // TODO: Implement this constructor
        if (str.empty())
        {
            cout << "Empty" << endl;
            number = "0";
            isNegative = false;
            return;
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
            cout << "Invalid input: " << str << endl;
            number = "0";
            isNegative = false;
            return;
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
        // TODO: Implement this constructor
        number = other.number;
        isNegative = other.isNegative;
    }

    // Destructor
    ~BigInt()
    {
        // TODO: Implement if needed
        // no destructor nedeed
    }

    // Assignment operator
    BigInt &operator=(const BigInt &other)
    {
        // TODO: Implement this operator
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
        // TODO: Implement negation logic
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
        // TODO: Implement this operator
        return *this; // return the copy
    }

    // Addition assignment operator (x += y)
    BigInt &operator+=(const BigInt &other)
    {
        // TODO: Implement this operator
        BigInt temp = *this;
        *this = temp + other;
        return *this;
    }

    // Subtraction assignment operator (x -= y)
    BigInt &operator-=(const BigInt &other)
    {
        // TODO: Implement this operator
        BigInt temp = *this;
        *this = temp - other;
        return *this;
    }

    // Multiplication assignment operator (x *= y)
    BigInt &operator*=(const BigInt &other)
    {
        // TODO: Implement this operator
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
        // TODO: Implement this operator
        return *this;
    }

    // Modulus assignment operator (x %= y)
    BigInt &operator%=(const BigInt &other)
    {
        // TODO: Implement this operator
        return *this;
    }

    // Pre-increment operator (++x)
    BigInt &operator++()
    {
        // TODO: Implement this operator
        return *this;
    }

    // Post-increment operator (x++)
    BigInt operator++(int)
    {
        BigInt temp = *this;
        // TODO: Implement this operator
        *this += BigInt(1);
        return temp;
    }

    // Pre-decrement operator (--x)
    BigInt &operator--()
    {
        // TODO: Implement this operator
        return *this;
    }

    // Post-decrement operator (x--)
    BigInt operator--(int)
    {
        // TODO: Implement this operator
        BigInt temp(*this);
        *this -= BigInt(1);
        return temp;
    }

    // Convert BigInt to string representation
    string toString() const
    {
        // TODO: Implement this function
        if (isNegative && number != "0")
        {
            return "-" + number; // return the number with the sign if negative
        }

        return number; // return the number if zero or positive
    }

    // Output stream operator (for printing)
    friend ostream &operator<<(ostream &os, const BigInt &num)
    {
        // TODO: Implement this operator
        os << num.toString();
        return os;
    }

    // Input stream operator (for reading from input)
    friend istream &operator>>(istream &is, BigInt &num)
    {
        // TODO: Implement this operator
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
        string resultStr;
        int carry = 0;
        int i = lhs.getNumber().size() - 1;
        int j = rhs.getNumber().size() - 1;

        while (i >= 0 || j >= 0 || carry)
        {
            int digit1 = i >= 0 ? lhs.getNumber()[i--] - '0' : 0;
            int digit2 = j >= 0 ? rhs.getNumber()[j--] - '0' : 0;
            int sum = digit1 + digit2 + carry;
            carry = sum / 10;
            resultStr.push_back((sum % 10) + '0'); // apply push_back for efficiency and faster O(n)
        }
        // reverse the result
        reverse(resultStr.begin(), resultStr.end());

        BigInt result;
        result.setNumber(resultStr);
        result.setIsNegative(lhs.getIsNegative());
        result.removeLeadingZeros();
        return result;
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

    if (lhs.compareMagnitude(rhs) < 0)
    {
        BigInt result = rhs - lhs;
        result.setIsNegative(!lhs.getIsNegative());
        return result;
    }

    string resultStr;
    int borrow = 0;
    int i = lhs.getNumber().size() - 1;
    int j = rhs.getNumber().size() - 1;

    while (i >= 0)
    {
        int digit1 = lhs.getNumber()[i--] - '0';
        int digit2 = j >= 0 ? rhs.getNumber()[j--] - '0' : 0;
        int diff = digit1 - digit2 - borrow;
        if (diff < 0)
        {
            diff += 10;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }
        // resultStr.insert(resultStr.begin(), diff + '0'); chang to push_back for efficienty
        resultStr.push_back(diff + '0');
    }
    reverse(resultStr.begin(), resultStr.end());
    BigInt result;
    result.setNumber(resultStr);
    result.setIsNegative(lhs.getIsNegative());
    result.removeLeadingZeros();
    return result;
}

// Binary multiplication operator (x * y)

BigInt operator*(BigInt lhs, const BigInt &rhs)
{
    // to handel multi by zero
    if (lhs.getNumber() == "0" || rhs.getNumber() == "0")
    {
        return BigInt(0);
    }
    int n = lhs.getNumber().size(), m = rhs.getNumber().size();
    // vector<int> result(n + m, 0); if not using vector library
    int *result = new int[n + m]();

    for (int i = n - 1; i >= 0; --i)
    {
        for (int j = m - 1; j >= 0; --j)
        {
            int mul = (lhs.getNumber()[i] - '0') * (rhs.getNumber()[j] - '0');
            int sum = result[i + j + 1] + mul;
            result[i + j + 1] = sum % 10;
            result[i + j] += sum / 10;
        }
    }

    string resultStr; // store the result from array to string
    bool leadingZero = true;
    for (int i = 0; i < n + m; ++i)
    {
        if (leadingZero && result[i] == 0)
        {
            continue;
        }
        leadingZero = false;
        result.push_back(result[i] + '0')
    }

    if (resultStr.empty())
        resultStr = "0";
    delete[] result; // for avoiding memory leak
    BigInt finalResult;
    finalResult.setNumber(resultStr);
    finalResult.setIsNegative(lhs.getIsNegative() != rhs.getIsNegative());
    return finalResult;
}

// Binary division operator (x / y)
BigInt operator/(BigInt lhs, const BigInt &rhs)
{
    if (rhs.getNumber() == "0")
    {
        throw runtime_error("Division by zero");
    }
    if (lhs.getNumber() == "0")
    {
        return BigInt(0);
    }
    BigInt dividend = lhs;
    BigInt divisor = rhs;
    dividend.setIsNegative(false);
    divisor.setIsNegative(false);

    string quotientStr;
    BigInt current;
    for (char digit : dividend.getNumber())
    {
        string temp = current.getNumber();
        temp.push_back(digit);
        current.setNumber(temp);
        int count = 0;
        while (current.compareMagnitude(divisor) >= 0)
        {
            current = current - divisor;
            ++count;
        }

        quotientStr.push_back(count + '0');
    }

    /**quotientStr.removeLeadingZeros();
    quotientStr.getIsNegative () = lhs.getIsNegative() != rhs.getIsNegative();
    */
    BigInt quotient(quotientStr);
    BigInt.removeLeadingZeros();
    quotient.setIsNegative(lhs.getIsNegative() != rhs.getIsNegative());
    return quotient;
}

// Binary modulus operator (x % y)
BigInt operator%(BigInt lhs, const BigInt &rhs)
{
    // return lhs - (lhs / rhs) * rhs; change it for correctness mathimatical modulus formula
    if (rhs.getNumber() == "0")
    {
        throw runtime_error("modulus by zero");
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
    return lhs.getIsNegative() == rhs.getIsNegative() && lhs.getNumber() == rhs.getNumber();
}
// Inequality comparison operator (x != y)
bool operator!=(const BigInt &lhs, const BigInt &rhs)
{
    // TODO: Implement this operator
    return !(lhs == rhs);
}

// Less-than comparison operator (x < y)
bool operator<(const BigInt &lhs, const BigInt &rhs)
{
    // TODO: Implement this operator
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
    // TODO: Implement this operator
    return (lhs < rhs) || (lhs == rhs);
}

// Greater-than comparison operator (x > y)
bool operator>(const BigInt &lhs, const BigInt &rhs)
{
    // TODO: Implement this operator
    return !(lhs <= rhs);
}

// Greater-than-or-equal comparison operator (x >= y)
bool operator>=(const BigInt &lhs, const BigInt &rhs)
{
    // TODO: Implement this operator
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

    /*
    // Test 1: Constructors and basic output
    cout << "1. Constructors and output:" << endl;
    BigInt a(12345);              // Should create BigInt from integer
    BigInt b("-67890");           // Should create BigInt from string
    BigInt c("0");                // Should handle zero correctly
    BigInt d = a;                 // Should use copy constructor
    cout << "a (from int): " << a << endl;        // Should print "12345"
    cout << "b (from string): " << b << endl;     // Should print "-67890"
    cout << "c (zero): " << c << endl;            // Should print "0"
    cout << "d (copy of a): " << d << endl << endl; // Should print "12345"

    // Test 2: Arithmetic operations
    cout << "2. Arithmetic operations:" << endl;
    cout << "a + b = " << a + b << endl;          // Should calculate 12345 + (-67890)
    cout << "a - b = " << a - b << endl;          // Should calculate 12345 - (-67890)
    cout << "a * b = " << a * b << endl;          // Should calculate 12345 * (-67890)
    cout << "b / a = " << b / a << endl;          // Should calculate (-67890) / 12345
    cout << "a % 100 = " << a % BigInt(100) << endl << endl; // Should calculate 12345 % 100

    // Test 3: Relational operators
    cout << "3. Relational operators:" << endl;
    cout << "a == d: " << (a == d) << endl;       // Should be true (12345 == 12345)
    cout << "a != b: " << (a != b) << endl;       // Should be true (12345 != -67890)
    cout << "a < b: " << (a < b) << endl;         // Should be false (12345 < -67890)
    cout << "a > b: " << (a > b) << endl;         // Should be true (12345 > -67890)
    cout << "c == 0: " << (c == BigInt(0)) << endl << endl; // Should be true (0 == 0)

    // Test 4: Unary operators and increments
    cout << "4. Unary operators and increments:" << endl;
    cout << "-a: " << -a << endl;                 // Should print "-12345"
    cout << "++a: " << ++a << endl;               // Should increment and print "12346"
    cout << "a--: " << a-- << endl;               // Should print "12346" then decrement
    cout << "a after decrement: " << a << endl << endl; // Should print "12345"

    // Test 5: Large number operations
    cout << "5. Large number operations:" << endl;
    BigInt num1("12345678901234567890");
    BigInt num2("98765432109876543210");
    cout << "Very large addition: " << num1 + num2 << endl;
    cout << "Very large multiplication: " << num1 * num2 << endl << endl;

    // Test 6: Edge cases and error handling
    cout << "6. Edge cases:" << endl;
    BigInt zero(0);
    BigInt one(1);
    try {
        BigInt result = one / zero;               // Should throw division by zero error
        cout << "Division by zero succeeded (unexpected)" << endl;
    } catch (const runtime_error& e) {
        cout << "Division by zero correctly threw error: " << e.what() << endl;
    }
    cout << "Multiplication by zero: " << one * zero << endl;        // Should be "0"
    cout << "Negative multiplication: " << BigInt(-5) * BigInt(3) << endl;  // Should be "-15"
    cout << "Negative division: " << BigInt(-10) / BigInt(3) << endl;       // Should be "-3"
    cout << "Negative modulus: " << BigInt(-10) % BigInt(3) << endl;        // Should be "-1"
    */

    return 0;
}
