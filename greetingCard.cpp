#define FREEGLUT_STATIC // Define a static library for calling functions
#include <GL/freeglut.h> // Include the header file
#include "math.h"
#include "windows.h"
#include <iostream>

#define MAX_CHAR 128
using namespace std;
GLfloat scale = 1;
GLint width = 800;
GLint height = 800;
int time_interval = 16;  //declare refresh interval in ms 
float wd = 1;
float hd = 1;
const GLdouble PI = 3.1415926;
int ballonState = 0;
int bstepx = 0;
int bstepy = 0;
float r[45];
float g[45];
float b[45];
float posx[45];
float posy[45];
float speed = 1;
float sped = 1;
float textColorFactor = 0;

// set colorful text
void selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0, charset, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}
void drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;
	if (isFirstCall) {
		isFirstCall = 0;
		lists = glGenLists(MAX_CHAR);
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	for (; *str != '\0'; ++str) {
		glCallList(lists + *str);
	}
}
void showText(GLfloat x, GLfloat y, const char s[50], int size) {
	selectFont(size, ANSI_CHARSET, "Comic Sans MS");
	float tr = textColorFactor * float(rand() % 10) / 10.0;
	float tg = textColorFactor * float(rand() % 10) / 10.0;
	float tb = textColorFactor * float(rand() % 10) / 10.0;
	glColor3f(tr, tg, tb);
	glRasterPos2f(x, y);
	drawString(s);
}

void keyboard_input(unsigned char key, int x, int y) { // keyboard interaction
	if (key == 'q' || key == 'Q')
		exit(0);
	else if (key == 'l' || key == 'L') // move balloons to the left
		bstepx--;
	else if (key == 'u' || key == 'U') // move balloons up
		bstepy++;
	else if (key == 'r' || key == 'R') // move balloons to the right
		bstepx++;
	else if (key == 'd' || key == 'D') // move balloons down
		bstepy--;
	else if (key == 's' || key == 'S') // shrink the balloons
	{
		if (scale > 1)
			scale -= 0.6;
	}
	else if (key == 'e' || key == 'E') // enlarge the balloons
		scale += 0.3;
	else if (key == 'b' || key == 'B') // make balloons begin to move
		ballonState = 1;
	else if (key == 'k' || key == 'K') // make balloons stop moving
		ballonState = 0;
	else if (key == 'm' || key == 'M')	// speed up balloons
		speed += 1;
	else if (key == 'n' || key == 'N')	// slow down balloons
	{
		if (speed > 0) {
			speed -= 1;
		}
	}
}

void mouse_input(int button, int state, int x, int y) // mouse interactions
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)	// make the text shining
		textColorFactor = 1; // decrement step
	else if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON)	// stop the text shining
		textColorFactor = 0; // increment step

	showText(x/wd - 15  , -y / hd - 10 , "XJTLU", 50);	// make mouse show text
	glFlush();
}

void ChangeSize(int w, int h) {
	wd = (float)w / (float)width;
	hd = (float)h / (float)height;
	GLfloat fAspect;
	if (h == 0) // Avoid division by zero
		h = 1;
	glViewport(0, 0, w, h); // Set viewport to window dimensions
	fAspect = (GLfloat)w / (GLfloat)h;
	glMatrixMode(GL_PROJECTION); // Reset coordinate system
	glLoadIdentity();
	gluPerspective(60.0f, fAspect, 1.0, 600.0); // Set viewing volume
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void when_in_mainloop()
{
	if (ballonState == 1) {
		glutPostRedisplay(); // force OpenGL to redraw the current window
	}
}

void OnTimer(int value) {
	// ballons will update in a time interval
	sped = 8;
	for (int i = 0; i < 40; i++) {
		posx[i] = rand() % width;
		posy[i] = rand() % 10 + 2;
		r[i] = float(rand() % 10) / 10.0;
		g[i] = float(rand() % 10) / 10.0;
		b[i] = float(rand() % 10) / 10.0;
	}
	glutTimerFunc(12000 / speed, OnTimer, 1);
}

void drawEllipse(GLint W, GLint H) {
	glBegin(GL_LINES);
	for (GLdouble i = 0.00; i < 2 * PI; i += 0.0005)
	{
		glVertex2d(0, 0);
		glVertex2d(W * cos(i), H * sin(i));
	}
	glEnd();
}

void drawleaf(void)
{
	glPushMatrix();
	glTranslatef(165, -450, 0);
	drawEllipse(45, 55);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(205, -410, 0);
	drawEllipse(49, 55);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(235, -450, 0);
	drawEllipse(65, 55);
	glPopMatrix();
}

void drawTree(void) {
	glColor3f(0.125, 0.051, 0.129);
	glPushMatrix();
	glTranslatef(217, -830, 0);
	glLineWidth(10);
	glBegin(GL_LINES);
	for (GLdouble l = 0.00; l < 30; l += 0.0005)
	{
		glVertex2d(l, 0);
		glVertex2d(l, l * l / 3);
	}
	glEnd();
	glBegin(GL_LINES);
	for (GLdouble l = 0.00; l > -30; l -= 0.0005)
	{
		glVertex2d(l + 70, 0);
		glVertex2d(l + 70, l * l / 3);
	}
	glEnd();
	glPopMatrix();

	glColor3f(0.18f, 0.318f, 0.098f);
	glPushMatrix();
	glTranslatef(-40, 20, 0);
	glScalef(1.2, 1.2, 0.8);
	drawleaf();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(40, 20, 0);
	glScalef(1.2, 1.2, 0.8);
	drawleaf();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(170, -320, 0);
	glScalef(0.6, 0.6, 0.8);
	drawleaf();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(80, -320, 0);
	glScalef(0.6, 0.6, 0.8);
	drawleaf();
	glPopMatrix();

}

void setBackground(void) {
	glBegin(GL_QUADS);
	glColor3f(0.996, 0.941, 0.906);
	glVertex2f(0, 0);
	glVertex2f(width, 0);
	glColor3f(0.98, 0.635, 0.478);
	glVertex2f(width, -440);
	glVertex2f(0, -440);
	glEnd();
	glColor3f(0.98, 0.973, 0.78);
	glPushMatrix();
	glTranslatef(230, -320, 0);
	drawEllipse(30, 30);
	glPopMatrix();
}

void drawBuilding(void) // Function for geometric creation
{
	glLineWidth(2);
	// Building background filling
	glColor3f(0, 0.059, 0.161);
	glBegin(GL_POLYGON);
	glVertex2f(449, -319);
	glVertex2f(650, -319);
	glVertex2f(650, -375);
	glVertex2f(449, -375);
	glEnd();


	//left
	glBegin(GL_POLYGON);
	glColor3f(0.455, 0.667, 0.604);
	//glColor3f(0.643, 0.58, 0.63);
	glVertex2f(0, -222);
	glVertex2f(158, -264);
	glVertex2f(158, -160);
	glColor3f(0.247, 0.373, 0.455);
	//glColor3f(0.5176, 0.721, 0.772);
	glVertex2f(4, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.643, 0.58, 0.63);
	glVertex2f(158, -264);
	glVertex2f(158, -302);
	glVertex2f(149, -302);
	glColor3f(0.5176, 0.721, 0.772);
	glVertex2f(147, -262);
	glEnd();
	

	// left-top/right 
	glColor3f(0.643, 0.58, 0.63);
	glBegin(GL_POLYGON);
	glVertex2f(22, 0);
	glVertex2f(64, -44);
	glVertex2f(64, -64);
	glVertex2f(4, 0);
	glEnd();

	glColor3f(0.713, 0.29, 0.278);
	glBegin(GL_POLYGON);
	glVertex2f(142, -144);
	glVertex2f(165, -144);
	glVertex2f(165, -164);
	glVertex2f(158, -164);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(165, -160);
	glVertex2f(165, -301);

	glVertex2f(158, -301);
	glVertex2f(158, -160);
	glEnd();

	//left -middle
	glBegin(GL_POLYGON);
	glVertex2f(0, -224);
	glVertex2f(147, -263);
	glVertex2f(148, -273);
	glVertex2f(0, -243);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(144, -272);
	glVertex2f(149, -302);
	glVertex2f(149, -274);
	glEnd();

	glBegin(GL_LINES);
	for (int i = 0; i <= 158; i += 10) {
		glVertex2f(i, -80.0/79.0*float(i));
		glVertex2f(i, -21.0 / 79.0 * float(i)-222);
	}
	int y1 = -6;
	for (int y2 = -160; y2 >= -264; y2 -= 15) {
		glVertex2f(158, y2);
		glVertex2f(0, y1);
		y1 -= 30;
	}
	glEnd();

	// left middle bottom
	glColor3f(0.5176, 0.721, 0.772);
	glBegin(GL_POLYGON);
	glVertex2f(34, -252);
	glVertex2f(34, -326);
	glVertex2f(139, -317);
	glVertex2f(132, -270);
	glEnd();


	glColor3f(0.031, 0.055, 0.141);
	glBegin(GL_POLYGON);
	glVertex2f(34, -252);
	glVertex2f(34, -570);
	glVertex2f(0, -570);
	glVertex2f(0, -244);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(138, -315);
	glVertex2f(134, -272);
	glVertex2f(144, -274);
	glVertex2f(149, -315);
	glEnd();

	// draw library
	glBegin(GL_POLYGON);
	glColor3f(0.549, 0.655, 0.698);
	glVertex2f(398, -201);
	glColor3f(0.404, 0.373, 0.424);
	glVertex2f(515, -201);
	glVertex2f(515, -242);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.706, 0.678, 0.710);
	glVertex2f(286, -317);
	glVertex2f(286, -202);
	glColor3f(0.404, 0.373, 0.424);
	glVertex2f(398, -201);
	glVertex2f(515, -243);
	glColor3f(0.459, 0.455, 0.525);
	glVertex2f(514, -316);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(288, -33);
	glVertex2f(423, -32);
	glVertex2f(417, -153);
	glVertex2f(288, -153);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.522, 0.631, 0.675);
	glVertex2f(420, -119);
	glVertex2f(481, -119);
	glColor3f(0.435, 0.467, 0.541);
	glVertex2f(475, -36);
	glVertex2f(424, -35);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.4, 0.45, 0.486);
	glVertex2f(474, -30);
	glColor3f(0.435, 0.467, 0.541);
	glVertex2f(491, -214);
	glVertex2f(611, -215);
	glVertex2f(610, -31);
	glEnd();

	// center of library
	glColor3f(0.212, 0.149, 0.325);
	glBegin(GL_POLYGON);
	glVertex2f(286, -202);
	glVertex2f(288, -153);
	glVertex2f(417, -153);
	glVertex2f(414, -200);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(420, -119);
	glVertex2f(481, -119);
	glVertex2f(491, -199);
	glVertex2f(414, -200);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(303, -181);
	glColor3f(0.635, 0.631, 0.694);
	glVertex2f(361, -181);
	glVertex2f(363, -167);
	glVertex2f(301, -167);
	glEnd();

	// library жљзг
	glBegin(GL_LINES);
	glVertex2f(308, -181);
	glVertex2f(308, -201);

	glVertex2f(355, -181);
	glVertex2f(354, -200);

	glVertex2f(398, -155);
	glVertex2f(398, -200);
	glEnd();

	// ground on the top of the wall
	glBegin(GL_POLYGON);
	glVertex2f(33, -326);
	glVertex2f(138, -315);
	glVertex2f(499, -318);
	glVertex2f(504, -326);
	glEnd();

	// glass on the top of the wall
	glColor4f(1, 1, 1, 0.4);
	glBegin(GL_POLYGON);
	glVertex2f(33, -326);
	glVertex2f(498, -326);
	glVertex2f(498, -302);
	glVertex2f(34, -302);
	glEnd();

	// the wall in the middle
	glBegin(GL_POLYGON);
	glColor3f(0, 0.035, 0.070);
	glVertex2f(33, -326);
	glVertex2f(488, -329);
	glColor3f(0.192, 0.216, 0.263);
	glVertex2f(499, -390);
	glColor3f(0.196, 0.301, 0.173);
	glVertex2f(464, -565);
	glVertex2f(33, -565);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0, 0.035, 0.070);
	glVertex2f(506, -329);
	glVertex2f(515, -366);
	glColor3f(0.192, 0.216, 0.263);
	glVertex2f(504, -370);
	glColor3f(0.196, 0.301, 0.173);
	glVertex2f(497, -329);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(497, -329);
	glVertex2f(504, -370);
	glVertex2f(498, -394);
	glVertex2f(488, -329);
	glEnd();

	// floors
	glColor3f(0.333, 0.553, 0.698);
	glBegin(GL_QUADS);
	glVertex2f(0, -544);
	glVertex2f(213, -432);
	glVertex2f(194, -424);
	glVertex2f(0, -525);

	glVertex2f(213, -432);
	glVertex2f(264, -431);
	glVertex2f(255, -421);
	glVertex2f(194, -424);

	glVertex2f(264, -431);
	glVertex2f(255, -421);
	glVertex2f(349, -367);
	glVertex2f(365, -374);

	glVertex2f(349, -367);
	glVertex2f(365, -374);
	glVertex2f(398, -376);
	glVertex2f(378, -370);

	glVertex2f(398, -376);
	glVertex2f(378, -370);
	glVertex2f(442, -327);
	glVertex2f(472, -328);
	glEnd();

	// right building
	glColor3f(0.69, 0.298, 0.329);
	glBegin(GL_TRIANGLES);
	glVertex2f(516, -154);
	glVertex2f(516, -146);
	glVertex2f(534, -145);
	glEnd();

	
	glBegin(GL_POLYGON);
	glColor3f(0.455, 0.667, 0.604);
	glVertex2f(516, -320);
	glColor3f(0.247, 0.373, 0.455);
	glVertex2f(516, -154);
	glVertex2f(650, -82);
	glVertex2f(650, -318);
	glEnd();

	glColor3f(0.427, 0.18, 0.16);
	// right floor
	glBegin(GL_LINES);
	glVertex2f(612, -101);
	glVertex2f(612, -250);
	glVertex2f(604, -254);
	glVertex2f(604, -304);
	glEnd();

	float n = 0;
	float y2 = -238;
	glBegin(GL_LINES);
	for (double y1 = -265; y1 <= -153; y1 += 28.0) {
		glVertex2f(516, y1);
		glVertex2f(650, y2);
		y2 += 149.0 / 4.0;
	}
	float y3 = -153 - (153 - 101) / 8;
	float y4 = -265 - (265 - 250) / 9;
	for (float x = 516; x <= 612; x += 12) {
		glVertex2f(x, y3);
		glVertex2f(x, y4);
		y3 += (153 - 101) / 8;
		y4 += (265 - 250) / 8;
	}
	glEnd();

	// Understair passage
	glColor3f(0.255, 0.282, 0.392);
	glBegin(GL_POLYGON);
	glVertex2f(0, -575);
	glVertex2f(463, -569);
	glVertex2f(467, -553);
	glColor3f(0, 0.035, 0.070);
	glVertex2f(0, -558);
	glEnd();

	// the middle of the aisle
	glBegin(GL_POLYGON);
	glColor3f(0.443, 0.224, 0.361);
	glVertex2f(0, -575);
	glColor3f(0.702, 0.290, 0.412);
	glVertex2f(463, -569);
	glVertex2f(450, -625);
	glVertex2f(0, -636);
	glEnd();

	// aisle
	glColor3f(0.306, 0.282, 0.471);
	glBegin(GL_POLYGON);
	glVertex2f(0, -645);
	glVertex2f(449, -636);
	glVertex2f(450, -625);
	glVertex2f(0, -636);
	glEnd();

	// right building -door
	glBegin(GL_POLYGON);
	glColor3f(0.376, 0.020, 0.141);
	glVertex2f(542, -274);
	glVertex2f(554, -274);
	glVertex2f(553, -319);
	glVertex2f(542, -319);
	glEnd();


	glColor3f(0.18f, 0.318f, 0.098f);
	glPushMatrix();
	glTranslatef(246, -82, 0);
	glScalef(1, 1, 0.8);
	drawleaf();
	glPopMatrix();

	// right stairs
	glColor3f(0.713, 0.29, 0.278);
	glBegin(GL_POLYGON);
	glVertex2f(447, -650);
	glVertex2f(650, -650);
	glVertex2f(650, -414);
	glVertex2f(504, -370);
	glEnd();

	// right middle stairs
	glBegin(GL_QUADS);
	glColor3f(0.973, 0.678, 0.667);
	glVertex2f(504, -370);
	glVertex2f(527, -365);
	glVertex2f(650, -400);
	glColor3f(0.725, 0.396, 0.357);
	glVertex2f(650, -414);

	
	glVertex2f(527, -365);
	glVertex2f(650, -400);
	glVertex2f(650, -390);
	glColor3f(0.745, 0.710, 0.588);
	glVertex2f(523, -359);

	glVertex2f(650, -390);
	glVertex2f(523, -359);
	glVertex2f(555, -358);
	glVertex2f(650, -379);
	glEnd();

	// right stairs-inside
	glColor3f(0.957, 0.765, 0.749);
	glBegin(GL_POLYGON);
	glVertex2f(460, -650);
	glVertex2f(650, -650);
	glVertex2f(650, -465);
	glVertex2f(505, -401);
	glEnd();

	glColor3f(0.184, 0.009, 0.141);
	float xx1 = 506;
	float yy1 = -403;
	glBegin(GL_LINES);
	for (int y = -478; y >= -720; y -= 13) {
		xx1 -= 2.2;
		yy1 -= 12.35;
		glVertex2f(650, y);
		glVertex2f(xx1, yy1);
	}
	glEnd();

	// right glass
	glColor4f(1, 1, 1, 0.4);
	glBegin(GL_POLYGON);
	glVertex2f(555, -351);
	glVertex2f(650, -372);
	glVertex2f(650, -325);
	glVertex2f(555, -314);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2f(555, -314);
	glVertex2f(650, -319);
	glVertex2f(650, -358);
	glVertex2f(555, -351);
	glEnd();


	glColor3f(0.918, 0.851, 0.882);
	glLineWidth(2);
	glBegin(GL_LINES);
	// Second pillar
	glVertex2f(572, -362);
	glVertex2f(572, -314);
	glVertex2f(572, -314);
	glVertex2f(581, -306);

	// third pillar
	glVertex2f(597, -366);
	glVertex2f(597, -315);
	glVertex2f(597, -315);
	glVertex2f(605, -306);

	// fourth pillar
	glVertex2f(624, -374);
	glVertex2f(624, -317);
	glVertex2f(624, -317);
	glVertex2f(634, -306);

	// cross-bar
	glVertex2f(559, -306);
	glVertex2f(650, -306);
	glEnd();	

	

	// bottom layer
	glBegin(GL_POLYGON);
	glColor3f(0.443, 0.224, 0.361);
	glVertex2f(0, -650);
	glColor3f(0.702, 0.290, 0.412);
	glVertex2f(0, -645);
	glVertex2f(448, -637);
	glVertex2f(446, -650);
	glEnd();

}

void drawLights(void) {
	glLineWidth(5);
	glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= width/10; i++) {
			float rr = float(rand() % 10) / 10.0;
			float gg = float(rand() % 10) / 10.0;
			float bb = float(rand() % 10) / 10.0;
			glColor4f(rr, gg, bb, 0.8);
			
			glVertex2f(i*10+rand() %5, -98 + sin(i)*5);
		}
		glEnd();

		glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= width / 10; i++) {
			float rr = float(rand() % 10) / 10.0;
			float gg = float(rand() % 10) / 10.0;
			float bb = float(rand() % 10) / 10.0;
			glColor4f(rr, gg, bb, 0.8);

			glVertex2f(i * 10 + rand() % 5, -98 + sin(i*2) * 15);
		
		}
		glEnd();
}

// draw a string tied to a balloon
void drawSin(int factor) {
	glBegin(GL_LINE_STRIP);
	for (float x = -5 * PI; x < 5 * PI * 5; x += 0.1f) {
		glVertex2f(factor*sin(x / 10) * 8, factor*1.1*x / (5 * PI) * 8);
	}
	glEnd();
}

void drawBallon(int i) {
	glColor4f(r[i], g[i], b[i], 0.5);
	
	glPushMatrix();
	glTranslatef(0+scale, -40*scale, 0);
	glRotatef(posx[i]/20, 0, 0, 1);
	drawSin(scale);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 0);
	float ew = r[i]*7 + 14;
	drawEllipse(ew*scale, ew*scale);
	glPopMatrix();
}

void drawBallons(void) {
	if (sped > height - 140) {
		sped = 8;
	}
	else {
		sped += speed * 1;
	}
	if (ballonState != 0) {
		for (int i = 0; i <= 40; i++) {
			glPushMatrix();
			glTranslatef(posx[i] + bstepx*10 + sped, posy[i] + bstepy*10 + sped*posy[i]*4-height, 0);
			drawBallon(i);
			glPopMatrix();
		}
	}
}

void myDisplay(void)
{
	glLineWidth(2);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	// Set the Dimensions
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, -height, 0);

	setBackground();

	// draw building
	glPushMatrix();
	glTranslatef(0, -148, 0);
	glScalef(1.23, 1, 1);
	drawBuilding();
	glPopMatrix();
	
	drawTree();

	glLineWidth(2);
	drawBallons();
	drawLights();
	
	showText(25, -78, "The 15th anniversary of Xi'an Jiaotong-liverpool University",36);
	glFlush();
}

void init() {
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char* argv[]) // Standard main function
{
	glutInit(&argc, argv);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Welcome Card");

	init();
	glutTimerFunc(160, OnTimer, 1);
	glutReshapeFunc(ChangeSize);
	glutMouseFunc(mouse_input); // mouse callback function
	glutKeyboardFunc(keyboard_input);	// keyboard callback function
	glutDisplayFunc(myDisplay);	// display
	
	glutIdleFunc(when_in_mainloop);
	glutMainLoop();
}