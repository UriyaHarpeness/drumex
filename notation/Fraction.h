#pragma once

#include <iostream>
#include <numeric>
#include <utility>
#include <vector>
#include <ctgmath>


using namespace std;

class Fraction {
public:
    Fraction();

    Fraction(int a, int b);

    Fraction(const Fraction &other);

    bool operator==(const Fraction &other) const;

    Fraction operator+(const Fraction &other) const;

    void operator+=(const Fraction &other);

    Fraction operator-(const Fraction &other) const;

    void operator-=(const Fraction &other);

    Fraction operator/(const Fraction &other) const;

    Fraction operator*(const Fraction &other) const;

    Fraction operator*(int other) const;

    Fraction operator%(const Fraction &other) const;

    bool operator<(const Fraction &other) const;

    bool operator>(const Fraction &other) const;

    bool operator<=(const Fraction &other) const;

    bool operator>=(const Fraction &other) const;

    explicit operator bool() const;

    explicit operator int() const;

    explicit operator double() const;

    friend ostream &operator<<(ostream &os, const Fraction &fraction);

private:
    static void equalize_denominators(Fraction &first, Fraction &second);

    static void simplify(Fraction &fraction);

    pair<int, int> m_value;
};
