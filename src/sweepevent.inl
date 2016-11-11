/*
 This version is licensed as : http://unlicense.org . Please see LICENSE and README.md for more information.
 Enjoy the code :)
 */

#include "sweepevent.h"
#include <algorithm>
#include <cassert>

namespace polyclip
{
    template <typename Segment_2>
    SweepEvent<Segment_2>::SweepEvent (bool b, const Point_2& p, SweepEvent* other, PolygonType pt, EdgeType et) :
    left (b), point (p), otherEvent (other), pol (pt), type (et), prevInResult (0), inResult (false)
    {
    }
    
    // le1 and le2 are the left events of line segments (le1->point, le1->otherEvent->point) and (le2->point, le2->otherEvent->point)
    template <typename Segment_2>
    bool SegmentComp<Segment_2>::operator() (SweepEvent<Segment_2>* le1, SweepEvent<Segment_2>* le2)
    {
        if (le1 == le2)
            return false;
        if (signedArea (le1->point, le1->otherEvent->point, le2->point) != 0 ||
            signedArea (le1->point, le1->otherEvent->point, le2->otherEvent->point) != 0) {
            // Segments are not collinear
            // If they share their left endpoint use the right endpoint to sort
            if (le1->point == le2->point)
                return le1->below (le2->otherEvent->point);
            // Different left endpoint: use the left endpoint to sort
            if (le1->point.x == le2->point.x)
                return le1->point.y < le2->point.y;
            SweepEventComp<Segment_2> comp;
            if (comp (le1, le2))  // has the line segment associated to e1 been inserted into S after the line segment associated to e2 ?
                return le2->above (le1->point);
            // The line segment associated to e2 has been inserted into S after the line segment associated to e1
            return le1->below (le2->point);
        }
        // Segments are collinear
        if (le1->pol != le2->pol)
            return le1->pol < le2->pol;
        // Just a consistent criterion is used
        if (le1->point == le2->point)
            return le1 < le2;
        SweepEventComp<Segment_2> comp;
        return comp (le1, le2);
    }
}
