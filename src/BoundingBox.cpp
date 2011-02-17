/*
 * BoundingBox.cpp
 *
 *  Created on: Feb 12, 2009
 *      Author: jima
 */

#include "BoundingBox.h"

BoundingBox::BoundingBox() : _valid(false) {}

BoundingBox::BoundingBox(vec2 min, vec2 max) : _min(min), _max(max), _valid(true) {}

bool BoundingBox::isValid() const {
    return _valid;
}

bool BoundingBox::getMin(vec2 &v) const {
    if (!_valid)
        return false;
    else {
        v = _min;
        return true;
    }
}

bool BoundingBox::getMax(vec2 &v) const {
    if (!_valid)
        return false;
    else {
        v = _max;
        return true;
    }
}

void BoundingBox::expand(const vec2 &p) {
    if (!_valid) {
        _min = p;
        _max = p;
        _valid = true;
    }
    else {
        for (int i = 0; i < 2; ++i) {
            _min[i] = MIN(p[i],_min[i]);
            _max[i] = MAX(p[i],_max[i]);
        }
    }
}

void BoundingBox::expand(const BoundingBox &bb) {
	if (!bb._valid)
		return;
	expand(bb._max);
	expand(bb._min);
}

void BoundingBox::draw() const {
    if (!_valid) // Don't draw a bounding box with zero points
        return;

    glBegin(GL_LINE_LOOP);
        glVertex2d(_min[0], _min[1]);
        glVertex2d(_max[0], _min[1]);
        glVertex2d(_max[0], _max[1]);
        glVertex2d(_min[0], _max[1]);
    glEnd();
}

