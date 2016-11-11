/*
 This version is licensed as : http://unlicense.org . Please see LICENSE and README.md for more information.
 Enjoy the code :)
 */
#ifndef sweepevent_h
#define sweepevent_h

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
    struct SweepEvent; // forward declaration
    
    template <typename Segment_2>
    struct SegmentComp : public std::binary_function<SweepEvent<Segment_2>*, SweepEvent<Segment_2>*, bool> { // for sorting edges in the sweep line (sl)
        bool operator() (SweepEvent<Segment_2>* le1, SweepEvent<Segment_2>* le2);
    };
    
    template <typename Segment_2>
    struct SweepEvent {
        typedef typename Segment_2::Point_2 Point_2;
        
        SweepEvent () {}
        SweepEvent (bool b, const Point_2& p, SweepEvent* other, PolygonType pt, EdgeType et = NORMAL);
        bool left;              // is point the left endpoint of the edge (point, otherEvent->point)?
        Point_2 point;          // point associated with the event
        SweepEvent* otherEvent; // event associated to the other endpoint of the edge
        PolygonType pol;        // Polygon to which the associated segment belongs to
        EdgeType type;
        //The following fields are only used in "left" events
        /**  Does segment (point, otherEvent->p) represent an inside-outside transition in the polygon for a vertical ray from (p.x, -infinite)? */
        bool inOut;
        bool otherInOut; // inOut transition for the segment from the other polygon preceding this segment in sl
        typename std::set<SweepEvent*, SegmentComp<Segment_2> >::iterator posSL; // Position of the event (line segment) in sl
        SweepEvent* prevInResult; // previous segment in sl belonging to the result of the boolean operation
        bool inResult;
        unsigned int pos;
        bool resultInOut;
        unsigned int contourId;
        // member functions
        /** Is the line segment (point, otherEvent->point) below point p */
        bool below (const Point_2& p) const { return (left) ? signedArea (point, otherEvent->point, p) > 0 :
            signedArea (otherEvent->point, point, p) > 0; }
        /** Is the line segment (point, otherEvent->point) above point p */
        bool above (const Point_2& p) const { return !below (p); }
        /** Is the line segment (point, otherEvent->point) a vertical line segment */
        bool vertical () const { return point.x == otherEvent->point.x; }
        /** Return the line segment associated to the SweepEvent */
        Segment_2 segment () const { return Segment_2 (point, otherEvent->point); }
    };
    
    template <typename Segment_2>
    struct SweepEventComp : public std::binary_function<SweepEvent<Segment_2>, SweepEvent<Segment_2>, bool> { // for sorting sweep events
        // Compare two sweep events
        // Return true means that e1 is placed at the event queue after e2, i.e,, e1 is processed by the algorithm after e2
        bool operator() (const SweepEvent<Segment_2>* e1, const SweepEvent<Segment_2>* e2)
        {
            if (e1->point.x > e2->point.x) // Different x-coordinate
                return true;
            if (e2->point.x > e1->point.x) // Different x-coordinate
                return false;
            if (e1->point.y != e2->point.y) // Different points, but same x-coordinate. The event with lower y-coordinate is processed first
                return e1->point.y > e2->point.y;
            if (e1->left != e2->left) // Same point, but one is a left endpoint and the other a right endpoint. The right endpoint is processed first
                return e1->left;
            // Same point, both events are left endpoints or both are right endpoints.
            if (signedArea (e1->point, e1->otherEvent->point, e2->otherEvent->point) != 0) // not collinear
                return e1->above (e2->otherEvent->point); // the event associate to the bottom segment is processed first
            return e1->pol > e2->pol;
        }
    };
}

#include "sweepevent.inl"

#endif /* sweepevent_h */
