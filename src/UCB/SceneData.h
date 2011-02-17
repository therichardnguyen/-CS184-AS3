/*
 * SceneData.h
 *
 *  These classes are used internally to wrap ParametricValues
 *  The Scene 'Node' class will not return them to users of the class.
 *
 *  Created on: Feb 8, 2009
 *      Author: jima
 */

#ifndef SCENEDATA_H_
#define SCENEDATA_H_

#include "ParametricValue.h"
#include <vector>

class Transform {
    friend class SceneLoader;

public:
    virtual mat4 getMatrix(int time) = 0; // pure virtual function
    virtual ~Transform() {}
};

class GeneralTransform : public Transform {
    vector<ParametricValue*> matrix;
    friend class SceneLoader;

public:
    mat4 getMatrix(int time) {
        mat4 out = 0;
        if (matrix.size() == 9) // 2d matrix
        {
            out[2][2] = 1;
            for (int i = 0; i < 3; i++)
            {
                int i_ind = i < 2 ? i : 3;

                for (int j = 0; j < 3; j++)
                {
                    int j_ind = j < 2 ? j : 3;

                    out[i_ind][j_ind] = matrix[i * 3 + j]->getValue(time);
                }
            }
        } else if (matrix.size() == 16) {
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    out[i][j] = matrix[i*4 + j]->getValue(time);
                }
            }
        } else {
            out = identity3D();
        }
        return out;
    }

    ~GeneralTransform()
    {
        for (vector<ParametricValue*>::iterator it = matrix.begin(); it != matrix.end(); ++it)
        {
            delete *it;
        }
    }

};

class Translate : public Transform {
    ParametricValue *translate[3];
    friend class SceneLoader;

public:
    mat4 getMatrix(int time) {
        vec3 tr;
        tr[0] = translate[0]->getValue(time);
        tr[1] = translate[1]->getValue(time);
        if (translate[2] == NULL)
            tr[2] = 0;
        else
            tr[2] = translate[2]->getValue(time);
        return translation3D(tr);
    }

    Translate() {
        for (int i = 0; i < 3; i++)
            translate[i] = NULL;
    }

    ~Translate() {
        for (int i = 0; i < 3; i++)
            delete translate[i];
    }
};

class Scale : public Transform {
    ParametricValue *scale[3];
    friend class SceneLoader;

public:
    mat4 getMatrix(int time) {
        vec3 sc;
        sc[0] = scale[0]->getValue(time);
        sc[1] = scale[1]->getValue(time);
        if (scale[2] == NULL)
            sc[2] = 1;
        else
            sc[2] = scale[2]->getValue(time);
        return scaling3D(sc);
    }

    Scale() {
        for (int i = 0; i < 3; i++)
            scale[i] = NULL;
    }

    ~Scale() {
        for (int i = 0; i < 3; i++)
            delete scale[i];
    }
};

class Rotate : public Transform {
    ParametricValue *angle;
    ParametricValue *axis[3];
    friend class SceneLoader;

public:
    mat4 getMatrix(int time) {
        vec3 ax;
        if (axis[2] == NULL) // 2D
        {
            ax = vec3(0,0,1);
        } else {
            ax = vec3(axis[0]->getValue(time),
                            axis[1]->getValue(time),
                                axis[2]->getValue(time));
        }
        return rotation3D(ax, angle->getValue(time));
    };

    Rotate()
    {
        angle = NULL;
        for (int i = 0; i < 3; i++)
            axis[i] = NULL;
    }

    ~Rotate() {
        delete angle;
        for (int i = 0; i < 3; i++)
            delete axis[i];
    }
};

class Color {
    ParametricValue *_color[3];
    friend class SceneLoader;

public:
    vec3 getColor(int time) {
        return vec3(_color[0]->getValue(time),
                _color[1]->getValue(time),
                    _color[2]->getValue(time));
    }

    Color()
    {
        for (int i = 0; i < 3; i++)
            _color[i] = NULL;
    }

    ~Color() {
        for (int i = 0; i < 3; i++)
            delete _color[i];
    }
};

class LOD {
    ParametricValue *_level;
    friend class SceneLoader;

public:
    int getLod(int time) { return int(_level->getValue(time)); }

    LOD() : _level(NULL) {}

    ~LOD() {
        delete _level;
    }
};

#endif /* SCENEDATA_H_ */
