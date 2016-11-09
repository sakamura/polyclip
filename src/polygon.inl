/*
 Original file by Francisco Martínez del Río.
 
 This version is licensed as : http://unlicense.org . Please see LICENSE and README.md for more information.
 Enjoy the code :)
 */

#include <limits>
#include <set>
#include <algorithm>
#include "polygon.h"

namespace cbop
{

Bbox_2 Contour::bbox () const
{
	if (nvertices () == 0)
		return Bbox_2 ();
	Bbox_2 b = vertex (0).bbox ();
	for (unsigned int i = 1; i < nvertices (); ++i)
		b = b + vertex (i).bbox ();
	return b;
}

bool Contour::counterclockwise ()
{
	if (_precomputedCC)
		return _CC;
	_precomputedCC = true;
	double area = 0.0;
	for (unsigned int c = 0; c < nvertices () - 1; c++)
		area += vertex (c).x () * vertex (c+1).y () - vertex (c+1).x () *  vertex (c).y ();
	area += vertex (nvertices ()-1).x () * vertex (0).y () - vertex (0).x () *  vertex (nvertices ()-1).y ();
	return _CC = area >= 0.0;
}

void Contour::move (double x, double y)
{
	for (unsigned int i = 0; i < points.size (); i++)
		points[i] = Point_2 (points[i].x () + x, points[i].y () + y);
}

void Polygon::join (const Polygon& pol)
{
	unsigned int size = ncontours ();
	for (unsigned int i = 0; i < pol.ncontours (); ++i) {
		push_back (pol.contour (i));
		back ().clearHoles ();
		for (unsigned int j = 0; j < pol.contour (i).nholes (); ++j)
			back ().addHole (pol.contour (i).hole (j) + size);
	}
}

unsigned Polygon::nvertices () const
{
	unsigned int nv = 0;
	for (unsigned int i = 0; i < ncontours (); i++)
		nv += contours[i].nvertices ();
	return nv;
}

Bbox_2 Polygon::bbox () const
{
	if (ncontours () == 0)
		return Bbox_2 ();
	Bbox_2 bb = contours[0].bbox ();
	for (unsigned int i = 1; i < ncontours (); i++)
		bb = bb + contours[i].bbox ();
	return bb;
}

void Polygon::move (double x, double y)
{
	for (unsigned int i = 0; i < contours.size (); i++)
		contours[i].move (x, y);
}

/*************************************************************************************************************
 * The following code is necessary for implementing the computeHoles member function
 * **********************************************************************************************************/

namespace { // start of anonymous namespace
	struct SweepEventHoles;
	struct SegmentCompHoles : public std::binary_function<SweepEventHoles*, SweepEventHoles*, bool> {
		bool operator() (SweepEventHoles* e1, SweepEventHoles* e2);
	};

	struct SweepEventHoles {
		Point_2 point;  // point associated with the event
		bool left;      // is the point the left endpoint of the segment (p, other->p)?
		int pol;        // Polygon to which the associated segment belongs to
		SweepEventHoles* otherEvent; // Event associated to the other endpoint of the segment
		/**  Does the segment (p, other->p) represent an inside-outside transition in the polygon for a vertical ray from (p.x, -infinite) that crosses the segment? */
		bool inOut;
		std::set<SweepEventHoles*, SegmentCompHoles>::iterator posSL; // Only used in "left" events. Position of the event (segment) in SL

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

	struct SweepEventCompHoles : public std::binary_function<SweepEventHoles*, SweepEventHoles*, bool> {
		bool operator() (SweepEventHoles* e1, SweepEventHoles* e2) {
			if (e1->point.x () < e2->point.x ()) // Different x coordinate
				return true;
			if (e2->point.x () < e1->point.x ()) // Different x coordinate
				return false;
			if (e1->point != e2->point) // Different points, but same x coordinate. The event with lower y coordinate is processed first
				return e1->point.y () < e2->point.y ();
			if (e1->left != e2->left) // Same point, but one is a left endpoint and the other a right endpoint. The right endpoint is processed first
				return !e1->left;
			// Same point, both events are left endpoints or both are right endpoints. The event associate to the bottom segment is processed first
			return e1->below (e2->otherEvent->point);
		}
	};
} // end of anonymous namespace

bool SegmentCompHoles::operator() (SweepEventHoles* le1, SweepEventHoles* le2) {
	if (le1 == le2)
		return false;
	if (signedArea (le1->point, le1->otherEvent->point, le2->point) != 0 || 
		signedArea (le1->point, le1->otherEvent->point, le2->otherEvent->point) != 0) {
		// Segments are not collinear
		// If they share their left endpoint use the right endpoint to sort
		if (le1->point == le2->point)
			return le1->below (le2->otherEvent->point);
		// Different points
		SweepEventCompHoles comp;
		if (comp (le1, le2))  // has the segment associated to e1 been sorted in evp before the segment associated to e2?
			return le1->below (le2->point);
		// The segment associated to e2 has been sorted in evp before the segment associated to e1
		return le2->above (le1->point);
	} 
	// Segments are collinear. Just a consistent criterion is used
	if (le1->point == le2->point)
		return le1 < le2;
	SweepEventCompHoles comp;
	return comp (le1, le2);
}

void Polygon::computeHoles ()
{
	if (ncontours () < 2) {
		if (ncontours () == 1 && contour (0).clockwise ())
			contour (0).changeOrientation ();
		return;
	}
	std::vector<SweepEventHoles> ev;
	std::vector<SweepEventHoles*> evp;
	ev.reserve (nvertices ()*2);
	evp.reserve (nvertices ()*2);
	for (unsigned i = 0; i < ncontours (); i++) {
		contour (i).setCounterClockwise ();
		for (unsigned j = 0; j < contour (i).nedges (); j++) {
			Segment_2 s = contour(i).segment (j);
			if (s.is_vertical ()) // vertical segments are not processed
				continue;
			ev.push_back (SweepEventHoles (s.source (), true, i));
			ev.push_back (SweepEventHoles (s.target (), true, i));
			SweepEventHoles* se1 = &ev[ev.size ()-2];
			SweepEventHoles* se2 = &ev[ev.size ()-1];
			se1->otherEvent = se2;
			se2->otherEvent = se1;
			if (se1->point.x () < se2->point.x ()) {
				se2->left = false;
				se1->inOut = false;
			} else {
				se1->left = false;
				se2->inOut = true;
			}
			evp.push_back (se1);
			evp.push_back (se2);
		}
	}
	sort (evp.begin (), evp.end (), SweepEventCompHoles ());

	std::set<SweepEventHoles*, SegmentCompHoles> SL; // Status line
	std::vector<bool> processed (ncontours (), false);
	std::vector<int> holeOf (ncontours (), -1);
	unsigned int nprocessed = 0;
	for (unsigned int i = 0; i < evp.size () && nprocessed < ncontours (); i++)  {
		SweepEventHoles* e = evp[i];

		if (e->left) { // the segment must be inserted into S
			e->posSL = SL.insert(e).first;
			if (!processed[e->pol]) {
				processed[e->pol] = true;
				nprocessed++;
				std::set<SweepEventHoles*, SegmentCompHoles>::iterator prev = e->posSL;
				if (prev == SL.begin ()) {
					contour (e->pol).setCounterClockwise ();
				} else {
					prev--;
					if (!(*prev)->inOut) {
						holeOf[e->pol] = (*prev)->pol;
						contour (e->pol).setExternal (false);
						contour ((*prev)->pol).addHole (e->pol);
						if (contour((*prev)->pol).counterclockwise ())
							contour (e->pol).setClockwise ();
						else
							contour (e->pol).setCounterClockwise ();
					} else if (holeOf[(*prev)->pol] != -1) {
						holeOf[e->pol] = holeOf[(*prev)->pol];
						contour (e->pol).setExternal (false);
						contour (holeOf[e->pol]).addHole (e->pol);
						if (contour(holeOf[e->pol]).counterclockwise ())
							contour (e->pol).setClockwise ();
						else
							contour (e->pol).setCounterClockwise ();
					} else {
						contour (e->pol).setCounterClockwise ();
					}
				}
			}
		} else { // the segment must be removed from S
			SL.erase (e->otherEvent->posSL);
		}
	}
}

}
