/*
 * Vertex.h
 *
 *  Created on: Feb 2, 2009
 *      Author: njoubert
 */

#ifndef VERTEX_H_
#define VERTEX_H_

#include "global.h"

class Vertex {
public:
    Vertex();
    Vertex(vec2 p);
    Vertex(double x, double y);

    /* Returns the position vector of this Vertex. */
    vec2 getPos();

    /* Sets the original position of this vertex. */
    void setPos(vec2 p);

private:
    vec2 _pos;
};

#endif /* VERTEX_H_ */
