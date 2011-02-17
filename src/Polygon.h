/*
 * Polygon.h
 *
 *  Created on: Feb 2, 2009
 *      Author: njoubert
 */

#ifndef POLYGON_H_
#define POLYGON_H_

#include "global.h"
#include "Vertex.h"
#include "BoundingBox.h"

class Polygon {
public:
    Polygon();

    /* Loads the first polygon found in an OBJ file */
    Polygon(string objfile);

    /* Draws the given polygon using OpenGL. */
    void draw(GLenum mode = GL_LINE_LOOP);

    /* Returns a reference to the polygon's bounding box */
    const BoundingBox& getBoundingBox();

private:
    bool _parseLine(string, vector<Vertex> &);
    vector<Vertex> _vertices;
    BoundingBox _bb;

    void addVertex(Vertex * v);
};

#endif /* POLYGON_H_ */
