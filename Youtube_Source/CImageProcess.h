#pragma once

// CImageProcess 명령 대상

class CImageProcess : public CObject
{
private:
	bool validImgPos(CImage& image, int x, int y);
	bool IsInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius);
	void DrawThreePointText(CPoint& p1, CPoint& p2, CPoint& p3, CDC* pDC, double fPointRadius);
public:
	CImageProcess();
	virtual ~CImageProcess();
	CPoint GetCirclePoint(CImage &image, COLORREF color, double& out_maxRadius);
	void DrawCrossOnCircle(CImage& image, CPoint cCenterPos, double maxRadius, double nCrossLen, int nThickness, COLORREF clLine);
	void DrawTextOffCircle(CImage& image, CPoint cCenterPos, double maxRadius, CString strText);
	void DrawCircumCircle(CImage& image, CPoint p1, CPoint p2, CPoint p3, double fPointRadius, CDC* m_memDC);
	void DrawPointText(CString& strText, CPoint& p1, CSize& textSize, CDC* pDC, double fPointRadius);
	void DrawCircle(CImage& image, int x, int y, int nRadius, COLORREF color);
	void DrawPointText(CPoint& p1, CDC* pDC, double fPointRadius);
};


