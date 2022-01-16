/*
 * Vector.cpp
 *
 * Created: 2016-02-07 오전 12:52:42
 *  Author: Blonix
 */ 

#include "../define.h"


double	VectorCalc::GetMagnitude(Vector vector)
{
	return (double)( sqrt( pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2) ) );
}

