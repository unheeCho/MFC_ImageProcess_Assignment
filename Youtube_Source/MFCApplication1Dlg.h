
// MFCApplication1Dlg.h: 헤더 파일
//

#pragma once
#include <vector>

// CMFCApplication1Dlg 대화 상자
class CMFCApplication1Dlg : public CDialogEx
{
private:
	CDC m_memDC;       // 백 버퍼용 DC
	CBitmap m_bitmap;  // 백 버퍼 비트맵
	CImage m_image;
	CEdit* pEditStartX;
	CEdit* pEditStartY;
	CEdit* pEditEndX;
	CEdit* pEditEndY;
	CEdit* m_EditResult;
	CButton* pBtnLoad;
	CButton* pBtnDraw;
	CButton* pBtnAction;
	CButton* pBtnFileLoad;
	CButton* pBtnClear;
	CButton* pBtnCircum;
	bool m_bCircum;
	bool m_bDrawing;
	int m_nSelRadius;
	int m_nClickStack;
	std::vector<CPoint> pThreePoint;

// 생성입니다.
public:
	CPoint m_StartPoint;
	CPoint m_EndPoint;

	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	int GetRandomNumber(int min, int max);
	void moveCircle(CString strFilePath);
	void UpdateDisplay();
	void InitBoard(unsigned char* fm, int nWidth, int nHeight, int nPitch, COLORREF color);
	bool IsPointInCircle(const CPoint& clickPoint, const CPoint& center, float radius);
	CString GetModulePath();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnDraw();
	afx_msg void OnBnClickedBtnAction();
	afx_msg void OnBnClickedBtnLoad();
	afx_msg void OnBnClickedBtnPingpong();
	afx_msg void OnBnClickedBtnTest();
	afx_msg void OnBnClickedBtnFileLoad();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnCircum();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnAction2();
	afx_msg void OnBnClickedBtnStart2();
	afx_msg void OnBnClickedBtnTest2();
};
