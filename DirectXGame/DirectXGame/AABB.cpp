#include "AABB.h"


//bool AABB::IsCollision(AABB aabb1, AABB aabb2) { 
//	// 当たってるか当たってないか判断するフラグ
//	bool ATFlag = false;
//
//	if (aabb1.max.y > aabb2.min.y && aabb1.min.y < aabb2.max.y) 
//	{
//
//		if (aabb1.max.x > aabb2.min.x && aabb1.min.x < aabb2.max.x)
//		{
//			ATFlag = true;
//		}
//	}
//	else 
//	{
//		ATFlag = false;
//	}
//
//	return ATFlag;
//
//
//bool ATFlag = false;
//
// bool IsCollision(AABB aabb1, AABB aabb2) { 
//	 // 当たってるか当たってないか判断するフラグ
//	 if (aabb1.max.y > aabb2.min.y && aabb1.min.y < aabb2.max.y) {
//
//		 if (aabb1.max.x > aabb2.min.x && aabb1.min.x < aabb2.max.x) {
//			 ATFlag = true;
//		 }
//	 }
//	 else {
//	 ATFlag = false;
//	 }
//
//	 return ATFlag;
// }
