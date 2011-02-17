/*
 * SceneGroup.h
 *
 *  Created on: Feb 12, 2009
 *      Author: jima
 */

#ifndef SCENEGROUP_H_
#define SCENEGROUP_H_

#include "../Polygon.h"
#include <vector>

using namespace std;

class SceneGroup {
public:
    string getName(); /* get the name of the group, useful for debugging */

    Polygon *getPolygon(); /* if this is a terminal node, it will have a polygon to display (otherwise this is null) */

    int getChildCount(); /* get the amount of instances which are in the group */
    SceneInstance *getChild(int i); /* get a child */

    virtual ~SceneGroup();

private:
    SceneGroup(); // private constructor to be called by SceneLoader only

    // private copy constructor and assignment operator to avoid copying this data
    SceneGroup(const SceneGroup&);
    SceneGroup& operator=(const SceneGroup&);

    string _name;
    vector<SceneInstance *> _children;
    Polygon * _poly;

    friend class SceneLoader;
};

#endif /* SCENEGROUP_H_ */
