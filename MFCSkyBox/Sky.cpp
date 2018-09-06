#include "stdafx.h"
#include "Sky.h"


CSky::CSky()
{
	Status = false;
	isLoadSun = false;
}


CSky::~CSky()
{
}

bool CSky::LoadGLTexture(char *filename)						// Load Bitmaps And Convert To Textures
{

	AUX_RGBImageRec *pImage;										// Create Storage Space For The Texture

	pImage = loadbmp(filename);									// Loads The Bitmap Specified By filename

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (pImage != NULL && pImage->data != NULL)					// If Texture Image Exists
	{
		glGenTextures(1, &textureName);									// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, textureName);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pImage->sizeX, pImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pImage->data);
		//glTexImage2D(GL_TEXTURE_2D, 0, 3, pImage->sizeX, pImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage->data);//�������������
		free(pImage->data);											// Free The Texture Image Memory
		free(pImage);												// Free The Image Structure
	}
	Status = true;
	isLoadSun = true;
	return Status;
}

AUX_RGBImageRec* CSky::loadbmp(char *Filename)
{
	FILE *File = NULL;												// File Handle

	if (!Filename)													// Make Sure A Filename Was Given
	{
		return NULL;												// If Not Return NULL
	}

	fopen_s(&File, Filename, "r");										// Check To See If The File Exists

	if (File)														// Does The File Exist?
	{
		fclose(File);												// Close The Handle
		return auxDIBImageLoad(Filename);							// Load The Bitmap And Return A Pointer
	}

	return NULL;
}


int CSky::CreatSky(float r, int jing, int wei)//������ʼ����View.cpp��InitializeOpenGL������
{
	if (!Status)
		LoadGLTexture("Data/sky.bmp");
	int List = glGenLists(1);//����һ����ʾ�б�Ϊ�����ִ��Ч��
	glNewList(List, GL_COMPILE);//��ʼװ�룬װ�������ִ��
	//qobj = gluNewQuadric();
	//gluQuadricDrawStyle(qobj, GLU_FILL);//���ö���ȴ����Ʒ��
	//gluQuadricNormals(qobj, GLU_SMOOTH);//���÷���,��ÿ�����㶼���㷨����������Ĭ�Ϸ�ʽ
	//gluQuadricTexture(qobj, GL_TRUE);//�����Ƿ��Զ���������Ĭ����GLU_FALSE
	glBindTexture(GL_TEXTURE_2D, textureName);

	//glTranslatef(0, -50, 0);
	//glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	//gluSphere(qobj, 900, 40, 40);

	float angle = 360 / jing;
	float height = r / wei;
	for (float h = 0; h < r; h += height)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (float jiao = 0; jiao < 360; jiao += angle)
		{
			float Rxy1 = sqrt(pow(r, 2) - pow(h + height, 2));//����ͼ�����Ȼ�СԲ
			float Rxy = sqrt(pow(r, 2) - pow(h, 2));
			float x1 = Rxy1*cos(jiao); float z1 = Rxy1*sin(jiao); float y1 = h + height;
			float x = Rxy*cos(jiao); float z = Rxy*sin(jiao); float y = h;
			glTexCoord2f(0.5+x1 / (2 * r), 0.5+z1 / (2 * r)); glVertex3f(x1, y1, z1);//0.5+x1 / (2 * r)�ҵ����Ӧ����������
			glTexCoord2f(0.5+x / (2 * r), 0.5+z / (2 * r)); glVertex3f(x, y, z);
		}
		glEnd();
	}

	glEndList();
	return List;
}

int CSky::CreatSun()
{
	//if (!isLoadSun)
		LoadGLTexture("data/NEBULA.bmp");
	//glDisable(GL_TEXTURE_2D);
		glColor4f(1, 0.5f, 0, 0.1);
	glBindTexture(GL_TEXTURE_2D, textureName);
	
	glBegin(GL_TRIANGLE_STRIP);	//��̫��					// Build Quad From A Triangle Strip
	glTexCoord2f(1, 1); glVertex4f(2, 2,0,0.5); // Top Right
	glTexCoord2f(0, 1); glVertex4f(-2, 2, 0,0.5); // Top Left
	glTexCoord2f(1, 0); glVertex4f(2, -2, 0,0.5); // Bottom Right
	glTexCoord2f(0, 0); glVertex4f(-2, -2, 0,0.5); // Bottom Left
	glEnd();
	return 0;
}