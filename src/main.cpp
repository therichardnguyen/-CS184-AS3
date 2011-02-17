#include "main.h"

using namespace std;

//****************************************************
// Global Variables
//****************************************************
Scene *scene;
UCB::ImageSaver * imgSaver;
int frameCount = 0, frameOutputCount = 0;
bool outputBmps = false;

/* The recursive render function.  
 Note: This is the only function you need to edit for this assignment! 
 (but feel free to edit anything you'd like to.) */
BoundingBox RenderGroup(SceneGroup *i, vec3 color, int lod, int depth);
BoundingBox RenderInstance(SceneInstance *n, vec3 color, int lod, int depth);

void setColor(int c) {
	switch(c) {
		case 0:								// if red
			glColor3f(1,0,0); break;		// use red
		case 1:								// if yellow
			glColor3f(1,1,0); break;		// use  yellow
		case 2: 							// if green
			glColor3f(0,1,0); break;		// use green
		case 3:								// if cyan
			glColor3f(0,1,1); break;		// use cyan
		case 4:								// if blue
			glColor3f(0,0,1); break;		// use blue
		case 5:								// if magenta
			glColor3f(1,0,1); break;		// use magenta
		default:							// otherwise
		glColor3f(.5,.5,.5); break;			// use white
	}
}

mat4 getCurrentModelviewMatrix() {
	// matrix container, column-major
	GLdouble mat[16];
	
	// get the matrix at the top fo the stack
	glGetDoublev(GL_MODELVIEW_MATRIX, mat);
	
	// return a row-major mat4
	return mat4(vec4(mat[0],mat[4],mat[8],mat[12]),
				vec4(mat[1],mat[5],mat[9],mat[13]),
				vec4(mat[2],mat[6],mat[10],mat[14]),
				vec4(mat[3],mat[7],mat[11],mat[15]));
}

BoundingBox transformBoundingBox(BoundingBox b, mat4 transform) {
	/* Bounding box
	 * ca-------cb
	 * |        |
	 * |        |
	 * |        |
	 * cd-------cc
	 */
	//mat4 transform = getCurrentModelviewMatrix();
	vec2 min2, max2,ca,cb,cc,cd;
	
	// get the min and max vec2's of the boundingBox
	b.getMin(min2);
	b.getMax(max2);
	
	// create the corner points of the bounding box
	ca[0] = min2[0]; ca[1] = max2[1];
	cb[0] = max2[0]; cb[1] = max2[1];
	cc[0] = max2[0]; cc[1] = min2[1];
	cd[0] = min2[0]; cd[1] = min2[1];
	
	// convert the four corner points into vec4
	vec4 ca4(ca[0],ca[1],0,1),
		cb4(cb[0],cb[1],0,1),
		cc4(cc[0],cc[1],0,1),
		cd4(cd[0],cd[1],0,1);
		
	// transform the corner points
	ca4 = transform*ca4;
	cb4 = transform*cb4;
	cc4 = transform*cc4;
	cd4 = transform*cd4;
	
	// find the new bounding box after the transformation
	min2[0] = min(min(min(ca4[0],cb4[0]),cc4[0]),cd4[0]);
	min2[1] = min(min(min(ca4[1],cb4[1]),cc4[1]),cd4[1]);
	max2[0] = max(max(max(ca4[0],cb4[0]),cc4[0]),cd4[0]);
	max2[1] = max(max(max(ca4[1],cb4[1]),cc4[1]),cd4[1]);
	
	// make the bounding box and return it
	return BoundingBox(min2,max2);
}

BoundingBox RenderGroup(SceneGroup *i, vec3 color, int lod, int depth) 
{
	int ii;
	BoundingBox bbox;
		
	// if this is a terminal node
	if (i->getChildCount() == 0) {
		
		// set the correct color for the polygon
		glColor3f(color[0],color[1],color[2]);
		
		// draw the polygon if LOD == 3
		if (lod == 3)
			i->getPolygon()->draw(GL_POLYGON);
			
		// draw the polygon outline if LOD == 2
		else if (lod == 2)
			i->getPolygon()->draw(GL_LINE_LOOP);
			
		// draw the bounding box
		setColor(depth);
		i->getPolygon()->getBoundingBox().draw();
		
		// return the polygon's bounding box
		return i->getPolygon()->getBoundingBox();
	} else {
		
		// expand and traverse this node
		for(ii=0; ii<i->getChildCount();ii++) 
			bbox.expand(RenderInstance(i->getChild(ii), color, lod, depth+1));
		
		// draw this group's bounding box
		setColor(depth);
		bbox.draw();
	}
	
	return bbox;
}

BoundingBox RenderInstance(SceneInstance *n, vec3 color, int lod, int depth)
{
	// vars for computing the transform, LOD and color
	mat4 nmatrix(vec4(1,0,0,0),vec4(0,1,0,0),vec4(0,0,1,0),vec4(0,0,0,1));
	int nlod = lod;
	vec3 ncolor = color;
	BoundingBox bbox;
	
	// get this instance's Level of Depth
	if (n->computeLOD(nlod,frameCount))
		nlod = min(nlod,lod);
	
	if (nlod == 0)
		return BoundingBox();
		
	// get this instance's color
	n->computeColor(ncolor,frameCount);
	
	
	// get this instance's transform matrix
	n->computeTransform(nmatrix,frameCount);
	
	// copy the previous matrix up to the this level
	glPushMatrix();
	
	// apply the transform
	GLdouble t[16] = {nmatrix[0][0],nmatrix[1][0],nmatrix[2][0],nmatrix[3][0],
					nmatrix[0][1],nmatrix[1][1],nmatrix[2][1],nmatrix[3][1],
					nmatrix[0][2],nmatrix[1][2],nmatrix[2][2],nmatrix[3][2],
					nmatrix[0][3],nmatrix[1][3],nmatrix[2][3],nmatrix[3][3]};
	glMultMatrixd(t);
	
	// render the group that this instance is
	bbox = transformBoundingBox(RenderGroup(n->getChild(),ncolor,nlod,depth),nmatrix);
	
	// pop this matrix off the stack and return to the previous matrix
	glPopMatrix();
	
	return bbox; 
}




//-------------------------------------------------------------------------------
/// You will be calling all of your drawing-related code from this function.
/// Nowhere else in your code should you use glBegin(...) and glEnd() except code
/// called from this method.
///
/// To force a redraw of the screen (eg. after mouse events or the like) simply call
/// glutPostRedisplay();
void display() {
	
    glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);					// indicate we are specifying camera transformations
	glLoadIdentity();							// make sure transformation is "zero'd"
	
  	BoundingBox bb = RenderInstance(scene->getRoot(), vec3(1,1,1), 3, 0); // render at solid LOD
	
	//Now that we've drawn on the buffer, swap the drawing buffer and the displaying buffer.
	glutSwapBuffers();
	if (outputBmps) {
		if (frameOutputCount <100) {
			GLdouble viewport[4];
			glGetDoublev(GL_VIEWPORT,viewport);
			imgSaver->saveFrame(viewport[2], viewport[3]);
			glutPostRedisplay();
			frameOutputCount++;
		} else {
			outputBmps = false;
			glutPostRedisplay();
		}
	}

}


//-------------------------------------------------------------------------------
/// \brief	Called when the screen gets resized.
/// This gives you the opportunity to set up all the relevant transforms.
///
void reshape(int w, int h) {
	//Set up the viewport to ignore any size change.
	glViewport(0,0,w,h);

	//Set up the PROJECTION transformationstack to be a simple orthographic [-1, +1] projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    if (w < h)
	    glOrtho(-20, 20, -h*20/float(w), h*20/float(w), 1, -1);	// resize type = stretch
    else
        glOrtho(-w*20/float(h), w*20/float(h), -20, 20, 1, -1);	// resize type = stretch

	//Set the MODELVIEW transformation stack to the identity.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


//-------------------------------------------------------------------------------
/// Called to handle keyboard events.
void myKeyboardFunc (unsigned char key, int x, int y) {
	switch (key) {
		case 27:			// Escape key
			exit(0);
			break;
		case 97:				// 'a' key
			outputBmps = true;
			glutPostRedisplay();
			break;
	}
}

//-------------------------------------------------------------------------------
/// Called to update the screen at 30 fps.
void frameTimer(int value){

    frameCount++;
    glutPostRedisplay();
    glutTimerFunc(1000/30, frameTimer, 1);
}


//-------------------------------------------------------------------------------
/// Initialize the environment
int main(int argc,char** argv) {
	//Initialize OpenGL
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);

	//Here we load a scene from a scene file.
	if (argc < 2) {
	    cout << "USAGE: show scene.scd" << endl;
	    exit(1);
	}
    scene = new Scene(argv[1]);

	//Initialize the screen capture class to save BMP captures
	//in the current directory, with the prefix "output"
	imgSaver = new UCB::ImageSaver("./", "output");

	//Create OpenGL Window
	glutInitWindowSize(600,600);
	glutInitWindowPosition(0,0);
	glutCreateWindow("CS184 as3 - Richard Nguyen && Samantha Paras");

	//Register event handlers with OpenGL.
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(myKeyboardFunc);

    frameTimer(0);

	//And Go!
	glutMainLoop();
}
