/*
 This version is licensed as : http://unlicense.org . Please see LICENSE and README.md for more information.
 Enjoy the code :)
 */

#include "sweepeventholes.h"
#include <algorithm>
#include <cassert>

namespace polyclip
{
    template <typename Segment_2>
    bool SegmentCompHoles<Segment_2>::operator() (SweepEventHoles<Segment_2>* le1, SweepEventHoles<Segment_2>* le2) {
        if (le1 == le2)
            return false;
        if (signedArea (le1->point, le1->otherEvent->point, le2->point) != 0 ||
            signedArea (le1->point, le1->otherEvent->point, le2->otherEvent->point) != 0) {
            // Segments are not collinear
            // If they share their left endpoint use the right endpoint to sort
            if (le1->point == le2->point)
                return le1->below (le2->otherEvent->point);
            // Different points
            SweepEventCompHoles<Segment_2> comp;
            if (comp (le1, le2))  // has the segment associated to e1 been sorted in evp before the segment associated to e2?
                return le1->below (le2->point);
            // The segment associated to e2 has been sorted in evp before the segment associated to e1
            return le2->above (le1->point);
        }
        // Segments are collinear. Just a consistent criterion is used
        if (le1->point == le2->point)
            return le1 < le2;
        SweepEventCompHoles<Segment_2> comp;
        return comp (le1, le2);
    }
}
