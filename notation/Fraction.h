#pragma once

#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

using namespace std;

class Fraction {
public:
    Fraction();

    Fraction(int a, int b);

    Fraction(const Fraction &other);

    bool operator==(const Fraction &other) const;

    Fraction operator+(const Fraction &other) const;

    Fraction operator-(const Fraction &other) const;

    Fraction operator/(const Fraction &other) const;

    Fraction operator*(const Fraction &other) const;

    Fraction operator*(int other) const;

    Fraction operator%(const Fraction &other) const;

    pair<int, Fraction> count(const Fraction &single) const;

    vector<Fraction> split(const Fraction &single) const;

    // todo: make everything const reference...
    bool operator<(const Fraction &other) const;

    bool operator<=(const Fraction &other) const;

    operator bool() const;

    operator int() const;

    operator double() const;

    friend ostream &operator<<(ostream &os, const Fraction &fraction);

private:
    static void equalize_denominators(Fraction &first, Fraction &second);

    static void simplify(Fraction &fraction);

    pair<int, int> m_value;
};
