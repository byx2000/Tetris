#pragma once

#include <vector>
#include <ctime>
#include <Windows.h>

/*
	存储形状的基本数据，包括每个方格的相对坐标和全局坐标，实现了旋转、移动算法
*/
class Shape
{
	friend class GameController;
public:
	//七种形状
	enum Type
	{
		L,
		RL,
		Z,
		RZ,
		T,
		Line,
		Square,
		Empty
	};

	//构造函数
	Shape() {}
	Shape(Type type);

	//成员函数
	void setType(Type type);
	Type getType() const;
	//COLORREF getColor() const;
	void setGlobalPos(int gr, int gc);
	void setBlocklPos(int index, int r, int c);
	int getGlobalPosRow() const;
	int getGlobalPosCol() const;
	int getBlockPosRow(int index) const;
	int getBlockPosCol(int index) const;
	int getBlockGlobalPosRow(int index) const;
	int getBlockGlobalPosCol(int index) const;
	int getMinCol() const; //获取方块最小列坐标
	int getMaxCol() const; //获取方块最大列坐标
	int getMinRow() const; //获取方块最小行坐标
	int getMaxRow() const; //获取方块最大行坐标

	void rotate(); //向右旋转90度
	void left(int step); //左移
	void right(int step); //右移
	void up(int step); //上移
	void down(int step); //下移

private:
	struct Pos { int r, c; };

	Type type; //类型
	int gr, gc; //左上角全局坐标
	Pos p[4]; //四个方块相对于左上角的坐标
	//COLORREF color; //形状颜色
};

/*
	存储游戏地图数据，包括每个方格的状态和颜色，实现了消行算法
*/
class GameMap
{
	friend class GameController;
public:
	//方块属性
	enum BlockState
	{
		BLANK,  //空白
		OCCUPY  //占用
	};

	//最大尺寸
	enum { MaxRow = 60, MaxCol = 60 };

	//构造函数
	GameMap();
	GameMap(int row, int col);

	//成员函数
	void reset(int row, int col);
	int getRow() const;
	int getCol() const;
	BlockState getBlockState(int r, int c) const;
	void setBlockState(int r, int c, BlockState state);
	//COLORREF getBlockColor(int r, int c) const;
	//void setBlockColor(int r, int c, COLORREF color);
	Shape::Type getBlockType(int r, int c) const;
	void setBlockType(int r, int c, Shape::Type type);
	void clear(); //清空地图
	int updata(); //消行算法 返回消除的行数
	void print() const;

private:
	struct BlockInfo
	{
		BlockState state;
		Shape::Type type;
	};

	int row, col;
	BlockInfo map[MaxRow][MaxCol];
};

/*
	管理游戏的一切资源：游戏地图数据、当前形状、下一个形状、得分
*/
class GameController
{
public:
	//游戏状态
	enum GameState
	{
		PAUSE,
		RUNNING,
		GAMEOVER
	};

	//构造函数
	GameController(int row, int col);

	//成员函数
	GameState getCurrentState() const;
	Shape getCurrentShape() const;
	Shape getNextShape() const;
	//int getTotalLine() const; //获取消除的行数
	int getScore() const; //获取得分
	int getLevel() const; //获取等级
	void setScore(int score);
	void setLevel(int level);
	int getRow() const;
	int getCol() const;
	GameMap::BlockState getBlockState(int r, int c) const;
	void setBlockState(int r, int c, GameMap::BlockState state);
	//COLORREF getBlockColor(int r, int c) const;
	//void setBlockColor(int r, int c, COLORREF color);
	Shape::Type getBlockType(int r, int c) const;
	void setBlockType(int r, int c, Shape::Type type);
	void setCurrentState(GameState state);
	
	//外部控制接口
	void start(); //开始游戏
	void pause(); //暂停游戏
	int run(); //运行游戏 (当前方块下移、触底检测、消行、检查游戏是否结束) 返回消除的行数
	void rotate(); //旋转当前形状
	void left(); //当前形状左移
	void right(); //当前形状右移
	void down(); //当前形状下移
	void fall(); //当前形状快速下落
	bool isGameOver(); //判断游戏是否结束
	Shape getCurrentShapePreview(); //获取当前方块预览图
	int updateMap(); //立即更新地图 返回消除的行数

	//调试专用
	void print() const;

private:
	GameState currentState; //当前游戏状态
	int row, col; //地图尺寸
	GameMap gameMap; //游戏地图
	static Shape shapes[7]; //七种形状
	Shape currentShape; //当前形状
	Shape nextShape; //下一个形状
	//int totalLine; //总共消除行数
	int score; //得分
	int level; //等级

	Shape randomShape(); //随机生成一个形状
	bool isReachBottom(); //判断当前形状是否触底
};