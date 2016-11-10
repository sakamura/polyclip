//
//  polyclip.cpp
//  polyclip
//

#include "booleanop.h"

extern const char* const POLYCLIP_VERSION = "1.0.0";

namespace cbop
{
    typedef Point<double> DefaultPoint;
    typedef Segment<DefaultPoint> DefaultSegment;
    typedef Contour<DefaultSegment> DefaultContour;
    typedef Polygon<DefaultContour> DefaultPolygon;

/*    void defaultCompute()
    {
        DefaultPolygon subj;
        DefaultPolygon clip;
        DefaultPolygon result;
        compute(subj, clip, result, BooleanOpType::DIFFERENCE);
    } */
}
