/*
 This version is licensed as : http://unlicense.org . Please see LICENSE and README.md for more information.
 Enjoy the code :)
 */
#ifndef sweepeventholes_h
#define sweepeventholes_h

#include <vector>
#include <list>
#include <set>
#include <queue>
#include <functional>

#include "polygon.h"
#include "polyclipdefs.h"

namespace polyclip
{
    template <typename Segment_2>
    struct SweepEventHoles;
    
    template <typename Segment_2>
    struct SegmentCompHoles : public std::binary_function<SweepEventHoles<Segment_2>*, SweepEventHoles<Segment_2>*, bool> {
        bool operator() (SweepEventHoles<Segment_2>* e1, SweepEventHoles<Segment_2>* e2);
    };
    
    template <typename Segment_2>
    struct SweepEventHoles {
        typedef typename Segment_2::Point_2 Point_2;
        
        Point_2 point;  // point associated with the event
        bool left;      // is the point the left endpoint of the segment (p, other->p)?
        int pol;        // Polygon to which the associated segment belongs to
        SweepEventHoles* otherEvent; // Event associated to the other endpoint of the segment
        /**  Does the segment (p, other->p) represent an inside-outside transition in the polygon for a vertical ray from (p.x, -infinite) that crosses the segment? */
        bool inOut;
        typename std::set<SweepEventHoles*, SegmentCompHoles<Segment_2> >::iterator posSL; // Only used in "left" events. Position of the event (segment) in SL
        
        /** Class constructor */
        SweepEventHoles (const Point_2& pp, bool b, int apl) : point (pp), left (b), pol (apl) {}
        /** Return the segment associated to the SweepEvent */
        Segment_2 segment () const { return Segment_2 (point, otherEvent->point); }
        /** Is the line segment (point, otherEvent->point) below point p */
        bool below (const Point_2& p) const { return (left) ? signedArea (point, otherEvent->point, p) > 0 :
            signedArea (otherEvent->point, point, p) > 0; }
        /** Is the line segment (point, otherEvent->point) above point p */
        bool above (const Point_2& p) const { return !below (p); }
    };
    
    template <typename Segment_2>
    struct SweepEventCompHoles : public std::binary_function<SweepEventHoles<Segment_2>*, SweepEventHoles<Segment_2>*, bool> {
        bool operator() (SweepEventHoles<Segment_2>* e1, SweepEventHoles<Segment_2>* e2) {
            if (e1->point.x < e2->point.x) // Different x coordinate
                return true;
            if (e2->point.x < e1->point.x) // Different x coordinate
                return false;
            if (e1->point != e2->point) // Different points, but same x coordinate. The event with lower y coordinate is processed first
                return e1->point.y < e2->point.y;
            if (e1->left != e2->left) // Same point, but one is a left endpoint and the other a right endpoint. The right endpoint is processed first
                return !e1->left;
            // Same point, both events are left endpoints or both are right endpoints. The event associate to the bottom segment is processed first
            return e1->below (e2->otherEvent->point);
        }
    };
}

#include "sweepeventholes.inl"

#endif /* sweepeventholes_h */
