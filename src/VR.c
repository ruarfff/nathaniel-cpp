/**
 * CS4404 Assignment 3
 * Virtual World - The Return Of Karel The Robot
 *
 * Mark Kavanagh - mbk1
 * Ashruff Mahadi - am15
 * George Boyle - gb3
 */

#include <GL/glut.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Colours and program constant */

#define DEG_TO_RAD 0.0174532925
#define FORWARDS   1
#define BACKWARDS -1
#define LINE_WIDTH 1

#define GRID_LINE_COLOUR 0.0, 1.0, 0.0
#define GRID_LINE_WIDTH 2
#define GRID_SIZE 21

#define BLACK 0.0, 0.0, 0.0
#define WHITE 1.0, 1.0, 1.0
#define RED 1.0, 0.0, 0.0
#define GREEN 0.0, 1.0, 0.0
#define BLUE 0.0, 0.0, 1.0
#define YELLOW 1.0, 1.0, 0.0

#define BOUNDARY_COLOUR 0.5, 0.5, 0.7
#define DOOR_COLOUR 0.5, 0.41, 0.4
#define SEA_COLOUR_1 BLUE
#define SEA_COLOUR_2 0.2, 0.2, 1.0
#define SEA_COLOUR_3 0.1, 0.7, 0.9
#define SEA_COLOUR_4 0.0, 0.0, 0.8
#define GRID_FLOOR_COLOUR 0.6, 0.6, 0.6
#define GRASS_COLOUR_1 0.248, 0.784, 0.22
#define GRASS_COLOUR_2 0.248, 0.784, 0.22
#define GRASS_COLOUR_3 0.248, 0.784, 0.22
#define GRASS_COLOUR_4 0.248, 0.784, 0.22
#define TUNNEL_COLOUR 0.6, 0.6, 0.7
#define BEACH_COLOUR_1 0.94, 0.9, 0.63
#define BEACH_COLOUR_2 0.8, 0.8, 0.0
#define SUN_COLOUR 0.97, 0.81, 0.53
#define CLOUD_COLOUR 0.9, 0.97, 0.97 
#define ARROW_COLOUR YELLOW
#define HUD_COLOUR YELLOW
#define FOREGROUND_TREE_COLOUR 0.24, 0.79, 0.22
#define BACKGROUND_TREE_COLOUR 0.08, 0.53, 0.055
#define FOREGROUND_TREE_TRUNK_COLOUR 0.51, 0.41, 0.05
#define BACKGROUND_TREE_TRUNK_COLOUR 0.364, 0.345, 0.067
#define TABLE_BASE_COLOUR 1.0, 0.9, 0.9
#define TABLE_TOP_COLOUR 0.98, 0.08, 0.145
#define TEAPOT_COLOUR YELLOW
#define DOG_COLOUR 0.9, 0.86, 0.62
#define DOG_FACE_COLOUR WHITE
#define DOG_NOSE_COLOUR BLACK
#define DOG_EYE_COLOUR WHITE
#define DOG_PUPIL_COLOUR BLACK
#define DOG_COLLAR_COLOUR RED
#define DOG_PAW_COLOUR WHITE
#define DOG_BOWL_COLOUR 0.19, 0.69, 0.94
#define DOG_BOWL_BASE_COLOUR 0.09, 0.63, 0.79
#define DOGHOUSE_COLOUR 0.97, 0.97, 0.49
#define DOGHOUSE_DOOR_COLOUR BLACK
#define DOGHOUSE_ROOF_COLOUR 0.95, 0.1, 0.1
#define ESC '\033'
#define GROUND_LEVEL -5.0

#define GRID_STEP_SIZE 10.0
#define RUN_STEP_SIZE 5.0
#define WALK_STEP_SIZE 1.0

bool karelAtWork = true;
bool foreground = true;
bool overhead = false;

/* State variables to control the position, step size,
 * viewing direction and viewing angles up-direction */
double xpos = 0.0,  ypos = 6.0,  zpos = 0.0;
double xat  = 1.0,  yat  = 0.0,  zat  = 0.0;
double xup  = 0.0,  yup  = 1.0,  zup  =  0.0;

double angle     = 0.0,  stepSize = GRID_STEP_SIZE, 
       tiltAngle = 0.02, rotationDelta = 3;

/* State variables for handling beepers */
int beepers[GRID_SIZE][GRID_SIZE];
GLUquadricObj *quad;

/* State variable for handling cloud movement */
int lastFrameTime = 0;
double cloudX, cloudY, cloudZ;

#define BOUNDARY_HEIGHT 30
#define BOUNDARY_VISIBLE 1
#define BOUNDARY_INVISIBLE 0

const double boundaries[][5] = {
    /* Grid Boundaries */
    {0, -305, 90, -305, BOUNDARY_VISIBLE},
    {90, -305, 90, -205, BOUNDARY_VISIBLE},
    {90, -205, -5, -205, BOUNDARY_VISIBLE},
    {-5, -205, -5, 205, BOUNDARY_VISIBLE},
    {-5, 205, 205, 205, BOUNDARY_VISIBLE},
    {205, 205, 205, -205, BOUNDARY_VISIBLE},
    {205, -205, 110, -205, BOUNDARY_VISIBLE},
    {110, -205, 110, -305, BOUNDARY_VISIBLE},
    {110, -305, 200, -305, BOUNDARY_VISIBLE},
    {200, -305, 200, -405, BOUNDARY_INVISIBLE},
    {200, -405, 0, -405, BOUNDARY_INVISIBLE},
    {0, -405, 0, -305, BOUNDARY_INVISIBLE},
    {205, -5, 15, -5, BOUNDARY_VISIBLE},
    /* Table Boundaries */
    {85, -100, 90, -110, BOUNDARY_INVISIBLE},
    {90, -110, 100, -115, BOUNDARY_INVISIBLE},
    {100, -115, 110, -110, BOUNDARY_INVISIBLE},
    {110, -110, 115, -100, BOUNDARY_INVISIBLE},
    {115, -100, 110, -90, BOUNDARY_INVISIBLE},
    {110, -90, 100, -85, BOUNDARY_INVISIBLE},
    {100, -85, 90, -90, BOUNDARY_INVISIBLE},
    {90, -90, 85, -100, BOUNDARY_INVISIBLE},  
    /* Doghouse Boundaries */
    {48, -112, 57, -133, BOUNDARY_INVISIBLE},  
    {57, -133, 44, -150, BOUNDARY_INVISIBLE},  
    {44, -150, 32, -139, BOUNDARY_INVISIBLE},  
    {32, -139, 48, -112, BOUNDARY_INVISIBLE},  
};

#define DOOR_OPEN 1
#define DOOR_CLOSED 0

double doors[][5] = {
    {-5, -5, 15, -5, DOOR_CLOSED}, {90, -205, 110, -205, DOOR_CLOSED}
};

void reLook();

/********************/
/* Action Functions */
/********************/

/*
 * Check if the grid is clear of beepers
 */
bool gridEmpty() {
    int i, j;
    for (i = 0; i < GRID_SIZE; ++i)
        for (j = 0; j < GRID_SIZE; ++j)
            if (beepers[i][j] != 0)
                return false;
    return true;
}

/*
 * Put down a beeper at the current grid position, up to a maximum of ten
 * beepers in any one grid position
 */
void putBeeper() {
    int x = (int) xpos / 10;
    int z = (int) zpos / 10;
    if (karelAtWork && beepers[x][z] < 10)
        beepers[x][z]++;
}

/*
 * Pick up a beeper from the current grid position, if there is one
 */
void pickBeeper() {
    void changeWorld(void);
    int x = (int) xpos / 10;
    int z = (int) zpos / 10;
    if (karelAtWork && beepers[x][z] > 0)
        beepers[x][z]--;
    if (gridEmpty())
        changeWorld();
}

/*
 * Clear all beepers from the grid
 */
void clearBeepers() {
    void changeWorld(void);
    int i, j;
    for (i = 0; i < GRID_SIZE; ++i)
        for (j = 0; j < GRID_SIZE; ++j)
            beepers[i][j] = 0;
    changeWorld();
}

/*
 * Check if the lines defined by {(x1,y1),(x2,y2)} and {(x3,y3),(x4,y4)}
 * intersect
 */
bool linesIntersect(double x1, double y1, double x2, double y2, double x3,
        double y3, double x4, double y4) {
    double point1, point2;
    if (x1 == x2)
        return (x3 < x1 && x4 >= x1) || (x3 > x1 && x4 <= x1);
    point1 = (y2 - y1) * (x3 - x1) / (x2 - x1) + y1 - y3;
    point2 = (y2 - y1) * (x4 - x1) / (x2 - x1) + y1 - y4;
    return (point1 < 0 && point2 >= 0) || (point1 > 0 && point2 <= 0);
}

/*
 * Check if either of the points (x3,y3) or (x4,y4) are in the x or y range
 * of the line segment [(x1,y1),(x2,y2)]
 */
bool pointsInRange(double x1, double y1, double x2, double y2, double x3,
        double y3, double x4, double y4) {
    double minx = x1 < x2 ? x1 : x2;
    double maxx = x1 > x2 ? x1 : x2;
    double miny = y1 < y2 ? y1 : y2;
    double maxy = y1 > y2 ? y1 : y2;
    if (maxx - minx > stepSize)
        return (minx <= x3 && x3 <= maxx) || (minx <= x4 && x4 <= maxx);
    else
        return (miny <= y3 && y3 <= maxy) || (miny <= y4 && y4 <= maxy);
}

/*
 * Check if the movement from point (x1,y1) to (x2,y2) would cause a
 * collision
 */
bool isCollision(double x1, double y1, double x2, double y2) {
    int i;
    for (i = 0; i < sizeof(boundaries) / (sizeof(double) * 5); ++i) {
        if (linesIntersect(boundaries[i][0], boundaries[i][1],
                    boundaries[i][2], boundaries[i][3], x1, y1, x2, y2)
                && pointsInRange(boundaries[i][0], boundaries[i][1],
                    boundaries[i][2], boundaries[i][3], x1, y1, x2, y2)) {
            return true;
        }
    }
    for (i = 0; i < sizeof(doors) / (sizeof(double) * 5); ++i) {
        if (doors[i][4] == DOOR_CLOSED
                && linesIntersect(doors[i][0], doors[i][1], doors[i][2],
                    doors[i][3], x1, y1, x2, y2)
                && pointsInRange(doors[i][0], doors[i][1], doors[i][2],
                    doors[i][3], x1, y1, x2, y2)) {
            return true;
        }
    }
    return false;
}

/*
 * Move 'direction'-ways along the xz plane
 */
void move(int direction) {
    double newx, newz;
    newx = xpos + xat * stepSize * direction;
    newz = zpos + zat * stepSize * direction;
    if (! isCollision(xpos, zpos, newx, newz)) {
        xpos = newx;
        zpos = newz;
    }
    reLook();
}

/*
 * Move left or right while still facing the same way
 */
void strafe(int direction) {
    if (! karelAtWork) {
        double xComp = cos(DEG_TO_RAD * (angle - 90));
        double zComp = sin(DEG_TO_RAD * (angle - 90));
        double newx, newz;
        newx = xpos + xComp * stepSize * direction;
        newz = zpos + zComp * stepSize * direction;
        if (! isCollision(xpos, zpos, newx, newz)) {
            xpos = newx;
            zpos = newz;
        }
    }
}

/*
 * Set the current step size
 */
    void setStepSize(double size) {
        if (! karelAtWork)
            stepSize = size;
    }

/*
 * Rotate the camera by 'rotationDelta' degrees
 */
void rotateCameraBy(double rotationDelta){
    double radAngle;
    angle += rotationDelta;
    if (angle >= 360)
        angle -= 360;
    if (angle < 0)
        angle += 360;
    radAngle = DEG_TO_RAD * angle;
    zat = sin(radAngle);
    xat = cos(radAngle);
}

/**************************/
/* View Control Functions */
/**************************/

/*
 * Change the world to turn on free movement and lighting
 */
void changeWorld(void) {
    glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT2);
    glClearColor(0.5, 0.5, 1.0, 0.0);
    karelAtWork = false;
    doors[0][4] = DOOR_OPEN;
    setStepSize(RUN_STEP_SIZE);
}

/*
 * Switch to overhead view
 */
void overheadView() {
    ypos = 300;
    yat = -50.0;
    overhead = true;
}

/*
 * Switch to normal view
 */
void normalView() {
    ypos = 5.0;
    yat = 0.0;
    overhead = false;
}

/*
 * Refresh the view
 */
void reLook() {
    glLoadIdentity();
    gluLookAt(xpos, ypos, zpos, xpos + xat, ypos + yat, zpos + zat,
            xup, yup, zup);
    glutPostRedisplay();
}

/*********************/
/* Drawing Functions */
/*********************/

/*
 * Draw beepers on the grid
 */
void drawBeepers() {
    int x, z, n;
    glColor3f(1.0, 0.0, 0.0);
    for (x = 0; x < GRID_SIZE; ++x)
        for (z = 0; z < GRID_SIZE; ++z)
            for (n = 0; n < beepers[x][z]; ++n) {
                glPushMatrix();
                glTranslatef(10.0 * x, GROUND_LEVEL + 0.3 * (n + 2), 10.0 * z);
                glRotatef(90.0, 1.0, 0.0, 0.0);
                gluDisk(quad, 0.0, 2.0, 12, 12);
                glPopMatrix();
            }
}

/*
 * Draw a bone
 */
void drawBone() {
    glColor3f(WHITE);
    glPushMatrix();
    glTranslatef(-130, -3, -130);
    glRotatef(30, 0.0, 0.0, 1.0);
    gluCylinder(quad, 10, 10, 3, 10, 10);
    glPopMatrix();
}

/*
 * Draw a bowl half-filled with water
 */
void drawBowl() {
    glPushMatrix();
    glRotatef(90, -1.0, 0.0, 0.0);
    glColor3f(DOG_BOWL_COLOUR);
    gluCylinder(quad, 2.5, 3.5, 3, 30, 30);   // Bowl
    glColor3f(DOG_BOWL_BASE_COLOUR);
    gluDisk(quad, 0, 2.5, 20, 20);   // Base
    glColor4f(DOG_BOWL_BASE_COLOUR, 0.5);
    glTranslatef(0.0, 0.0, 1.5);
    gluDisk(quad, 0, 3.0, 20, 20);   // Water
    glPopMatrix();
}

/*
 * Draw a dog
 */
void drawDog() {
    /* Body */
    glPushMatrix();
    glColor3f(DOG_COLOUR);
    glutSolidSphere(3.5, 10.0, 10.0);
    glTranslatef(0.0, 0.0, -3.0);
    glutSolidSphere(3.5, 10.0, 10.0);
    glTranslatef(0.0, 0.0, -3.0);
    glutSolidSphere(3.5, 10.0, 10.0);

    /* Front Left Paw */
    glPushMatrix();
    glTranslatef(2, -2, 7);
    gluCylinder(quad, 1, 1, 6, 15, 15);
    glColor3f(DOG_PAW_COLOUR);
    glTranslatef(0, 0, 6);
    glutSolidSphere(1.2, 10, 10);
    glPopMatrix();

    /* Front Right Paw */
    glColor3f(DOG_COLOUR);
    glPushMatrix();
    glTranslatef(-1.6, -2, 8.5);
    gluCylinder(quad, 1, 1, 6, 15, 15);
    glColor3f(DOG_PAW_COLOUR);
    glTranslatef(0, 0, 6);
    glutSolidSphere(1.2, 10, 10);
    glPopMatrix();

    /* Front Left Leg */
    glPushMatrix();
    glTranslatef(-137, -4.8, -133);
    glRotatef(45, 0.0, 1.0, 0.0);
    gluCylinder(quad, 1, 1, 6, 15, 15);
    glPopMatrix();

    /* Front Right Paw */
    glPushMatrix();
    glTranslatef(-136, -5, -128);
    glutSolidSphere(1.2, 10, 10);
    glPopMatrix();

    /* Neck/Collar */
    glColor3f(DOG_COLLAR_COLOUR);
    glTranslatef(0, 0.6, 7.4);
    gluCylinder(quad, 2, 0.9, 5, 15, 15);

    /* Head */
    glColor3f(DOG_COLOUR);
    glTranslatef(0.0, 2.0, 5.0);
    glutSolidSphere(3.5, 10.0, 10.0);

    /* Face */
    glColor3f(DOG_FACE_COLOUR);
    glPushMatrix();
    glTranslatef(0, -1.5, 1.6);
    glutSolidSphere(2, 10, 10);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, -1.1, 2.3);
    gluCylinder(quad, 2, 1, 3, 15, 15);
    glTranslatef(0, 0, 2.8);
    glutSolidSphere(1.1, 10, 10);
    glTranslatef(0, 0, 1.1);
    glColor3f(DOG_NOSE_COLOUR);
    glutSolidSphere(0.5, 10.0, 10.0);
    glPopMatrix();

    /* Left Eye */
    glPushMatrix();
    glColor3f(DOG_EYE_COLOUR);
    glTranslatef(0.8, 0.7, 2.8);
    glutSolidSphere(1.0, 10.0, 10.0);
    glColor3f(DOG_PUPIL_COLOUR);
    glTranslatef(0.0, 0.0, 0.65);
    glutSolidSphere(0.5, 10.0, 10.0);
    glPopMatrix();

    /* Right Eye */
    glPushMatrix();
    glColor3f(DOG_EYE_COLOUR);
    glTranslatef(-0.8, 0.7, 2.8);
    glutSolidSphere(1.0, 10.0, 10.0);
    glColor3f(DOG_PUPIL_COLOUR);
    glTranslatef(0.0, 0.0, 0.65);
    glutSolidSphere(0.5, 10.0, 10.0);
    glPopMatrix();

    /* Left Ear */
    glPushMatrix();
    glColor3f(DOG_COLOUR);
    glTranslatef(2.0, 2.0, 0.2);
    glRotatef(65, 0.0, 0.0, -1.0);
    glRotatef(40, -1.0, 0.0, 0.0);
    glutSolidCone(1, 4, 10, 10);
    glPopMatrix();

    /* Right Ear */
    glPushMatrix();
    glColor3f(DOG_COLOUR);
    glTranslatef(-2.0, 2.0, 0.2);
    glRotatef(34, 0.0, 0.0, 1.0);
    glRotatef(55, -1.0, 0.0, 0.0);
    glutSolidCone(1, 4, 10, 10);
    glPopMatrix();

    glPopMatrix();
}

/*
 * Draw a dog-house
 */
void drawDogHouse() {

    char name[] = "Joe";
    char *ptr;

    glColor3f(DOGHOUSE_COLOUR);

    /* Main house */
    glPushMatrix();
    glTranslatef(0.0, 7.5, 0.0);
    glutSolidCube(15);
    glPopMatrix();

    /* Front and rear facades */
    glPushMatrix();
    glTranslatef(0.0, 15.0, -7.5);
    glBegin(GL_TRIANGLES);
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(-7.5, 0.0, 0.0);
    glVertex3f(7.5, 0.0, 0.0);
    glVertex3f(0.0, 6.0, 0.0);
    glEnd();
    glTranslatef(0.0, 0.0, 15.0);
    glBegin(GL_TRIANGLES);
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(-7.5, 0.0, 0.0);
    glVertex3f(7.5, 0.0, 0.0);
    glVertex3f(0.0, 6.0, 0.0);
    glEnd();
    glPopMatrix();

    /* Door */
    glPushMatrix();
    glColor3f(DOGHOUSE_DOOR_COLOUR);
    glTranslatef(0.0, 0.0, 7.6);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(-4.0, 0.0, 0.0);
    glVertex3f(-4.0, 9.0, 0.0);
    glVertex3f(4.0, 9.0, 0.0);
    glVertex3f(4.0, 0.0, 0.0);
    glEnd();
    glTranslatef(0.0, 9.0, 0.0);
    gluPartialDisk(quad, 0, 4, 30, 30, 270, 180);
    glPopMatrix();

    /* Roof */
    glPushMatrix();
    glColor3f(DOGHOUSE_ROOF_COLOUR);
    glTranslatef(-7.5, 15.0, 0.0);
    glRotatef(38.66, 0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, -7.5);
    glVertex3f(9.6, 0.0, -7.5);
    glVertex3f(9.6, 0.0, 7.5);
    glVertex3f(0.0, 0.0, 7.5);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(7.5, 15.0, 0.0);
    glRotatef(38.66, 0.0, 0.0, -1.0);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, -7.5);
    glVertex3f(-9.6, 0.0, -7.5);
    glVertex3f(-9.6, 0.0, 7.5);
    glVertex3f(0.0, 0.0, 7.5);
    glEnd();
    glPopMatrix();

    /* Name */
    glPushMatrix();
    glColor3f(DOGHOUSE_ROOF_COLOUR);
    glTranslatef(-3.2, 14, 7.6);
    glScalef(0.02, 0.02, 0.5);
    for (ptr = name; *ptr; ++ptr)
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *ptr);
    glPopMatrix();
}

/*
 * Draw a dog with his doghouse and bowl
 */
void drawDogAndHouse() {
    glPushMatrix();
    glTranslatef(10, 0, 10);
    drawDogHouse();

    glPushMatrix();
    glTranslatef(-12.0, 0.0, 17.5);
    drawBowl();
    glPopMatrix();

    glTranslatef(0.0, 2.2, 9.0);
    drawDog();
    glPopMatrix();
}

/*
 * Draw a sun
 */
void drawSun(){
    glColor4f(SUN_COLOUR, 0.5);
    glColor3f(SUN_COLOUR);
    glPushMatrix();
    glTranslatef(100.0, 200, -750.0);
    glutSolidSphere(30, 20, 20);
    glPopMatrix();
}

/*
 * Draw a cloud
 */
void drawCloud(double xTrans, double yTrans, double zTrans){
    //(double cloudX, double cloudY, double cloudZ){
    glColor3f(CLOUD_COLOUR);

    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    //cloudX += 0.3;

    if(cloudX > windowWidth)
        cloudX -= windowWidth;

    glPushMatrix();
    glTranslatef(cloudX, 10.0, 0.0);
    glTranslatef(xTrans, yTrans, zTrans);
    glRotatef(90.0, 0.0, 1.0, 0.0);

    glTranslatef(0.0, 0.0, 0.0);
    glutSolidSphere(3, 10, 10);

    glTranslatef(0.0, 2.5, 4.0);
    glutSolidSphere(3.5, 10, 10);

    glTranslatef(0.0,-4.5, 0.0);
    glutSolidSphere(3.0, 10, 10);

    glTranslatef(-1.0, 4.5, 3.0);
    glutSolidSphere(4.5, 10, 10);

    glTranslatef(-1.0,-4.0, 2.0);
    glutSolidSphere(4.3, 10, 10);

    glTranslatef(0.0, 4.0, 2.0);
    glutSolidSphere(4, 10, 10);

    glTranslatef(0.0,-3.0, 2.0);
    glutSolidSphere(3, 10, 10);

    glTranslatef(0.0, 1.0, 2.0);
    glutSolidSphere(3, 10, 10);

    glPopMatrix();

    reLook();
}

/*
 * Draw Clouds in the sky 
 */
void drawClouds(){
    drawCloud( 200, 200,-160);
    drawCloud( 150, 220,-200);
    drawCloud( 100, 140,-250);
    drawCloud(-100, 160,-740);
    drawCloud(-160, 310,-700);
    drawCloud(-230, 200,-780);
    drawCloud(-290, 160,-940);
    drawCloud(  90, 185,-270);
    drawCloud(  80, 250,-320);
    drawCloud(  60, 130,-340);
    drawCloud(  30, 300,-380);
    drawCloud(  20, 180,-480);
    drawCloud(   0, 110,-540);
    drawCloud( -20, 310,-680);
    drawCloud( -50, 240,-680);
}

/*
 * Draw a heads-up display when Karel is in restricted mode
 */
void drawHUD() {
    int x = (int) xpos / 10;
    int z = (int) zpos / 10;
    int numBeepers = beepers[x][z];
    char locStr[] = "Location: (";
    char beepStr[] = "Beepers:  ";
    char *ptr;

    glPushMatrix();
    glColor3f(HUD_COLOUR);
    glTranslatef(xpos, 0.0, zpos);
    glRotatef(angle, 0.0, -1.0, 0.0);
    glTranslatef(1.0, ypos + 0.5, 0.2);
    glRotatef(90, 0.0, -1.0, 0.0);
    glScalef(0.0002, 0.0002, 0.0);
    for (ptr = locStr; *ptr; ++ptr)
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *ptr);
    if (x >= 10)
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, '0' + (char) (x / 10));
    glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, '0' + (char) (x % 10));
    glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, ',');
    if (z >= 10)
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, '0' + (char) (z / 10));
    glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, '0' + (char) (z % 10));
    glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, ')');
    glPopMatrix();
    glPushMatrix();
    glTranslatef(xpos, 0.0, zpos);
    glRotatef(angle, 0.0, -1.0, 0.0);
    glTranslatef(1.0, ypos + 0.45, 0.2);
    glRotatef(90, 0.0, -1.0, 0.0);
    glScalef(0.0002, 0.0002, 0.0);
    for (ptr = beepStr; *ptr; ++ptr)
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *ptr);
    if (numBeepers >= 10)
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, '0'
                + (char) (numBeepers / 10));
    glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, '0' + (char) (numBeepers % 10));
    glPopMatrix();
}

/*
 * Draw a tree
 */
void drawTree() {
    glPushMatrix();
    glRotatef(90.0, -1.0, 0.0, 0.0);
    if (foreground)
        glColor3f(FOREGROUND_TREE_TRUNK_COLOUR);
    else
        glColor3f(BACKGROUND_TREE_TRUNK_COLOUR);
    gluCylinder(quad, 5, 5, 5, 15, 15);
    if (foreground)
        glColor3f(FOREGROUND_TREE_COLOUR);
    else
        glColor3f(BACKGROUND_TREE_COLOUR);
    glTranslatef(0.0, 0.0, 5.0);
    glutSolidCone(10, 20, 15, 15);
    glTranslatef(0.0, 0.0, 10.0);
    glutSolidCone(10, 20, 15, 15);
    glTranslatef(0.0, 0.0, 10.0);
    glutSolidCone(10, 20, 15, 15);
    glPopMatrix();
}

/*
 * Draw a forest of trees
 */
void drawTrees() {
    int i;
    foreground = false;

    glPushMatrix();
    glTranslatef(10.0, GROUND_LEVEL, -185.0);

    glPushMatrix();
    for (i = 0; i < 4; ++i) {
        drawTree();
        glTranslatef(0.0, 0.0, 40.0);
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(180.0, 0.0, 0.0);
    for (i = 0; i < 5; ++i) {
        drawTree();
        glTranslatef(0.0, 0.0, 40.0);
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(150.0, 0.0, 170.0);
    for (i = 0; i < 4; ++i) {
        drawTree();
        glTranslatef(-40.0, 0.0, 0.0);
    }
    glPopMatrix();

    foreground = true;

    glPushMatrix();
    glTranslatef(150.0, 0.0, 20.0);
    for (i = 0; i < 3; ++i) {
        drawTree();
        glTranslatef(0.0, 0.0, 40.0);
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(130.0, 0.0, 140.0);
    for (i = 0; i < 3; ++i) {
        drawTree();
        glTranslatef(-40.0, 0.0, 0.0);
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(30.0, 0.0, 20.0);
    drawTree();
    glTranslatef(30.0, 0.0, -20.0);
    drawTree();
    glTranslatef(60.0, 0.0, 0.0);
    drawTree();
    glPopMatrix();

    glPopMatrix();
}

/*
 * Draw a table with a teapot
 */
void drawTable() {
    glPushMatrix();

    glPushMatrix();
    glRotatef(90.0, -1.0, 0.0, 0.0);
    glColor3f(TABLE_BASE_COLOUR);
    gluCylinder(quad, 4, 4, 5, 10, 10);   /* Base */
    glColor3f(TABLE_TOP_COLOUR);
    glTranslatef(0.0, 0.0, 5.0);
    gluDisk(quad, 0, 15, 35, 35);   /* Top */

    /* Plates */
    glColor3f(WHITE);
    glTranslatef(0.0, 0.0, 0.2);

    glPushMatrix();
    glTranslatef(-8.0, -1.0, 0.0);
    gluDisk(quad, 0, 2, 35, 35);
    gluCylinder(quad, 2, 2.5, 0.4, 35, 35);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(9.0, 0.8, 0.0);
    gluDisk(quad, 0, 2, 35, 35);
    gluCylinder(quad, 2, 2.5, 0.4, 35, 35);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2.0, -10.8, 0.0);
    gluDisk(quad, 0, 2, 35, 35);
    gluCylinder(quad, 2, 2.5, 0.4, 35, 35);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 10.0, 0.0);
    gluDisk(quad, 0, 2, 35, 35);
    gluCylinder(quad, 2, 2.5, 0.4, 35, 35);
    glPopMatrix();

    glPopMatrix();

    /* Teapot */
    glColor3f(TEAPOT_COLOUR);
    glTranslatef(0.0, 6.5, 0.0);
    glutSolidTeapot(2.0);

    glPopMatrix();
}

/*
 * Draw the main world
 */
void drawGrid() {
    int x, z = 200;

    glPushMatrix();
    glLineWidth(GRID_LINE_WIDTH);
    glColor3f(GRID_LINE_COLOUR);

    glBegin(GL_LINES);
    for(x = 0; x < GRID_SIZE * 10; x +=10){
        glVertex3f(x, GROUND_LEVEL + 0.3, -5);
        glVertex3f(x, GROUND_LEVEL + 0.3, z + 5);
        glVertex3f(-5, GROUND_LEVEL + 0.3, x);
        glVertex3f(z + 5, GROUND_LEVEL + 0.3, x);
    }
    glEnd();

    glLineWidth(LINE_WIDTH);
    glPopMatrix();
}

/*
 * Draw the boundaries and doors for the entire world
 */
void drawBoundaries() {
    int i;

    glBegin(GL_QUADS);

    /* Main boundaries */
    glColor3f(BOUNDARY_COLOUR);
    for (i = 0; i < sizeof(boundaries) / (sizeof(double) * 5); ++i) {
        if (boundaries[i][4] == BOUNDARY_VISIBLE) {
            glVertex3i(boundaries[i][0], GROUND_LEVEL, boundaries[i][1]);
            glVertex3i(boundaries[i][0], BOUNDARY_HEIGHT, boundaries[i][1]);
            glVertex3i(boundaries[i][2], BOUNDARY_HEIGHT, boundaries[i][3]);
            glVertex3i(boundaries[i][2], GROUND_LEVEL, boundaries[i][3]);
        }
    }

    /* Doors */
    glColor3f(DOOR_COLOUR);
    for (i = 0; i < sizeof(doors) / (sizeof(double) * 5); ++i) {
        if (doors[i][4] == DOOR_CLOSED) {
            glVertex3i(doors[i][0], GROUND_LEVEL, doors[i][1]);
            glVertex3i(doors[i][0], BOUNDARY_HEIGHT, doors[i][1]);
            glVertex3i(doors[i][2], BOUNDARY_HEIGHT, doors[i][3]);
            glVertex3i(doors[i][2], GROUND_LEVEL, doors[i][3]);
        }
    }

    glEnd();

    if (doors[1][4] == DOOR_CLOSED)
        glColor3f(RED);
    else
        glColor3f(GREEN);

    glPushMatrix();
    glTranslatef(112, 4, -204.9);
    glBegin(GL_QUADS);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 2.0, 0.0);
    glVertex3f(2.0, 2.0, 0.0);
    glVertex3f(2.0, 0.0, 0.0);
    glEnd();
    glPopMatrix();
}

/*
 * Draw the ground and sea for the entire world
 */
void drawGround() {
    glPushMatrix();

    /* Sea */
    glBegin(GL_QUADS);
    glColor3f(SEA_COLOUR_1);
    glVertex3f(-410, GROUND_LEVEL - 0.5, 405);
    glColor3f(SEA_COLOUR_2);
    glVertex3f(-410, GROUND_LEVEL - 0.5, -615);
    glColor3f(SEA_COLOUR_3);
    glVertex3f(610, GROUND_LEVEL - 0.5, -615);
    glColor3f(SEA_COLOUR_4);
    glVertex3f(610, GROUND_LEVEL - 0.5, 405);
    glEnd();

    /* Grid Floor */
    glColor3f(GRID_FLOOR_COLOUR);
    glBegin(GL_QUADS);
    glVertex3f(-5, GROUND_LEVEL, -5);
    glVertex3f(-5, GROUND_LEVEL, 205);
    glVertex3f(205, GROUND_LEVEL, 205);
    glVertex3f(205, GROUND_LEVEL, -5);
    glEnd();

    /* Grass */
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glColor3f(GRASS_COLOUR_1);
    glTexCoord2i(0, 0);
    glVertex3i(-5, GROUND_LEVEL, -5);
    glColor3f(GRASS_COLOUR_2);
    glTexCoord2i(0, 1);
    glVertex3i(-5, GROUND_LEVEL, -205);
    glColor3f(GRASS_COLOUR_3);
    glTexCoord2i(1, 1);
    glVertex3i(205, GROUND_LEVEL, -205);
    glColor3f(GRASS_COLOUR_4);
    glTexCoord2i(1, 0);
    glVertex3i(205, GROUND_LEVEL, -5);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    /* Tunnel */
    glColor3f(TUNNEL_COLOUR);
    glBegin(GL_QUADS);
    glVertex3f(90, GROUND_LEVEL, -205);
    glVertex3f(110, GROUND_LEVEL, -205);
    glVertex3f(110, GROUND_LEVEL, -305);
    glVertex3f(90, GROUND_LEVEL, -305);
    glVertex3f(90, BOUNDARY_HEIGHT, -205);
    glVertex3f(110, BOUNDARY_HEIGHT, -205);
    glVertex3f(110, BOUNDARY_HEIGHT, -305);
    glVertex3f(90, BOUNDARY_HEIGHT, -305);
    glEnd();

    /* Beach */
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glColor3f(BEACH_COLOUR_1);
    glTexCoord2i(0, 0);
    glVertex3f(0, GROUND_LEVEL, -405);
    glTexCoord2i(0, 1);
    glVertex3f(200, GROUND_LEVEL, -405);
    glColor3f(BEACH_COLOUR_2);
    glTexCoord2i(1, 1);
    glVertex3f(200, GROUND_LEVEL, -305);
    glTexCoord2i(1, 0);
    glVertex3f(0, GROUND_LEVEL, -305);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

/***************************/
/* GLUT Callback Functions */
/***************************/

/*
 * glutDisplayFunc callback
 */
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawGround();
    drawBoundaries();
    drawGrid();
    drawBeepers();
    drawTrees();
    drawSun();
    drawClouds();

    glPushMatrix();
    glTranslatef(30, GROUND_LEVEL + 0.2, -140);
    glRotatef(40, 0.0, 1.0, 0.0);
    drawDogAndHouse();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(100.0, GROUND_LEVEL, -100.0);
    drawTable();
    glPopMatrix();

    /* Draw yellow arrow for overhead view */
    if (overhead) {
        glPushMatrix();
        glColor3f(ARROW_COLOUR);
        glTranslatef(xpos, 0.0, zpos);
        glRotatef(angle, 0.0, -1.0, 0.0);
        glBegin(GL_POLYGON);
        glVertex3f(4, 0, 0);
        glVertex3f(-4, 0, 2.5);
        glVertex3f(-4, 0, -2.5);
        glEnd();
        glPopMatrix();
    }

    if (karelAtWork)
        drawHUD();

    glutSwapBuffers();
}

/*
 * glutKeyboardFunc callback
 */
void key(unsigned char key, int x, int y) {
    switch (key) {
        case 'l':
            glEnable(GL_LIGHT0); 
            glEnable(GL_LIGHT2); break;
        case 'L':
            glDisable(GL_LIGHT0); 
            glDisable(GL_LIGHT2); break;
        case 'u':  /* look 'up'.*/
            if (! overhead) yat += tiltAngle; break;
        case 'U':  /* look 'down'. */
            if (! overhead) yat -= tiltAngle; break;
        case 'h': /* reset camera's y-rotation */
            if (! overhead) yat = 0.0; break;
        case 'r':
            setStepSize(RUN_STEP_SIZE); break;
        case 'R':
            setStepSize(WALK_STEP_SIZE); break;
        case 's': /* strafe right */
            strafe(-1); break;
        case 'S': /* strafe left */
            strafe(1); break;
        case 'o':
            overheadView(); break;
        case 'O':
            normalView(); break;
        case 'p':
            putBeeper(); break;
        case 'P':
            pickBeeper(); break;
        case 'c':
            clearBeepers(); break;
        case 'y':
            glEnable(GL_LIGHT1); break;
        case 'Y':
            glDisable(GL_LIGHT1); break;
        case ESC:
            exit(0); break;
    }
    reLook();
}

/*
 * glutSpecialFunc callback
 */
void special(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            if (karelAtWork) {
                int tmp = xat;
                xat = zat;
                zat = -tmp;
                angle = angle - 90;
            }
            else
                rotateCameraBy(-rotationDelta);
            break;
        case GLUT_KEY_RIGHT:
            if (karelAtWork) {
                int tmp = zat;
                zat = xat;
                xat = -tmp;
                angle = angle + 90;
            }
            else
                rotateCameraBy(rotationDelta);
            break;
        case GLUT_KEY_UP:
            move(FORWARDS);
            break;
        case GLUT_KEY_DOWN:
            move(BACKWARDS);
            break;
    }
    reLook();
}

/*
 * glutMouseFunc callback
 */
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN
            && xpos >= 95 && xpos <= 118 && zpos > -205 && zpos <= -180
            && angle > 240 && angle < 300)
        doors[1][4] = doors[1][4] == DOOR_CLOSED ? DOOR_OPEN : DOOR_CLOSED;
    glutPostRedisplay();
}

/*
 * glutIdleFunc callback
 */
void moveCloud(){
    cloudX += 0.2;
}

/*******************/
/* Setup Functions */
/*******************/

/*
 * Setup Materials
 */
void SetMaterial(void) {
    GLfloat material_ambient[4] = {0.2, 0.2, 0.2, 0.0};
    GLfloat material_diffuse[4] = {0.5, 0.5, 0.5, 0.0};
    GLfloat material_specular[4] = {0.2, 0.2, 0.2, 1.0};
    GLfloat material_emission[4] = {0.1, 0.1, 0.1, 1.0};
    GLfloat material_shininess[1] = {128.0};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_emission);
}

/*
 * Setup Lighting
 */
void SetLight(void) {
    // Light0 settings
    GLfloat light_position[4] = {1.0, 1.0, 1.0, 0.0};
    GLfloat light_ambient [4] = {0.2, 0.2, 0.2, 1.0};
    GLfloat light_diffuse [4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat global_ambient[4] = {0.2, 0.2, 0.2, 1.0};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular); 
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    // Spotlight settings
    GLfloat spot_position[4] = {0.0, 2.0, 2.0, 1.0};
    GLfloat spot_direction[3] = {0.0, 0.0, -1.0};
    GLfloat spot_ambient [4] = {1.4, 1.4, 1.4, 1.0};
    GLfloat spot_diffuse [4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat spot_specular[4] = {1, 1, 1, 1.0};

    glLightfv(GL_LIGHT1, GL_POSITION, spot_position);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightfv(GL_LIGHT1, GL_AMBIENT, spot_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, spot_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spot_specular); 
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 12.0);

    // Sun settings
    GLfloat sun_position[4] = {100.0, 150.0, -600.0, 1.0};
    GLfloat sun_ambient [4] = {0.2, 0.2, 0.2, 1.0};
    GLfloat sun_diffuse [4] = {0.5, 0.5, 0.5, 1.0};
    GLfloat sun_specular[4] = {1., 1., 1., 1.0};

    glLightfv(GL_LIGHT2, GL_POSITION, sun_position);
    glLightfv(GL_LIGHT2, GL_AMBIENT, sun_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, sun_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, sun_specular); 
}

/*
 * Setup Textures
 */
void SetupTexture() {
    const int imagewidth = 1024, imageheight = 1024;
    int i, j;

    GLubyte image[imagewidth][imageheight][3];

    for (i = 0; i < imagewidth; i ++)
        for (j = 0; j < imageheight; j ++) {
            int x;
            x = rand() % 256;
            image[i][j][0] = x;
            image[i][j][1] = x;
        } 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);		
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);		
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imagewidth, imageheight, 0, GL_RGB,
            GL_UNSIGNED_BYTE, image);
}

/*
 * Initial Setup
 */
void init(void) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    quad = gluNewQuadric();
    beepers[0][0] = 6;
    beepers[1][0] = 4;
    beepers[2][0] = 2;
    beepers[0][1] = 4;
    beepers[0][2] = 2;
    beepers[1][1] = 4;
    beepers[2][2] = 2;
    beepers[3][3] = 2;
    beepers[4][4] = 1;

    SetLight();
    SetMaterial();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 0.05, 700.0);

    glMatrixMode(GL_MODELVIEW);
    SetupTexture();
    reLook();
}

/*
 * Main Program
 */
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("CS4404 Assignment 3");
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutMouseFunc(mouse);
    glutSpecialFunc(special);
    glutIdleFunc(moveCloud);

    init();
    glutMainLoop();
    return 0;
}
