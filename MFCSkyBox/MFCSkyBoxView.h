
// MFCSkyBoxView.h : CMFCSkyBoxView ��Ľӿ�
//
#include "MainFrm.h"

#include "Terrain.h"
#include "Sky.h"
#include "Plane.h"
#include "House.h"
#include "water.h"

#include <GL\glaux.h>
#include <GL\glut.h>

#pragma once
#define PI  3.1415926

class CMFCSkyBoxView : public CView
{
	CTerrain terrain;//���ζ���
	CSky sky;//��ն���
	CPlane Plane;
	CHouse house;
	CWater water;
	GLfloat xtexa1, xtexa2, ytexa1, ytexa2;
	//����תģ����ر���
	CPoint mousePoint;
	//������ת��λ�õı���
	GLfloat m_xAngle;//��תͼ��Ƕ�
	GLfloat m_yAngle;
	float c = PI / 180;
	//��ʾ�б���ر���
	int listTerrain;
	int listLight;
	int listGrid;//����
	int listSky;
	int listPlane;
	int listHouse;
	int listWater;
	//����ʱ��
	
	bool moveLeft;//�������ƣ�����a��
	bool moveForward;//�Ƿ�����ǰ�ƣ�����w��
	bool moveRight;//�Ƿ���������
	bool altDown;//alt���£����ƳԼ�����
	float fowardSpeed;
	float l_r_Speed;//���ƻ������ٶ�
	float moveSky;//�ƶ�����ת
	int a;//ˮ����
protected: // �������л�����
	CMFCSkyBoxView();
	DECLARE_DYNCREATE(CMFCSkyBoxView)

// ����
public:
	CMFCSkyBoxDoc* GetDocument() const;

// ����
public:
	void RenderScene();
	int LightModle();
	void FogModle();
// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CMFCSkyBoxView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	HGLRC m_hRC;    //Rendering Context��ɫ������  
	CClientDC* m_pDC;        //Device Context�豸������  
	int m_wide, m_height;    //���ڿ��


// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	// ��ʼ��OpenGL
	BOOL InitializeOpenGL();
	// �������ظ�ʽ
	BOOL SetupPixelFormat();



	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // MFCSkyBoxView.cpp �еĵ��԰汾
inline CMFCSkyBoxDoc* CMFCSkyBoxView::GetDocument() const
   { return reinterpret_cast<CMFCSkyBoxDoc*>(m_pDocument); }
#endif

