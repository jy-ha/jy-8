/*
 * Vector.h
 *
 * Created: 2016-02-07 오전 12:29:42
 *  Author: Blonix
 */ 


#ifndef VECTOR_H_
#define VECTOR_H_


//======================================================================================//
//=============================  Vector Calculation  ===================================//
//======================================================================================//


struct Vector
{
	double	x;
	double	y;
	double	z;
};

namespace VectorCalc{
	
	double	GetMagnitude(Vector);
	
}





#endif /* VECTOR_H_ */