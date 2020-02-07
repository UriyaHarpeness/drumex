#pragma once

#include "../Enums.h"

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

    // use at some other places.
    Fraction(TimeSignature signature);

    Fraction(const Fraction &other);

    bool operator==(const Fraction &other) const;

    bool operator!=(const Fraction &other) const;

    Fraction operator+(const Fraction &other) const;

    void operator+=(const Fraction &other);

    Fraction operator-(const Fraction &other) const;

    void operator-=(const Fraction &other);

    Fraction operator/(const Fraction &other) const;

    Fraction operator*(const Fraction &other) const;

    Fraction operator*(int other) const;

    Fraction operator%(const Fraction &other) const;

    Fraction operator&(const Fraction &other) const;

    bool operator<(const Fraction &other) const;

    bool operator>(const Fraction &other) const;

    bool operator<=(const Fraction &other) const;

    bool operator>=(const Fraction &other) const;

    explicit operator bool() const;

    explicit operator int() const;

    explicit operator double() const;

    [[nodiscard]] inline pair<int, int> get_value() { return m_value; };

    friend ostream &operator<<(ostream &os, const Fraction &fraction);

private:
    static void equalize_denominators(Fraction &first, Fraction &second);

    static void simplify(Fraction &fraction);

    pair<int, int> m_value;
};
