
// MFCApplication1Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include <random>
#include <iostream>
#include <cmath>
#include "CImageProcess.h"
#include <thread>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
#define IMAGE_WIDTH 500
#define IMAGE_HEIGHT 400

#define IMAGE_START_X 20
#define IMAGE_START_Y 55

#define COLOR_BLACK RGB(0, 0, 0)
#define COLOR_WHITE RGB(255, 255, 255)
#define COLOR_RED RGB(255, 0, 0)
#define COLOR_GREEN RGB(0, 255, 0)
#define COLOR_BLUE RGB(0, 0, 255)

#define CLICK_CIRCLE_RADIUS 5

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 대화 상자



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_DRAW, &CMFCApplication1Dlg::OnBnClickedBtnDraw)
	ON_BN_CLICKED(IDC_BTN_ACTION, &CMFCApplication1Dlg::OnBnClickedBtnAction)
	ON_BN_CLICKED(IDC_BTN_PINGPONG, &CMFCApplication1Dlg::OnBnClickedBtnPingpong)
	ON_BN_CLICKED(IDC_BTN_FILE_LOAD, &CMFCApplication1Dlg::OnBnClickedBtnFileLoad)
	ON_BN_CLICKED(IDC_BTN_START, &CMFCApplication1Dlg::OnBnClickedBtnStart)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_CIRCUM, &CMFCApplication1Dlg::OnBnClickedBtnCircum)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CMFCApplication1Dlg::OnBnClickedBtnClear)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CMFCApplication1Dlg 메시지 처리기

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 백 버퍼 초기화
	CClientDC dc(this);
	m_memDC.CreateCompatibleDC(&dc);
	m_bitmap.CreateCompatibleBitmap(&dc, IMAGE_WIDTH, IMAGE_HEIGHT);
	m_memDC.SelectObject(&m_bitmap);

	// 초기 백 버퍼를 흰색으로 채우기
	m_memDC.FillSolidRect(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, RGB(255, 255, 255));

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	

	pBtnLoad = (CButton*)GetDlgItem(IDC_BTN_START);
	pBtnDraw = (CButton*)GetDlgItem(IDC_BTN_DRAW);
	pBtnAction = (CButton*)GetDlgItem(IDC_BTN_ACTION);

	pEditStartX = (CEdit*)GetDlgItem(IDC_EDIT_START_X);
	pEditStartY = (CEdit*)GetDlgItem(IDC_EDIT_START_Y);
	pEditEndX = (CEdit*)GetDlgItem(IDC_EDIT_END_X);
	pEditEndY= (CEdit*)GetDlgItem(IDC_EDIT_END_Y);
	pBtnFileLoad = (CButton*)GetDlgItem(IDC_BTN_FILE_LOAD);
	pBtnClear = (CButton*)GetDlgItem(IDC_BTN_CLEAR);
	pBtnCircum = (CButton*)GetDlgItem(IDC_BTN_CIRCUM);

	pEditStartX->SetWindowTextW(L"120");
	pEditStartY->SetWindowTextW(L"120");
	pEditEndX->SetWindowTextW(L"250");
	pEditEndY->SetWindowTextW(L"250");
	
	m_nSelRadius = 0;
	m_nClickStack = 0;
	pThreePoint.clear();
	m_bCircum = false;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CMFCApplication1Dlg::GetRandomNumber(int min, int max)
{
	// 랜덤 엔진과 분포 초기화
	static std::random_device rd;                              // 랜덤 시드 생성
	static std::mt19937 generator(rd());                       // 난수 생성 엔진
	std::uniform_int_distribution<int> distribution(min, max); // 정수 범위 설정

	// 랜덤 숫자 반환
	return distribution(generator);
}

void CMFCApplication1Dlg::InitBoard(unsigned char* fm, int nWidth, int nHeight, int nPitch, COLORREF color)
{
	int nBpp = m_image.GetBPP() / 8; // 바이트당 픽셀 수 (예: 24비트 = 3바이트)

	unsigned char red = GetRValue(color);
	unsigned char green = GetGValue(color);
	unsigned char blue = GetBValue(color);

	for (int y = 0; y < nHeight; ++y)
	{
		for (int x = 0; x < nWidth; ++x)
		{
			unsigned char* pixel = fm + y * nPitch + x * nBpp;

			pixel[0] = blue;
			pixel[1] = green;
			pixel[2] = red;
		}
	}
}

void CMFCApplication1Dlg::moveCircle(CString strFilePath)
{
	CString strText;
	
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int nPitch = m_image.GetPitch();

	// 이동 단계 설정
	const int nSteps = 50; // 이동 단계 수
	const double dx = static_cast<double>(m_EndPoint.x - m_StartPoint.x) / nSteps;
	const double dy = static_cast<double>(m_EndPoint.y - m_StartPoint.y) / nSteps;
	CImageProcess imageprocess;

	for (int i = 0; i <= nSteps; ++i)
	{
		int currentX = static_cast<int>(m_StartPoint.x + i * dx);
		int currentY = static_cast<int>(m_StartPoint.y + i * dy);

		// 비트맵 초기화
		unsigned char* fm = (unsigned char*)m_image.GetBits();
		int nBytes = abs(m_image.GetPitch()) * m_image.GetHeight();
		InitBoard(fm, nWidth, nHeight, nPitch, COLOR_WHITE);

		imageprocess.DrawCircle(m_image, currentX, currentY, m_nSelRadius, COLOR_BLUE);;

		UpdateDisplay();
		
		strText.Format(_T("%s\\ActionDebug_%02d.bmp"), strFilePath, i+1);

		m_image.Save(strText);

		Sleep(20);
	}
	
}

void CMFCApplication1Dlg::UpdateDisplay()
{
	CClientDC dc(this);
	m_image.Draw(dc, IMAGE_START_X, IMAGE_START_Y);
}

CString CMFCApplication1Dlg::GetModulePath()
{
	TCHAR path[256] = { 0, };
	GetModuleFileName(NULL, path, 256);

	CString folderPath(path);
	folderPath = folderPath.Left(folderPath.ReverseFind('\\'));

	return folderPath;

}

void CMFCApplication1Dlg::OnBnClickedBtnStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	pBtnDraw->EnableWindow(1);
	pBtnLoad->EnableWindow(0);
	pBtnFileLoad->EnableWindow(0);
	pBtnClear->EnableWindow(1);

	int nWidth = IMAGE_WIDTH;
	int nHeight = IMAGE_HEIGHT;
	int nBpp = 24;

	m_image.Create(nWidth, nHeight, nBpp);
	int nPitch = m_image.GetPitch();
	if (nBpp == 8)
	{
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
		{
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
			m_image.SetColorTable(0, 256, rgb);
		}
	}
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	
	InitBoard(fm, nWidth, nHeight, nPitch, COLOR_WHITE);

	UpdateDisplay();
}

void CMFCApplication1Dlg::OnBnClickedBtnDraw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (pBtnAction->IsWindowEnabled() == FALSE)
		pBtnAction->EnableWindow(1);

	CString strText;
	static int nSttX = 0;
	static int nSttY = 0;
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int nPitch = m_image.GetPitch();
	int nRadius = GetRandomNumber(6, 25);
	m_nSelRadius = nRadius;
	pEditStartX->GetWindowText(strText);
	nSttX = _wtoi(strText);
	pEditStartY->GetWindowText(strText);
	nSttY = _wtoi(strText);

	unsigned char* fm = (unsigned char*)m_image.GetBits();
	InitBoard(fm, nWidth, nHeight, nPitch, COLOR_WHITE);

	CImageProcess imageprocess;
	imageprocess.DrawCircle(m_image, nSttX, nSttY, nRadius, COLOR_BLUE);

	UpdateDisplay();
}

void  threadProcess(CWnd* pParent, CString strFolderName)
{
	CMFCApplication1Dlg *pWnd = (CMFCApplication1Dlg*)pParent;
	CString* pStr = new CString(strFolderName);
	pWnd->moveCircle(strFolderName);
}

void CMFCApplication1Dlg::OnBnClickedBtnAction()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strText;
	static int nX = 0;
	static int nY = 0;
	
	// 현재 중심 좌표 가져오기
	pEditStartX->GetWindowText(strText);
	nX = _wtoi(strText);
	pEditStartY->GetWindowText(strText);
	nY = _wtoi(strText);
	m_StartPoint = CPoint(nX, nY);

	// 목표 중심 좌표 가져오기
	pEditEndX->GetWindowText(strText);
	nX = _wtoi(strText);
	pEditEndY->GetWindowText(strText);
	nY = _wtoi(strText);
	m_EndPoint = CPoint(nX, nY);

	CString strFolderName = GetModulePath() + _T("\\image");
	if (GetFileAttributes(strFolderName) == -1)
		CreateDirectory(strFolderName, NULL);

	std::thread _thread0(threadProcess, this, strFolderName);
	_thread0.detach();
	//moveCircle(strFolderName);

	CButton* pBtnFileLoad = (CButton*)GetDlgItem(IDC_BTN_FILE_LOAD);
	pBtnFileLoad->EnableWindow(1);
	pBtnClear->EnableWindow(1);
}

void CMFCApplication1Dlg::OnBnClickedBtnPingpong()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strText;
	static int nSttX = 0;
	static int nSttY = 0;
	
	// 현재 중심 좌표 가져오기
	pEditStartX->GetWindowText(strText);
	nSttX = _wtoi(strText);
	pEditStartY->GetWindowText(strText);
	nSttY = _wtoi(strText);

	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int nPitch = m_image.GetPitch();
	int nCurrentX = nSttX;
	int nCurrentY = nSttY;
	int nGap = 3;
	bool bXplus = TRUE;
	bool bYplus = TRUE;
	CImageProcess imageprocess;

	for (int i = 0; i <= 5000; ++i)
	{
		if (bXplus == TRUE)
			nCurrentX = static_cast<int>(nCurrentX + nGap);
		if (bXplus == FALSE)
			nCurrentX = static_cast<int>(nCurrentX - nGap);
		if (bYplus == TRUE)
			nCurrentY = static_cast<int>(nCurrentY + nGap);
		if (bYplus == FALSE)
			nCurrentY = static_cast<int>(nCurrentY - nGap);

		if (nCurrentX - m_nSelRadius < 0)
		{
			if (bXplus == FALSE)
			{
				bXplus = !bXplus;
				nCurrentX = nCurrentX - m_nSelRadius;
				nCurrentX = -nCurrentX;
			}
		}
		else if (nCurrentX + m_nSelRadius > nWidth)
		{
			if (bXplus == TRUE)
			{
				bXplus = !bXplus;
				nCurrentX = nWidth - (nCurrentX + m_nSelRadius - nWidth);
			}
		}

		if (nCurrentY - m_nSelRadius < 0)
		{
			if (bYplus == FALSE)
			{
				bYplus = !bYplus;
				nCurrentY = nCurrentY - m_nSelRadius;
				nCurrentY = -nCurrentY;
			}
		}
		else if (nCurrentY + m_nSelRadius > nHeight)
		{
			if (bYplus == TRUE)
			{
				bYplus = !bYplus;
				nCurrentY = nHeight - (nCurrentY + m_nSelRadius - nHeight);
			}
		}

		unsigned char* fm = (unsigned char*)m_image.GetBits();
		int nBytes = abs(m_image.GetPitch()) * m_image.GetHeight();
		InitBoard(fm, nWidth, nHeight, nPitch, COLOR_WHITE);

		// 원 그리기
		imageprocess.DrawCircle(m_image, nCurrentX, nCurrentY, m_nSelRadius, COLOR_BLUE);


		UpdateDisplay();
		Sleep(20);
	}
}

void CMFCApplication1Dlg::OnBnClickedBtnFileLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str = _T("All files(*.*)|*.*|"); // 모든 파일 표시
	CFileDialog dlg(TRUE, _T("*.bmp, *.jpg"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);
	CString strPathName = GetModulePath();;

	dlg.m_ofn.lpstrInitialDir = strPathName;

	if (dlg.DoModal() == IDOK)
	{
		strPathName = dlg.GetPathName();
	}
	else
		return;

	if (m_image != NULL)
	{
		m_image.Destroy();
	}
	m_image.Load(strPathName);
	
	CImageProcess imageprocess;
	double maxRadius = 0;
	CPoint cCenterPos = imageprocess.GetCirclePoint(m_image, COLOR_BLUE, maxRadius);
	if (cCenterPos.x == -1)
	{
		AfxMessageBox(_T("Could not find it."));
		return;
	}
	imageprocess.DrawCrossOnCircle(m_image, cCenterPos, maxRadius, 0.3, 1, RGB(255, 255, 255));
	UpdateDisplay();
	
}

void CMFCApplication1Dlg::OnBnClickedBtnClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	pBtnDraw->EnableWindow(1);
	pBtnLoad->EnableWindow(0);
	//pBtnFileLoad->EnableWindow(0);
	//pBtnClear->EnableWindow(0);

	m_nClickStack = 0;
	pThreePoint.clear();
	m_bCircum = false;

	int nWidth = IMAGE_WIDTH;
	int nHeight = IMAGE_HEIGHT;
	int nBpp = 24;
	if (m_image != NULL)
	{
		m_image.Destroy();
	}

	m_image.Create(nWidth, nHeight, nBpp);
	int nPitch = m_image.GetPitch();
	if (nBpp == 8)
	{
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
		{
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
			m_image.SetColorTable(0, 256, rgb);
		}
	}
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	InitBoard(fm, nWidth, nHeight, nPitch, COLOR_WHITE);

	UpdateDisplay();
}

void CMFCApplication1Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_image != NULL)
	{
		CImageProcess imageprocess;
		int nWidth = m_image.GetWidth();
		int nHeight = m_image.GetHeight();
		int nPitch = m_image.GetPitch();
		unsigned char* fm = (unsigned char*)m_image.GetBits();
		CButton* pBtnwCircum = (CButton*)GetDlgItem(IDC_BTN_CIRCUM);

		int x = point.x - IMAGE_START_X;
		int y = point.y - IMAGE_START_Y;

		if (IMAGE_WIDTH < x || IMAGE_HEIGHT < y)
			return;
		
		if (m_bCircum == false)
		{
			if (m_nClickStack >= 3)
				return;

			if (m_nClickStack == 2)
			{
				pBtnwCircum->EnableWindow(1);
				pThreePoint.push_back(CPoint(x, y));
				imageprocess.DrawCircle(m_image, x, y, CLICK_CIRCLE_RADIUS, COLOR_BLUE);
			}
			else if (m_nClickStack == 1)
			{
				pBtnwCircum->EnableWindow(0);
				pThreePoint.push_back(CPoint(x, y));
				imageprocess.DrawCircle(m_image, x, y, CLICK_CIRCLE_RADIUS, COLOR_GREEN);
			}
			else
			{
				pBtnwCircum->EnableWindow(0);
				pThreePoint.push_back(CPoint(x, y));
				imageprocess.DrawCircle(m_image, x, y, CLICK_CIRCLE_RADIUS, COLOR_RED);
			}
		}
		else
		{
			if (m_nClickStack >= 3)
			{
				if (IsPointInCircle(CPoint(x, y), pThreePoint[2], CLICK_CIRCLE_RADIUS) == true)
					m_bDrawing = true; // 드로잉 시작
			}
			
		}
		
		UpdateDisplay();
		++m_nClickStack;
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMFCApplication1Dlg::OnBnClickedBtnCircum()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_image != NULL && m_nClickStack == 3)
	{
		CClientDC dc(this);
		CImageProcess imageprocess;
		imageprocess.DrawCircumCircle(m_image, pThreePoint[0], pThreePoint[1], pThreePoint[2], CLICK_CIRCLE_RADIUS, NULL);
		UpdateDisplay();
		m_bCircum = true;
	}
}
bool CMFCApplication1Dlg::IsPointInCircle(const CPoint& clickPoint, const CPoint& center, float radius)
{
	// 두 점 사이의 거리 계산
	float dx = static_cast<float>(clickPoint.x - center.x);
	float dy = static_cast<float>(clickPoint.y - center.y);

	// 거리의 제곱과 반지름의 제곱 비교
	return (dx * dx + dy * dy) <= (radius * radius);
}
void CMFCApplication1Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bDrawing)
	{
		m_bDrawing = false; // 드로잉 종료

		CClientDC dc(this);
		CImageProcess imageprocess;
		imageprocess.DrawCircumCircle(m_image, pThreePoint[0], pThreePoint[1], pThreePoint[2], CLICK_CIRCLE_RADIUS, NULL);
		UpdateDisplay();
		m_bCircum = true;
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CMFCApplication1Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bDrawing)
	{
		CClientDC dc(this);
		int x = point.x - IMAGE_START_X;
		int y = point.y - IMAGE_START_Y;

		if (m_memDC.m_hDC == NULL)  // m_memDC가 아직 초기화되지 않았다면 초기화
		{
			m_memDC.CreateCompatibleDC(&dc);
			m_memDC.SelectObject(&m_bitmap); // 백버퍼 비트맵 선택
		}

		// 백버퍼 초기화: 이전에 그린 내용 지우기
		m_memDC.FillSolidRect(CRect(IMAGE_START_X, IMAGE_START_Y, IMAGE_START_X + IMAGE_WIDTH, IMAGE_START_Y + IMAGE_HEIGHT), RGB(255, 255, 255)); // 흰색으로 초기화

		
		CImageProcess imageprocess;
		imageprocess.DrawCircumCircle(m_image, pThreePoint[0], pThreePoint[1], CPoint(x,y), CLICK_CIRCLE_RADIUS, &m_memDC);

		pThreePoint[2] = CPoint(x, y);
		// 백 버퍼 내용을 화면에 복사
		dc.BitBlt(IMAGE_START_X, IMAGE_START_Y, IMAGE_WIDTH, IMAGE_HEIGHT, &m_memDC, 0, 0, SRCCOPY);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}
