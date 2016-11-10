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

namespace polyclip {
    
    template <typename T>
    class Point {
    public:
        typedef Bbox<T> Bbox_2;
        typedef T value_type;
        
        Point (value_type x = 0.0, value_type y = 0.0): _x (x), _y (y) {}
        value_type x () const { return _x; }
        value_type y () const { return _y; }
        Bbox_2 bbox () const { return Bbox_2 (_x, _y, _x, _y); }
        /** Distance to other point */
        value_type dist (const Point& p) const
        {
            value_type dx = x () - p.x ();
            value_type dy = y () - p.y ();
            return sqrt (dx * dx + dy * dy);
        }
    private:
        /** coordinates */
        value_type _x, _y;
    };
    
    template <typename T>
    inline bool operator== (const Point<T>& p1, const Point<T>& p2) { return (p1.x () == p2.x ()) && (p1.y () == p2.y ()); }
    template <typename T>
    inline bool operator!= (const Point<T>& p1, const Point<T>& p2) { return !(p1 == p2); }
    
} // end of namespace polyclip
#endif
