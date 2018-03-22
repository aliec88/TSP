#pragma once
#include "stdafx.h"
#include <limits>
inline wstring dtos(double d)
{
	wostringstream s;	
	//int prec=numeric_limits<double>::digits10;
	s.precision(40);
	s<<d;
	return s.str();
}
inline wstring itos(int i)
{
	wostringstream s;
	s<<i;
	return s.str();
}
inline int RandInt(int a,int b)
{
	return rand()%(b-a)+a;
}
inline double RandDouble()
{
	return rand()/(double)RAND_MAX;
}
