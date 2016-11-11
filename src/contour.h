/*
 This version is licensed as : http://unlicense.org . Please see LICENSE and README.md for more information.
 Enjoy the code :)
 */
#ifndef contour_h
#define contour_h

#include <vector>
#include <algorithm>
#include "utilities.h"
#include "bbox.h"

namespace polyclip
{
    template <typename Point_2>
    class Contour {
    public:
        typedef Segment<Point_2> Segment_2;
        typedef typename Point_2::value_type value_type;
        typedef Bbox<value_type> Bbox_2;
        
        typedef typename std::vector<Point_2>::iterator iterator;
        typedef typename std::vector<Point_2>::const_iterator const_iterator;
        
        Contour () : points (), holes (), _external (true), _precomputedCC (false) {}
        
        /** Get the p-th vertex of the external contour */
        Point_2& vertex (unsigned int p) { return points[p]; }
        Point_2 vertex (unsigned int p) const { return points[p]; }
        Segment_2 segment (unsigned p) const { return (p == nvertices () - 1) ? Segment_2 (points.back (), points.front ()) :
            Segment_2 (points[p], points[p+1]); }
        /** Number of vertices and edges */
        unsigned nvertices () const { return (unsigned)points.size (); }
        unsigned nedges () const { return (unsigned)points.size (); }
        /** Get the bounding box */
        Bbox_2 bbox () const;
        /** Return if the contour is counterclockwise oriented */
        bool counterclockwise ();
        /** Return if the contour is clockwise oriented */
        bool clockwise () { return !counterclockwise (); }
        void changeOrientation () { std::reverse (points.begin (), points.end ()); _CC = !_CC; }
        void setClockwise () { if (counterclockwise ()) changeOrientation (); }
        void setCounterClockwise () { if (clockwise ()) changeOrientation (); }
        
        void move (value_type x, value_type y);
        void add (const Point_2& s) { points.push_back (s); }
        void erase (iterator i) { points.erase (i); }
        void clear () { points.clear (); holes.clear (); }
        void clearHoles () { holes.clear (); }
        iterator begin () { return points.begin (); }
        iterator end () { return points.end (); }
        const_iterator begin () const { return points.begin (); }
        const_iterator end () const { return points.end (); }
        Point_2& back () { return points.back (); }
        const Point_2& back () const { return points.back (); }
        void addHole (unsigned int ind) { holes.push_back (ind); }
        unsigned int nholes () const { return (unsigned int)holes.size (); }
        unsigned int hole (unsigned int p) const { return holes[p]; }
        bool external () const { return _external; }
        void setExternal (bool e) { _external = e; }
        
    private:
        /** Set of points conforming the external contour */
        std::vector<Point_2> points;
        /** Holes of the contour. They are stored as the indexes of the holes in a polygon class */
        std::vector<unsigned int> holes;
        bool _external; // is the contour an external contour? (i.e., is it not a hole?)
        bool _precomputedCC;
        bool _CC;
    };

}

#include "contour.inl"

#endif /* contour_h */
