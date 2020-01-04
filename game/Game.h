#pragma once

#include <vector>
#include <ctime>
#include <Windows.h>

/*
	�洢��״�Ļ������ݣ�����ÿ���������������ȫ�����꣬ʵ������ת���ƶ��㷨
*/
class Shape
{
	friend class GameController;
public:
	//������״
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

	//���캯��
	Shape() {}
	Shape(Type type);

	//��Ա����
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
	int getMinCol() const; //��ȡ������С������
	int getMaxCol() const; //��ȡ�������������
	int getMinRow() const; //��ȡ������С������
	int getMaxRow() const; //��ȡ�������������

	void rotate(); //������ת90��
	void left(int step); //����
	void right(int step); //����
	void up(int step); //����
	void down(int step); //����

private:
	struct Pos { int r, c; };

	Type type; //����
	int gr, gc; //���Ͻ�ȫ������
	Pos p[4]; //�ĸ�������������Ͻǵ�����
	//COLORREF color; //��״��ɫ
};

/*
	�洢��Ϸ��ͼ���ݣ�����ÿ�������״̬����ɫ��ʵ���������㷨
*/
class GameMap
{
	friend class GameController;
public:
	//��������
	enum BlockState
	{
		BLANK,  //�հ�
		OCCUPY  //ռ��
	};

	//���ߴ�
	enum { MaxRow = 60, MaxCol = 60 };

	//���캯��
	GameMap();
	GameMap(int row, int col);

	//��Ա����
	void reset(int row, int col);
	int getRow() const;
	int getCol() const;
	BlockState getBlockState(int r, int c) const;
	void setBlockState(int r, int c, BlockState state);
	//COLORREF getBlockColor(int r, int c) const;
	//void setBlockColor(int r, int c, COLORREF color);
	Shape::Type getBlockType(int r, int c) const;
	void setBlockType(int r, int c, Shape::Type type);
	void clear(); //��յ�ͼ
	int updata(); //�����㷨 ��������������
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
	������Ϸ��һ����Դ����Ϸ��ͼ���ݡ���ǰ��״����һ����״���÷�
*/
class GameController
{
public:
	//��Ϸ״̬
	enum GameState
	{
		PAUSE,
		RUNNING,
		GAMEOVER
	};

	//���캯��
	GameController(int row, int col);

	//��Ա����
	GameState getCurrentState() const;
	Shape getCurrentShape() const;
	Shape getNextShape() const;
	//int getTotalLine() const; //��ȡ����������
	int getScore() const; //��ȡ�÷�
	int getLevel() const; //��ȡ�ȼ�
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
	
	//�ⲿ���ƽӿ�
	void start(); //��ʼ��Ϸ
	void pause(); //��ͣ��Ϸ
	int run(); //������Ϸ (��ǰ�������ơ����׼�⡢���С������Ϸ�Ƿ����) ��������������
	void rotate(); //��ת��ǰ��״
	void left(); //��ǰ��״����
	void right(); //��ǰ��״����
	void down(); //��ǰ��״����
	void fall(); //��ǰ��״��������
	bool isGameOver(); //�ж���Ϸ�Ƿ����
	Shape getCurrentShapePreview(); //��ȡ��ǰ����Ԥ��ͼ
	int updateMap(); //�������µ�ͼ ��������������

	//����ר��
	void print() const;

private:
	GameState currentState; //��ǰ��Ϸ״̬
	int row, col; //��ͼ�ߴ�
	GameMap gameMap; //��Ϸ��ͼ
	static Shape shapes[7]; //������״
	Shape currentShape; //��ǰ��״
	Shape nextShape; //��һ����״
	//int totalLine; //�ܹ���������
	int score; //�÷�
	int level; //�ȼ�

	Shape randomShape(); //�������һ����״
	bool isReachBottom(); //�жϵ�ǰ��״�Ƿ񴥵�
};