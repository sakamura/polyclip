/*
 This version is licensed as : http://unlicense.org . Please see LICENSE and README.md for more information.
 Enjoy the code :)
 */
#include "contour.h"

namespace polyclip
{
    template <typename Point_2>
    typename Contour<Point_2>::Bbox_2 Contour<Point_2>::bbox () const
    {
        if (nvertices () == 0)
            return Bbox_2 ();
        Bbox_2 b(vertex (0));
        for (unsigned int i = 1; i < nvertices (); ++i)
            b = b + Bbox_2(vertex (i));
        return b;
    }
    
    template <typename Point_2>
    bool Contour<Point_2>::counterclockwise ()
    {
        if (_precomputedCC)
            return _CC;
        _precomputedCC = true;
        value_type area = 0.0;
        for (unsigned int c = 0; c < nvertices () - 1; c++)
            area += vertex (c).x * vertex (c+1).y - vertex (c+1).x *  vertex (c).y;
        area += vertex (nvertices ()-1).x * vertex (0).y - vertex (0).x *  vertex (nvertices ()-1).y;
        return _CC = area >= 0.0;
    }
    
    template <typename Point_2>
    void Contour<Point_2>::move (value_type x, value_type y)
    {
        for (unsigned int i = 0; i < points.size (); i++)
            points[i] = Point_2 (points[i].x + x, points[i].y + y);
    }
    

}
