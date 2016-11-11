/*
 Original file by Francisco Martínez del Río.
 
 This version is licensed as : http://unlicense.org . Please see LICENSE and README.md for more information.
 Enjoy the code :)
 */

#ifndef POLYGON_H
#define POLYGON_H

#include "contour.h"

namespace polyclip {

    template <typename Point_2>
    class Polygon {
    public:
        typedef Contour<Point_2> Contour_2;
        typedef Segment<Point_2> Segment_2;
        typedef typename Point_2::value_type value_type;
        typedef Bbox<value_type> Bbox_2;

        typedef typename std::vector<Contour_2>::iterator iterator;
        typedef typename std::vector<Contour_2>::const_iterator const_iterator;
        
        Polygon () : contours () {}
        
        void join (const Polygon& pol);
        /** Get the p-th contour */
        Contour_2& contour (unsigned int p) { return contours[p]; }
        const Contour_2& contour (unsigned int p) const { return contours[p]; }
        Contour_2& operator[] (unsigned int p) { return contours[p]; }
        const Contour_2& operator[] (unsigned int p) const { return contours[p]; }
        /** Number of contours */
        unsigned int ncontours () const { return (unsigned int)contours.size (); }
        /** Number of vertices */
        unsigned int nvertices () const;
        /** Get the bounding box */
        Bbox_2 bbox () const;
        
        void move (value_type x, value_type y);
        
        void push_back (const Contour_2& c) { contours.push_back (c); }
        Contour_2& back () { return contours.back (); }
        const Contour_2& back () const { return contours.back (); }
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
        std::vector<Contour_2> contours;
    };
    
} // end of namespace polyclip

#include "polygon.inl"

#endif
