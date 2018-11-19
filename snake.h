#ifndef SNAKE_H
#define SNAKE_H

#include <QWidget>

enum Direction             		//蛇移动的方向
{
    left_, right_, up_, down_       //分别为0, 1, 2, 3
};

extern int maps[12][16];

class Node
{
public:
    Node()
    {
        this->x = 0;
        this->y = 0;
        num = maps[this->x][this->y];
        this->di = -1;	//该点的初始方向值-1，表示还没查找过从此出发的其他4个方向
    }
    Node(int x, int y)
    {
        this->x = x;
        this->y = y;
        this->num = maps[this->x][this->y];
        this->di = -1;
    }

    void setX(int x)
    {
        this->x = x;
    }
    void setY(int y)
    {
        this->y = y;
    }
    void setNum(int num)
    {
        this->num = num;
    }

    bool operator==(Node &secondNode)
    {
        if (this->x == secondNode.x && this->y == secondNode.y)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

//private:
    int x;
    int y;
    int di;
    int prex;	//前一节点坐标
    int prey;	//前一节点坐标
    int dis;	//记录是第几层访问的节点
    int num;	//当前坐标的数字
};




class Snake : public QWidget
{
    Q_OBJECT
public:
    explicit Snake(QWidget *parent = 0);

    int random(int temp);
    void Create_Snake(Snake* pHead);		//创建一个点
    void Snake_Move(Snake* pHead);				//蛇的移动
    void Eat_Point(Snake* pHead);
    QPoint drawSrandPoint(Snake* pHead);
    Snake* Judge_Direction(Snake* pHead);					//判断蛇运动的方向
    void Judge_Wall(Snake* pHead);					//判断蛇运动的方向
    void Judge_itself(Snake* pHead);					//判断蛇运动的方向
    void Game_Over(void);

    bool moveSnakeVirtual(Snake* pHead);
//    bool moveSnakeVirtual2(Snake* pHead);
    bool BFS(Node& Vs, Node& Vd, int maps[12][16]);
    bool findPath(Node& Vs, Node& Vd);
    bool isValid(Node Vw);
    Node getACellThatIsFarthestToGoal(Snake* pHead);
    void moveOneStep(Snake *pHead, Node temp);

    int getLength()
    {
        return length;
    }
    QPoint getSrandPoint()
    {
        return srandPoint;
    }
    QPoint getNow()
    {
        return now;
    }
    QPoint getFormer()
    {
        return former;
    }

    Snake* getpNext()
    {
        return pNext;
    }
    Snake* getpTail()
    {
        return pTail;
    }
    int getDirection()
    {
        return Direction;
    }
    void setDirection(int direction)
    {
        Direction = direction;
    }
    int getSize()
    {
        return size;
    } 

signals:

public slots:

private:

    int widthOfWindow;
    int heightOfWindow;
    int size;

    bool eatFoodFlag;

    Snake* pNext;		//指向节点的下一个节点
    Snake* pTail;		//指向尾节点
    QPoint now;				//蛇移动的当前坐标
    QPoint former;			//蛇移动的下一个坐标
    int length;					//蛇神的长度
    int Direction;				//蛇运动的方向
    QPoint srandPoint;  //任意点  也就是食物的位置
};

#endif // SNAKE_H
