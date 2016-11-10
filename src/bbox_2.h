/*
 Original file by Francisco Martínez del Río.
 
 This version is licensed as : http://unlicense.org . Please see LICENSE and README.md for more information.
 Enjoy the code :)
 */

// ------------------------------------------------------------------
// Bbox_2 Class : A bounding box
// ------------------------------------------------------------------

#ifndef BBOX_2_H
#define BBOX_2_H

namespace polyclip {
    
    template <typename T>
    class Bbox {
    public:
        Bbox () :
        _xmin (0), _ymin (0), _xmax (0), _ymax (0) {}
        Bbox (T x_min, T y_min, T x_max, T y_max) :
        _xmin (x_min), _ymin (y_min), _xmax (x_max), _ymax (y_max) {}
        Bbox (T x, T y) :
        _xmin (x), _ymin (y), _xmax (x), _ymax (y) {}
        template <typename Point>
        Bbox (const Point& point) :
        _xmin (point.x), _ymin (point.y), _xmax (point.x), _ymax (point.y) {}
        T xmin () const { return _xmin; }
        T xmax () const { return _xmax; }
        T ymin () const { return _ymin; }
        T ymax () const { return _ymax; }
        Bbox operator+ (const Bbox& b) const { return Bbox (std::min (_xmin, b.xmin ()),
                                                            std::min (_ymin, b.ymin ()),
                                                            std::max (_xmax, b.xmax ()),
                                                            std::max (_ymax, b.ymax ())); }
    private:
        T _xmin, _ymin, _xmax, _ymax;
    };
    
} // end of namespace polyclip
#endif
