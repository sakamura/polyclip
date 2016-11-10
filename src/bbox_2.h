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

namespace cbop {
    
    template <typename T>
    class Bbox {
    public:
        Bbox (T x_min = 0, T y_min = 0, T x_max = 0, T y_max = 0) :
        _xmin (x_min), _ymin (y_min), _xmax (x_max), _ymax (y_max) {}
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
    
} // end of namespace cbop
#endif
