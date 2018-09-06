
// MFCSkyBoxView.cpp : CMFCSkyBoxView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "MFCSkyBox.h"
#endif

#include "MFCSkyBoxDoc.h"
#include "MFCSkyBoxView.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCSkyBoxView

IMPLEMENT_DYNCREATE(CMFCSkyBoxView, CView)

BEGIN_MESSAGE_MAP(CMFCSkyBoxView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

// CMFCSkyBoxView ����/����

CMFCSkyBoxView::CMFCSkyBoxView()
{
	// TODO:  �ڴ˴���ӹ������
	xtexa1 = xtexa2 = 0;
	m_xAngle = m_yAngle = 0;
	fowardSpeed = l_r_Speed = 0;
	moveForward = false;
	moveLeft = false;
	moveRight = false;
	altDown == false;
	moveSky = 0;
	a = 0;
}

CMFCSkyBoxView::~CMFCSkyBoxView()
{
}

BOOL CMFCSkyBoxView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMFCSkyBoxView ����

void CMFCSkyBoxView::OnDraw(CDC* /*pDC*/)
{
	CMFCSkyBoxDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
	RenderScene();
}


// CMFCSkyBoxView ��ӡ

BOOL CMFCSkyBoxView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMFCSkyBoxView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMFCSkyBoxView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}


// CMFCSkyBoxView ���

#ifdef _DEBUG
void CMFCSkyBoxView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCSkyBoxView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCSkyBoxDoc* CMFCSkyBoxView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCSkyBoxDoc)));
	return (CMFCSkyBoxDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCSkyBoxView ��Ϣ�������


// ��ʼ��OpenGL
BOOL CMFCSkyBoxView::InitializeOpenGL()
{
	PIXELFORMATDESCRIPTOR pfd;
	int n;
	m_pDC = new CClientDC(this);
	ASSERT(m_pDC != NULL);
	// ���õ�ǰ�Ļ�ͼ���ظ�ʽ  
	if (!SetupPixelFormat())
	{
		return FALSE;
	}

	n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);
	// ������ͼ������  
	m_hRC = wglCreateContext(m_pDC->GetSafeHdc());
	if (m_hRC == NULL)
	{
		return FALSE;
	}
	// ʹ��ͼ������Ϊ��ǰ�����̵ֳĵ�ǰ��ͼ������  
	if (wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC) == FALSE)
	{
		return FALSE;
	}
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	
	listSky = sky.CreatSky(720,30,32);//����������ܱ�360������γ������ܱ��뾶����
	listTerrain = terrain.CreatTerrain();//���ʼ�������ڵ�ʱ����Ƚ��б����ã�������ֱ����
	listLight = LightModle();//������ģ���б����ã���Ҫ��rendersence�е��ô��У���Ȼû������κ�Ч��
	
	listGrid = terrain.CreatGrid();
	listPlane = Plane.CreatPlane();
	listHouse = house.CreatHouse();
	//listWater = water.CreatWater(a);
	return TRUE;
}


// �������ظ�ʽ
BOOL CMFCSkyBoxView::SetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // pfd�ṹ�Ĵ�С   
		1,                              // �汾��   
		PFD_DRAW_TO_WINDOW |            // ֧���ڴ����л�ͼ   
		PFD_SUPPORT_OPENGL |            // ֧�� OpenGL   
		PFD_DOUBLEBUFFER,               // ˫����ģʽ   
		PFD_TYPE_RGBA,                  // RGBA ��ɫģʽ   
		24,                             // 24 λ��ɫ���   
		0, 0, 0, 0, 0, 0,               // ������ɫλ   
		0,                              // û�з�͸���Ȼ���   
		0,                              // ������λλ   
		0,                              // ���ۼƻ���   
		0, 0, 0, 0,                     // �����ۼ�λ   
		32,                             // 32 λ��Ȼ���       
		0,                              // ��ģ�建��   
		0,                              // �޸�������   
		PFD_MAIN_PLANE,                 // ����   
		0,                              // ����   
		0, 0, 0                         // ���Բ�,�ɼ��Ժ������ģ   

	};
	int pixelFormat;
	// Ϊ�豸������õ���ƥ������ظ�ʽ   
	if ((pixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0)
	{
		MessageBox(_T("ChoosePixelFormat failed"));
		return FALSE;
	}
	// ������ƥ������ظ�ʽΪ��ǰ�����ظ�ʽ   
	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelFormat, &pfd) == FALSE)
	{
		MessageBox(_T("SetPixelFormat failed"));
		return FALSE;
	}
	return TRUE;
}


int CMFCSkyBoxView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	SetTimer(1, 50, NULL);
	//SetTimer(2, 50, NULL);

	if (InitializeOpenGL())
	{
		return 0;
	}
	return 0;
}

BOOL CMFCSkyBoxView::OnEraseBkgnd(CDC* pDC)//����windows�Լ��Ĵ���ˢ����˸
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}


void CMFCSkyBoxView::OnDestroy()
{
	CView::OnDestroy();

	// TODO:  �ڴ˴������Ϣ����������
	m_hRC = ::wglGetCurrentContext();
	if (::wglMakeCurrent(0, 0) == FALSE)
	{
		MessageBox(_T("Could not make RC non-current"));
	}

	if (m_hRC)
	{
		if (::wglDeleteContext(m_hRC) == FALSE)
		{
			MessageBox(_T("Could not delete RC"));
		}
	}

	if (m_pDC)
	{
		delete m_pDC;
	}
	glDeleteLists(1, 2);//ɾ����1��ʼ����������2������ʾ�б���ɾ��1��2������ʾ�б�
	m_pDC = NULL;
	KillTimer(1);
}

void CMFCSkyBoxView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	m_wide = cx; m_height = cy;
	// TODO:  �ڴ˴������Ϣ����������
	//�������Ϊ0  
	if (cy == 0)
	{
		cy = 1;
	}
	//�����ӿ��봰�ڵĴ�С  
	glViewport(0, 0, cx, cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)cx / (GLfloat)cy, 0.9f, 500000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int CMFCSkyBoxView::LightModle()
{
	//��Դ
	GLuint List = glGenLists(1);
	glNewList(List, GL_COMPILE);
	GLfloat lgtposition[] = { 500, 200, 0, 0 };
	GLfloat ambi[] = { 0.1, 0.1, 0.1, 1 };
	GLfloat diff[] = { 1, 1, 1, 1 };
	GLfloat spec[] = { 1, 1, 1, 1 };
	GLfloat _SPOT_DIRECTION[] = { -50.0f, -20.0f, 0.0f };//��ʾ��ķ���ԭ�㵽�ĵ�Ϊ����
	GLfloat _GL_SPOT_EXPONENT[] = { 30.0f };//�ۺ϶ȣ�Խ��Խ�ۣ���λ�ù�Դ����Ч
	GLfloat _GL_SPOT_CUTOFF[] = { 30.0f };//�ֵ緢��Ƕȵ�һ�룬��λ�ù�Դ����Ч

	glLightfv(GL_LIGHT0, GL_POSITION, lgtposition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, _SPOT_DIRECTION);
	glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, _GL_SPOT_EXPONENT);
	glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, _GL_SPOT_CUTOFF);

	GLfloat ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuse[] = { 1, 1, 1, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat emission[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //����΢���ù⣬�˴�û��������
	GLfloat shininess = 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_COLOR_MATERIAL);
	glEndList();
	return List;
}

void CMFCSkyBoxView::FogModle()
{
	// ������Ч��
	float fog_r = 211.f / 255.f;
	float fog_g = 237.f / 255.f;
	float fog_b = 254.f / 255.f;
	GLuint	fogMode[] = { GL_EXP, GL_EXP2, GL_LINEAR };	// �����������ģʽ
	GLuint	fogfilter = 0;								// ��ǰʹ�õ����ģʽ 
	GLfloat	fogColor[4] = { fog_r, fog_g, fog_b, 1 };		// �����ɫ

	glFogi(GL_FOG_MODE, fogMode[2]);			        // �������ģʽ
	glFogfv(GL_FOG_COLOR, fogColor);					// ���������ɫ
	glFogf(GL_FOG_DENSITY, 0.295f);						// �������Ũ��
	glHint(GL_FOG_HINT, GL_NICEST);					    // Fog Hint Value
	glFogf(GL_FOG_START, 200.0f);						// ������Ŀ�ʼ���
	glFogf(GL_FOG_END, 700);							// ������Ľ������
	glEnable(GL_FOG);									// ʹ����
}
//�������̷ɻ�����Ұgllookat����һֱû�䣬�����ƶ��ǿ��Ƶĵ����ƶ�
//��������ɻ��ڶ�����������Ŀ����Ϊ�������ת��������ɻ�ת��
void CMFCSkyBoxView::RenderScene()
{
	//����������ɫ 
	glClearColor(211.f / 255.f, 237.f / 255.f, 254.f / 255.f, 1);		
	//�����ɫ����������Ȼ����� 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//͸��ͶӰ�任  
	glEnable(GL_TEXTURE_2D);//�����������ܵ�
//ʹ�ù���,���������Դ������Ұ�ƶ����ƶ�
	//glCallList(listLight);;
	//�ӽǱ任  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

/////////////////////////////////////////////////////////////////////////////////////////////////

	//gluLookAt(0, 500, 300, 0, 5, -30, 0, 1, 0);//5��Ϊ����Ұ�ȷɻ��Ըߣ��ɻ���ԭ�㣩30��Ϊ����Ұ�ڷɻ������


	//gluLookAt(0, 5, 30, 0, 5, -30, 0, 1, 0);//5��Ϊ����Ұ�ȷɻ��Ըߣ��ɻ���ԭ�㣩30��Ϊ����Ұ�ڷɻ������
	//gluLookAt(0, 70, 50, 0, 70, -50, 0, 1, 0);
	gluLookAt(0, 500, 1800, 0, 0, 0, 0, 1, 0);//��ȫ�������д��룬ע������һ�в��ر�����ע������FogModle()
//ʹ�ù��գ��������Դ����������Ұ�ƶ����ƶ���������תʱ�����Ӱ�ӻ�仯
	glCallList(listLight);//listLight��initialopengl�����б���ʼ��
	//sky.CreatSun();
//ʹ����
	//FogModle();

	//altû��ʱ��������ƶ�
	//if (altDown == false)
	//{
	//	glRotatef(m_xAngle, 0, 1, 0);//����ת
	//	glRotatef(m_yAngle, cos(m_xAngle*c), 0, sin(m_xAngle*c));//����ת
	//}
	
//���Ʒɻ�
	glPushMatrix();//��ջ�ĺô��Ǻô��Ƕ�ģ�ͽ���ƽ�ƺ���ת����Ӱ������ģ��
	glRotated(-90, 1, 0, 0);//��ת��z������
	glRotated(180, 0, 0, 1);//�����ɻ�
	glCallList(listPlane);
	glPopMatrix();

	glTranslated(l_r_Speed, 0, -fowardSpeed);//�������꣬Ϊ���Ƶ�����׼��
	glRotatef(m_xAngle, 0, 1, 0);//����ת
	glRotatef(m_yAngle, cos(m_xAngle*c), 0, sin(m_xAngle*c));//����ת
//���Ƶ��Σ�������Ե��ξ�ֹ�����Կɷ���ͬһ��ջ��
	glPushMatrix();//��ջ�ĺô��Ǻô��Ƕ�ģ�ͽ���ƽ�ƺ���ת����Ӱ������ģ��

	//glTranslated(l_r_Speed, -70, -fowardSpeed);//�������꣬Ϊ���Ƶ�����׼��
	glTranslated(0, -40, -0);//�������꣬Ϊ���Ƶ�����׼��

	//�ٽ��������ƣ�˳���ܽ���
	//glTranslated(l_r_Speed, -70, -fowardSpeed);//�õ����½�70��Ϊ������Էɻ�Ϊ�����ƶ���Ұ������glrotated����(0,0,0)��ĳ�������ת���ģ�
							//���Էɻ����뻭��ԭ���λ�ò���ʹ������Էɻ�Ϊ���Ķ������Ե���Ϊ�����ƶ���Ұ�ģ�
							//���Ա�����ִ��glrotated����ԭ�㼴�ɻ�����λ����ת���󽫵������ƣ����Բ���ѡ��gllookat��yֵΪ70
	glCallList(listTerrain);
//���Ʒ���
	glRotated(-90, 1, 0, 0);//��תʹ�����������˷���z������
	glTranslated(20, 0, 6);//��z������6����ź͵���Ӵ���z�᲻�ǵ�ͼ��z�ᣬ�Ƿ��ӵ�z��
	glCallList(listHouse);
	//����ڶ�������
	glTranslated(10, 150, 0);//��һ�������Ѿ������������ᣬ���Ի��ڶ��������ƶ�
	glRotated(90, 0, 0, 1);
	glCallList(listHouse);
	glPopMatrix();
	
	//glCallList(listGrid);//��ʾ����ģ�ͣ���ע��glCallList(listTerrain);
//�������
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glRotatef(moveSky, 0, 1, 0);//������ʱ����ת�����������ڶ�
	glTranslated(0, -150, 0);
	//glCallList(listSky);
	glPopMatrix();

	//glCallList(listWater);
	glPushMatrix();
	glTranslated(-255, -33, 0);
	water.CreatWater(a);
	glPopMatrix();
/////////////////////////////////////////////////////////////////////////////////
	//glCallList(listTerrain);

	//glTranslated(l_r_Speed, 40, fowardSpeed);//����ƽ��40�����Ʒɻ�
	//glRotatef(m_xAngle, 0, 1, 0);//����ת
	//glRotatef(m_yAngle, cos(m_xAngle*c), 0, sin(m_xAngle*c));//����ת

	//gluLookAt(0, 5, 30, 0, 5, -30, 0, 1, 0);//5��Ϊ����Ұ�ȷɻ��Ըߣ��ɻ���ԭ�㣩30��Ϊ����Ұ�ڷɻ������


	//float t1, t2;
	//for (t1 = 0; t1 <=128; t1 += 8)
	//{
	//		glBegin(GL_TRIANGLE_STRIP);
	//		for (t2 = 0; t2 <=128; t2 += 8)
	//		{
	//			xtexa1 = t1 / 128;
	//			xtexa2 = (t1 + 8) / 128;
	//			ytexa1 = t2 / 128;
	//			ytexa2 = (t2 + 8) / 128;
	//			glTexCoord2f(xtexa2, ytexa1);   glVertex3f(t1 + 8, 0, t2);
	//			glTexCoord2f(xtexa1, ytexa1);   glVertex3f(t1, 0, t2);
	//		}
	//		glEnd();
	//}

////	����������ͼ
//	glBegin(GL_TRIANGLES);
//	for (t1 = 0; t1 < 128; t1 += 8)
//	{
//		for (t2 = 0; t2 < 128; t2 += 8)
//		{
//			xtexa1 = t1 / 128;
//			xtexa2 = (t1 + 8) / 128;
//			ytexa1 = t2 / 128;
//			ytexa2 = (t2 + 8) / 128;
//			glTexCoord2f(xtexa1, ytexa1);   glVertex3f(t1, 0, t2);
//			glTexCoord2f(xtexa2, ytexa1);   glVertex3f(t1 + 8, 0, t2);
//			glTexCoord2f(xtexa1, ytexa2);   glVertex3f(t1, 0, t2 + 8);
//	//�ڶ���������
//			glTexCoord2f(xtexa1, ytexa2);   glVertex3f(t1, 0, t2+8);
//			glTexCoord2f(xtexa2, ytexa2);   glVertex3f(t1+8, 0, t2+8);
//			glTexCoord2f(xtexa2, ytexa1);   glVertex3f(t1 + 8, 0, t2);
//		}
//	}
//	glEnd();

	
	glFinish();
	SwapBuffers(wglGetCurrentDC());
}

void CMFCSkyBoxView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	mousePoint = point;
	CView::OnLButtonDown(nFlags, point);
}

void CMFCSkyBoxView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nFlags&MK_LBUTTON){
		m_yAngle += (point.y - mousePoint.y) / 3.6;
		m_xAngle += (point.x - mousePoint.x) / 3.6;
		//Set the mouse point
		mousePoint = point;
		InvalidateRect(NULL, FALSE);

	}
	CView::OnMouseMove(nFlags, point);
}


void CMFCSkyBoxView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (moveForward == true)
		fowardSpeed-=2;//speed�����ٶȣ���ÿ��λʱ���ƶ��ľ��룬ֵԽ��
					     //�������ٶ�Խ��,��-=����Ϊgllookat�Ǵ�z�������ῴ�򸺰���
	if (moveLeft == true)
		l_r_Speed += 2;
	if (moveRight == true)
		l_r_Speed -= 2;

	moveSky += 0.04;//�˶����ƶ�����ת����ʵ�ֵ�
	if (moveSky >= 360)
		moveSky = 0;

	a++;
	if (a > 8)a = 0;

	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}


void CMFCSkyBoxView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nChar == 87 || nChar == 119)		// ����'W'��
		moveForward = true;//����ʱ�䣬���ż�OnTimer����
	if (nChar == 65 || nChar == 97)
		moveLeft = true;
	if (nChar == 68 || nChar == 100)
		moveRight = true;
	if (nChar==32)//alt����
		altDown == true;

	//����F11�л�ȫ����ʾ
	if (nChar == 122)
	{
		CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;
		pMain->FullScreenModeSwitch();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMFCSkyBoxView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nChar == 87 || nChar == 119)		// 'W'���ɿ�
		moveForward = false;//����ʱ�䣬���ż�OnTimer����
	if (nChar == 65 || nChar == 97)
		moveLeft = false;
	if (nChar == 68 || nChar == 100)
		moveRight = false;
	if (nChar==32)//alt�ɿ�
		altDown == false;
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}
