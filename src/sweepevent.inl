/*
 This version is licensed as : http://unlicense.org . Please see LICENSE and README.md for more information.
 Enjoy the code :)
 */

#include "sweepevent.h"
#include <algorithm>
#include <cassert>

namespace polyclip
{
    // le1 and le2 are the left events of line segments (le1->point, le1->otherEvent->point) and (le2->point, le2->otherEvent->point)
    template <typename Segment_2, bool inverseDir, typename FinalType>
    bool SweepEvent<Segment_2, inverseDir, FinalType>::SegmentComp::operator() (const SweepEvent* le1, const SweepEvent* le2)
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
            Comp comp;
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
        Comp comp;
        return comp (le1, le2);
    }
    
    template <typename Segment_2, bool inverseDir, typename FinalType>
    bool SweepEvent<Segment_2, inverseDir, FinalType>::Comp::operator() (const SweepEvent* e1, const SweepEvent* e2)
    {
        if (e1->point.x > e2->point.x) // Different x-coordinate
            return !inverseDir;
        if (e2->point.x > e1->point.x) // Different x-coordinate
            return inverseDir;
        if (e1->point.y != e2->point.y) // Different points, but same x-coordinate. The event with lower y-coordinate is processed first
            return inverseDir ^ (e1->point.y > e2->point.y);
        if (e1->left != e2->left) // Same point, but one is a left endpoint and the other a right endpoint. The right endpoint is processed first
            return inverseDir ^ e1->left;
        // Same point, both events are left endpoints or both are right endpoints.
        if (inverseDir)
            return e1->below(e2->otherEvent->point);
        if (signedArea (e1->point, e1->otherEvent->point, e2->otherEvent->point) != 0) // not collinear
            return e1->above (e2->otherEvent->point); // the event associate to the bottom segment is processed first
        return e1->pol > e2->pol;
    }
}
