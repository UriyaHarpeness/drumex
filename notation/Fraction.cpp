#include <tgmath.h>
#include "Fraction.h"


Fraction::Fraction() : m_value({0, 0}) {
}

Fraction::Fraction(int a, int b) : m_value({a, b}) {
    simplify(*this);
}

Fraction::Fraction(const Fraction &other) = default;

Fraction Fraction::operator+(const Fraction &other) const {
    Fraction result(*this);
    Fraction second(other);
    if (!(m_value.second && second.m_value.second)) {
        return result;
    }
    equalize_denominators(result, second);
    result.m_value.first += second.m_value.first;
    simplify(result);
    return result;
}

Fraction Fraction::operator-(const Fraction &other) const {
    Fraction result(*this);
    Fraction second(other);
    if (!(m_value.second && second.m_value.second)) {
        return result;
    }
    equalize_denominators(result, second);
    result.m_value.first -= second.m_value.first;
    simplify(result);
    return result;
}

Fraction Fraction::operator/(const Fraction &other) const {
    Fraction result(*this);
    Fraction second(other);
    if (!(m_value.second && second.m_value.second)) {
        return result;
    }
    result.m_value.first *= second.m_value.second;
    result.m_value.second *= second.m_value.first;
    simplify(result);
    return result;
}

Fraction Fraction::operator%(const Fraction &other) const {
    Fraction result(*this);
    Fraction second(other);
    if (!(m_value.second && second.m_value.second)) {
        return result;
    }
    equalize_denominators(result, second);
    result.m_value.first %= second.m_value.first;
    simplify(result);
    return result;
}

pair<int, Fraction> Fraction::count(const Fraction &single) const {
    Fraction temp(*this / single);
    return make_pair(temp.m_value.first / temp.m_value.second, *this % single);
}

vector<Fraction> Fraction::split(const Fraction &single) const {
    pair<int, Fraction> counts = count(single);
    vector<Fraction> fractions(counts.first, single);
    if (counts.second) {
        fractions.push_back(counts.second);
    }
    return fractions;
}

bool Fraction::operator<(const Fraction &other) const {
    Fraction a(*this);
    Fraction b(other);
    equalize_denominators(a, b);
    return a.m_value.first < b.m_value.first;
}

bool Fraction::operator<=(const Fraction &other) const {
    Fraction a(*this);
    Fraction b(other);
    equalize_denominators(a, b);
    return a.m_value.first <= b.m_value.first;
}

Fraction::operator bool() const {
    return m_value.first;
}

Fraction::operator int() const {
    return (int) log2((double) m_value.first / m_value.second);
}

Fraction::operator double() const {
    return (double) m_value.first / m_value.second;
}

ostream &operator<<(ostream &os, const Fraction &fraction) {
    os << fraction.m_value.first << '/' << fraction.m_value.second;
    return os;
}

void Fraction::equalize_denominators(Fraction &first, Fraction &second) {
    first.m_value.first *= second.m_value.second;
    second.m_value.first *= first.m_value.second;
    first.m_value.second *= second.m_value.second;
    second.m_value.second = first.m_value.first;
}

void Fraction::simplify(Fraction &fraction) {
    int fraction_gcd = gcd(fraction.m_value.first, fraction.m_value.second);
    fraction.m_value.first /= fraction_gcd;
    fraction.m_value.second /= fraction_gcd;
}