/*
 This version is licensed as : http://unlicense.org . Please see LICENSE and README.md for more information.
 Enjoy the code :)
 */
#ifndef polyclip_h
#define polyclip_h

#include "booleanop.h"

namespace polyclip
{
    template <typename Point>
    inline void compute (const Polygon<Point>& subj, const Polygon<Point>& clip, Polygon<Point>& result, BooleanOpType op)
    {
        BooleanOpImp<Point> boi (subj, clip, result, op);
        boi.run ();
    }
}
#endif /* polyclip_h */
