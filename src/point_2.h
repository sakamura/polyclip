/*
 Original file by Francisco Martínez del Río.
 
 This version is licensed as : http://unlicense.org . Please see LICENSE and README.md for more information.
 Enjoy the code :)
 */

// ------------------------------------------------------------------
// Point_2 Class - A point in the plane
// ------------------------------------------------------------------

#ifndef POINT_2_H
#define POINT_2_H

#include <cmath>
#include "bbox_2.h"

namespace cbop {

class Point_2 {
public:
	Point_2 (double x = 0.0, double y = 0.0): _x (x), _y (y) {}
	double x () const { return _x; }
	double y () const { return _y; }
	Bbox_2 bbox () const { return Bbox_2 (_x, _y, _x, _y); }
/** Distance to other point */
	float dist (const Point_2& p) const
	{
		float dx = x () - p.x ();
		float dy = y () - p.y ();
		return sqrt (dx * dx + dy * dy);
	}
private:
	/** coordinates */
	double _x, _y;
};

inline bool operator== (const Point_2& p1, const Point_2& p2) { return (p1.x () == p2.x ()) && (p1.y () == p2.y ()); }
inline bool operator!= (const Point_2& p1, const Point_2& p2) { return !(p1 == p2); }

} // end of namespace cbop
#endif
