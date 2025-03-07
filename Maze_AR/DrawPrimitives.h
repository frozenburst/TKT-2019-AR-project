
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
//#include <GL/gl.h>

#include <math.h>
#include <time.h>
#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

/* PI */
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

char b = char(219);
int w = 28; //width
int h = 28; //height
vector < vector < int > > arr = {};
vector < vector < int > > arr2 = {};
vector < string > arr3 = {};
vector < vector < int > > neigh = {};
int r = 1; //row
int c = 1; //column
vector < int > nxt = {};
int maze_create_cnt = 1;

void drawSphere(double r, int lats, int longs) {
	int i, j;
	for(i = 0; i <= lats; i++) {
		double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
		double z0  = r * sin(lat0);
		double zr0 = r *  cos(lat0);

		double lat1 = M_PI * (-0.5 + (double) i / lats);
		double z1  = r * sin(lat1);
		double zr1 = r * cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= longs; j++) {
			double lng = 2 * M_PI * (double) (j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			glNormal3f((GLfloat)(x * zr0), (GLfloat)(y * zr0), (GLfloat)z0);
			glVertex3f((GLfloat)(x * zr0), (GLfloat)(y * zr0), (GLfloat)z0);
			glNormal3f((GLfloat)(x * zr1), (GLfloat)(y * zr1), (GLfloat)z1);
			glVertex3f((GLfloat)(x * zr1), (GLfloat)(y * zr1), (GLfloat)z1);
		}
		glEnd();
	}
}


void solidCone(GLdouble base, GLdouble height, GLint slices, GLint stacks)
{
	glBegin(GL_LINE_LOOP);
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);

	gluCylinder(quadric, base, 0, height, slices, stacks);

	gluDeleteQuadric(quadric);
	glEnd();
}

// Added in Exercise 9 - Start *****************************************************************
void drawSnowman( bool female = 0)
{
	glRotatef( -90, 1, 0, 0 );
	glScalef(0.03f, 0.03f, 0.03f);

	// draw 3 white spheres
	glColor4f( 1.0, 1.0, 1.0, 1.0 );
	drawSphere( 0.8, 10, 10 );
	glTranslatef( 0.0f, 0.8f, 0.0f );
	drawSphere( 0.6, 10, 10 );
	if(female)
	{
		glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glTranslatef(-0.2f, 0.05f, 0.3f);
		drawSphere( 0.32, 10, 10 );
		glTranslatef(0.4f, 0, 0);
		drawSphere( 0.32, 10, 10 );
		glPopMatrix();
	}
	glTranslatef( 0.0f, 0.6f, 0.0f );
	drawSphere( 0.4, 10, 10 );

	// draw the eyes
	glPushMatrix();
	glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );
	glTranslatef( 0.2f, 0.2f, 0.2f );
	drawSphere( 0.066, 10, 10 );
	glTranslatef( 0, 0, -0.4f );
	drawSphere( 0.066, 10, 10 );
	glPopMatrix();

	// draw a nose
	glColor4f( 1.0f, 0.5f, 0.0f, 1.0f );
	glTranslatef( 0.3f, 0.0f, 0.0f );
	glRotatef( 90, 0, 1, 0 );
	solidCone( 0.1, 0.3, 10, 10 );
}
// Added in Exercise 9 - End *****************************************************************

void create() {
    neigh.clear();
    //== counter to end recursive call quickly
    if (maze_create_cnt >= (h / 2) * (w / 2)) return;
    
    //==== getting the unvisited neighbors
    if (r + 2 < h - 1 && arr[r + 2][c] == 0) {
        neigh.push_back({r + 2, c}); //down
    }
    if (r - 2 > 0 && arr[r - 2][c] == 0) {
        neigh.push_back({r - 2, c}); //up
    }
    if (c + 2 < w - 1 && arr[r][c + 2] == 0) {
        neigh.push_back({r, c + 2}); //right
    }
    if (c - 2 > 0 && arr[r][c - 2] == 0) {
        neigh.push_back({r, c - 2}); //left
    }
    //=== forward
    if (neigh.size() > 0) {
        nxt.clear();
        nxt = neigh[rand() % neigh.size()];
        arr[nxt[0]][nxt[1]] = 2; //mark it as visited
        maze_create_cnt++; //counter to know if all cells are already visited
        arr2.push_back(nxt); //push to the stack
        
        //remove the walls
        if (c - nxt[1] < 0) {
            arr[r][c + 1] = 0;
            c += 2;
            create();
        } else if (c - nxt[1] > 0) {
            arr[r][c - 1] = 0;
            c -= 2;
            create();
        } else if (r - nxt[0] < 0) {
            arr[r + 1][c] = 0;
            r += 2;
            create();
        } else if (r - nxt[0] > 0) {
            arr[r - 1][c] = 0;
            r -= 2;
            create();
        }
    }
    //==== backtrack
    else if (arr2.size() > 0) {
        arr2.pop_back();
        r = arr2[arr2.size() - 1][0];
        c = arr2[arr2.size() - 1][1];
        create();
    }
}

vector < vector < int > > generate_maze() {
    //adjusting the size
    if (h % 2 == 0) h++;
    if (w % 2 == 0) w++;
    
    srand((unsigned int)(time(NULL)));
    
    //===== creating the base of the maze
    arr.resize(h);
    for (int j = 0; j < h; j++) {
        arr[j].resize(w);
        for (int i = 0; i < w; i++) {
            if (j % 2 == 1 && i % 2 == 1) {
                arr[j][i] = 0;
            } else if ((j == 0 && i == 1) || (j == h - 1 && i == w - 2)) {
                arr[j][i] = 0;
            } else {
                arr[j][i] = 1;
            }
        }
    }
    //==== starting point of maze creation using depth-first search and recursive backtracking
    arr[r][c] = 2;
    arr2.push_back({r, c});
    
    //=== start of recursive call
    create();
    return arr;
}


void drawCuboid(float width, float height, float depth)
{
    glBegin(GL_QUADS);
    
    //前
    glNormal3f(0.0,0.0,-1.0);
    glVertex3f(width/2,height/2,depth/2);
    glVertex3f(-width/2,height/2,depth/2);
    glVertex3f(-width/2,-height/2,depth/2);
    glVertex3f(width/2,-height/2,depth/2);
    
    //左
    glNormal3f(1.0,0.0,0.0);
    glVertex3f(width/2,height/2,depth/2);
    glVertex3f(width/2,height/2,-depth/2);
    glVertex3f(width/2,-height/2,-depth/2);
    glVertex3f(width/2,-height/2,depth/2);
    
    //右
    glNormal3f(-1.0,0.0,0.0);
    glVertex3f(-width/2,height/2,-depth/2);
    glVertex3f(-width/2,height/2,depth/2);
    glVertex3f(-width/2,-height/2,depth/2);
    glVertex3f(-width/2,-height/2,-depth/2);
    
    //上
    glNormal3f(0.0,1.0,0.0);
    glVertex3f(width/2,height/2,depth/2);
    glVertex3f(-width/2,height/2,depth/2);
    glVertex3f(-width/2,height/2,-depth/2);
    glVertex3f(width/2,height/2,-depth/2);
    
    //下
    glNormal3f(0.0,-1.0,0.0);
    glVertex3f(width/2,-height/2,depth/2);
    glVertex3f(-width/2,-height/2,depth/2);
    glVertex3f(-width/2,-height/2,-depth/2);
    glVertex3f(width/2,-height/2,-depth/2);
    glEnd();
    
    //後
    glNormal3f(0.0,0.0,1.0);
    glVertex3f(width/2,height/2,-depth/2);
    glVertex3f(-width/2,height/2,-depth/2);
    glVertex3f(-width/2,-height/2,-depth/2);
    glVertex3f(width/2,-height/2,-depth/2);
}

void drawMaze(vector<vector<int>>& maze){
    // base
    glColor4f(1,1,1,1);
    drawCuboid(0.028f, 0.028f, 0.001f);
    // wall
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            if (maze[j][i] == 1) {
                glPushMatrix();
                glColor4f(1,0,0,1);
                glTranslatef( -0.0135f + j * 0.001f, 0.0135f + i * (-0.001f), 0.001f );
                drawCuboid(0.001f, 0.001f, 0.001f);
                glPopMatrix();
            } else if (j == 0 && i == 1) {//start
                glPushMatrix();
                glColor4f(0,1,1,1);
                glTranslatef( -0.0135f + j * 0.001f, 0.0135f + i * (-0.001f), 0.001f );
                drawCuboid(0.001f, 0.001f, 0.001f);
                glPopMatrix();
            } else if (j == h - 1 && i == w - 2) {//end
                glPushMatrix();
                glColor4f(1,1,0,1);
                glTranslatef( -0.0135f + j * 0.001f, 0.0135f + i * (-0.001f), 0.001f );
                drawCuboid(0.001f, 0.001f, 0.001f);
                glPopMatrix();
            }
        }
    }
}
