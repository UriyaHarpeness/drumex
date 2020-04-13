#pragma once

#include "../Enums.h"

#include <iostream>
#include <numeric>
#include <utility>
#include <vector>
#include <ctgmath>
#include <cassert>


using namespace std;

class Fraction {
public:
    Fraction();

    Fraction(int a, int b, bool do_simplify = true);

    static Fraction FloatingFraction(int a, double b, bool do_simplify = true);

    Fraction(int a);

    Fraction(const Fraction &other);

    bool operator==(const Fraction &other) const;

    bool operator!=(const Fraction &other) const;

    Fraction operator+(const Fraction &other) const;

    void operator+=(const Fraction &other);

    Fraction operator-(const Fraction &other) const;

    void operator-=(const Fraction &other);

    Fraction operator/(const Fraction &other) const;

    Fraction operator/(const int other) const;

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

    [[nodiscard]] inline pair<int, int> get_value() const { return m_value; };

    friend ostream &operator<<(ostream &os, const Fraction &fraction);

protected:
    static void equalize_denominators(Fraction &first, Fraction &second);

    static void simplify(Fraction &fraction);

    pair<int, int> m_value;
};
