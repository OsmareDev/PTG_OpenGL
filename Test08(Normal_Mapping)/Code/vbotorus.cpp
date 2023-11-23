#include "vbotorus.h"
#include <cmath>

void generateVerts(float * verts, float * norms, float * tangs, float * tex, unsigned int * el, 
				   float outerRadius, float innerRadius,
				   int rings, int sides)
{
	const double TWOPI = 2* 3.14159265358979323846;
    float ringFactor  = (float)(TWOPI / rings);
    float sideFactor = (float)(TWOPI / sides);
    int idx = 0, tidx = 0;
    for( int ring = 0; ring <= rings; ring++ ) {
        float u = ring * ringFactor;
        float cu = cos(u);
        float su = sin(u);
        for( int side = 0; side < sides; side++ ) {
            float v = side * sideFactor;
            float cv = cos(v);
            float sv = sin(v);
            float r = (outerRadius + innerRadius * cv);
            verts[idx] = r * cu;
            verts[idx + 1] = r * su;
            verts[idx + 2] = innerRadius * sv;
            norms[idx] = cv * cu * r;
            norms[idx + 1] = cv * su * r;
            norms[idx + 2] = sv * r;
            tangs[idx] = -su * r;
            tangs[idx + 1] = cu * r;
            tangs[idx + 2] = 0;
            tex[tidx] = (float)(u / TWOPI);
            tex[tidx+1] = (float)(v / TWOPI);
            tidx += 2;
            // Normalize
            float len = sqrt( norms[idx] * norms[idx] +
                              norms[idx+1] * norms[idx+1] +
                              norms[idx+2] * norms[idx+2] );
            norms[idx] /= len;
            norms[idx+1] /= len;
            norms[idx+2] /= len;
            len = sqrt( tangs[idx] * tangs[idx] +
                              tangs[idx+1] * tangs[idx+1] +
                              tangs[idx+2] * tangs[idx+2] );
            tangs[idx] /= len;
            tangs[idx+1] /= len;
            tangs[idx+2] /= len;
            idx += 3;
        }
    }

    idx = 0;
    for( int ring = 0; ring < rings; ring++ ) {
        int ringStart = ring * sides;
        int nextRingStart = (ring + 1) * sides;
        for( int side = 0; side < sides; side++ ) {
            int nextSide = (side+1) % sides;
            // The quad
            el[idx] = (ringStart + side);
            el[idx+1] = (nextRingStart + side);
            el[idx+2] = (nextRingStart + nextSide);
            el[idx+3] = ringStart + side;
            el[idx+4] = nextRingStart + nextSide;
            el[idx+5] = (ringStart + nextSide);
            idx += 6;
        }
    }
}
