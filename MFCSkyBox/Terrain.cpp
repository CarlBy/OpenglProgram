#include "stdafx.h"
#include <windows.h>
#include "Terrain.h"



CTerrain::CTerrain()
{
	isLoadTexture = false;
	isReadRawfile = false;
	isCaculateNomal = false;
}


CTerrain::~CTerrain()
{
}

bool CTerrain::ReadRawFile(char* filename)
{
	FILE *fp;
	if (fopen_s(&fp, "Data/Terrain.raw", "rb") != 0)
	{
		MessageBox(NULL, "�߶�ͼ�ļ���ʧ��!\nCan't open the height map.", "Error", MB_OK);
		return false;//�ļ���ʧ��
	}
	fread(pheight, 1, 1024*1024, fp);//�Ҷ�ͼ��1024*1024��С
	if (ferror(fp))
		return false;//��ȡ����
	fclose(fp);
	//free(fp);
	isReadRawfile = true;
}

BYTE CTerrain::GetHeight(int x, int z)//x��ʾ��ͼ�����겻�ǻҶ�ͼ������
{
	if (!isReadRawfile)//��ͼ��ʼ���ڣ�-512��-512������ԭ��
		ReadRawFile("data/Terrain.raw");//�Ժ���Ҫ��ȡ�߶ȾͲ��õ�������readrawfile��
	int X = (x-(-512)) / stepsize;
	int Z =(z-(-512)) / stepsize;
	return pheight[X][Z];//1024Ϊ�߶�ͼ�Ŀ�
}

AUX_RGBImageRec* CTerrain::LoadBmp(char *filename)
{
	FILE *fp;
	if (!filename)//δ�����ļ���
		return 0;
	fopen_s(&fp, filename, "r");
	if (fp)//�ļ��򿪳ɹ�
	{
		fclose(fp);
		return auxDIBImageLoad(filename);
		free(fp);
	}
	return 0;
}

bool CTerrain::LoadTexture()
{
	AUX_RGBImageRec *pimage;
	char *bmpname = { "data/Terrain.bmp" };
	pimage=LoadBmp(bmpname);
	if (pimage->data&&pimage)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexImage2D(GL_TEXTURE_2D, 0, 3, pimage->sizeX, pimage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pimage->data);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pimage->sizeX, pimage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pimage->data);
		isLoadTexture = true;
		free(pimage->data);											// Free The Texture Image Memory
		free(pimage);
	}
	return isLoadTexture;
}

int CTerrain::CreatTerrain()
{
	if (!isLoadTexture)
		LoadTexture();//�ж��Ƿ��Ѿ���������һ��Ҫ�жϣ���Ȼÿ����ת��ִ��loadtexture��ܿ�
	stepsize =1;//1024�ǻҶ�ͼ�ĳ��Ϳ����Ծ����ñ������棬�Ա�ʹ�������ͨ���ԣ�ֻ�����ڻ���֪����λ�ȡ�Ҷ�ͼ����
	//stepsize��36����߶��õ�
	
	if (isCaculateNomal == false)
		GetNormal();

	int List = glGenLists(1);//����һ����ʾ�б�Ϊ�����ִ��Ч��
	glNewList(List, GL_COMPILE);//��ʼװ�룬װ�������ִ��
	glBindTexture(GL_TEXTURE_2D, texture);//���б���װ����ʾ�б����ûװ��ȥ����RenderScence��ֻ������glcalllist���ػ�ʱ
										  //��û��ִ�д��У���δ�������󶨵Ŀ��ܾ���������������յģ�������
	for (int i = -512; i < 511; i++)//1024�ǻҶ�ͼ�Ŀ�
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (int j = -512; j < 511; j ++)
		{
			float a=128;//ͼƬ�ظ�����Ϊ1024/a��������float��
			//I��J����Ϊ���Σ���Ϊ�����в����Ǹ�����
			int I = i + 512, J = j + 512;//��ͼ��ʼ���ڣ�-512��-512������ԭ��,��������Ҫ��0��ʼ����Ҫ��512
			xtexa1 = I / a;
			xtexa2 = (I + 1) / a;
			ytexa1 = J / a;
			ytexa2 = (J + 1) / a;
			//ע�⵽�����ô�д�Ļ���Сд�ı�������д�Ĵ�0��ʼ�����ڷ�����������
			glNormal3f(normal[I + 1][J].x, normal[I + 1][J].y, normal[I + 1][J].z);
			glTexCoord2f(xtexa2, ytexa1);   glVertex3f(i + 1, GetHeight(i + 1, j), j);

			glNormal3f(normal[I][J].x, normal[I][J].y, normal[I][J].z);
			glTexCoord2f(xtexa1, ytexa1);   glVertex3f(i, GetHeight(i, j), j);
		}
		glEnd();
	}
	glEndList();
	return List;
}


void CTerrain::GetNormal(/*float x, float z*/)//x,zΪ��ͼ��������
{
	vec3  va, vb, vc, vd, ve, vf,//����o��abc...f�����������va��vb...vf
		norm1, norm2, norm3, norm4, norm5;//�����ĵ���ص�5�������η������ֱ�Ϊnorm1��6

	int x, z;
	for (x = -511; x < 511; x += stepsize)
	{
		for (z = -511; z < 511; z += stepsize)
		{
			va.x = 0; va.y = GetHeight(x, z + stepsize) - GetHeight(x, z); va.z = stepsize;
			vb.x = -stepsize; vb.y = GetHeight(x - stepsize, z) - GetHeight(x, z); vb.z = 0;
			vc.x = -stepsize; vc.y = GetHeight(x - stepsize, z - stepsize) - GetHeight(x, z); vc.z = -stepsize;
			vd.x = 0; vd.y = GetHeight(x, z - stepsize) - GetHeight(x, z); vd.z = -stepsize;
			ve.x = stepsize; ve.y = GetHeight(x + stepsize, z) - GetHeight(x, z); ve.z = 0;
			vf.x = stepsize; vf.y = GetHeight(x + stepsize, z + stepsize) - GetHeight(x, z); vf.z = stepsize;
			//����Ե�⣬ÿ�������5����������أ����������5�������εķ�����
			norm1 = glm::cross(va, vb);    //ֱ�����ò�˺������ɼ���
			norm2 = glm::cross(vb, vc);
			norm3 = glm::cross(vc, vd);
			norm4 = glm::cross(vd, ve);
			norm5 = glm::cross(ve, vf);
			int X = x + 512, Z = z + 512;//��ͼ��ʼ���ڣ�-512��-512������ԭ��,����Ҫ��0��ʼ����Ҫ��512
			//X��Z����СֵΪ1�����Ϊ1023����Ϊ�����Χ��Ӧ�Ķ�����Χ�������������Σ���
			//������û��ר��Ϊ��Ե�������÷���������Դ�����������������û��5����������
			//�����ط�Ҳû��Ϊ��Ե�������÷���������Ե�����Ӧ�ķ�������normal(0,0)û��
			//��ʼ����֪��Ϊɶ����Ҳû����������ϵͳ��Ĭ�ϵĳ�ʼ��
			normal[X][Z].x = -(norm1.x + norm2.x + norm3.x + norm4.x + norm5.x);//��֪��ΪʲôҪ�Ӹ��ţ����ǲ����Ƿ�����й���
			normal[X][Z].y = -(norm1.y + norm2.y + norm3.y + norm4.y + norm5.y);
			normal[X][Z].z = -(norm1.z + norm2.z + norm3.z + norm4.z + norm5.z);

			glm::normalize(normal[X][Z]);
		}
	}
	isCaculateNomal = true;
}

int CTerrain::CreatGrid()
{
	int List = glGenLists(1);//����һ����ʾ�б�Ϊ�����ִ��Ч��
	glNewList(List, GL_COMPILE);//��ʼװ�룬װ�������ִ��
	for (int i = -512; i < 511; i++)//1024�ǻҶ�ͼ�Ŀ�
	{
		
		for (int j = -512; j < 511; j++)
		{
			glBegin(GL_LINE_LOOP);
			glVertex3f(i, GetHeight(i, j), j);
			glVertex3f(i + 1, GetHeight(i + 1, j), j);
			glVertex3f(i+1, GetHeight(i+1, j+1), j+1);
			glVertex3f(i, GetHeight(i, j+1), j+1);
			glVertex3f(i + 1, GetHeight(i + 1, j), j);
			glEnd();
		}
		
	}
	glEndList();
	return List;
}