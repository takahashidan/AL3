#include "Vector3.h"

Vector3& Vector3::operator+=(const Vector3& a) 
{
	this->x += a.x;
	this->y += a.y;
	this->z += a.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& i) {
	this->x -= i.x;
	this->y -= i.y;
	this->z -= i.z;
	return *this;
}


Vector3 operator+(const Vector3& a, const Vector3& b) 
{
	return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}
