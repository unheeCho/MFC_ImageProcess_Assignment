// CImageProcess.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "CImageProcess.h"


// CImageProcess

CImageProcess::CImageProcess()
{
}

CImageProcess::~CImageProcess()
{
}


/// <summary>
/// 이미지에서 원 찾기
/// </summary>
/// <param name="image">대상 이미지</param>
/// <param name="color">찾을색</param>
/// <param name="out_maxRadius">반지름 정보</param>
/// <returns>찾은 원의 좌표</returns>
CPoint CImageProcess::GetCirclePoint(CImage& image, COLORREF color, double & out_maxRadius)
{
    out_maxRadius = 0;
    int nWidth = image.GetWidth();
    int nHeight = image.GetHeight();

    long long sumX = 0, sumY = 0;
    int darkPixelCount = 0;
    bool bFirstCheck = false;
    int maxX = -1;
    for (int y = 0; y < nHeight; ++y)
    {
        for (int x = 0; x < nWidth; ++x)
        {
            COLORREF PixelColor = image.GetPixel(x, y);

            if (color == PixelColor)
            {
                sumX += x;
                sumY += y;
                darkPixelCount++;

                maxX = max(maxX, x);
            }
        }
    }

    if (darkPixelCount == 0)
        return CPoint(-1, -1);

    // 찾아낸 각축에 따른 갯수와 합계를 가지고 평균을 내면 각좌표의 중심이다. 
    int centerX = static_cast<int>(sumX / darkPixelCount);
    int centerY = static_cast<int>(sumY / darkPixelCount);

    // 반지름 계산
    double maxRadius = static_cast<double>(maxX - centerX);
    
    out_maxRadius = maxRadius;
    return CPoint(centerX, centerY);
}

/// <summary>
/// 특정좌표에 X그리기 
/// </summary>
/// <param name="image">그려야 하는 이미지</param>
/// <param name="cCenterPos">찾은 원의 좌표</param>
/// <param name="maxRadius">찾은 원의 반지름</param>
/// <param name="nCrossLen">반지름 대비 X표시의 길이 </param>
/// <param name="nThickness">X표시의 굵기</param>
/// <param name="clLine">그릴 색</param>
void CImageProcess::DrawCrossOnCircle(CImage& image, CPoint cCenterPos, double maxRadius, double nCrossLen, int nThickness, COLORREF clLine)
{
    int nWidth = image.GetWidth();
    int nHeight = image.GetHeight();
    int nPitch = image.GetPitch();

    // X자 길이 제한
    int crossLength = static_cast<int>(maxRadius * nCrossLen);

    int thickness = nThickness; // 선의 두께

    for (int offset = 0; offset <= crossLength; ++offset)
    {
        for (int t = -thickness; t <= thickness; ++t)
        {
            if (cCenterPos.x + offset < nWidth && cCenterPos.y + offset + t < nHeight && cCenterPos.y + offset + t >= 0)
                image.SetPixel(cCenterPos.x + offset, cCenterPos.y + offset + t, clLine);
            if (cCenterPos.x - offset >= 0 && cCenterPos.y - offset + t < nHeight && cCenterPos.y - offset + t >= 0)
                image.SetPixel(cCenterPos.x - offset, cCenterPos.y - offset + t, clLine);

            if (cCenterPos.x + offset < nWidth && cCenterPos.y - offset + t < nHeight && cCenterPos.y - offset + t >= 0)
                image.SetPixel(cCenterPos.x + offset, cCenterPos.y - offset + t, clLine);
            if (cCenterPos.x - offset >= 0 && cCenterPos.y + offset + t < nHeight && cCenterPos.y + offset + t >= 0)
                image.SetPixel(cCenterPos.x - offset, cCenterPos.y + offset + t, clLine);
        }
    }

    CString strText;
    strText.Format(_T("(%d, %d)"), cCenterPos.x, cCenterPos.y);

    DrawTextOffCircle(image, cCenterPos, maxRadius, strText);

}

/// <summary>
/// 원 밖에 텍스트 쓰기
/// </summary>
/// <param name="image">그려야 하는 이미지</param>
/// <param name="cCenterPos">찾은 원의 좌표</param>
/// <param name="maxRadius">찾은 원의 반지름</param>
/// <param name="strText">입력할 텍스트</param>
void CImageProcess::DrawTextOffCircle(CImage& image, CPoint cCenterPos, double maxRadius, CString strText)
{
    // 이미지의 DC를 얻기
    CDC* pDC = CDC::FromHandle(image.GetDC());
   
    CSize textSize = pDC->GetTextExtent(strText);

    int x = cCenterPos.x + maxRadius;
    int y = cCenterPos.y + maxRadius;
    // 텍스트 그리기
    pDC->DrawText(strText, CRect(x, y, x + textSize.cx, y + textSize.cy), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    image.ReleaseDC();
}


/// <summary>
/// 원3개가 지나는 원만들기
/// </summary>
/// <param name="image">그려야 하는 이미지</param>
/// <param name="p1">첫번째 포인트</param>
/// <param name="p2">두번째 포인트</param>
/// <param name="p3">세번째 포인트</param>
/// <param name="m_memDC">더블버퍼DC</param>
void CImageProcess::DrawCircumCircle(CImage& image, CPoint p1, CPoint p2, CPoint p3, double fPointRadius, CDC* m_memDC)
{
    CDC* pDC = CDC::FromHandle(image.GetDC());

    // 3개의 점 (p1, p2, p3)을 이용해 외접원의 중심 (cx, cy)와 반지름 R 계산
    double x1 = p1.x, y1 = p1.y;
    double x2 = p2.x, y2 = p2.y;
    double x3 = p3.x, y3 = p3.y;

    // D 계산
    double D = 2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));

    // 중심 (cx, cy) 계산
    double cx = ((x1 * x1 + y1 * y1) * (y2 - y3) +
        (x2 * x2 + y2 * y2) * (y3 - y1) +
        (x3 * x3 + y3 * y3) * (y1 - y2)) / D;

    double cy = ((x1 * x1 + y1 * y1) * (x3 - x2) +
        (x2 * x2 + y2 * y2) * (x1 - x3) +
        (x3 * x3 + y3 * y3) * (x2 - x1)) / D;

    // 반지름 R 계산 (p1을 기준으로 계산)
    double bRadius = sqrt((x1 - cx) * (x1 - cx) + (y1 - cy) * (y1 - cy));
  
    CBrush backgroundBrush(RGB(255, 255, 255)); // 흰색 배경
    CRect rect(0, 0, image.GetWidth(), image.GetHeight()); // 이미지 전체 크기
   


    CBrush Redbrush = RGB(255, 0, 0);
    CBrush Greenbrush = RGB(0, 255, 0);
    CBrush Bluebrush = RGB(0, 0, 255);
  
    if (m_memDC == NULL)
    {
        pDC->FillRect(&rect, &backgroundBrush);

        DrawThreePointText(p1, p2, p3, pDC, fPointRadius);
                
        CBrush* pOldBrush = pDC->SelectObject(&Redbrush);
        pDC->Ellipse(CRect(p1.x - fPointRadius, p1.y - fPointRadius, p1.x + fPointRadius, p1.y + fPointRadius));
        pOldBrush = pDC->SelectObject(&Greenbrush);
        pDC->Ellipse(CRect(p2.x - fPointRadius, p2.y - fPointRadius, p2.x + fPointRadius, p2.y + fPointRadius));
        pOldBrush = pDC->SelectObject(&Bluebrush);
        pDC->Ellipse(CRect(p3.x - fPointRadius, p3.y - fPointRadius, p3.x + fPointRadius, p3.y + fPointRadius));

        pDC->Arc(CRect(cx - bRadius, cy - bRadius, cx + bRadius, cy + bRadius), CPoint(cx, cy), CPoint(cx, cy));
        pDC->SelectObject(pOldBrush);
    }
    else
    {
        m_memDC->FillRect(&rect, &backgroundBrush);

        DrawThreePointText(p1, p2, p3, m_memDC, fPointRadius);
        
        CBrush* pOldBrush = m_memDC->SelectObject(&Redbrush);
        m_memDC->Ellipse(CRect(p1.x - fPointRadius, p1.y - fPointRadius, p1.x + fPointRadius, p1.y + fPointRadius));
        pOldBrush = m_memDC->SelectObject(&Greenbrush);
        m_memDC->Ellipse(CRect(p2.x - fPointRadius, p2.y - fPointRadius, p2.x + fPointRadius, p2.y + fPointRadius));
        pOldBrush = m_memDC->SelectObject(&Bluebrush);
        m_memDC->Ellipse(CRect(p3.x - fPointRadius, p3.y - fPointRadius, p3.x + fPointRadius, p3.y + fPointRadius));

        m_memDC->Arc(CRect(cx - bRadius, cy - bRadius, cx + bRadius, cy + bRadius), CPoint(cx, cy), CPoint(cx, cy));
        m_memDC->SelectObject(pOldBrush);
    }
    image.ReleaseDC();
}

/// <summary>
/// 원그리기
/// </summary>
/// <param name="image">그려야 하는 이미지</param>
/// <param name="x">중심점X</param>
/// <param name="y">중심점Y</param>
/// <param name="nRadius">반지름</param>
/// <param name="color">색깔</param>
void CImageProcess::DrawCircle(CImage& image, int x, int y, int nRadius, COLORREF color)
{
    unsigned char* fm = (unsigned char*)image.GetBits();

    int nCenterX = x;
    int nCenterY = y;
    int nPitch = image.GetPitch();
    int nBpp = image.GetBPP() / 8; // 바이트당 픽셀 수 (예: 24비트 = 3바이트)

    unsigned char red = GetRValue(color);
    unsigned char green = GetGValue(color);
    unsigned char blue = GetBValue(color);

    for (int j = y - nRadius; j < y + nRadius; j++)
    {
        for (int i = x - nRadius; i < x + nRadius; i++)
        {
            if (IsInCircle(i, j, nCenterX, nCenterY, nRadius))
            {
                if (validImgPos(image, i, j))
                {
                    unsigned char* pixel = fm + j * nPitch + i * nBpp;

                    pixel[0] = blue;
                    pixel[1] = green;
                    pixel[2] = red;
                }
            }
        }
    }
}

bool CImageProcess::IsInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius)
{
    bool bRet = FALSE;

    double dX = static_cast<double>(i - nCenterX);
    double dY = static_cast<double>(j - nCenterY);
    double dDist = (dX * dX) + (dY * dY);

    if (dDist < static_cast<double>(nRadius * nRadius))
        bRet = TRUE;

    return bRet;
}

bool CImageProcess::validImgPos(CImage& image, int x, int y)
{
    int nWidth = image.GetWidth();
    int nHeight = image.GetHeight();
    CRect rect(0, 0, nWidth, nHeight);

    return rect.PtInRect(CPoint(x, y));
}

void CImageProcess::DrawThreePointText(CPoint& p1, CPoint& p2, CPoint& p3, CDC* pDC, double fPointRadius)
{
    DrawPointText(p1, pDC, fPointRadius);
    DrawPointText(p2, pDC, fPointRadius);
    DrawPointText(p3, pDC, fPointRadius);

}

void CImageProcess::DrawPointText(CPoint& p1, CDC* pDC, double fPointRadius)
{
    CString strText;
    CSize textSize;

    strText.Format(_T("(%d, %d)"), p1.x, p1.y);
    textSize = pDC->GetTextExtent(strText);
    int x = p1.x + fPointRadius;
    int y = p1.y + fPointRadius;
    pDC->DrawText(strText, CRect(x, y, x + textSize.cx, y + textSize.cy), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}