#include "GL/freeglut.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include "vecmath.h"
using namespace std;

// Globals

// This is the list of points (3D vectors)
vector<Vector3f> vecv;

// This is the list of normals (also 3D vectors)
vector<Vector3f> vecn;

// This is the list of faces (indices into vecv and vecn)
vector<vector<unsigned>> vecf;

// You will need more global variables to implement color and position changes
int teapotColor = 0;
float lightPosX, lightPostY = 1.0f;

// These are convenience functions which allow us to call OpenGL 
// methods on Vec3d objects
inline void glVertex(const Vector3f &a) 
{ glVertex3fv(a); }

inline void glNormal(const Vector3f &a) 
{ glNormal3fv(a); }


// This function is called whenever a "Normal" key press is received.
void keyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 27: // Escape key
        exit(0);
        break;
    case 'c':
        teapotColor = (teapotColor + 1) % 4;
        break;
    default:
        cout << "Unhandled key press " << key << "." << endl;        
    }

	// this will refresh the screen so that the user sees the color change
    glutPostRedisplay();
}

// This function is called whenever a "Special" key press is received.
// Right now, it's handling the arrow keys.
void specialFunc( int key, int x, int y )
{
    switch ( key )
    {
    case GLUT_KEY_UP:
        lightPostY += .5f;
		break;
    case GLUT_KEY_DOWN:
        lightPostY -= .5f;
		break;
    case GLUT_KEY_LEFT:
        lightPosX -= .5f;
		break;
    case GLUT_KEY_RIGHT:
        lightPosX += .5f;
		break;
    }

	// this will refresh the screen so that the user sees the light position
    glutPostRedisplay();
}

void drawLoadedObject()
{
    vector<unsigned> faceVertex1 = vecf[0];
    vector<unsigned> faceVertex2 = vecf[1];
    vector<unsigned> faceVertex3 = vecf[2];

    // Each face is defined using nine numbers in the following format: a/b/c d/e/f g/h/i.

    unsigned a = faceVertex1[0];
    unsigned b = faceVertex1[1];
    unsigned c = faceVertex1[2];

    unsigned d = faceVertex2[0];
    unsigned e = faceVertex2[1];
    unsigned f = faceVertex2[2];

    unsigned g = faceVertex3[0];
    unsigned h = faceVertex3[1];
    unsigned i = faceVertex3[2];

    // a, d, g represents components of each of the 3 vertexes of a face
    // c, f, i represents normals of each of the 3 vertexes of a face

    glBegin(GL_TRIANGLES);

    // First triangle point. a and c
    glNormal3d(vecn[c-1][0], vecn[c-1][1], vecn[c-1][2]);
    glVertex3d(vecv[a-1][0], vecv[a-1][1], vecv[a-1][2]);
    cout << vecv[a - 1][0] << "," << vecv[a - 1][1] << "," << vecv[a - 1][2] << endl;


    // Second triangle point. d and f
    glNormal3d(vecn[f - 1][0], vecn[f - 1][1], vecn[f - 1][2]);
    glVertex3d(vecv[d - 1][0], vecv[d - 1][1], vecv[d - 1][2]);
	cout << vecv[d - 1][0] << "," << vecv[d - 1][1] << "," << vecv[d - 1][2] << endl;

    // Third triangle point. g and i
    glNormal3d(vecn[i - 1][0], vecn[i - 1][1], vecn[i - 1][2]);
    glVertex3d(vecv[g - 1][0], vecv[g - 1][1], vecv[g - 1][2]);
	cout << vecv[g - 1][0] << "," << vecv[g - 1][1] << "," << vecv[g - 1][2] << endl;

    glEnd();
}

// This function is responsible for displaying the object.
void drawScene(void)
{
    int i;

    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rotate the image
    glMatrixMode( GL_MODELVIEW );  // Current matrix affects objects positions
    glLoadIdentity();              // Initialize to the identity

    // Position the camera at [0,0,5], looking at [0,0,0],
    // with [0,1,0] as the up direction.
    gluLookAt(0.0, 0.0, 5.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    // Set material properties of object

	// Here are some colors you might use - feel free to add more
    GLfloat diffColors[4][4] = { {0.5, 0.5, 0.9, 1.0},
                                 {0.9, 0.5, 0.5, 1.0},
                                 {0.5, 0.9, 0.3, 1.0},
                                 {0.3, 0.8, 0.9, 1.0} };
    
	// Here we use the first color entry as the diffuse color
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColors[teapotColor]);

	// Define specular color and shininess
    GLfloat specColor[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shininess[] = {100.0};

	// Note that the specular color and shininess can stay constant
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  
    // Set light properties

    // Light color (RGBA)
    GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
    // Light position
	GLfloat Lt0pos[] = {lightPosX, lightPostY, 5.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

    drawLoadedObject();

    // Dump the image to the screen.
    glutSwapBuffers();
}

// Initialize OpenGL's rendering modes
void initRendering()
{
    glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
    glEnable(GL_LIGHTING);     // Enable lighting calculations
    glEnable(GL_LIGHT0);       // Turn on light #0.
}

// Called when the window is resized
// w, h - width and height of the window in pixels.
void reshapeFunc(int w, int h)
{
    // Always use the largest square viewport possible
    if (w > h) {
        glViewport((w - h) / 2, 0, h, h);
    } else {
        glViewport(0, (h - w) / 2, w, w);
    }

    // Set up a perspective view, with square aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 50 degree fov, uniform aspect ratio, near = 1, far = 100
    gluPerspective(50.0, 1.0, 1.0, 100.0);
}

vector<string> split(string str, char delimeter)
{
	vector<string> splitWords;
	string word;
	for (int i = 0; i < str.size(); i++)
	{
		const char letter = str[i];
		if (letter == delimeter)
		{
			splitWords.push_back(word);
			word.clear();
		}
		else
		{
			word.append(sizeof(letter), letter);
		}
	}
	splitWords.push_back(word);
	return splitWords;
}

void loadInput()
{
    // v -0.654509 0.587785 0.475528
    // vn 0.273568 -0.454044 0.847941
    // f 22 / 23 / 1 21 / 22 / 2 2 / 2 / 3
    while(true)
    {
		const int MAX_BUFFER_SIZE = 256;
		char buffer[MAX_BUFFER_SIZE];

        cin.getline(buffer, MAX_BUFFER_SIZE);
		if (cin.eof())
		{
			break;
		}

        stringstream ss(buffer);

        string token;
		ss >> token;

        if (token == "v")
        {
            // Vertices.
            float vertex1, vertex2, vertex3;
            ss >> vertex1 >> vertex2 >> vertex3;
            
            Vector3f triangleVertices = Vector3f(vertex1, vertex2, vertex3);
            vecv.push_back(triangleVertices);
        }
        else if (token == "vn")
        {
            // Normals.
			float vertex1, vertex2, vertex3;
			ss >> vertex1 >> vertex2 >> vertex3;

			Vector3f normal = Vector3f(vertex1, vertex2, vertex3);
			vecn.push_back(normal);
        }
        else if (token == "f")
        {
            // Face.
            // 3 vertices per face
            for (int i = 1; i <= 3; i++)
            {
                string vertex;
                ss >> vertex;
                vector<string> components = split(vertex, '/');
                vector<unsigned> componentsInt;
                for (string component : components)
                {
                    componentsInt.push_back(stoul(component));
                }
                vecf.push_back(componentsInt);
            }
        }
        else
        {
        }
    }
}

// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char** argv )
{
    loadInput();

    glutInit(&argc,argv);

    // We're going to animate it, so double buffer 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    // Initial parameters for window position and size
    glutInitWindowPosition( 60, 60 );
    glutInitWindowSize( 360, 360 );
    glutCreateWindow("Assignment 0");

    // Initialize OpenGL parameters.
    initRendering();

    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys

     // Set up the callback function for resizing windows
    glutReshapeFunc( reshapeFunc );

    // Call this whenever window needs redrawing
    glutDisplayFunc( drawScene );

    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop( );

    return 0;	// This line is never reached.
}
