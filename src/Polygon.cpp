/*
 * Polygon.cpp
 *
 *  Created on: Feb 2, 2009
 *      Author: njoubert
 */

#include "Polygon.h"

Polygon::Polygon() {
    // Leave it empty.
}

/**
 * This parses an OBJ file for the first polygon available in it.
 */
Polygon::Polygon(string objfile) {
    // Leave this as it is.
    std::cout << "Parsing OBJ file " << objfile << std::endl;

    vector<Vertex> tempVerts;
    ifstream inFile(objfile.c_str(), ifstream::in);
    if (!inFile) {
        std::cout << "Could not open given obj file " << objfile << std::endl;
    }
    while (inFile.good()) {
        string line;
        getline(inFile, line);
        if (!_parseLine(line, tempVerts)) {
            std::cout << "Failed to parse OBJ file." << std::endl;
            break;
        }
        if (_vertices.size() > 0) // take the first face in the file.
            break;
    }
    inFile.close();

    std::cout << "Parsed an OBJ file with " << _vertices.size() << " vertices."
            << endl;
}

bool Polygon::_parseLine(string line, vector<Vertex> & temp) {
    string operand;
    bool success = true;

    if (line.empty())
        return true;
    stringstream ss(stringstream::in | stringstream::out);
    ss.str(line);
    ss >> operand;

    if (operand[0] == '#') {

        return true;

    } else if (operand == "v") {

        double x, y;
        ss >> x >> y;
        temp.push_back(Vertex(x, y));

    } else if (operand == "f") {

        while (!ss.eof()) {
            int i;
            ss >> i;
            addVertex(&temp[i - 1]); // copy vertex in to polygon
        }

    } else {

        cout << "Unknown operand in scene file, skipping line: " << operand
                << endl;

    }

    if (ss.fail()) {
        std::cout
                << "The bad bit of the input file's line's stringstream is set! Couldn't parse:"
                << std::endl;
        std::cout << "  " << line << std::endl;
        success = false;
    }

    return success;
}

void Polygon::draw(GLenum mode) {
    if (1 > _vertices.size())
        return;
    glBegin(mode);
    for (vector<Vertex>::iterator it = _vertices.begin(); it
            != _vertices.end(); it++)
        glVertex2d(it->getPos()[0], it->getPos()[1]);
    glEnd();
}

void Polygon::addVertex(Vertex * v) {
    _bb.expand(v->getPos());
    _vertices.push_back(*v);
}

const BoundingBox& Polygon::getBoundingBox() {
    return _bb;
}