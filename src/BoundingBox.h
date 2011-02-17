/*
 * BoundingBox.h
 *
 *  Created on: Feb 12, 2009
 *      Author: jima
 */

#ifndef BOUNDINGBOX_H_
#define BOUNDINGBOX_H_

#include "global.h"

class BoundingBox {
public:
    BoundingBox();
    BoundingBox(vec2 min, vec2 max);

    /* If there is a valid bounding box (enclosing at least one point), sets v to the bottom-left corner and returns true.
        Returns false otherwise. */
    bool getMin(vec2 &v) const;
    /* If there is a valid bounding box (enclosing at least one point), sets v to the top-right corner and returns true.
        Returns false otherwise. */
    bool getMax(vec2 &v) const;

    /* Expands the bounding box to bound a new point */
    void expand(const vec2 &p);
	void expand(const BoundingBox &bb);

    /* Draws the bounding box using OpenGL */
    void draw() const;

    /* Returns false if the bounding box does not yet bound a valid min/max (of at least one point) */
    bool isValid() const;

private:
    vec2 _min, _max;
    bool _valid;
};

#endif /* BOUNDINGBOX_H_ */

