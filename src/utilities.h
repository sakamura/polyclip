/*
 Original file by Francisco Martínez del Río.
 
 This version is licensed as : http://unlicense.org . Please see LICENSE and README.md for more information.
 Enjoy the code :)
 */

// Utility functions

#ifndef UTILITIES_H
#define UTILITIES_H

#include <algorithm>
#include "point_2.h"
#include "segment_2.h"

namespace cbop {
    
    template <typename Segment_2>
    int findIntersection (const Segment_2& seg0, const Segment_2& seg1, typename Segment_2::Point_2& ip0, typename Segment_2::Point_2& ip1);
    
    /** Signed area of the triangle (p0, p1, p2) */
    template <typename Point_2>
    inline float signedArea (const Point_2& p0, const Point_2& p1, const Point_2& p2)
    {
        return (p0.x ()- p2.x ())*(p1.y () - p2.y ()) - (p1.x () - p2.x ()) * (p0.y () - p2.y ());
    }
    
    /** Signed area of the triangle ( (0,0), p1, p2) */
    template <typename Point_2>
    inline float signedArea (const Point_2& p1, const Point_2& p2)
    {
        return -p2.x ()*(p1.y () - p2.y ()) - -p2.y ()*(p1.x () - p2.x ());
    }
    
    /** Sign of triangle (p1, p2, o) */
    template <typename Point_2>
    inline int sign (const Point_2& p1, const Point_2& p2, const Point_2& o)
    {
        typename Point_2::value_type det = (p1.x () - o.x ()) * (p2.y () - o.y ()) - (p2.x () - o.x ()) * (p1.y () - o.y ());
        return (det < 0 ? -1 : (det > 0 ? +1 : 0));
    }
    
    template <typename Segment_2>
    inline bool pointInTriangle (const Segment_2& s, const typename Segment_2::Point_2& o, const typename Segment_2::Point_2& p)
    {
        int x = sign (s.source (), s.target (), p);
        return ((x == sign (s.target (), o, p)) && (x == sign (o, s.source (), p)));
    }
    
} // end of namespace cbop

#include "utilities.inl"

#endif
