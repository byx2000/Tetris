
// TetrisDoc.h: CTetrisDoc 类的接口
//


#pragma once

#include "game/Game.h"

class CTetrisDoc : public CDocument
{
protected: // 仅从序列化创建
	CTetrisDoc() noexcept;
	DECLARE_DYNCREATE(CTetrisDoc)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CTetrisDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
	static int row, col;
	static int unit;
	GameController game;

public:
	static int getRow();
	static int getCol();
	static int getUnit();
	GameMap::BlockState getBlockState(int r, int c);
	Shape getCurrentShape();
	void start();
	void pause();
	GameController::GameState getCurrentState();
	int run();
	void left();
	void right();
	void rotate();
	void reset();
	void fall();
	Shape getCurrentShapePreview();
	Shape getNextShape();
	int getScore();
	int getLevel();
	//COLORREF getBlockColor(int r, int c);
	Shape::Type getBlockType(int r, int c);
	bool saveGame(const char* filename);
	bool loadGame(const char* filename);
	bool saveGameRecord();
	int loadGameRecord();
	int updateMap();
	void setScore(int score);
	void setLevel(int level);
};
