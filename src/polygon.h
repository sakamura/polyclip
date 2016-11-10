/*
 Original file by Francisco Martínez del Río.
 
 This version is licensed as : http://unlicense.org . Please see LICENSE and README.md for more information.
 Enjoy the code :)
 */

#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <algorithm>
#include "utilities.h"
#include "bbox_2.h"

namespace polyclip {

    template <typename Segment>
    class Contour {
    public:
        typedef Segment Segment_2;
        typedef typename Segment_2::Point_2 Point_2;
        typedef typename Point_2::value_type value_type;
        typedef typename Point_2::Bbox_2 Bbox_2;
        
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
    
    template <typename Segment_2>
    std::ostream& operator<< (std::ostream& o, Contour<Segment_2>& c);
    
    template <typename Contour>
    class Polygon {
    public:
        typedef typename std::vector<Contour>::iterator iterator;
        typedef typename std::vector<Contour>::const_iterator const_iterator;
        typedef typename Contour::Segment_2 Segment_2;
        typedef typename Segment_2::Point_2 Point_2;
        typedef typename Point_2::value_type value_type;
        typedef typename Point_2::Bbox_2 Bbox_2;
        
        Polygon () : contours () {}
        
        // Get the polygon from a text file */
        bool open (const std::string& filename);
        void join (const Polygon& pol);
        /** Get the p-th contour */
        Contour& contour (unsigned int p) { return contours[p]; }
        const Contour& contour (unsigned int p) const { return contours[p]; }
        Contour& operator[] (unsigned int p) { return contours[p]; }
        const Contour& operator[] (unsigned int p) const { return contours[p]; }
        /** Number of contours */
        unsigned int ncontours () const { return (unsigned int)contours.size (); }
        /** Number of vertices */
        unsigned int nvertices () const;
        /** Get the bounding box */
        Bbox_2 bbox () const;
        
        void move (value_type x, value_type y);
        
        void push_back (const Contour& c) { contours.push_back (c); }
        Contour& back () { return contours.back (); }
        const Contour& back () const { return contours.back (); }
        void pop_back () { contours.pop_back (); }
        void erase (iterator i) { contours.erase (i); }
        void clear () { contours.clear (); }
        
        iterator begin () { return contours.begin (); }
        iterator end () { return contours.end (); }
        const_iterator begin () const { return contours.begin (); }
        const_iterator end () const { return contours.end (); }
        void computeHoles ();
    private:
        /** Set of contours conforming the polygon */
        std::vector<Contour> contours;
    };
    
} // end of namespace polyclip

#include "polygon.inl"

#endif
