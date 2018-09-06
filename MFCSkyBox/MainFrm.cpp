
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "MFCSkyBox.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO:  �ڴ���ӳ�Ա��ʼ������
	this->m_bFullScreenMode = false;//����ȫ����ʾΪ��
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO:  �������Ҫ��ͣ������������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
	//�Լ���ӵģ�Ϊ��ȫ����ʾ
	this->m_menuMainWnd.LoadMenu(IDR_MAINFRAME);
	this->SetMenu(&m_menuMainWnd);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::FullScreenModeOn()
{
	m_bToolBarWasVisible = (m_wndToolBar.IsWindowVisible() != 0);//���ع�����֮ǰ����������ʾ״̬    
	m_wndToolBar.ShowWindow(SW_HIDE);//���ع����� 


	m_bStatusBarWasVisible = (m_wndStatusBar.IsWindowVisible() != 0);//����״̬��֮ǰ״̬������ʾ״̬ 
	m_wndStatusBar.ShowWindow(SW_HIDE);//����״̬�� 


	//���ز˵��� 
	SetMenu(NULL);


	// ������ǰ��λ����Ϣ 
	GetWindowRect(&m_mainRect);


	// ȥ�������ڵı��� 
	LONG style = ::GetWindowLong(m_hWnd, GWL_STYLE);

	style &= ~WS_CAPTION;

	::SetWindowLong(m_hWnd, GWL_STYLE, style);



	//�õ���ǰϵͳ�ķֱ��� 
	int screenx = GetSystemMetrics(SM_CXSCREEN);

	int screeny = GetSystemMetrics(SM_CYSCREEN);



	// ȫ����ʾ 
	SetWindowPos(NULL, -4, -4, screenx + 8, screeny + 8, SWP_NOZORDER);

	style = ::GetWindowLong(this->m_hWnd, GWL_STYLE);


	//����ȫ����ʾ��־ 
	m_bFullScreenMode = true;
}


void CMainFrame::FullScreenModeOff()
{
	//�ָ����ڱ��� 
	LONG style = ::GetWindowLong(m_hWnd, GWL_STYLE);

	style |= WS_CAPTION;

	::SetWindowLong(m_hWnd, GWL_STYLE, style);

	//�����Ҫ����ʾ������ 
	if (m_bToolBarWasVisible)

		m_wndToolBar.ShowWindow(SW_SHOW);

	//�����Ҫ����ʾ״̬�� 
	if (m_bStatusBarWasVisible)

		m_wndStatusBar.ShowWindow(SW_SHOW);

	//�ָ�������ǰ�Ĵ�С 
	MoveWindow(&m_mainRect);


	//�ָ��˵��� 
	this->SetMenu(&m_menuMainWnd);


	//����ȫ����ʾ��־ 
	m_bFullScreenMode = false;
}


void CMainFrame::FullScreenModeSwitch()
{
	if (this->m_bFullScreenMode)
	{
		this->FullScreenModeOff();
	}
	else
	{
		this->FullScreenModeOn();
	}
}