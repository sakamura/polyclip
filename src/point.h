/*
 Original file by Francisco Martínez del Río.
 
 This version is licensed as : http://unlicense.org . Please see LICENSE and README.md for more information.
 Enjoy the code :)
 */

// ------------------------------------------------------------------
// Point Class - A point in the plane
// ------------------------------------------------------------------

#ifndef POINT_H
#define POINT_H

#include <cmath>
#include "bbox.h"

namespace polyclip {
    
    template <typename T>
    struct ValuePrecision
    {
        static const T Precision;
    };
    template<> const double ValuePrecision<double>::Precision = 0.0000001;
    template<> const float ValuePrecision<float>::Precision = 0.0001f;
    
    template <typename T>
    class Point {
    public:
        typedef T value_type;
        
        Point (value_type _x = 0.0, value_type _y = 0.0): x (_x), y (_y) {}

        value_type x, y;
    };
    
    template <typename T>
    inline bool operator== (const Point<T>& p1, const Point<T>& p2) { return (p1.x == p2.x) && (p1.y == p2.y); }
    template <typename T>
    inline bool operator!= (const Point<T>& p1, const Point<T>& p2) { return !(p1 == p2); }
    template <typename Point_2>
    inline bool isClose(const Point_2& a, const Point_2& b)
    {
        typedef typename Point_2::value_type value_type;
        const value_type dx = a.x - b.x;
        const value_type dy = a.y - b.y;
        return sqrt (dx * dx + dy * dy) < ValuePrecision<value_type>::Precision;
    }
    
} // end of namespace polyclip
#endif
