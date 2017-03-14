/*-----------------------------------------------------------
Pool Game.cpp

Computer Science Msc ( Advanced Programming )
Student: Dean Beckerton (BEC12375372)
Module: CMP9133M
-----------------------------------------------------------*/

#include <stdio.h>
#include <tchar.h>
#include"simulation.h"
#include <glut.h>
#include <math.h>
#include "stdafx.h"


// Draw Initilise
bool DRAW_SOLID = true;
bool DRAW_AXIS = false;	

// Function
char char_FrameRate[50];
char Display_Player[50];
char Display_ShotsLeft[50];
char Display_Colour[50];
char Display_BlackBall_Status[50];
char Display_Winner[50];

// Intilise Cue
float gCueAngle = 0.0f;
float gCuePower = 0.25f;
float gCueAngleSpeed = 2.0f; 
float gCuePowerSpeed = 0.25f;
float gCuePowerMax = 0.75f;
float gCuePowerMin = 0.1f;
float gCueBallFactor = 8.0f;
bool gDoCue = true;
bool gCueControl[4] = { false,false,false,false };

// Camera variables
vec3<float> gCamPos = vec3<float>(0.0f, 3.0f, 3.0f);
vec3<float> gCamLookAt = vec3<float>(0.0f, 0.0f, 0.0f);
bool gCamRotate = true;
float gCamRotSpeed = 0.8f;
float gCamMoveSpeed = 0.8f;
bool gCamL = false;
bool gCamR = false;
bool gCamU = false;
bool gCamD = false;
bool gCamZin = false;
bool gCamZout = false;

// Colours Within Game
GLfloat Blue_Ball_Colour[] = { 0, 0, 1.0f, 1.0f };			// Blue Ball Colour
GLfloat Yellow_Ball_Colour[] = { 1.0f, 1.0f, 0.0f };		// Yellow Ball Colour
GLfloat Black_Ball_Colour[] = { 0, 0, 0, 0 };				// Black Ball Colour 
GLfloat White_Ball_Colour[] = { 1, 1 ,1 ,1  };				// White Ball Colour 
GLfloat Silver_Cushion_Colour[] = { 0.75f, 0.75f, 0.75f };	// Silver Cushion Colour 
GLfloat Green_Table_Colour[] = { 0.2f, 1.0f, 0.0f };	// Green Table Colour

// Function Declare
void UpdateScene(int ms);
void RenderScene(void);
void InitLights(void);
void DoCamera(int ms);
void Draw_Solid_Circle(vec3<float> &pos, float radius, int segments);
void Display_PoolBalls(void);
void Display_Pool_Table(void);
void Display_Pool_Cue(void);
void Dispaly_Text(const vec3<float> &pos, const vec3<float> &colour, GLvoid *font, char* text);
void Game_Display(void);
void Keyboard_SpecialKey_Down(int key, int x, int y);
void Keyboard_SpecialKey_Up(int key, int x, int y);
void Keyboard_Key_Down(unsigned char key, int x, int y);
void Keyboard_Key_Up(unsigned char key, int x, int y);

/*-----------------------------------------------------------
	UPDATE SCENE CODE
-----------------------------------------------------------*/

void UpdateScene(int ms)
{
	if (gTable.AnyBallsMoving() == false) gDoCue = true;
	else gDoCue = false;
	glEnable(GL_LIGHTING);
	DRAW_SOLID = true;

	if (gDoCue)
	{
		glEnable(GL_LIGHTING);
		DRAW_SOLID = true;
		if (gCueControl[0]) gCueAngle -= ((gCueAngleSpeed * ms) / 1000);
		if (gCueControl[1]) gCueAngle += ((gCueAngleSpeed * ms) / 1000);
		if (gCueAngle <0.0) gCueAngle += TWO_PI;
		if (gCueAngle >TWO_PI) gCueAngle -= TWO_PI;

		if (gCueControl[2]) gCuePower += ((gCuePowerSpeed * ms) / 1000);
		if (gCueControl[3]) gCuePower -= ((gCuePowerSpeed * ms) / 1000);
		if (gCuePower > gCuePowerMax) gCuePower = gCuePowerMax;
		if (gCuePower < gCuePowerMin) gCuePower = gCuePowerMin;
	}
	glEnable(GL_LIGHTING);
	DRAW_SOLID = true;

	DoCamera(ms);
	gTable.Update(ms);
	gTable.CountAllBalls();
	glutTimerFunc(SIM_UPDATE_MS, UpdateScene, SIM_UPDATE_MS);
	glutPostRedisplay();
}


/*-----------------------------------------------------------
	RENDER GAME
-----------------------------------------------------------*/

void RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_LIGHTING);
	DRAW_SOLID = true;


	//set camera
	glLoadIdentity();
	gluLookAt(gCamPos(0), gCamPos(1), gCamPos(2), gCamLookAt(0), gCamLookAt(1), gCamLookAt(2), 0.0f, 1.0f, 0.0f);

	Display_PoolBalls();		// Balls
	Display_Pool_Table();		// Table
	Display_Pool_Cue();			// Cue
	Game_Display();				// Text Display
	
	glPopMatrix();
	glFlush();
	glutSwapBuffers();
	

}

/*-----------------------------------------------------------
	GAME LIGHTING
-----------------------------------------------------------*/

void InitLights(void)
{
	if (DRAW_SOLID)		
	{
		GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat mat_shininess[] = { 50.0 };
		GLfloat light_position[] = { 0.0, 2.0, 0.0, 1.0 };
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glShadeModel(GL_SMOOTH);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
		glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);

		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);

	}

}
/*-----------------------------------------------------------
	CAMERA CONTROLS 
-----------------------------------------------------------*/
	void DoCamera(int ms)
	{
		static const vec3<float> up(0.0, 1.0, 0.0);

		if (gCamRotate)		// Rotate Camera
		{
			if (gCamL)		// Left
			{
				vec3<float> camDir = (gCamLookAt - gCamPos).Normalised();
				vec3<float> localL = up.Cross(camDir);
				vec3<float> inc = (localL * ((gCamRotSpeed * ms) / 1000.0));
				gCamLookAt = gCamPos + camDir + inc;
			}
			if (gCamR)		// Right
			{
				vec3<float> camDir = (gCamLookAt - gCamPos).Normalised();
				vec3<float> localR = up.Cross(camDir);
				vec3<float> inc = (localR * ((gCamRotSpeed * ms) / 1000.0));
				gCamLookAt = gCamPos + camDir - inc;
			}
			if (gCamU)		// Up
			{
				vec3<float> camDir = (gCamLookAt - gCamPos).Normalised();
				vec3<float> localR = camDir.Cross(up);
				vec3<float> localUp = localR.Cross(camDir);
				vec3<float> inc = (localUp * ((gCamMoveSpeed * ms) / 1000.0));
				gCamLookAt = gCamPos + camDir + inc;
			}
			if (gCamD)		// Down
			{
				vec3<float> camDir = (gCamLookAt - gCamPos).Normalised();
				vec3<float> localR = camDir.Cross(up);
				vec3<float> localUp = localR.Cross(camDir);
				vec3<float> inc = (localUp * ((gCamMoveSpeed*ms) / 1000.0));
				gCamLookAt = gCamPos + camDir - inc;
			}
		}
		else
		{
			if (gCamL)		// left movement.
			{
				vec3<float> camDir = (gCamLookAt - gCamPos).Normalised();
				vec3<float> localL = up.Cross(camDir);
				vec3<float> inc = (localL * ((gCamMoveSpeed * ms) / 1000.0));
				gCamPos += inc;
				gCamLookAt += inc;
			}
			if (gCamR)		// right movement.
			{
				vec3<float> camDir = (gCamLookAt - gCamPos).Normalised();
				vec3<float> localR = camDir.Cross(up);
				vec3<float> inc = (localR * ((gCamMoveSpeed * ms) / 1000.0));
				gCamPos += inc;
				gCamLookAt += inc;
			}
			if (gCamU)		// up movement.
			{
				vec3<float> camDir = (gCamLookAt - gCamPos).Normalised();
				vec3<float> localR = camDir.Cross(up);
				vec3<float> localUp = localR.Cross(camDir);
				vec3<float> inc = (localUp * ((gCamMoveSpeed * ms) / 1000.0));
				gCamPos += inc;
				gCamLookAt += inc;
			}
			if (gCamD)		// down movement.
			{
				vec3<float> camDir = (gCamLookAt - gCamPos).Normalised();
				vec3<float> localR = camDir.Cross(up);
				vec3<float> localDown = camDir.Cross(localR);
				vec3<float> inc = (localDown * ((gCamMoveSpeed * ms) / 1000.0));
				gCamPos += inc;
				gCamLookAt += inc;
			}
		}

		if (gCamZin)	// zoom in.
		{
			vec3<float> camDir = (gCamLookAt - gCamPos).Normalised();
			vec3<float> inc = (camDir * ((gCamMoveSpeed * ms) / 1000.0));
			gCamPos += inc;
			gCamLookAt += inc;
		}
		if (gCamZout)	// zoom out.
		{
			vec3<float> camDir = (gCamLookAt - gCamPos).Normalised();
			vec3<float> inc = (camDir * ((gCamMoveSpeed * ms) / 1000.0));
			gCamPos -= inc;
			gCamLookAt -= inc;
		}
	}



/*-----------------------------------------------------------
				DRAW SOLID CIRCLE
-----------------------------------------------------------*/

// Used For Pocket Circle Output
void Draw_Solid_Circle(vec3<float> &pos, float radius, int segments)
{
	
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(pos(0), pos(1), pos(2));
	for (int n = 0; n <= segments; ++n)
	{
		float const t = 2 * 3.14 * (float)n / (float)segments;
		glVertex3f(pos(0) + sin(t) * radius, pos(1), pos(2) + cos(t) * radius);
	}
	glEnd();
}

/*-----------------------------------------------------------
				COLOUR POOL BALLS
-----------------------------------------------------------*/

//Colour all Balls
void Display_PoolBalls(void)
{
	for (int i = 0; i < NUM_BALLS; i++)
	{
		switch (gTable.balls[i].colour)
		{
		case 0:
		{
			// White ball
			glMaterialfv(GL_FRONT, GL_DIFFUSE, White_Ball_Colour);
			break;
		}
		case 1:
		{
			// Black ball
			glMaterialfv(GL_FRONT, GL_DIFFUSE, Black_Ball_Colour);
			break;
		}
		case 2:
		{
			// Blue ball
			glMaterialfv(GL_FRONT, GL_DIFFUSE, Blue_Ball_Colour);
			break;
		}
		case 3:
		{
			// Yellow ball
			glMaterialfv(GL_FRONT, GL_DIFFUSE, Yellow_Ball_Colour);
			break;
		}
		}
		glPushMatrix();
		glTranslatef(gTable.balls[i].position(0), gTable.balls[i].position(1), gTable.balls[i].position(2));			
		glutSolidSphere(gTable.balls[i].radius, 32, 32); // Draw as Solid
		glPopMatrix();
	}
}


/*-----------------------------------------------------------
				Create Pool Table Surface
-----------------------------------------------------------*/

void Display_Pool_Table(void)
{
	// The function for Rendering the Table.
	glPushMatrix();
	if (DRAW_SOLID)		// If solid mode is on.
	{
		// Solid mode.
		glMaterialfv(GL_FRONT, GL_DIFFUSE, Green_Table_Colour);			// Green Colour to Table
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-TABLE_X, 0.0, -TABLE_Z);
		glVertex3f(-TABLE_X, 0.0, TABLE_Z);
		glVertex3f(TABLE_X, 0.0, TABLE_Z);
		glVertex3f(TABLE_X, 0.0, -TABLE_Z);
		glEnd();

/*-----------------------------------------------------------
					Create Cushions
-----------------------------------------------------------*/

glMaterialfv(GL_FRONT, GL_DIFFUSE, Silver_Cushion_Colour);	//Silver Colour
	
	for (int i = 0; i < NUM_CUSHIONS; i++)
		{
			// Draw each cushion face.
			for (int j = 0; j < NUM_CUSHIONSECTIONS; j++)
			{
				glBegin(GL_POLYGON);
				glNormal3f(gTable.cushions[i].cushionFaces[j].normal(0), gTable.cushions[i].cushionFaces[j].normal(1), gTable.cushions[i].cushionFaces[j].normal(2));
				glVertex3f(gTable.cushions[i].cushionFaces[j].vertices[0](0), gTable.cushions[i].cushionFaces[j].vertices[0](1), gTable.cushions[i].cushionFaces[j].vertices[0](2));
				glVertex3f(gTable.cushions[i].cushionFaces[j].vertices[1](0), gTable.cushions[i].cushionFaces[j].vertices[1](1), gTable.cushions[i].cushionFaces[j].vertices[1](2));
				glVertex3f(gTable.cushions[i].cushionFaces[j].vertices[2](0), gTable.cushions[i].cushionFaces[j].vertices[2](1), gTable.cushions[i].cushionFaces[j].vertices[2](2));
				glVertex3f(gTable.cushions[i].cushionFaces[j].vertices[3](0), gTable.cushions[i].cushionFaces[j].vertices[3](1), gTable.cushions[i].cushionFaces[j].vertices[3](2));
				glEnd();
			}
		}

/*-----------------------------------------------------------
				Draw Pockets
-----------------------------------------------------------*/

	for (int i = 0; i < NUMBER_OF_POCKETS; i++)
		{
			if (DRAW_SOLID = false)		//Enable Light
			{
				glEnable(GL_LIGHTING);
			}
	
			Draw_Solid_Circle(gTable.pockets[i].position, gTable.pockets[i].radius, 100);
			if (DRAW_SOLID)		// Draw Pocket as a Solid Circle
			{
				glEnable(GL_LIGHTING);
			}	
			
		}
	}

	if (DRAW_SOLID)
	{
		glDisable(GL_LIGHTING);		// Line Loop For Start Line
	}
	
	// Draw Start Line
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);				// Line Colour (White)
	glVertex3f(-0.7f, 0.001f, TABLE_Z / 2);		// POS
	glVertex3f(0.7f, 0.001f, TABLE_Z / 2);		// POS
	glEnd();

}


/*-----------------------------------------------------------
				Create Pool CUE
-----------------------------------------------------------*/

void Display_Pool_Cue(void)
{
	// The function for Rendering the Cue.
	if (gDoCue)	// If the cue can be used.
	{
		float cueX = sin(gCueAngle) * gCuePower;	// X offset.
		float cueZ = cos(gCueAngle) * gCuePower;	// Z offset.
	
		// Wireframe
		glBegin(GL_LINES);
		glVertex3f(gTable.balls[0].position(0), BALL_RADIUS, gTable.balls[0].position(2));					
		glVertex3f((gTable.balls[0].position(0) + cueX), BALL_RADIUS, (gTable.balls[0].position(2) + cueZ));	
		glEnd();
		}
	}

/*-----------------------------------------------------------
				Create Text
-----------------------------------------------------------*/

void Dispaly_Text(const vec3<float> &pos, const vec3<float> &colour, GLvoid *font, char* text)
{
	// Disable Lighting to Display text to screen
	glDisable(GL_LIGHTING);
	DRAW_SOLID = true;
	glColor3f(1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 300, 0, 300, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRasterPos3i(pos(0), pos(1), pos(2));
	while (*text)
	{
		glutBitmapCharacter(font, *text);
		text++;
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}



/*-----------------------------------------------------------
				DISPLAY
-----------------------------------------------------------*/

void Game_Display(void)
{
	// Player Playing
	int displayPlayerOutput = gTable.CurrentPlayerCount + 1;
	sprintf(Display_Player, "Current Player is Number %1i", displayPlayerOutput);

	// Colour for Player
	std::string colour = "N/A";
	
	// Checks if Player has a Colour Set
	if (gTable.players[gTable.CurrentPlayerCount]->BallColourValue == 2)		
	{
		colour = "Blue"; // Blue
	}
	else if (gTable.players[gTable.CurrentPlayerCount]->BallColourValue == 3)	
	{
		colour = "Yellow"; // Yellow
	}
	sprintf(Display_Colour, "Your Colour is ball: %s", colour.c_str());

	// Shots Remaining
	int displayNumberOfShots = gTable.players[gTable.CurrentPlayerCount]->PlayerShotsLeft;
	sprintf(Display_ShotsLeft, "Your Shots Left: %1i", displayNumberOfShots);

	// Checks if Black Ball is on Table
	std::string blackPotted = "No";
	if (gTable.CheckBlackBall)		
	{
		blackPotted = "Yes";
	}
	sprintf(Display_BlackBall_Status, "Has the Black Ball been Potted: %s", blackPotted.c_str());

	// Display the Player Winner
	int displayWinner = gTable.Winner + 1;
	sprintf(Display_Winner, "Your the Winner: Player %1i!", displayWinner);
	
	// Display Output ( Position, Font, Output)
	Dispaly_Text(vec3<float>(5, 290, 0), vec3<float>(1.0, 1.0, 1.0), GLUT_BITMAP_9_BY_15, "--Pool Game Stats--");
	Dispaly_Text(vec3<float>(5, 280, 0), vec3<float>(1.0, 1.0, 1.0), GLUT_BITMAP_9_BY_15, Display_Player);
	Dispaly_Text(vec3<float>(5, 272, 0), vec3<float>(1.0, 1.0, 1.0), GLUT_BITMAP_9_BY_15, Display_Colour);
	Dispaly_Text(vec3<float>(5, 264, 0), vec3<float>(1.0, 1.0, 1.0), GLUT_BITMAP_9_BY_15, Display_ShotsLeft);
	Dispaly_Text(vec3<float>(5, 256, 0), vec3<float>(1.0, 1.0, 1.0), GLUT_BITMAP_9_BY_15, Display_BlackBall_Status);
	Dispaly_Text(vec3<float>(5, 244, 0), vec3<float>(1.0, 1.0, 1.0), GLUT_BITMAP_9_BY_15, "--Controls--");
	Dispaly_Text(vec3<float>(5, 234, 0), vec3<float>(1.0, 1.0, 1.0), GLUT_BITMAP_9_BY_15, "F1 / F2  Swap View Point");
	Dispaly_Text(vec3<float>(5, 224, 0), vec3<float>(1.0, 1.0, 1.0), GLUT_BITMAP_9_BY_15, "W / A / S / D - Camera");
	Dispaly_Text(vec3<float>(5, 214, 0), vec3<float>(1.0, 1.0, 1.0), GLUT_BITMAP_9_BY_15, "R / F - Zoom / Zoom out");
	Dispaly_Text(vec3<float>(5, 204, 0), vec3<float>(1.0, 1.0, 1.0), GLUT_BITMAP_9_BY_15, "Hold Space - Move Camera");
	Dispaly_Text(vec3<float>(120, 290, 0), vec3<float>(1.0, 1.0, 1.0), GLUT_BITMAP_9_BY_15, "Dean Beckerton (BEC12375372)");

	// Display if a Player Wins
	if (gTable.Winner != -1)	
	{
		Dispaly_Text(vec3<float>(125, 320, 0), vec3<float>(1.0, 1.0, 1.0), GLUT_BITMAP_9_BY_15, Display_Winner);
	}
}

void Keyboard_SpecialKey_Down(int key, int x, int y)
{
	// The function for handling the Special Keyboard keys being pressed.
	switch (key)
	{
	case GLUT_KEY_LEFT:
	{
		gCueControl[0] = true;
		break;
	}
	case GLUT_KEY_RIGHT:
	{
		gCueControl[1] = true;
		break;
	}
	case GLUT_KEY_UP:
	{
		gCueControl[2] = true;
		break;
	}
	case GLUT_KEY_DOWN:
	{
		gCueControl[3] = true;
		break;
	}

	// Camera positions.
	case GLUT_KEY_F1:
	{
		gCamPos = vec3<float>(0.0f, 2.0f, 2.5f);
		gCamLookAt = vec3<float>(0.0f, 0.0f, 0.0f);
		break;
	}
	
	case GLUT_KEY_F2:
	{
		gCamPos = vec3<float>(0.0f, 4.0f, 0.01f);
		gCamLookAt = vec3<float>(0.0f, 0.0f, 0.0f);
		break;
	}
	
	}
}
void Keyboard_SpecialKey_Up(int key, int x, int y)
{
	// The function for handling the Special Keyboard keys not being pressed.
	switch (key)
	{
	case GLUT_KEY_LEFT:
	{
		gCueControl[0] = false;
		break;
	}
	case GLUT_KEY_RIGHT:
	{
		gCueControl[1] = false;
		break;
	}
	case GLUT_KEY_UP:
	{
		gCueControl[2] = false;
		break;
	}
	case GLUT_KEY_DOWN:
	{
		gCueControl[3] = false;
		break;
	}
	}
}
void Keyboard_Key_Down(unsigned char key, int x, int y)
{
	// The function for handling the Keyboard keys being pressed.
	switch (key)
	{
	case(13):
	{
		if (gDoCue)
		{
			vec3<float> imp((-sin(gCueAngle) * gCuePower * gCueBallFactor), 0.0f, (-cos(gCueAngle) * gCuePower * gCueBallFactor));
			gTable.balls[0].ApplyImpulse(imp);
		}
		break;
	}
	case(27):
	{
		gTable.Reset();
		break;
	}
	case(32):
	{
		gCamRotate = false;
		break;
	}
	case('a'):
	{
		gCamL = true;
		break;
	}
	case('d'):
	{
		gCamR = true;
		break;
	}
	case('w'):
	{
		gCamU = true;
		break;
	}
	case('s'):
	{
		gCamD = true;
		break;
	}
	case('r'):
	{
		gCamZin = true;
		break;
	}
	case('f'):
	{
		gCamZout = true;
		break;
	}
	}
}
void Keyboard_Key_Up(unsigned char key, int x, int y)
{
	// The function for handling the Keyboard keys not being pressed.
	switch (key)
	{
	case(32):
	{
		gCamRotate = true;
		break;
	}
	case('a'):
	{
		gCamL = false;
		break;
	}
	case('d'):
	{
		gCamR = false;
		break;
	}
	case('w'):
	{
		gCamU = false;
		break;
	}
	case('s'):
	{
		gCamD = false;
		break;
	}
	case('r'):
	{
		gCamZin = false;
		break;
	}
	case('f'):
	{
		gCamZout = false;
		break;
	}
	}
}

void ChangeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
	{
		h = 1;
	}
	float ratio = 1.0 * w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45, ratio, 0.2, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(gCamPos(0), gCamPos(1), gCamPos(2), gCamLookAt(0), gCamLookAt(1), gCamLookAt(2), 0.0f, 1.0f, 0.0f);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, ((char **)argv));
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1000, 700);
	glutCreateWindow("Msc: Advanced Programming (BEC12375372)");
	InitLights();		// Initialises the OpenGL lights.
	glutDisplayFunc(RenderScene);
	glutTimerFunc(SIM_UPDATE_MS, UpdateScene, SIM_UPDATE_MS);
	glutReshapeFunc(ChangeSize);
	glutIdleFunc(RenderScene);
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(Keyboard_Key_Down);
	glutKeyboardUpFunc(Keyboard_Key_Up);
	glutSpecialFunc(Keyboard_SpecialKey_Down);
	glutSpecialUpFunc(Keyboard_SpecialKey_Up);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}






