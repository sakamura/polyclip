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
    template <typename Segment_2, bool inverseDir, typename FinalType>  // inverseDir defines if we are looking at normal sweep (false) or holes that must be calculated the other direction (true)
    struct SweepEvent {
        struct SegmentComp : public std::binary_function<SweepEvent*, SweepEvent*, bool> { // for sorting edges in the sweep line (sl)
            bool operator() (const SweepEvent* le1, const SweepEvent* le2);
        };
        struct Comp : public std::binary_function<SweepEvent, SweepEvent, bool> { // for sorting sweep events
            // Compare two sweep events
            // Return true means that e1 is placed at the event queue after e2, i.e,, e1 is processed by the algorithm after e2
            bool operator() (const SweepEvent* e1, const SweepEvent* e2);
        };
        typedef typename std::set<FinalType*, SegmentComp > EventSet;
        typedef typename Segment_2::Point_2 Point_2;
        
        SweepEvent () {}
        SweepEvent (bool b, const Point_2& p, FinalType* other, PolygonType pt) :
            left (b), point (p), otherEvent (other), pol (pt)
        {}
        SweepEvent (const Point_2& p, bool b, PolygonType pt) :
            left (b), point (p), pol (pt)
        {}
        
        bool left;              // is point the left endpoint of the edge (point, otherEvent->point)?
        Point_2 point;          // point associated with the event
        FinalType* otherEvent; // event associated to the other endpoint of the edge
        PolygonType pol;        // Polygon to which the associated segment belongs to
        
        //The following fields are only used in "left" events
        /**  Does segment (point, otherEvent->p) represent an inside-outside transition in the polygon for a vertical ray from (p.x, -infinite)? */
        bool inOut;
        typename EventSet::iterator posSL; // Position of the event (line segment) in sl
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
    template <typename Segment_2, bool inverseDir>
    struct SweepEventDefault : public SweepEvent<Segment_2, inverseDir, SweepEventDefault<Segment_2, inverseDir> >
    {
    private:
        typedef SweepEvent<Segment_2, inverseDir, SweepEventDefault<Segment_2, inverseDir> > Super;
    public:
        typedef typename Super::Point_2 Point_2;
        SweepEventDefault () {}
        SweepEventDefault (bool b, const Point_2& p, SweepEventDefault* other, PolygonType pt) : Super(b, p, other, pt) {}
        SweepEventDefault (const Point_2& p, bool b, PolygonType pt) : Super(p, b, pt) {}
    };
}

#include "sweepevent.inl"

#endif /* sweepevent_h */
