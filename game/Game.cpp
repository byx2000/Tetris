#include "Game.h"

#include <iostream>
//#include <cassert>

using namespace std;

GameMap::GameMap()
{
	reset(0, 0);
}

GameMap::GameMap(int row, int col)
{
	reset(row, col);
}

void GameMap::reset(int row, int col)
{
	//assert(row >= 0 && col >= 0 && row <= MaxRow && col <= MaxCol);

	/*map.resize(row);
	for (int i = 0; i < row; ++i)
	{
		map[i].resize(col);
		for (int j = 0; j < col; ++j)
		{
			map[i][j].state = BLANK;
			map[i][j].color = RGB(255, 255, 255);
		}
	}*/

	for (int i = 0; i < row; ++i)
	{
		//map[i].resize(col);
		for (int j = 0; j < col; ++j)
		{
			map[i][j].state = BLANK;
			map[i][j].type = Shape::Empty;
		}
	}

	this->row = row;
	this->col = col;
}

int GameMap::getRow() const
{
	return row;
}

int GameMap::getCol() const
{
	return col;
}

GameMap::BlockState GameMap::getBlockState(int r, int c) const
{
	//assert(r >= 0 && r < row && c >= 0 && c < col);
	return map[r][c].state;
}

void GameMap::setBlockState(int r, int c, BlockState state)
{
	//assert(r >= 0 && r < row && c >= 0 && c < col);
	map[r][c].state = state;
}

/*COLORREF GameMap::getBlockColor(int r, int c) const
{
	assert(r >= 0 && r < row && c >= 0 && c < col);
	return map[r][c].color;
}*/

/*void GameMap::setBlockColor(int r, int c, COLORREF color)
{
	assert(r >= 0 && r < row && c >= 0 && c < col);
	map[r][c].color = color;
}*/

Shape::Type GameMap::getBlockType(int r, int c) const
{
	return map[r][c].type;
}

void GameMap::setBlockType(int r, int c, Shape::Type type)
{
	//assert(r >= 0 && r < row && c >= 0 && c < col);
	map[r][c].type = type;
}

void GameMap::clear()
{
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			map[i][j].state = BLANK;
		}
	}
}

//消行算法 返回消除的行数
int GameMap::updata()
{
	int cLine = 0; //消除的行数
	for (int i = row - 1; i >= 0; --i)
	{
		bool flag = true;
		for (int j = 0; j < col; ++j)
		{
			if (map[i][j].state == BLANK)
			{
				flag = false;
				break;
			}
		}

		if (flag)
		{
			cLine++;

			for (int t = i; t >= 1; --t)
			{
				for (int j = 0; j < col; ++j)
				{
					map[t][j] = map[t - 1][j];
				}
			}

			for (int t = 0; t < col; ++t)
			{
				map[0][t].state = BLANK;
			}

			i++;
		}
	}

	return cLine;
}

void GameMap::print() const
{
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			if (map[i][j].state == BLANK)
			{
				cout << "  ";
			}
			else
			{
				cout << "■";
			}
		}
		cout << endl;
	}
}

Shape::Shape(Type type)
{
	this->type = type;

	switch (type)
	{
	case L:
		setBlocklPos(0, 0, 1);
		setBlocklPos(1, 1, 1);
		setBlocklPos(2, 2, 1);
		setBlocklPos(3, 2, 2);
		//color = RGB(0xef, 0x53, 0x50);
		break;
	case RL:
		setBlocklPos(0, 0, 2);
		setBlocklPos(1, 1, 2);
		setBlocklPos(2, 2, 2);
		setBlocklPos(3, 2, 1);
		//color = RGB(0xef, 0x53, 0x50);
		break;
	case Z:
		setBlocklPos(0, 1, 0);
		setBlocklPos(1, 1, 1);
		setBlocklPos(2, 2, 1);
		setBlocklPos(3, 2, 2);
		//color = RGB(0xff, 0x8a, 0x65);
		break;
	case RZ:
		setBlocklPos(0, 1, 3);
		setBlocklPos(1, 1, 2);
		setBlocklPos(2, 2, 2);
		setBlocklPos(3, 2, 1);
		//color = RGB(0xff, 0x8a, 0x65);
		break;
	case T:
		setBlocklPos(0, 1, 1);
		setBlocklPos(1, 1, 2);
		setBlocklPos(2, 1, 3);
		setBlocklPos(3, 2, 2);
		//color = RGB(77, 182, 172);
		break;
	case Line:
		setBlocklPos(0, 0, 1);
		setBlocklPos(1, 1, 1);
		setBlocklPos(2, 2, 1);
		setBlocklPos(3, 3, 1);
		//color = RGB(118, 204, 222);
		break;
	case Square:
		setBlocklPos(0, 1, 1);
		setBlocklPos(1, 1, 2);
		setBlocklPos(2, 2, 1);
		setBlocklPos(3, 2, 2);
		//color = RGB(253, 216, 53);
		break;
	default:
		//assert(1 < 0);
		break;
	}

	gr = gc = 0;

	/*Pos p[4] =
	{
		{ 1, 1 },
		{ 1, 2 },
		{ 2, 2 },
		{ 3, 2 }
	};*/
}

void Shape::setType(Type type)
{
	this->type = type;
}

Shape::Type Shape::getType() const
{
	return type;
}

/*COLORREF Shape::getColor() const
{
	return color;
}*/

void Shape::setGlobalPos(int gr, int gc)
{
	this->gr = gr;
	this->gc = gc;
}

void Shape::setBlocklPos(int index, int r, int c)
{
	//assert(index >= 0 && index < 4);
	p[index].r = r;
	p[index].c = c;
}

int Shape::getGlobalPosRow() const
{
	return gr;
}

int Shape::getGlobalPosCol() const
{
	return gc;
}

int Shape::getBlockPosRow(int index) const
{
	//assert(index >= 0 && index < 4);
	return p[index].r;
}

int Shape::getBlockPosCol(int index) const
{
	//assert(index >= 0 && index < 4);
	return p[index].c;
}

int Shape::getBlockGlobalPosRow(int index) const
{
	//assert(index >= 0 && index < 4);
	return gr + p[index].r;
}

int Shape::getBlockGlobalPosCol(int index) const
{
	//assert(index >= 0 && index < 4);
	return gc + p[index].c;
}

int Shape::getMinCol() const
{
	int ret = 100;
	for (int i = 0; i < 4; ++i)
	{
		if (p[i].c < ret)
		{
			ret = p[i].c;
		}
	}

	return ret;
}

int Shape::getMaxCol() const
{
	int ret = -1;
	for (int i = 0; i < 4; ++i)
	{
		if (p[i].c > ret)
		{
			ret = p[i].c;
		}
	}

	return ret;
}

int Shape::getMinRow() const
{
	int ret = 100;
	for (int i = 0; i < 4; ++i)
	{
		if (p[i].r < ret)
		{
			ret = p[i].r;
		}
	}

	return ret;
}

int Shape::getMaxRow() const
{
	int ret = -1;
	for (int i = 0; i < 4; ++i)
	{
		if (p[i].r > ret)
		{
			ret = p[i].r;
		}
	}

	return ret;
}

void Shape::rotate()
{
	for (int i = 0; i < 4; ++i)
	{
		int t = p[i].r;
		p[i].r = p[i].c;
		p[i].c = 3 - t;
	}
}

void Shape::left(int step)
{
	gc -= step;
}

void Shape::right(int step)
{
	gc += step;
}

void Shape::up(int step)
{
	gr -= step;
}

void Shape::down(int step)
{
	gr += step;
}

Shape GameController::shapes[7] =
{ Shape(Shape::L), Shape(Shape::RL), Shape(Shape::Z), Shape(Shape::RZ), Shape(Shape::T), Shape(Shape::Line), Shape(Shape::Square) };

GameController::GameController(int row, int col)
	: row(row), col(col), currentShape(Shape::L)
{
	//assert(row >= 5 && col >= 5);

	//初始游戏状态为暂停
	currentState = PAUSE;

	//初始化游戏地图
	gameMap.reset(row, col);

	//随机生成当前形状
	srand((unsigned int)time(NULL));
	//randomCurrentShape();
	currentShape = randomShape();

	//随机生成下一个形状
	nextShape = randomShape();

	//初始消除行数为0
	//totalLine = 0;

	//初始得分为0
	score = 0;

	//初始等级为1
	level = 1;
}

GameController::GameState GameController::getCurrentState() const
{
	return currentState;
}

Shape GameController::getCurrentShape() const
{
	return currentShape;
}

Shape GameController::getNextShape() const
{
	return nextShape;
}

int GameController::getScore() const
{
	return score;
}

int GameController::getLevel() const
{
	return level;
}

/*int GameController::getTotalLine() const
{
	return totalLine;
}*/

void GameController::setScore(int score)
{
	this->score = score;
}

void GameController::setLevel(int level)
{
	this->level = level;
}

int GameController::getRow() const
{
	return row;
}

int GameController::getCol() const
{
	return col;
}

GameMap::BlockState GameController::getBlockState(int r, int c) const
{
	return gameMap.getBlockState(r, c);
}

void GameController::setBlockState(int r, int c, GameMap::BlockState state)
{
	gameMap.setBlockState(r, c, state);
}

/*COLORREF GameController::getBlockColor(int r, int c) const
{
	return gameMap.getBlockColor(r, c);
}*/

/*void GameController::setBlockColor(int r, int c, COLORREF color)
{
	gameMap.setBlockColor(r, c, color);
}*/

Shape::Type GameController::getBlockType(int r, int c) const
{
	return gameMap.getBlockType(r, c);
}

void GameController::setBlockType(int r, int c, Shape::Type type)
{
	gameMap.setBlockType(r, c, type);
}

void GameController::setCurrentState(GameState state)
{
	this->currentState = state;
}

void GameController::start()
{
	currentState = RUNNING;
}

void GameController::pause()
{
	currentState = PAUSE;
}

int GameController::run()
{
	if (currentState != RUNNING)
	{
		return 0;
	}

	int cLine = 0; //消除行数

	//当前方块到达底部
	if (isReachBottom())
	{
		//更新游戏地图
		for (int i = 0; i < 4; ++i)
		{
			int r = currentShape.getBlockGlobalPosRow(i);
			int c = currentShape.getBlockGlobalPosCol(i);
			gameMap.setBlockState(r, c, GameMap::OCCUPY);
			gameMap.setBlockType(r, c, currentShape.getType());
		}

		//消行
		cLine = gameMap.updata();

		//更新总消除行数
		//totalLine += cLine;

		//更新得分
		int appendScore[] = { 0, 10, 30, 60, 100 };
		score += appendScore[cLine];

		//更新等级
		if (score / 200 >= level)
		{
			level++;
		}

		//更新当前形状
		currentShape = nextShape;

		//生成下一个形状
		nextShape = randomShape();
	}
	//当前方块未到达底部
	else
	{
		//当前形状下移
		down();
	}

	//游戏结束
	if (isGameOver())
	{
		currentState = GAMEOVER;
	}

	//返回本次消除行数
	return cLine;
}

void GameController::rotate()
{
	if (currentState != RUNNING)
	{
		return;
	}

	if (isReachBottom())
	{
		return;
	}

	Shape backup = currentShape;

	//旋转当前形状
	currentShape.rotate();

	//防止旋转后的形状超出边界
	int minRow = currentShape.getMinRow();
	if (currentShape.getGlobalPosRow() + minRow < 0)
	{
		currentShape.down(-(currentShape.getGlobalPosRow() + minRow));
	}

	int minCol = currentShape.getMinCol();
	if (currentShape.getGlobalPosCol() + minCol < 0)
	{
		currentShape.right(-(currentShape.getGlobalPosCol() + minCol));
	}

	int maxCol = currentShape.getMaxCol();
	if (currentShape.getGlobalPosCol() + maxCol > col - 1)
	{
		currentShape.left(-(col - 1 - currentShape.getGlobalPosCol() - maxCol));
	}

	for (int i = 0; i < 4; ++i)
	{
		if (gameMap.getBlockState(currentShape.getBlockGlobalPosRow(i), currentShape.getBlockGlobalPosCol(i)) == GameMap::OCCUPY)
		{
			currentShape = backup;
			return;
		}
	}
}

void GameController::left()
{
	if (currentState != RUNNING)
	{
		return;
	}

	//底部滑动
	int gMinCol = currentShape.getGlobalPosCol() + currentShape.getMinCol();
	if (gMinCol <= 0)
	{
		return;
	}

	for (int i = 0; i < 4; ++i)
	{
		int cRow = currentShape.getBlockGlobalPosRow(i);
		int cCol = currentShape.getBlockGlobalPosCol(i);
		if (gameMap.getBlockState(cRow, cCol - 1) == GameMap::OCCUPY)
		{
			return;
		}
	}

	currentShape.left(1);
}

void GameController::right()
{
	if (currentState != RUNNING)
	{
		return;
	}

	//底部滑动
	int gMaxCol = currentShape.getGlobalPosCol() + currentShape.getMaxCol();
	if (gMaxCol >= col - 1)
	{
		return;
	}

	for (int i = 0; i < 4; ++i)
	{
		int cRow = currentShape.getBlockGlobalPosRow(i);
		int cCol = currentShape.getBlockGlobalPosCol(i);
		if (gameMap.getBlockState(cRow, cCol + 1) == GameMap::OCCUPY)
		{
			return;
		}
	}

	currentShape.right(1);
}

void GameController::down()
{
	if (currentState != RUNNING)
	{
		return;
	}

	if (!isReachBottom())
	{
		currentShape.down(1);
	}
}

void GameController::fall()
{
	if (currentState != RUNNING)
	{
		return;
	}

	while (!isReachBottom())
	{
		currentShape.down(1);
	}
}

bool GameController::isGameOver()
{
	for (int i = 0; i < col; ++i)
	{
		if (gameMap.getBlockState(0, i) == GameMap::OCCUPY)
		{
			return true;
		}
	}
	return false;
}

Shape GameController::getCurrentShapePreview()
{
	Shape restore = currentShape;
	while (!isReachBottom())
	{
		currentShape.down(1);
	}
	Shape res = currentShape;
	currentShape = restore;
	return res;
}

int GameController::updateMap()
{
	for (int i = 0; i < 4; ++i)
	{
		int r = currentShape.getBlockGlobalPosRow(i);
		int c = currentShape.getBlockGlobalPosCol(i);
		gameMap.setBlockState(r, c, GameMap::OCCUPY);
		gameMap.setBlockType(r, c, currentShape.getType());
	}

	int cLine = gameMap.updata();

	//更新得分
	int appendScore[] = { 0, 10, 30, 60, 100 };
	score += appendScore[cLine];

	//更新等级
	if (score / 150 >= level)
	{
		level++;
	}

	return cLine;
}

//调试专用
void GameController::print() const
{
	vector<vector<int>> v(row);
	for (int i = 0; i < row; ++i)
	{
		v[i].resize(col);
		for (int j = 0; j < col; ++j)
		{
			v[i][j] = 0;
		}
	}

	for (int i = 0; i < 4; ++i)
	{
		v[currentShape.getBlockGlobalPosRow(i)][currentShape.getBlockGlobalPosCol(i)] = 1;
	}

	for (int j = 0; j < col + 2; ++j)
	{
		cout << "■";
	}
	cout << endl;

	for (int i = 0; i < row; ++i)
	{
		cout << "■";
		for (int j = 0; j < col; ++j)
		{
			if (gameMap.getBlockState(i, j) == GameMap::OCCUPY || v[i][j])
			{
				cout << "□";
			}
			else
			{
				cout << "  ";
			}
		}
		cout << "■" << endl;
	}

	for (int j = 0; j < col + 2; ++j)
	{
		cout << "■";
	}
	cout << endl;
}

Shape GameController::randomShape()
{
	//随机选择形状
	Shape shape = shapes[rand() % 7];

	//随机旋转
	int t = rand() % 3;
	for (int i = 0; i < t; ++i)
	{
		shape.rotate();
	}

	//随机指定位置
	shape.setGlobalPos(0, rand() % (col - 4));

	//调整位置
	int minRow = shape.getMinRow();
	shape.up(minRow);

	return shape;
}

bool GameController::isReachBottom()
{
	for (int i = 0; i < 4; ++i)
	{
		int cRow = currentShape.getBlockGlobalPosRow(i);
		int cCol = currentShape.getBlockGlobalPosCol(i);
		if (cRow == row - 1)
		{
			return true;
		}
		if (gameMap.getBlockState(cRow, cCol) == GameMap::OCCUPY)
		{
			return true;
		}
		if (gameMap.getBlockState(cRow + 1, cCol) == GameMap::OCCUPY)
		{
			return true;
		}
	}

	return false;
}
