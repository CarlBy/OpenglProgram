#pragma once
#include "gl/glew.h"
#include "gl/glut.h"
#include "gl/glaux.h"
#include "glm/glm.hpp"
#include <glm/vec3.hpp> // glm::vec3
using namespace glm;
class CTerrain
{
	int stepsize;//ÿ�������ڶ���֮��ĺ㣨�ݣ�����֮��,����Ҷ�ͼ��������
	
	bool isReadRawfile;
	//��Ϊ1024��ԭ�򣺵�ͼ��Ϊ1023��˵��ÿһ����1024����
	//��ס���Ҷ�ͼ���߶�ͼ����Ϊa����ô��ͼ��Ϊa-1����ͬ��
	BYTE pheight[1024][1024];

	GLfloat xtexa1, xtexa2, ytexa1, ytexa2;

	vec3 normal[1024][1024];//�����ж�����Ϊ1024*1024������ñ������棬ʹ�����ͨ��ͨ����

public:
	CTerrain();
	~CTerrain();
	bool ReadRawFile(char* filename);
	BYTE GetHeight(int x, int z);
	AUX_RGBImageRec* LoadBmp(char *filename);
	bool LoadTexture();
	int CreatTerrain();
	void GetNormal(/*float x, float z*/);
	int CreatGrid();

public:
	int mapsize;
	GLuint texture;//��������
	bool isLoadTexture;
	bool isCaculateNomal;
};

