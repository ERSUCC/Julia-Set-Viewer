#include "Complex.h"

Complex::Complex(double a, double b) : a(a), b(b) {};

double Complex::magnitude()
{
	return sqrt(sqrmagnitude());
}

double Complex::sqrmagnitude()
{
	return std::pow(a, 2) + std::pow(b, 2);
}

void Complex::add(Complex other)
{
	a += other.a;
	b += other.b;
}

void Complex::add(double a, double b)
{
	this->a += a;
	this->b += b;
}

void Complex::mul(Complex other)
{
	double na = a * other.a - b * other.b;
	double nb = a * other.b + b * other.a;

	a = na;
	b = nb;
}

void Complex::mul(double a, double b)
{
	double na = this->a * a - this->b * b;
	double nb = this->a * b + this->b * a;

	this->a = na;
	this->b = nb;
}

void Complex::pow(int n)
{
	for (int i = 0; i < n - 1; i++)
	{
		mul(*this);
	}
}
