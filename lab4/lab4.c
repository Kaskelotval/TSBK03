// Demo of heavily simplified sprite engine
// by Ingemar Ragnemalm 2009
// used as base for lab 4 in TSBK03.
// OpenGL 3 conversion 2013.

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// uses framework Cocoa
#else
	#include <GL/gl.h>
	#include "MicroGlut.h"
#endif

#include <stdlib.h>
#include "LoadTGA.h"
#include "SpriteLight.h"
#include "GL_utilities.h"
#include <math.h>

// L�gg till egna globaler h�r efter behov.
GLfloat kMaxDistance = 90.0f;
GLfloat kDogAvoidance = 0.1; 
GLfloat kCohWeight = 0.002;
GLfloat kAvoidWeight = 0.7;
GLfloat kAlignWeight = 0.04;
GLfloat kRandom = 2.0f;
GLfloat kFoodWeight = 0.9f;

TextureData *sheepFace, *blackFace, *dogFace, *foodFace;


SpriteRec *dog;
SpriteRec *blackSheep;
SpriteRec *food;


FPoint Avoidance(SpritePtr i, SpritePtr j){
	//distance between the positions
	FPoint a = (FPoint){
		i->position.h - j->position.h,
		i->position.v - j->position.v
		};
	float lenght = sqrt(pow(a.h, 2.0f) + pow(a.v, 2.0f));

	a.h *= (1.0f-lenght/100)/lenght;
	a.v *= (1.0f-lenght/100)/lenght;

	return a;
}

void SpriteBehavior() // Din kod!
{
// L�gg till din labbkod h�r. Det g�r bra att �ndra var som helst i
// koden i �vrigt, men mycket kan samlas h�r. Du kan utg� fr�n den
// globala listroten, gSpriteRoot, f�r att kontrollera alla sprites
// hastigheter och positioner, eller arbeta fr�n egna globaler.

	GLint counter;
	SpritePtr otherOne = gSpriteRoot;
	SpritePtr thisOne = gSpriteRoot;
	// if(thisOne->face ==) //do different speeds
	
	while(thisOne != NULL){
		
		counter = 0;
		thisOne->speedDiff = (FPoint){0,0}; //initial speed differance
		thisOne->averagePos = (FPoint){0,0}; //initial distance
		thisOne->avoidVec = (FPoint){0,0};//initial avoidance vec
		otherOne = gSpriteRoot;	


		while(otherOne != NULL){
			float distance = sqrt(
				pow(thisOne->position.h-otherOne->position.h,2)
				+
				pow(thisOne->position.v-otherOne->position.v,2)
			);
			if(thisOne != otherOne && distance < kMaxDistance && thisOne->face != dogFace && otherOne->face != dogFace && thisOne->face != foodFace && otherOne->face != foodFace){
				//speed differance
				thisOne->speedDiff.h += otherOne->speed.h -thisOne->speed.h;
				thisOne->speedDiff.v += otherOne->speed.v -thisOne->speed.v;

				// - - - cohesion - - - //
				//find all creatures nearby
				//uses averagePos added in SpriteLight.h
				thisOne->averagePos.h += otherOne->position.h;
				thisOne->averagePos.v += otherOne->position.v;
				
				// - - - Separation/avoidance - - - //
				FPoint avoidance = Avoidance(thisOne,otherOne);
				thisOne->avoidVec.h += avoidance.h;
				otherOne->avoidVec.v += avoidance.v;

				counter++;
			}
			if(otherOne->face == dogFace && distance < kMaxDistance)
			{				
				thisOne->speed.h += 0.1/(distance + 0.01)*kDogAvoidance*(thisOne->position.h - otherOne->position.h);
				thisOne->speed.v += 0.1/(distance + 0.01)*kDogAvoidance*(thisOne->position.v - otherOne->position.v);
			}
			else if(thisOne->face != dogFace && otherOne->face == foodFace && thisOne->face != foodFace)
			{
				thisOne->speed.h += 1/(distance + 0.1)*kFoodWeight*(otherOne->position.h - thisOne->position.h);
				thisOne->speed.v += 1/(distance + 0.1)*kFoodWeight*(otherOne->position.v - thisOne->position.v);
			}

			otherOne = otherOne->next;

		}
		if(counter > 0 && thisOne->face != foodFace){

			thisOne->speedDiff.h /= counter;
			thisOne->speedDiff.v /= counter;

			thisOne->averagePos.h/=counter;
			thisOne->averagePos.v/=counter;
			
			thisOne->avoidVec.h/=counter;
			thisOne->avoidVec.v/=counter;
			
		}
		thisOne = thisOne->next;
	}

	thisOne = gSpriteRoot;

	//do the flocking for the sheeps (not dogs or food)
	while(thisOne != NULL){		
		if(thisOne->face == dogFace)
			thisOne = thisOne->next;
		if(thisOne->face != foodFace)
		{	
			// - - - cohesion - - - //	
			thisOne->speed.h += (thisOne->averagePos.h-thisOne->position.h)*kCohWeight;
			thisOne->speed.v += (thisOne->averagePos.v-thisOne->position.v)*kCohWeight;
			
			// - - - separation - - - //
			thisOne->speed.h += thisOne->avoidVec.h*kAvoidWeight;
			thisOne->speed.v += thisOne->avoidVec.v*kAvoidWeight;

			// - - - alignment - - - //
			thisOne->speed.h += thisOne->speedDiff.h*kAlignWeight;
			thisOne->speed.v += thisOne->speedDiff.v*kAlignWeight;
			
			thisOne->position.h += thisOne->speed.h;
			thisOne->position.v += thisOne->speed.v;
			
			if(thisOne == blackSheep){			
				thisOne->speed.h += kRandom * (rand() / (float)RAND_MAX) - kRandom/2.0f;
				thisOne->speed.v += kRandom * (rand() / (float)RAND_MAX) - kRandom/2.0f;
			}
			
		}	
		
		thisOne = thisOne->next;
	}

}

// Drawing routine
void Display()
{
	SpritePtr sp;
	
	glClearColor(0, 0, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	DrawBackground();
	SpriteBehavior(); // Din kod!		
	
	
// Loop though all sprites. (Several loops in real engine.)
	sp = gSpriteRoot;
	do
	{
		HandleSprite(sp); // Callback in a real engine
		DrawSprite(sp);
		sp = sp->next;
	} while (sp != NULL);
	
	glutSwapBuffers();
}

void Reshape(int h, int v)
{
	glViewport(0, 0, h, v);
	gWidth = h;
	gHeight = v;
}

void Timer(int value)
{
	glutTimerFunc(20, Timer, 0);
	glutPostRedisplay();
}

// Example of user controllable parameter
float someValue = 0.0;

void Key(unsigned char key,
         __attribute__((unused)) int x,
         __attribute__((unused)) int y)
{
  switch (key)
  {
    case '+':
    	kRandom += 0.1f;
    	printf("someValue = %f\n", kRandom);
    	break;
    case '-':
    	kRandom -= 0.1f;
    	printf("someValue = %f\n", kRandom);
		break;
	case '.':
		spawnFood();
		break;
	case ',':
		spawnSheep();
		break;
	case 'm':
		spawnDog();
		break;

    case 0x1b:
      exit(0);
  }
}

void Init()
{
	
	LoadTGATextureSimple("bilder/leaves.tga", &backgroundTexID); // Bakgrund
	
	sheepFace = GetFace("bilder/sheep.tga"); // Ett f�r
	blackFace = GetFace("bilder/blackie.tga"); // Ett svart f�r
	dogFace = GetFace("bilder/dog.tga"); // En hund
	foodFace = GetFace("bilder/mat.tga"); // Mat
	
	for(int n = 0; n < 10; n++) {
		NewSprite(sheepFace,
			rand() % 700 + 50,
			rand() % 500 + 50,
			((rand() % 200) - 100) / 100.0,
			((rand() % 200) - 100) / 100.0
		);
	}
	blackSheep = NewSprite(blackFace, 100, 200, 1, 1);
	//dog = NewSprite(dogFace, 100, 300, 1, 1);

}

void spawnFood()
{	
	NewSprite(foodFace,
		rand() % 700 + 50,
		rand() % 500 + 50,
		0,
		0
	);
}
void spawnSheep()
{	
	NewSprite(sheepFace,
		rand() % 700 + 50,
		rand() % 500 + 50,
		((rand() % 200) - 100) / 100.0,
		((rand() % 200) - 100) / 100.0
	);
}
void spawnDog()
{	
	NewSprite(dogFace, 100, 300, 1, 1);
}
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 2);
	glutCreateWindow("SpriteLight demo / Flocking");
	
	glutDisplayFunc(Display);
	glutTimerFunc(20, Timer, 0); // Should match the screen synch
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);
	
	InitSpriteLight();
	Init();
	
	glutMainLoop();
	return 0;
}
