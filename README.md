⚡ TitanInt

A Big Integer Class in C++

Handle numbers way beyond the limits of int and long long 🚀

✨ What is TitanInt?

TitanInt is a custom C++ class for arbitrary precision integers.
It lets you work with numbers that have hundreds or thousands of digits, making it perfect for:

🔐 Cryptography

💰 Finance

🔬 Scientific Computing

🔑 Features

Store integers of any size (limited only by memory).

Standard math operations: +, -, *, /, %.

Comparison operators: <, <=, >, >=, ==, !=.

Unary negation (-x).

Stream-friendly (<<, >>).

Handles negatives, zeros, and errors (like division by zero).

💻 Example
#include "TitanInt.h"
#include <iostream>
using namespace std;

int main() {
    TitanInt a("12345678901234567890");
    TitanInt b("98765432109876543210");

    cout << "Sum: " << (a + b) << endl;
    cout << "Product: " << (a * b) << endl;
}


👉 Output:

Sum: 111111111011111111100
Product: 1219326311370217952237463801111263526900

⚡ TitanInt → Because integers should have no limits.
