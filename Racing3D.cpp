#include <GL/freeglut.h>   // or <GL/glut.h> if using old GLUT
#include <cmath>

// Global state (camera, rotation, etc.)
float angleX = 0.0f;
float angleY = 0.0f;
float zoom   = -5.0f;

// Drawing function (put your shapes here)
void drawScene() {
    // Example: draw a colorful rotating cube
    glBegin(GL_QUADS);

    // Front face (red)
    glColor3f(1,0,0);
    glVertex3f(-1,-1, 1);
    glVertex3f( 1,-1, 1);
    glVertex3f( 1, 1, 1);
    glVertex3f(-1, 1, 1);

    // Back face (green)
    glColor3f(0,1,0);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1, 1,-1);
    glVertex3f( 1, 1,-1);
    glVertex3f( 1,-1,-1);

    // Left face (blue)
    glColor3f(0,0,1);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,-1, 1);
    glVertex3f(-1, 1, 1);
    glVertex3f(-1, 1,-1);

    // Right face (yellow)
    glColor3f(1,1,0);
    glVertex3f(1,-1,-1);
    glVertex3f(1, 1,-1);
    glVertex3f(1, 1, 1);
    glVertex3f(1,-1, 1);

    // Top face (cyan)
    glColor3f(0,1,1);
    glVertex3f(-1,1,-1);
    glVertex3f(-1,1, 1);
    glVertex3f( 1,1, 1);
    glVertex3f( 1,1,-1);

    // Bottom face (magenta)
    glColor3f(1,0,1);
    glVertex3f(-1,-1,-1);
    glVertex3f( 1,-1,-1);
    glVertex3f( 1,-1, 1);
    glVertex3f(-1,-1, 1);

    glEnd();
}

// Display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Camera transform
    glTranslatef(0.0f, 0.0f, zoom);
    glRotatef(angleX, 1, 0, 0);
    glRotatef(angleY, 0, 1, 0);

    drawScene();

    glutSwapBuffers();
}

// Reshape callback
void reshape(int w, int h) {
    if (h == 0) h = 1; // avoid divide by zero
    float aspect = (float)w / h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspect, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Keyboard input
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0);  // ESC to quit
}

// Arrow keys for rotation
void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:    angleX -= 5; break;
        case GLUT_KEY_DOWN:  angleX += 5; break;
        case GLUT_KEY_LEFT:  angleY -= 5; break;
        case GLUT_KEY_RIGHT: angleY += 5; break;
    }
    glutPostRedisplay();
}

// Mouse wheel zoom
void mouse(int button, int state, int x, int y) {
    if (button == 3) zoom += 0.2f; // scroll up
    if (button == 4) zoom -= 0.2f; // scroll down
    glutPostRedisplay();
}

// Initialization
void initOpenGL() {
    glEnable(GL_DEPTH_TEST); // enable 3D depth
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // dark background
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("FreeGLUT Template");

    initOpenGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}
