#include "Fraction.h"

Fraction::Fraction() : m_value({0, 1}) {}

Fraction::Fraction(int a, int b, bool do_simplify) : m_value({a, b}) {
    if (do_simplify) simplify(*this);
}

Fraction Fraction::FloatingFraction(int a, double b, bool do_simplify) {
    if (b != 0) {
        assert(log2(b) == int(log2(b)));
    }
    if (b < 1) {
        return {(int) (a / b), 1, do_simplify};
    }
    return {a, (int) b, do_simplify};
}

Fraction::Fraction(int a) : m_value({a, 1}) {
    simplify(*this);
}

Fraction::Fraction(const Fraction &other) = default;

bool Fraction::operator==(const Fraction &other) const {
    Fraction result(*this);
    Fraction second(other);
    if (!(m_value.second && second.m_value.second)) {
        return static_cast<bool>(result);
    }
    equalize_denominators(result, second);
    return result.m_value.first == second.m_value.first;
}

bool Fraction::operator!=(const Fraction &other) const {
    Fraction result(*this);
    Fraction second(other);
    if (!(m_value.second && second.m_value.second)) {
        return static_cast<bool>(result);
    }
    equalize_denominators(result, second);
    return result.m_value.first != second.m_value.first;
}

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

void Fraction::operator+=(const Fraction &other) {
    Fraction second(other);
    if (!(m_value.second && second.m_value.second)) {
        return;
    }
    equalize_denominators(*this, second);
    m_value.first += second.m_value.first;
    simplify(*this);
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

void Fraction::operator-=(const Fraction &other) {
    Fraction second(other);
    if (!(m_value.second && second.m_value.second)) {
        return;
    }
    equalize_denominators(*this, second);
    m_value.first -= second.m_value.first;
    simplify(*this);
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

Fraction Fraction::operator/(const int other) const {
    return *this / Fraction(other, 1);
}

Fraction Fraction::operator*(const Fraction &other) const {
    Fraction result(*this);
    Fraction second(other);
    if (!(m_value.second && second.m_value.second)) {
        return result;
    }
    result.m_value.first *= second.m_value.first;
    result.m_value.second *= second.m_value.second;
    simplify(result);
    return result;
}

Fraction Fraction::operator*(int other) const {
    Fraction result(*this);
    if (!(m_value.second)) {
        return result;
    }
    result.m_value.first *= other;
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

Fraction Fraction::operator&(const Fraction &other) const {
    Fraction result(*this);
    Fraction second(other);
    if (!(m_value.second && second.m_value.second)) {
        return result;
    }
    equalize_denominators(result, second);
    result.m_value.first = lcm(result.m_value.first, second.m_value.first);
    simplify(result);
    return result;
}

bool Fraction::operator<(const Fraction &other) const {
    Fraction a(*this);
    Fraction b(other);
    equalize_denominators(a, b);
    return a.m_value.first < b.m_value.first;
}

bool Fraction::operator>(const Fraction &other) const {
    Fraction a(*this);
    Fraction b(other);
    equalize_denominators(a, b);
    return a.m_value.first > b.m_value.first;
}

bool Fraction::operator<=(const Fraction &other) const {
    Fraction a(*this);
    Fraction b(other);
    equalize_denominators(a, b);
    return a.m_value.first <= b.m_value.first;
}

bool Fraction::operator>=(const Fraction &other) const {
    Fraction a(*this);
    Fraction b(other);
    equalize_denominators(a, b);
    return a.m_value.first >= b.m_value.first;
}

Fraction::operator bool() const {
    return m_value.first;
}

Fraction::operator int() const {
    return floor(log2((double) m_value.first / m_value.second));
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
    second.m_value.second = first.m_value.second;
}

void Fraction::simplify(Fraction &fraction) {
    int fraction_gcd = gcd(fraction.m_value.first, fraction.m_value.second);
    fraction.m_value.first /= fraction_gcd;
    fraction.m_value.second /= fraction_gcd;
}
