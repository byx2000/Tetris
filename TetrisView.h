
// TetrisView.h: CTetrisView 类的接口
//

#pragma once

#include <gdiplus.h>

using namespace Gdiplus;

class CTetrisView : public CView
{
protected: // 仅从序列化创建
	CTetrisView() noexcept;
	DECLARE_DYNCREATE(CTetrisView)

// 特性
public:
	enum { ID_GAMETIMER = 1001, ID_TOASTTIMER = 1002, INIT_DURATION = 400 };

	CTetrisDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CTetrisView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
	int row, col;
	int unit;
	int cx, cy;
	CMemDC* pMemDC;
	int toastDeep;
	int toastDeepAddFlag;
	int toastBottom;
	bool toastFlag;
	CString toastText;
	bool musicFlag;
	Image blockImg1, blockImg2, blockImg3, blockImg4, blockImg5;
	
public:
	void drawSquare(CDC* pDC, int r, int c, COLORREF color);
	void drawBorder(CDC* pDC);
	void drawGameMap(CDC* pDC);
	void drawCurrentShape(CDC* pDC);
	void drawGame(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void clearClient(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void clearHdc(HDC hdc, int cxClient, int cyClient);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void drawShapePreview(CDC* pDC);
	afx_msg void OnGameStart();
	int duration;
	void startGameTimer();
	void endGameTimer();
	void startGame();
	void pauseGame();
	afx_msg void OnUpdateGameStart(CCmdUI* pCmdUI);
	afx_msg void OnGamePause();
	afx_msg void OnUpdateGamePause(CCmdUI* pCmdUI);
	void restartGame();
	afx_msg void OnGameRestart();
	void drawNextShape(CDC* pDC);
	int currentScore, currentLevel;
	void drawScore(CDC* pDC);
	void setFontBold(CFont& font);
	void updateTitle();
	void drawHelp(CDC* pDC);
	void drawState(CDC* pDC);
	void drawPauseGame(CDC* pDC);
	afx_msg void OnHelp();
	afx_msg void OnAbout();
	afx_msg void OnGameSave();
	afx_msg void OnGameLoad();
	int getDuration(int level);
	afx_msg void OnGameRecord();
	void updateRecord();
	void drawToast(CDC* pDC, const TCHAR* text, int deep, int bottom, int msec);
	void showToast(CString text);
	void showLevelToast();
	void showPauseToast();
	void showScoreToast(int cLine);
	afx_msg void OnMusic();
	afx_msg void OnUpdateMusic(CCmdUI* pCmdUI);
	void playBGM();
	void pauseBGM();
	void replayBGM();
	void drawBlock(CDC* pDC, int r, int c, Shape::Type type);
	bool getMusicFlag();
};

#ifndef _DEBUG  // TetrisView.cpp 中的调试版本
inline CTetrisDoc* CTetrisView::GetDocument() const
   { return reinterpret_cast<CTetrisDoc*>(m_pDocument); }
#endif

