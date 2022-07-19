#pragma once

#include <iostream>

struct Complex
{
	double a;
	double b;

	Complex(double a, double b);

	double magnitude();
	double sqrmagnitude();
	
	void add(Complex other);
	void add(double a, double b);

	void mul(Complex other);
	void mul(double a, double b);

	void pow(int n);
};

