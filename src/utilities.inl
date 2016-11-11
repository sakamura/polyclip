/*
 Original file by Francisco Martínez del Río.
 
 This version is licensed as : http://unlicense.org . Please see LICENSE and README.md for more information.
 Enjoy the code :)
 */

#include <algorithm>
#include "utilities.h"

namespace polyclip
{    
    template <typename value_type>
    int findIntersection (value_type u0, value_type u1, value_type v0, value_type v1, value_type w[2])
    {
        if ((u1 < v0) || (u0 > v1))
            return 0;
        if (u1 > v0) {
            if (u0 < v1) {
                w[0] = (u0 < v0) ? v0 : u0;
                w[1] = (u1 > v1) ? v1 : u1;
                return 2;
            } else {
                // u0 == v1
                w[0] = u0;
                return 1;
            }
        } else {
            // u1 == v0
            w[0] = u1;
            return 1;
        }
    }
    
    template <typename Segment_2>
    int findIntersection (const Segment_2& seg0, const Segment_2& seg1, typename Segment_2::Point_2& pi0, typename Segment_2::Point_2& pi1)
    {
        typedef typename Segment_2::Point_2 Point_2;
        typedef typename Segment_2::value_type value_type;
        
        Point_2 p0 = seg0.source ();
        Point_2 d0 (seg0.target ().x - p0.x, seg0.target ().y - p0.y);
        Point_2 p1 = seg1.source ();
        Point_2 d1 (seg1.target ().x - p1.x, seg1.target ().y - p1.y);
        value_type sqrEpsilon = 0.0000001; // it was 0.001 before
        Point_2 E (p1.x - p0.x, p1.y - p0.y);
        value_type kross = d0.x * d1.y - d0.y * d1.x;
        value_type sqrKross = kross * kross;
        value_type sqrLen0 = d0.x * d0.x + d0.y * d0.y;
        value_type sqrLen1 = d1.x * d1.x + d1.y * d1.y;
        
        if (sqrKross > sqrEpsilon * sqrLen0 * sqrLen1) {
            // lines of the segments are not parallel
            value_type s = (E.x * d1.y - E.y * d1.x) / kross;
            if ((s < 0) || (s > 1)) {
                return 0;
            }
            value_type t = (E.x * d0.y - E.y * d0.x) / kross;
            if ((t < 0) || (t > 1)) {
                return 0;
            }
            // intersection of lines is a point an each segment
            pi0 = Point_2 (p0.x + s * d0.x, p0.y + s * d0.y);
            if (isClose(pi0,seg0.source ())) pi0 = seg0.source ();
            if (isClose(pi0,seg0.target ())) pi0 = seg0.target ();
            if (isClose(pi0, seg1.source ())) pi0 = seg1.source ();
            if (isClose(pi0, seg1.target ())) pi0 = seg1.target ();
            return 1;
        }
        
        // lines of the segments are parallel
        value_type sqrLenE = E.x * E.x + E.y * E.y;
        kross = E.x * d0.y - E.y * d0.x;
        sqrKross = kross * kross;
        if (sqrKross > sqrEpsilon * sqrLen0 * sqrLenE) {
            // lines of the segment are different
            return 0;
        }
        
        // Lines of the segments are the same. Need to test for overlap of segments.
        value_type s0 = (d0.x * E.x + d0.y * E.y) / sqrLen0;  // so = Dot (D0, E) * sqrLen0
        value_type s1 = s0 + (d0.x * d1.x + d0.y * d1.y) / sqrLen0;  // s1 = s0 + Dot (D0, D1) * sqrLen0
        value_type smin = std::min (s0, s1);
        value_type smax = std::max (s0, s1);
        value_type w[2];
        int imax = findIntersection<value_type> (0.0, 1.0, smin, smax, w);
        
        if (imax > 0) {
            pi0 = Point_2 (p0.x + w[0] * d0.x, p0.y + w[0] * d0.y);
            if (isClose(pi0, seg0.source ())) pi0 = seg0.source ();
            if (isClose(pi0, seg0.target ())) pi0 = seg0.target ();
            if (isClose(pi0, seg1.source ())) pi0 = seg1.source ();
            if (isClose(pi0, seg1.target ())) pi0 = seg1.target ();
            if (imax > 1)
                pi1 = Point_2 (p0.x + w[1] * d0.x, p0.y + w[1] * d0.y);
        }
        return imax;
    }
    
}
