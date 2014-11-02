#include "Utility.h"

VECTOR2 utilityNS::rotateVector(VECTOR2 v, float d){
	VECTOR2 result;
	result.x = v.x*cos(d) - v.y*sin(d);
	result.y = v.x*sin(d) + v.y*cos(d);
	return result;
}

float utilityNS::rand01(){return ((rand()%100)/100.0);}