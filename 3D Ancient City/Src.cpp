#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <SOIL2.h>

constexpr float PI = 3.14159265358979323846;

int width;
int height;

//variables to move the camera
GLfloat camX = 0.0; GLfloat camY = 0.0; GLfloat camZ = 0.0;

//variables to move the scene
GLfloat sceRX = 0.0; GLfloat sceRY = 0.0; GLfloat sceRZ = 0.0;
GLfloat sceTX = 0.0; GLfloat sceTY = 0.0; GLfloat sceTZ = 0.0;

////variables to move the objects
GLfloat objRX = 0.0; GLfloat objRY = 0.0; GLfloat objRZ = 0.0;
GLfloat objTX = 0.0; GLfloat objTY = 0.0; GLfloat objTZ = 0.0;


//To on/off grids and axes
int gridOn = 0;
int axesOn = 0;

//texture image
GLuint texture_image_1;

unsigned char* image;
GLuint tex;

void drawGrid() {
	GLfloat step = 1.0f;
	GLint line;

	glBegin(GL_LINES);
	for (line = -20; line <= 20; line += step) {
		glVertex3f(line, -0.4, 20);
		glVertex3f(line, -0.4, -20);

		glVertex3f(20, -0.4, line);
		glVertex3f(-20, -0.4, line);
	}
	glEnd();
}

void drawAxes() {

	glBegin(GL_LINES);

	glLineWidth(1.5);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-200, 0, 0);
	glVertex3f(200, 0, 0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, -200, 0);
	glVertex3f(0, 200, 0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, -200);
	glVertex3f(0, 0, 200);

	glEnd();
}

// Number of textures
#define NUM_TEXTURES 3

// Global variables for texture IDs
GLuint textureIDs[NUM_TEXTURES];

// Paths to texture images
const char* textureFiles[NUM_TEXTURES] = {
	"textures/roundWalls.jpg",
	"textures/roof.jpg",
	"textures/floor.jpg"
};

// Function to load textures
void loadTextures() {
	int width, height;
	unsigned char* image;

	glGenTextures(NUM_TEXTURES, textureIDs); // Generate texture IDs

	for (int i = 0; i < NUM_TEXTURES; i++) {
		// Load image
		image = SOIL_load_image(textureFiles[i], &width, &height, 0, SOIL_LOAD_RGB);
		if (image == NULL) {
			printf("Error loading texture %d: %s\n", i, SOIL_last_result());
			continue;
		}

		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, textureIDs[i]);

		// Create the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Free the image memory
		SOIL_free_image_data(image);

		printf("Loaded texture %d from %s\n", i, textureFiles[i]);
	}

	// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
}


void init(void) {
	glClearColor(0.0, 0.8, 0.8, 1.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	loadTextures();
}


void roundWall()
{

	glPushMatrix();
	glScalef(0.8, 2, 0.8);
	glRotatef(90.0, 1, 0, 0);

	// Disable lighting temporarily
	glDisable(GL_LIGHTING);

	// Set texture environment
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	// Set white base color
	glColor3f(1.0f, 1.0f, 1.0f);

	GLUquadric* quad = gluNewQuadric();

	gluQuadricTexture(quad, GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
	gluCylinder(quad, .8, 1.0, 3, 100, 100);
	gluDisk(quad, 0, 0.8, 64, 32);
	glTranslatef(0, 0, 3);
	gluDisk(quad, 0, 0.8, 64, 32);
	glDisable(GL_TEXTURE_2D);
	gluDeleteQuadric(quad);

	glPopMatrix();

}


//void roundWallsGroup() {
//	float spacing = 4.0f; // Distance between the centers of adjacent walls
//
//	for (int row = 0; row < 3; row++) {       // Loop through rows
//		for (int col = 0; col < 3; col++) {   // Loop through columns
//			glPushMatrix();
//
//			// Calculate position for each wall
//			float x = (col - 1) * spacing;    // Column position (-spacing, 0, spacing)
//			float z = (row - 1) * spacing;    // Row position (-spacing, 0, spacing)
//
//			// Translate to position
//			glTranslatef(x, 0, z);
//
//			// Draw the round wall
//			roundWall();
//
//			glPopMatrix();
//		}
//	}
//}


void slantedRoof(float width, float length, float height) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureIDs[1]); // Use the texture ID for the roof texture

	glColor3f(1.0f, 1.0f, 1.0f); // Base color for the texture

	// Draw the roof
	glBegin(GL_TRIANGLES);

	// Left slanted face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width / 2.0f, 0.0f, -length / 2.0f); // Bottom-left
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-width / 2.0f, 0.0f, length / 2.0f);  // Bottom-right
	glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, height, 0.0f);                 // Apex (top)

	// Right slanted face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(width / 2.0f, 0.0f, -length / 2.0f);  // Bottom-left
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width / 2.0f, 0.0f, length / 2.0f);   // Bottom-right
	glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, height, 0.0f);                 // Apex (top)

	// Front slanted face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width / 2.0f, 0.0f, length / 2.0f);  // Bottom-left
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width / 2.0f, 0.0f, length / 2.0f);   // Bottom-right
	glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, height, 0.0f);                 // Apex (top)

	// Back slanted face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width / 2.0f, 0.0f, -length / 2.0f); // Bottom-left
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width / 2.0f, 0.0f, -length / 2.0f);  // Bottom-right
	glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, height, 0.0f);                 // Apex (top)

	glEnd();

	// Optional: Add flat quadrilateral roof base if needed
	glBegin(GL_QUADS);

	// Base of the roof (flat rectangle on top of walls)
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width / 2.0f, 0.0f, -length / 2.0f); // Bottom-left
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width / 2.0f, 0.0f, -length / 2.0f);  // Bottom-right
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width / 2.0f, 0.0f, length / 2.0f);   // Top-right
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width / 2.0f, 0.0f, length / 2.0f);  // Top-left

	glEnd();

	glDisable(GL_TEXTURE_2D);
}


void roofWithPediments() {
	float roofWidth = 16.0f;   // Width of the roof
	float roofLength = 36.0f;  // Length of the roof
	float roofHeight = 5.0f;   // Height of the roof

	glPushMatrix();

	// Slanted roof
	glTranslatef(0.0f, 0.0f, -roofLength / 2.0f);
	slantedRoof(roofWidth, roofLength, roofHeight);

	glPopMatrix();
}


void createFloor(float width, float length) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureIDs[2]); // Use the texture ID for the floor texture

	glColor3f(1.0f, 1.0f, 1.0f); // Base color for the texture

	glBegin(GL_QUADS);

	// Define the floor vertices and map the texture
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width / 2.0f, 0.0f, -length / 2.0f); // Bottom-left
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width / 2.0f, 0.0f, -length / 2.0f);  // Bottom-right
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width / 2.0f, 0.0f, length / 2.0f);   // Top-right
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width / 2.0f, 0.0f, length / 2.0f);  // Top-left

	glEnd();

	glDisable(GL_TEXTURE_2D);
}


void createSquareBase(float size, float height) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureIDs[0]); // Use a texture for the base

	glColor3f(1.0f, 1.0f, 1.0f); // Set base color

	glBegin(GL_QUADS);

	// Top face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size / 2.0f, height, -size / 2.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size / 2.0f, height, -size / 2.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size / 2.0f, height, size / 2.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size / 2.0f, height, size / 2.0f);

	// Bottom face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size / 2.0f, 0.0f, -size / 2.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size / 2.0f, 0.0f, -size / 2.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size / 2.0f, 0.0f, size / 2.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size / 2.0f, 0.0f, size / 2.0f);

	// Front face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size / 2.0f, 0.0f, size / 2.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size / 2.0f, 0.0f, size / 2.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size / 2.0f, height, size / 2.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size / 2.0f, height, size / 2.0f);

	// Back face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size / 2.0f, 0.0f, -size / 2.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size / 2.0f, 0.0f, -size / 2.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size / 2.0f, height, -size / 2.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size / 2.0f, height, -size / 2.0f);

	// Left face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size / 2.0f, 0.0f, -size / 2.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-size / 2.0f, 0.0f, size / 2.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-size / 2.0f, height, size / 2.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size / 2.0f, height, -size / 2.0f);

	// Right face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(size / 2.0f, 0.0f, -size / 2.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(size / 2.0f, 0.0f, size / 2.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(size / 2.0f, height, size / 2.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(size / 2.0f, height, -size / 2.0f);

	glEnd();

	glDisable(GL_TEXTURE_2D);
}

//void mainHallStayer() {
//
//
//
//}


void mainHall() {
	// Draw the 3x3 grid of round walls
	float spacing = 4.0f;

	glTranslatef(0, 0, -30);
	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 4; col++) {
			glPushMatrix();

			// Calculate position for each wall
			float x = (col - 1) * spacing;
			float z = (row - 1) * spacing;

			// Translate to position
			glTranslatef(x, 0, z);

			// Draw the round wall
			roundWall();
			glTranslatef(0, -7, 0);
			createSquareBase(2, 1);

			glPopMatrix();
		}
	}
	// Draw the roof
	glTranslatef(2, 0, 30);
	roofWithPediments();

	// Draw the floor 
	glTranslatef(0, -7, -20);
	createFloor(18, 40);
}



//void roundWallsGroupWithRoof() {
//	// Draw the 3x3 grid of round walls
//	float spacing = 4.0f;
//
//	for (int row = 0; row < 3; row++) {
//		for (int col = 0; col < 3; col++) {
//			glPushMatrix();
//
//			// Calculate position for each wall
//			float x = (col - 1) * spacing;
//			float z = (row - 1) * spacing;
//
//			// Translate to position
//			glTranslatef(x, 0, z);
//
//			// Draw the round wall
//			roundWall();
//			glTranslatef(0, -7, 0);
//			createSquareBase(2, 1);
//
//			glPopMatrix();
//		}
//	}
//	// Draw the roof
//	glTranslatef(0, 0, 6);
//	roofWithPediments();
//
//	// Draw the floor 
//	glTranslatef(0, -7, -6);
//	createFloor(12);
//}






//void topseat() {
//
//
//	glColor3f(0.65, 0.16, 0.16);
//
//
//	GLUquadric* quad = gluNewQuadric();
//	glPushMatrix();
//
//	glTranslatef(0, 2.65, 0);
//	glRotatef(-30, 0, 0, 1);
//	glScalef(1.5, 0.5, 0.8);
//	glutSolidCube(0.3);
//	glPopMatrix();
//
//	gluDeleteQuadric(quad);
//
//	fork_n_seat();
//
//
//}







//void setLighting() {
//
//	// Lighting set up
//	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
//	glEnable(GL_LIGHTING);
//	//glEnable(GL_LIGHT0);
//	//glEnable(GL_LIGHT1);
//
//	// Set lighting intensity and color - light 0
//	GLfloat qaAmbientLight[] = { 0.2, 0.2, 0.2, 1.0 };
//	GLfloat qaDiffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
//	GLfloat qaSpecularLight[] = { 1.0, 1.0, 1.0, 1.0 };
//	glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
//	glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);
//
//	// Set the light position - light 0
//	GLfloat qaLightPosition[] = { -10.0, 1.0, -.5, 1.0 };
//	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
//
//	// Set lighting intensity and color - light 1
//	GLfloat qaAmbientLight1[] = { 0.2, 0.2, 0.2, 1.0 };
//	GLfloat qaDiffuseLight1[] = { 0.8, 0.8, 0.8, 1.0 };
//	GLfloat qaSpecularLight1[] = { 1.0, 1.0, 1.0, 1.0 };
//	glLightfv(GL_LIGHT1, GL_AMBIENT, qaAmbientLight1);
//	glLightfv(GL_LIGHT1, GL_DIFFUSE, qaDiffuseLight1);
//	glLightfv(GL_LIGHT1, GL_SPECULAR, qaSpecularLight1);
//
//	// Set the light position - light 1
//	GLfloat qaLightPosition1[] = { 10.0, 1.0, 0.5, 1.0 };
//	glLightfv(GL_LIGHT1, GL_POSITION, qaLightPosition1);
//
//	GLfloat SpecRef[] = { 0.7,0.7,0.7,1.0 };
//	GLint Shine = 128;
//	glShadeModel(GL_SMOOTH);
//	glEnable(GL_COLOR_MATERIAL);                   //Enable color tracking
//	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);//Set material to follow
//	glMaterialfv(GL_FRONT, GL_SPECULAR, SpecRef);//Set specular reflectivity and shine
//	glMateriali(GL_FRONT, GL_SHININESS, Shine);
//}








void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	// camera orientation (eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ)
	gluLookAt(0.0, 0.0 + camY, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// move the scene (all the rendered environment) using keyboard keys
	glTranslatef(sceTX, sceTY, sceTZ);
	glRotatef(sceRY, 0.0, 1.0, 0.0);

	//setLighting();

	//Draw a grid on the xz plane
	glColor3f(0.1, 0.1, 0.1);
	if (gridOn == 1)
		drawGrid();
	//draw the three axes
	if (axesOn == 1)
		drawAxes();

	//glScalef(1.0, 1.0, 1.0);
	//roundWall();
	//roundWallsGroup();
	mainHall();
	//cube();
	//createFloor(10);
	//createSquareBase(12,5);

	glPopMatrix();
	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	GLfloat aspect_ratio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Define the Perspective projection frustum
	// (FOV_in_vertical, aspect_ratio, z-distance to the near plane from the camera position, z-distance to far plane from the camera position)
	gluPerspective(120.0, aspect_ratio, 1.0, 100.0);

}

void keyboardSpecial(int key, int x, int y) {
	if (key == GLUT_KEY_UP)
		camY += 1;

	if (key == GLUT_KEY_DOWN)
		camY -= 1;

	if (key == GLUT_KEY_RIGHT)
		sceTX += 1;

	if (key == GLUT_KEY_LEFT)
		sceTX -= 1;

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 'l')
		objRY += 1;

	if (key == 'r')
		objRY -= 1;

	if (key == 'Z')
		sceTZ += 1;

	if (key == 'z')
		sceTZ -= 1;

	if (key == 'w')
		sceTX += 1;

	if (key == 's')
		sceTX -= 1;

	if (key == 'y')
		sceRY += 1;

	if (key == 'Y')
		sceRY -= 1;

	//if (key == '!')
	//	glDisable(GL_LIGHT0); // Light at -x

	//if (key == '1')
	//	glEnable(GL_LIGHT0);

	//if (key == '@')
	//	glDisable(GL_LIGHT1); // Light at +x

	//if (key == '2')
	//	glEnable(GL_LIGHT1);

	//Grids and axes
	if (key == 'G')
		gridOn = 1;
	if (key == 'g')
		gridOn = 0;
	if (key == 'A')
		axesOn = 1;
	if (key == 'a')
		axesOn = 0;


	glutPostRedisplay();
}

int main(void) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("3D Ancient City");
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	init();
	glutMainLoop();
	return 0;
}



