/*
 Original file by Francisco Martínez del Río.
 
 This version is licensed as : http://unlicense.org . Please see LICENSE and README.md for more information.
 Enjoy the code :)
 */

#include <limits>
#include <set>
#include <algorithm>
#include "polygon.h"
#include "sweepevent.h"

namespace polyclip
{
    template <typename Point_2>
    void Polygon<Point_2>::join (const Polygon& pol)
    {
        unsigned int size = ncontours ();
        for (unsigned int i = 0; i < pol.ncontours (); ++i) {
            push_back (pol.contour (i));
            back ().clearHoles ();
            for (unsigned int j = 0; j < pol.contour (i).nholes (); ++j)
                back ().addHole (pol.contour (i).hole (j) + size);
        }
    }
    
    template <typename Point_2>
    unsigned Polygon<Point_2>::nvertices () const
    {
        unsigned int nv = 0;
        for (unsigned int i = 0; i < ncontours (); i++)
            nv += contours[i].nvertices ();
        return nv;
    }
    
    template <typename Point_2>
    typename Polygon<Point_2>::Bbox_2 Polygon<Point_2>::bbox () const
    {
        if (ncontours () == 0)
            return Bbox_2 ();
        Bbox_2 bb = contours[0].bbox ();
        for (unsigned int i = 1; i < ncontours (); i++)
            bb = bb + contours[i].bbox ();
        return bb;
    }
    
    template <typename Point_2>
    void Polygon<Point_2>::move (value_type x, value_type y)
    {
        for (unsigned int i = 0; i < contours.size (); i++)
            contours[i].move (x, y);
    }
    
    template <typename Point_2>
    void Polygon<Point_2>::computeHoles ()
    {
        typedef SweepEventDefault<Segment_2, true> SweepEventHoles;
        typedef typename SweepEventHoles::Comp SweepEventCompHoles;
        typedef typename SweepEventHoles::SegmentComp SegmentCompHoles;
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
                if (se1->point.x < se2->point.x) {
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
        sort (evp.begin (), evp.end (), SweepEventCompHoles());
        
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
                    typename std::set<SweepEventHoles*, SegmentCompHoles >::iterator prev = e->posSL;
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
