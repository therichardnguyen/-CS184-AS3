/*
 * SceneGroup.cpp
 *
 *  Created on: Feb 12, 2009
 *      Author: jima
 */

#include "SceneInstance.h"
#include "SceneGroup.h"


SceneGroup::SceneGroup()
{
    _name = "unassigned";

    _poly = NULL;
}

// don't delete children, just local stuff; freeing children is SceneLoader's job
// (because cleaning up after a DAG with only local info is not good)
SceneGroup::~SceneGroup() 
{
    delete _poly;
}

int SceneGroup::getChildCount()
{
    return int(_children.size());
}

SceneInstance *SceneGroup::getChild(int i){
    return _children[i];
}

string SceneGroup::getName() {
    return _name;
}

Polygon *SceneGroup::getPolygon() {
    return _poly;
}


