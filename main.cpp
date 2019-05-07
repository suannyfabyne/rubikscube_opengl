// #include<windows.h>
#include <GL/glut.h>
#include <iostream>
#include<stdlib.h>
#include "SOIL.h"

GLfloat angle, fAspect;


unsigned char* img;
int img_width;
int img_height;
int img_channels;

GLuint *textures = new GLuint[3];

GLfloat aspect = 1.0f;
GLfloat tex_ss = 1.0f;
GLfloat tex_st = 1.0f;
GLuint render_object = 0;

int C[6][10];
int xro[28], yro[28], zro[28];

int done = 0, di = 1, rot = 0, yes = 1, k = 0;
float ang = 0.0;
int dragging, unix, uniy, uniz, unif, clicked = 0;
int rotate_x = -25, rotate_y = 35;
int insp = 10;
int pause = -1, pressed = 0, solving = 0, click = 0;
int tmi = 0, tm = 0, tse = 0;
char st[8] = { '0',':','0','0',':','0','0','\0' }; // variavel do timer

void xrotation(int, int);
void yrotation(int, int);
void rmy(int, int, int, int, int);
void zrotation(int, int);
void rmz(int, int, int, int, int);
void fr(int);
void franti(int);
void yfinal(int);
void xfinal(int);
void zfinal(int);
void wholr(int, int);

void mouse_click(int, int, int, int);
void mouse_move(int, int);

int mpx, mpy, mpz;
int diry, layy, dirx, layx, dirz, layz;


void Ilumin(void)
{
	GLfloat luzAmbiente[4] = { 0.2,0.2,0.2,1.0 };
	GLfloat luzDifusa[4] = { 0.7,0.7,0.7,1.0 };	   // "cor" 
	GLfloat luzEspecular[4] = { 0.4, 0.4, 0.4, 1.0 };// "brilho" 
	GLfloat posicaoLuz[4] = { 0.5, 0.5, 0.0, 1.0 };
	// Capacidade de brilho do material
	GLfloat especularidade[4] = { 1.0,1.0,1.0,1.0 };
	GLint especMaterial = 60;

	// Especifica que a cor de fundo da janela será preta
	glClearColor(0.8, 0.8, 0.8, 1.0);

	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Define a refletância do material 
	glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT, GL_SHININESS, especMaterial);

	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);

	angle = 45;
}

void ChangeSize(int w, int h)
{
	glViewport(0, 0, w, h);
}

void scramble(int x)
{
	int i;
	for (i = 0; i < 20; i++)
	{
		switch ((rand() + x) % 20)
		{
		case 1:xrotation(3, -1); break;
		case 2:xrotation(2, -1); break;
		case 3:xrotation(1, -1); break;
		case 4:xrotation(1, 1); break;
		case 5:xrotation(3, 1); break;
		case 6:xrotation(2, 1); break;
		case 7:yrotation(3, -1); break;
		case 8:yrotation(2, -1); break;
		case 9:yrotation(1, -1); break;
		case 10:yrotation(1, 1); break;
		case 11:yrotation(3, 1); break;
		case 12:yrotation(2, 1); break;
		case 13:zrotation(3, -1); break;
		case 14:zrotation(2, -1); break;
		case 15:zrotation(1, -1); break;
		case 16:zrotation(1, 1); break;
		case 17:zrotation(3, 1); break;
		case 18:zrotation(2, 1); break;

		default:break;
		}
	}
}
void renderBitmapString(float x, float y, void *font, const char *string) {
	const char *c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void specialKeys(int key, int x, int y)
{
	if (yes)
	{
		if (key == GLUT_KEY_RIGHT) {
			wholr(0, -1);
		}

		else if (key == GLUT_KEY_LEFT) {
			wholr(0, 1);
		}

		else if (key == GLUT_KEY_UP) {
			wholr(1, 1);
		}

		else if (key == GLUT_KEY_DOWN) {
			wholr(1, -1);
		}
	}
}
//_________________________________ANIMATION___
void wholr(int ax, int dir)
{
	int i;
	di = dir;
	for (i = 0; i < 28; i++)
	{
		if (ax)
			xro[i] = 1;
		else
			yro[i] = 1;
	}

	rot = 10 + 5 * ax + dir;
}
void anim(int t)
{
	glutPostRedisplay();
	glutTimerFunc(15, anim, 1);
}
void yfinal(int com)
{

	int i;
	diry = di = 2 * (com / 6) - 1;
	layy = (com - 1) % 3 + 1;

	for (i = 1; i < 10; i++)
		yro[(layy - 1) * 9 + i] = 1;
	rot = 1;
}
void xfinal(int com)
{
	dirx = di = 2 * (com / 6) - 1;
	layx = (com - 1) % 3 + 1;
	if (layx == 1)
	{
		xro[7] = 1;
		xro[8] = 1;
		xro[9] = 1;
		xro[16] = 1;
		xro[17] = 1;
		xro[18] = 1;
		xro[25] = 1;
		xro[26] = 1;
		xro[27] = 1;
	}
	if (layx == 2)
	{
		xro[4] = 1;
		xro[5] = 1;
		xro[6] = 1;
		xro[13] = 1;
		xro[14] = 1;
		xro[15] = 1;
		xro[22] = 1;
		xro[23] = 1;
		xro[24] = 1;
	}
	if (layx == 3)
	{
		xro[1] = 1;
		xro[2] = 1;
		xro[3] = 1;
		xro[10] = 1;
		xro[11] = 1;
		xro[12] = 1;
		xro[19] = 1;
		xro[20] = 1;
		xro[21] = 1;
	}

	rot = 2;
}
void zfinal(int com)
{
	dirz = -1 * (di = 2 * (com / 6) - 1);
	layz = (com - 1) % 3 + 1;
	if (layz == 1)
	{
		zro[9] = 1;
		zro[6] = 1;
		zro[3] = 1;
		zro[18] = 1;
		zro[15] = 1;
		zro[12] = 1;
		zro[21] = 1;
		zro[24] = 1;
		zro[27] = 1;
	}
	if (layz == 2)
	{
		zro[2] = 1;
		zro[5] = 1;
		zro[8] = 1;
		zro[11] = 1;
		zro[14] = 1;
		zro[17] = 1;
		zro[20] = 1;
		zro[23] = 1;
		zro[26] = 1;
	}
	if (layz == 3)
	{
		zro[1] = 1;
		zro[4] = 1;
		zro[7] = 1;
		zro[10] = 1;
		zro[13] = 1;
		zro[16] = 1;
		zro[19] = 1;
		zro[22] = 1;
		zro[25] = 1;
	}
	rot = 3;
}
void setcol(int cc)
{
	switch (cc)
	{
	case 0:glColor3f(1.0, 1.0, 1.0); break;
	case 1:glColor3f(0.0, 0.0, 1.0); break;
	case 2:glColor3f(1.0, 1.0, 0.0); break;
	case 3:glColor3f(0.0, 1.0, 0.0); break;
	case 4:glColor3f(1.0, 0.0, 0.0); break;
	case 5:glColor3f(1.0, 0.5, 0.0); break;
	case 6:glColor3f(0.1, 0.0, 0.0); break;
	default:break;
	}
}
//___________________________________________________ ____(SINGLE) CUBE
void mycube(int pic, float siz, float x, float y, float z, int d, int f, int u, int b, int r, int l)
{
	siz /= 2;
	glPushMatrix();

	if (yro[pic])
		glRotatef(ang*di, 0.0, 1.0, 0.0);
	if (xro[pic])
		glRotatef(ang*di, 1.0, 0.0, 0.0);
	if (zro[pic])
		glRotatef(ang*di, 0.0, 0.0, 1.0);
	//glScalef(ang/15,ang/15,ang/15);
	//glPopMatrix();
	//glPushMatrix();
	glTranslatef(x, y, z);

	glBegin(GL_POLYGON);
	setcol(d);
	glVertex3f(siz, -siz, siz);
	glVertex3f(siz, -siz, -siz);
	glVertex3f(-siz, -siz, -siz);
	glVertex3f(-siz, -siz, siz);
	glEnd();

	glBegin(GL_POLYGON);
	setcol(b);
	glVertex3f(siz, siz, siz);
	glVertex3f(siz, -siz, siz);
	glVertex3f(-siz, -siz, siz);
	glVertex3f(-siz, siz, siz);
	glEnd();

	glBegin(GL_POLYGON);
	setcol(u);
	glVertex3f(siz, siz, siz);
	glVertex3f(siz, siz, -siz);
	glVertex3f(-siz, siz, -siz);
	glVertex3f(-siz, siz, siz);
	glEnd();

	glBegin(GL_POLYGON);
	setcol(f);
	glVertex3f(siz, siz, -siz);
	glVertex3f(siz, -siz, -siz);
	glVertex3f(-siz, -siz, -siz);
	glVertex3f(-siz, siz, -siz);
	glEnd();

	glBegin(GL_POLYGON);
	setcol(r);
	glVertex3f(siz, siz, siz);
	glVertex3f(siz, -siz, siz);
	glVertex3f(siz, -siz, -siz);
	glVertex3f(siz, siz, -siz);
	glEnd();

	glBegin(GL_POLYGON);
	setcol(l);
	glVertex3f(-siz, siz, siz);
	glVertex3f(-siz, -siz, siz);
	glVertex3f(-siz, -siz, -siz);
	glVertex3f(-siz, siz, -siz);
	glEnd();

	glPopMatrix();
}
//_________________________________________________________RUBIK'S CUBE
void rubic(float siz, float x)
{
	mycube(1, siz, x, x, x, 6, 6, C[2][3], C[3][9], C[4][3], 6);
	mycube(2, siz, x, x, 0, 6, 6, C[2][6], 6, C[4][2], 6);
	mycube(3, siz, x, x, -x, 6, C[1][3], C[2][9], 6, C[4][1], 6);

	mycube(4, siz, 0, x, x, 6, 6, C[2][2], C[3][8], 6, 6);
	mycube(5, siz, 0, x, 0, 6, 6, C[2][5], 6, 6, 6);
	mycube(6, siz, 0, x, -x, 6, C[1][2], C[2][8], 6, 6, 6);

	mycube(7, siz, -x, x, x, 6, 6, C[2][1], C[3][7], 6, C[5][3]);
	mycube(8, siz, -x, x, 0, 6, 6, C[2][4], 6, 6, C[5][2]);
	mycube(9, siz, -x, x, -x, 6, C[1][1], C[2][7], 6, 6, C[5][1]);
	//_____MIDDLE_LAYER
	mycube(10, siz, x, 0, x, 6, 6, 6, C[3][6], C[4][6], 6);
	mycube(11, siz, x, 0, 0, 6, 6, 6, 6, C[4][5], 6);
	mycube(12, siz, x, 0, -x, 6, C[1][6], 6, 6, C[4][4], 6);

	mycube(13, siz, 0, 0, x, 6, 6, 6, C[3][5], 6, 6);
	//mycube(2,siz, 0, 0, 0,6       ,6      ,6      ,6      ,6      ,6);
	mycube(15, siz, 0, 0, -x, 6, C[1][5], 6, 6, 6, 6);

	mycube(16, siz, -x, 0, x, 6, 6, 6, C[3][4], 6, C[5][6]);
	mycube(17, siz, -x, 0, 0, 6, 6, 6, 6, 6, C[5][5]);
	mycube(18, siz, -x, 0, -x, 6, C[1][4], 6, 6, 6, C[5][4]);
	//_________BOTTOM LATER____
	mycube(19, siz, x, -x, x, C[0][9], 6, 6, C[3][3], C[4][9], 6);
	mycube(20, siz, x, -x, 0, C[0][6], 6, 6, 6, C[4][8], 6);
	mycube(21, siz, x, -x, -x, C[0][3], C[1][9], 6, 6, C[4][7], 6);

	mycube(22, siz, 0, -x, x, C[0][8], 6, 6, C[3][2], 6, 6);
	mycube(23, siz, 0, -x, 0, C[0][5], 6, 6, 6, 6, 6);
	mycube(24, siz, 0, -x, -x, C[0][2], C[1][8], 6, 6, 6, 6);

	mycube(25, siz, -x, -x, x, C[0][7], 6, 6, C[3][1], 6, C[5][9]);
	mycube(26, siz, -x, -x, 0, C[0][4], 6, 6, 6, 6, C[5][8]);
	mycube(27, siz, -x, -x, -x, C[0][1], C[1][7], 6, 6, 6, C[5][7]);
}

//_________________________________________________________DISPLAY
void draw()
{

	int i, j, flag = 0;
	float siz = 0.2, x, sp = .02;
	x = siz + sp;
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);
	if (rot)
	{
		ang += insp;
		if (ang > 90)
		{
			ang = 0;
			for (i = 0; i < 28; i++)
			{
				xro[i] = 0;
				yro[i] = 0;
				zro[i] = 0;
			}
			if (rot == 1)
				yrotation(layy, diry);
			if (rot == 2)
				xrotation(layx, dirx);
			if (rot == 3)
				zrotation(layz, dirz);
			if (rot == 9)
			{
				yrotation(1, -1);
				yrotation(2, -1);
				yrotation(3, -1);
			}
			if (rot == 11)
			{
				yrotation(1, 1);
				yrotation(2, 1);
				yrotation(3, 1);
			}
			if (rot == 14)
			{
				xrotation(1, -1);
				xrotation(2, -1);
				xrotation(3, -1);
			}
			if (rot == 16)
			{
				xrotation(1, 1);
				xrotation(2, 1);
				xrotation(3, 1);
			}

			rot = 0;
			yes = 1;
		}
	}
	rubic(siz, x);
	glColor3f(0.0, 0.0, 0.0);

	if (click) {
		tmi += 2;
		tse += tmi / 100;
		tm += tse / 60;
		tmi %= 100;
		tse %= 60;
		st[0] = 48 + tm;
		st[2] = 48 + tse / 10;
		st[3] = 48 + tse % 10;
		st[5] = 48 + tmi / 10;
		st[6] = 48 + tmi % 10;
		renderBitmapString(0.8, -0.8, GLUT_BITMAP_TIMES_ROMAN_24, st);
	}
	else if (pause == 0) {
		renderBitmapString(0.8, -0.8, GLUT_BITMAP_TIMES_ROMAN_24, st);
	}
	else {
		st[0] = 48 + tm;
		st[2] = 48 + tse / 10;
		st[3] = 48 + tse % 10;
		st[5] = 48 + tmi / 10;
		st[6] = 48 + tmi % 10;
		renderBitmapString(0.8, -0.8, GLUT_BITMAP_TIMES_ROMAN_24, st);
	}


	for (i = 0; i < 6; i++)
		for (j = 1; j < 10; j++)
		{
			if (C[i][5] != C[i][j])
			{
				flag = 1;
			}
		}
	if (!flag) {
		pressed = 0;
		solving = 0;
		pause = 0;
		click = 0;
		renderBitmapString(-0.25, 0.7, GLUT_BITMAP_TIMES_ROMAN_24, "RESOLVIDO!");

	}

	glColor3f(1.0, 1.0, 1.0);

	//glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, aspect, 0.01f, 10.0f);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(tex_ss, tex_st, 1);

	glMatrixMode(GL_MODELVIEW);


	// // RETANGULOS
	glLoadIdentity(); // Reseta a matriz de transforma��o ( joga a matriz identidade )

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTranslatef(-0.705f, -0.65f, 0.5f);
	glScalef(0.4f, 0.1f, 0.1);
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);

	if (render_object == 0) // render the quad
	{
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 0.0);
		glVertex2d(-0.5, -0.5);

		glTexCoord2d(1.0, 0.0);
		glVertex2d(0.5, -0.5);

		glTexCoord2d(1.0, 1.0);
		glVertex2d(0.5, 0.5);

		glTexCoord2d(0.0, 1.0);
		glVertex2d(-0.5, 0.5);
		glEnd();
	}
	else // render the teapot
	{
		glutSolidTeapot(0.5f);
	}

	// // RETANGULOS
	glLoadIdentity(); // Reseta a matriz de transforma��o ( joga a matriz identidade )

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTranslatef(-0.705f, -0.53f, 0.5f);
	glScalef(0.4f, 0.1f, 0.1);
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);

	if (render_object == 0) // render the quad
	{
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 0.0);
		glVertex2d(-0.5, -0.5);

		glTexCoord2d(1.0, 0.0);
		glVertex2d(0.5, -0.5);

		glTexCoord2d(1.0, 1.0);
		glVertex2d(0.5, 0.5);

		glTexCoord2d(0.0, 1.0);
		glVertex2d(-0.5, 0.5);
		glEnd();
	}
	else // render the teapot
	{
		glutSolidTeapot(0.5f);
	}

	// // RETANGULOS
	glLoadIdentity(); // Reseta a matriz de transforma��o ( joga a matriz identidade )

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glTranslatef(-0.0f, -0.83f, 0.5f);
	glScalef(0.3f, 0.3f, 0.1);
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);

	if (render_object == 0) // render the quad
	{
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 0.0);
		glVertex2d(-0.5, -0.5);

		glTexCoord2d(1.0, 0.0);
		glVertex2d(0.5, -0.5);

		glTexCoord2d(1.0, 1.0);
		glVertex2d(0.5, 0.5);

		glTexCoord2d(0.0, 1.0);
		glVertex2d(-0.5, 0.5);
		glEnd();
	}
	else // render the teapot
	{
		glutSolidTeapot(0.5f);
	}

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

//_________________________________________________________MOUSE
void mouse_click(int button, int state, int x, int y)
{
	float fux, fuy, fax, fay;
	int nf[6], i, j;
	nf[3] = 0;
	nf[4] = 0;
	nf[5] = 0;
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_UP) {
			if (x > 33 && x < 171 && y>561 && y < 593) { // botao misturar
				scramble(x);
				click = 0;
				pause = -1;
				solving = 1;
				pressed = 0;
				tmi = 0; tm = 0; tse = 0;

			}
			/*if (x > 34 && x < 117 && y>525 && y < 544) { // bot�o "play-pause"
				if (solving)
					pause = (-1)*pause;
				//printf("%d %d \n",x,y);
			}*/
			if (x > 33 && x < 171 && y>519 && y < 551) // botao resetar
			{
				pressed = 0;
				solving = 0;
				click = 0;
				pause = -1;
				for (i = 0; i < 6; i++)
					for (j = 0; j < 10; j++)
						C[i][j] = i;
				for (i = 0; i < 28; i++)
				{
					xro[i] = 0;
					yro[i] = 0;
					zro[i] = 0;
				}

				tmi = 0; tm = 0; tse = 0;
			}
			//******* botoes esquerdo
			if (x > 125 && x < 200 && y>250 && y < 315) { //triangulos da esquerda 
				yfinal(7); yes = 0;
				click = 1;
			}
			else if (x > 125 && x < 200 && y>340 && y < 385) { //triangulos da esquerda 
				yfinal(8); yes = 0;
				click = 1;
			}
			else if (x > 125 && x < 200 && y>410 && y < 470) { //triangulos da esquerda 
				yfinal(9); yes = 0;
				click = 1;
			}
			//******* botoes direito 
			else if (x > 500 && x < 575 && y>240 && y < 290) { //triangulos da direita
				yfinal(1); yes = 0;
				click = 1;
			}
			else if (x > 500 && x < 575 && y>300 && y < 350) { //triangulos da direita
				yfinal(2); yes = 0;
				click = 1;
			}
			else if (x > 500 && x < 575 && y>370 && y < 410) { //triangulos da direita
				yfinal(3); yes = 0;
				click = 1;
			}
			else if (x > 332 && x < 368 && y>652 && y < 691) { //CONTROL DOWN
				wholr(1, -1);
				click = 1;
			}
			else if (x > 330 && x < 367 && y>589 && y < 626) { //CONTROL UP
				wholr(1, 1);
				click = 1;
			}
			else if (x > 298 && x < 336 && y>620 & y < 660) { //CONTROL LEFT
				wholr(0, 1);
				click = 1;
			}
			else if (x > 364 && x < 398 && y>621 & y < 656) { //CONTROL RIGHT
				wholr(0, -1);
				click = 1;
			}
			//******* botoes norte direita
			else if (x > 460 && x < 510 && y>200 && y < 240) { //triangulos da direita
				xfinal(9); yes = 0;
				click = 1;
			}
			else if (x > 390 && x < 450 && y>180 && y < 220) { //triangulos da direita
				xfinal(8); yes = 0;
				click = 1;
			}
			else if (x > 300 && x < 390 && y>155 && y < 195) { //triangulos da direita
				xfinal(7); yes = 0;
				click = 1;
			}
			//******* botoes norte esquerda
			else if (x > 173 && x < 233 && y>226 && y < 272) { //triangulos da direita
				zfinal(7); yes = 0;
				click = 1;
			}
			else if (x > 216 && x < 272 && y>202 && y < 245) { //triangulos da direita
				zfinal(8); yes = 0;
				click = 1;
			}
			else if (x > 261 && x < 316 && y>174 && y < 220) { //triangulos da direita
				zfinal(9); yes = 0;
				click = 1;
			}
			//******* botoes baixo
			else if (x > 190 && x < 250 && y>470 && y < 520) { //triangulos da direita
				xfinal(1); yes = 0;
				click = 1;
			}

			else if (x > 155 && x < 310 && y>490 && y < 530) { //triangulos da direita
				xfinal(2); yes = 0;
				click = 1;
			}

			else if (x > 320 && x < 375 && y>500 && y < 550) { //triangulos da direita
				xfinal(3); yes = 0;
				click = 1;
			}

			//******* botoes baixo direita

			else if (x > 380 && x < 430 && y>494 && y < 550) { //triangulos da direita
				zfinal(1); yes = 0;
				click = 1;
			}
			else if (x > 425 && x < 470 && y>470 && y < 520) { //triangulos da direita
				zfinal(2); yes = 0;
				click = 1;
			}
			else if (x > 479 && x < 510 && y>440 && y < 490) { //triangulos da direita
				zfinal(3); yes = 0;
				click = 1;
			}
		}
		if (state == GLUT_DOWN)
		{
			dragging = 1;


			if (x > 192 && x < 507)
			{
				unix = x - 192;
				if (unix / 62 < 3) {
					nf[0] = 4;
					unix = unix / 62;
				}
				else
				{
					nf[0] = 1;
					unix -= 186;
					unix = 3 + unix / 43;
				}

				fux = x - 195;
				fuy = y - 261;
				fux = fux + 1.63*fuy;
				if (fux > 0 && fux < 600)
				{

					if (fux < 270) {
						nf[1] = 4;
						uniy = fux / 90;
					}
					else {
						nf[1] = 2;
						uniy = 3 + (fux - 270) / 110;
					}

					fax = x - 322;
					fay = y - 183;
					fax = 3.31*fay - fax;
					if (fax > 0 && fax < 1050)
					{
						if (fax < 381) {
							nf[2] = 1;
							uniz = fax / 127;
						}
						else {
							nf[2] = 2;
							uniz = 3 + (fax - 381) / 223;
						}
						for (i = 0; i < 3; i++)
						{
							if (nf[i] == 1)nf[3] = 1;
							if (nf[i] == 2)nf[4] = 1;
							if (nf[i] == 4)nf[5] = 1;
						}
						if (nf[3] == 0)unif = 1;
						if (nf[4] == 0)unif = 2;
						if (nf[5] == 0)unif = 4;
						clicked = 1;
						//printf("%d %d %d %d\n",unix,uniy,uniz,unif);
						//allocm(f,unix,uniy,uniz);
					}
				}
			}

		}
		else
			dragging = 0;
	}
}

//__________________________ALL ROTATIONS__________________NON_GLUT_____________________________________________________
void xrotation(int lay, int dir)  //lay can be 1,2,3;dir can be -1 or 1
{
	int i, j, temp, te1, te2, te3;
	if (dir == -1)
	{
		temp = lay;
		te1 = C[0][lay];
		te2 = C[0][lay + 3];
		te3 = C[0][lay + 6];
		for (j = 1; j < 4; j++)
			for (i = 0, lay = temp; i < 3; i++, lay += 3)
				C[j - 1][lay] = C[j][lay];
		lay = temp;
		C[3][lay] = te1;
		C[3][lay + 3] = te2;
		C[3][lay + 6] = te3;
		if (temp == 3)
			franti(4);
		if (temp == 1)
			franti(5);

	}
	else
	{
		temp = lay;
		te1 = C[3][lay];
		te2 = C[3][lay + 3];
		te3 = C[3][lay + 6];
		for (j = 3; j > 0; j--)
			for (i = 0, lay = temp; i < 3; i++, lay += 3)
				C[j][lay] = C[j - 1][lay];
		lay = temp;
		C[0][lay] = te1;
		C[0][lay + 3] = te2;
		C[0][lay + 6] = te3;
		if (temp == 3)
			fr(4);
		if (temp == 1)
			fr(5);
	}
}
void yrotation(int lay, int dir)
{
	if (lay == 1)
	{
		rmy(1, 3, 9, 1, dir);
		rmy(2, 2, 8, 2, dir);
		rmy(3, 1, 7, 3, dir);
		if (dir == 1)
			fr(2);
		if (dir == -1)
			franti(2);
	}
	if (lay == 2)
	{
		rmy(4, 6, 6, 4, dir);
		rmy(5, 5, 5, 5, dir);
		rmy(6, 4, 4, 6, dir);
	}
	if (lay == 3)
	{
		rmy(7, 9, 3, 7, dir);
		rmy(8, 8, 2, 8, dir);
		rmy(9, 7, 1, 9, dir);
		if (dir == 1)
			franti(0);
		else
			fr(0);
	}
}
void rmy(int fr, int le, int ba, int ri, int dir)
{
	int te;
	if (dir == 1)
	{
		te = C[4][ri];
		C[4][ri] = C[3][ba];
		C[3][ba] = C[5][le];
		C[5][le] = C[1][fr];
		C[1][fr] = te;
	}
	else
	{
		te = C[4][ri];
		C[4][ri] = C[1][fr];
		C[1][fr] = C[5][le];
		C[5][le] = C[3][ba];
		C[3][ba] = te;

	}
}
void zrotation(int lay, int dir)
{
	if (lay == 1)
	{
		rmz(7, 1, 3, 7, dir);
		rmz(8, 4, 2, 4, dir);
		rmz(9, 7, 1, 1, dir);
		if (dir == 1)
			fr(1);
		else
			franti(1);
	}
	if (lay == 2)
	{
		rmz(4, 2, 6, 8, dir);
		rmz(5, 5, 5, 5, dir);
		rmz(6, 8, 4, 2, dir);
	}
	if (lay == 3)
	{
		rmz(1, 3, 9, 9, dir);
		rmz(2, 6, 8, 6, dir);
		rmz(3, 9, 7, 3, dir);
		if (dir == 1)
			franti(3);
		else
			fr(3);
	}
}
void rmz(int up, int ri, int dw, int le, int dir)
{
	int te;
	if (dir == 1)
	{
		te = C[4][ri];
		C[4][ri] = C[2][up];
		C[2][up] = C[5][le];
		C[5][le] = C[0][dw];
		C[0][dw] = te;
	}
	else
	{
		te = C[4][ri];
		C[4][ri] = C[0][dw];
		C[0][dw] = C[5][le];
		C[5][le] = C[2][up];
		C[2][up] = te;

	}
}
void fr(int face)
{
	int temp;
	temp = C[face][2];
	C[face][2] = C[face][4];
	C[face][4] = C[face][8];
	C[face][8] = C[face][6];
	C[face][6] = temp;
	temp = C[face][1];
	C[face][1] = C[face][7];
	C[face][7] = C[face][9];
	C[face][9] = C[face][3];
	C[face][3] = temp;
}
void franti(int face)
{
	int temp;
	temp = C[face][2];
	C[face][2] = C[face][6];
	C[face][6] = C[face][8];
	C[face][8] = C[face][4];
	C[face][4] = temp;
	temp = C[face][1];
	C[face][1] = C[face][3];
	C[face][3] = C[face][9];
	C[face][9] = C[face][7];
	C[face][7] = temp;
}

//_________________________________________________________MAIN
int main(int argc, char** argv)
{
	int i, j;
	for (i = 0; i < 6; i++) 
		for (j = 0; j < 10; j++)
			C[i][j] = i; 
	for (i = 0; i < 28; i++) 
	{
		xro[i] = 0;
		yro[i] = 0;
		zro[i] = 0;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(660, 0);
	glutCreateWindow("CUBO MAGICO - TRABALHO CG");


	glEnable(GL_DEPTH_TEST);

	img = 	SOIL_load_image
			(
				"imgs/misturar.png",
				&img_width,
				&img_height,
				&img_channels,
				SOIL_LOAD_AUTO
			);

	// prints out image information
	if (img)
	{
		std::clog << " SOIL texture loading: success!\n";
		std::clog << "  Image width...... : " << img_width << "\n";
		std::clog << "  Image height..... : " << img_height << "\n";
		std::clog << "  Image channels... : " << img_channels << "\n";
	}
	else
	{
		std::cerr << "SOIL loading error: " << SOIL_last_result() << "\n";
	}

	// generates a texture id
	glGenTextures(3, textures);

	// binds the newly generated texture
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	// loads image's byte array into a texture
	//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, img_width, img_height, GL_RGBA, GL_UNSIGNED_BYTE, img);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	img = 	SOIL_load_image
			(
				"imgs/resetar.png",
				&img_width,
				&img_height,
				&img_channels,
				SOIL_LOAD_AUTO
			);


	// prints out image information
	if (img)
	{
		std::clog << " SOIL texture loading: success!\n";
		std::clog << "  Image width...... : " << img_width << "\n";
		std::clog << "  Image height..... : " << img_height << "\n";
		std::clog << "  Image channels... : " << img_channels << "\n";
	}
	else
	{
		std::cerr << "SOIL loading error: " << SOIL_last_result() << "\n";
	}

	// binds the newly generated texture
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	// loads image's byte array into a texture
	//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, img_width, img_height, GL_RGBA, GL_UNSIGNED_BYTE, img);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	img = 	SOIL_load_image
			(
				"imgs/control.png",
				&img_width,
				&img_height,
				&img_channels,
				SOIL_LOAD_AUTO
			);


	// prints out image information
	if (img)
	{
		std::clog << " SOIL texture loading: success!\n";
		std::clog << "  Image width...... : " << img_width << "\n";
		std::clog << "  Image height..... : " << img_height << "\n";
		std::clog << "  Image channels... : " << img_channels << "\n";
	}
	else
	{
		std::cerr << "SOIL loading error: " << SOIL_last_result() << "\n";
	}

	// binds the newly generated texture
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	// loads image's byte array into a texture
	//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, img_width, img_height, GL_RGBA, GL_UNSIGNED_BYTE, img);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	std::clog << "  CARREGADO TODOS OS MODELOS" << "\n";

	glutDisplayFunc(draw);

	glutMouseFunc(mouse_click);
	glutTimerFunc(100, anim, 1);
	glutSpecialFunc(specialKeys);
	glutReshapeFunc(ChangeSize);
	Ilumin();

	glutMainLoop();
	return 0;
}