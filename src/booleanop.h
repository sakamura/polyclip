/*
 Original file by Francisco Martínez del Río.
 
 This version is licensed as : http://unlicense.org . Please see LICENSE and README.md for more information.
 Enjoy the code :)
 */

#ifndef BOOLEANOP_H
#define BOOLEANOP_H

#include <vector>
#include <list>
#include <set>
#include <queue>
#include <functional>

#include "polygon.h"
#include "polyclipdefs.h"
#include "sweepevent.h"

namespace polyclip {
    template <typename Point_2>
    class BooleanOpImp
    {
    public:
        typedef Polygon<Point_2> Polygon_2;
        typedef Contour<Point_2> Contour_2;
        typedef Segment<Point_2> Segment_2;
        typedef typename Point_2::value_type value_type;
        typedef Bbox<value_type> Bbox_2;

        struct SweepEvent_2 : public SweepEvent<Segment_2, false, SweepEvent_2>
        {
            typedef SweepEvent<Segment_2, false, SweepEvent_2> Super;
            SweepEvent_2 () {}
            SweepEvent_2(bool b, const Point_2& p, SweepEvent_2* other, PolygonType pt, EdgeType et = NORMAL) :
                Super(b, p, other, pt),
                type (et), prevInResult (0), inResult (false)
            {}

            EdgeType type;
            bool otherInOut; // inOut transition for the segment from the other polygon preceding this segment in sl
            bool inResult;
            unsigned int pos;
            bool resultInOut;
            unsigned int contourId;
            SweepEvent_2* prevInResult; // previous segment in sl belonging to the result of the boolean operation
        };
        typedef typename SweepEvent_2::SegmentComp SegmentComp_2;
        typedef typename SweepEvent_2::Comp SweepEventComp_2;

        BooleanOpImp (const Polygon_2& subj, const Polygon_2& clip, Polygon_2& result, BooleanOpType op);
        void run ();

    private:
        const Polygon_2& subject;
        const Polygon_2& clipping;
        Polygon_2& result;
        BooleanOpType operation;
        std::priority_queue<SweepEvent_2*, std::vector<SweepEvent_2*>, SweepEventComp_2> eq; // event queue (sorted events to be processed)
        std::set<SweepEvent_2*, SegmentComp_2> sl; // segments intersecting the sweep line
        std::deque<SweepEvent_2> eventHolder;    // It holds the events generated during the computation of the boolean operation
        SweepEventComp_2 sec;                    // to compare events
        std::deque<SweepEvent_2*> sortedEvents;
        bool trivialOperation (const Bbox_2& subjectBB, const Bbox_2& clippingBB);
        /** @brief Compute the events associated to segment s, and insert them into pq and eq */
        void processSegment (const Segment_2& s, PolygonType pt);
        /** @brief Store the SweepEvent e into the event holder, returning the address of e */
        SweepEvent_2 *storeSweepEvent (const SweepEvent_2& e) { eventHolder.push_back (e); return &eventHolder.back (); }
        /** @brief Process a posible intersection between the edges associated to the left events le1 and le2 */
        int possibleIntersection (SweepEvent_2* le1, SweepEvent_2* le2);
        /** @brief Divide the segment associated to left event le, updating pq and (implicitly) the status line */
        void divideSegment (SweepEvent_2* le, const Point_2& p);
        /** @brief return if the left event le belongs to the result of the Boolean operation */
        bool inResult (SweepEvent_2* le);
        /** @brief compute several fields of left event le */
        void computeFields (SweepEvent_2* le, const typename std::set<SweepEvent_2*, SegmentComp_2>::iterator& prev);
        // connect the solution edges to build the result polygon
        void connectEdges ();
        int nextPos (int pos, const std::vector<SweepEvent_2*>& resultEvents, const std::vector<bool>& processed);
    };
    
} // end of namespace polyclip

#include "booleanop.inl"

#endif
