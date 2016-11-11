/*
 This version is licensed as : http://unlicense.org . Please see LICENSE and README.md for more information.
 Enjoy the code :)
 */
#ifndef polyclipdefs_h
#define polyclipdefs_h

namespace polyclip {
    
    enum BooleanOpType { INTERSECTION, UNION, DIFFERENCE, XOR };
    enum EdgeType { NORMAL, NON_CONTRIBUTING, SAME_TRANSITION, DIFFERENT_TRANSITION };
    enum PolygonType { SUBJECT, CLIPPING };

}

#endif /* polyclipdefs_h */
