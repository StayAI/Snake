#include "snake.h"
#include <QDebug>
#include <QTime>
#include <QQueue>
#include <QVector>
#include <qmath.h>

QVector <Node> minPath;//findPath 存储最短路径
QVector <Node> maxPath;//findPath 存储最长路径

QVector <Node> qpath;//存储的是蛇要走的路径节点 BFS

int maps[12][16] =
{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

Snake::Snake(QWidget *parent) : QWidget(parent)
{
    widthOfWindow = 640;
    heightOfWindow = 480;
    size = 40;

    now.setX(size);//蛇头初始坐标X
    now.setY(size);//蛇头初始坐标Y
    former.setX(size);//蛇头之前坐标X
    former.setY(size);//蛇头之前坐标Y
    length = 1;//蛇头的长度初始为1
    pNext = 0;//蛇头下一个指向为空
    pTail = this;

    eatFoodFlag = false;
    Direction = right_;				//蛇的初始运动方向向右
    srandPoint = drawSrandPoint(this);		//画出一个任意点，也就是食物
}


void Snake::Create_Snake(Snake* pHead)		//创建一个点
{
    Snake* p = pHead;			//声明一个变量，此变量用来存放的是最后一个节点数据，此变量指向q
    Snake* q = pHead;	//声明一个变量，此变量指向pHead
    Snake* pAdd = new Snake;//声明一个变量，为变量分配内存，这是增加的一个变量

    while(q != 0)	//q = pHead; q不为空，
    {
        p = q;			//p = q;
        q = q->pNext;	//q指向q的下一个   这段程序主要是为了找到最后一个节点数据，这个数据就是p；
    }					//如果不多声明一个变量p；等q指向空的时候就不知道最后一个节点是哪个了
    if(0 == pAdd)	//内存分配失败
    {
        qDebug() << "Create_Snake中内存分配失败;";
        return ;
    }
    else
    {
        pAdd->Direction = p->Direction;  //先将他的前一个节点的方向赋值到自己的方向变量中。
        pHead->length++;				 //pHead->lengeh主要用于存放节点的长度，也就是蛇的长度
        switch(pAdd->Direction)			 //判断他的方向，然后进行
        {
            case left_ :					 //如果他之前的节点向左走，那么他的方向也向左，则他的坐标为
            {							 //他之前节点的有一个，所以x+1;y不变
                pAdd->now.setX( p->now.x() + size );
                pAdd->now.setY( p->now.y() );
            }break;
            case right_ :					 //同上
            {
                pAdd->now.setX( p->now.x() - size );
                pAdd->now.setY( p->now.y() );
            }break;
            case up_ :					//同上
            {
                pAdd->now.setY( p->now.y() + size );
                pAdd->now.setX( p->now.x() );
            }break;
            case down_ : 				//同上
            {
                pAdd->now.setY( p->now.y() - size );
                pAdd->now.setX( p->now.x() );
            }break;

        }
        pAdd->former.setX( p->now.x() );		//创建新节点之后，他自己节点之前的数据为本身
        pAdd->former.setY( p->now.y() );
        pTail = pAdd;
        p->pNext = pAdd;				//将新创建的节点的上一个节点的结构体指针变量pNext指向当前新创建的节点
        p->pNext->pNext = 0;//BIGBIGBIGBIGBIGBIGBIG  BUGBUGBUGBUGBUGBUGBUGBUG
    }
}


void Snake::moveOneStep(Snake *pHead, Node temp)
{
    if (pHead->now.y()/size < temp.x)
    {
        pHead->Direction = down_;
    }
    if (pHead->now.y()/size > temp.x)
    {
        pHead->Direction = up_;
    }
    if (pHead->now.x()/size < temp.y)
    {
        pHead->Direction = right_;
    }
    if (pHead->now.x()/size > temp.y)
    {
        pHead->Direction = left_;
    }
}

int findFoodAndTail = 0;

void Snake::Snake_Move(Snake* pHead)				//蛇的移动
{
    Node start;
    Node stop;
    start.x = pHead->now.y() / size;
    start.y = pHead->now.x() / size;
    start.num = maps[start.x][start.y];
    stop.x = srandPoint.y() / size;
    stop.y = srandPoint.x() / size;
    stop.num = maps[stop.x][stop.y];


    if (pHead->length <=30)
    {
        BFS(start, stop, maps);
        moveOneStep(pHead, qpath[0]);
    }else
    {
        Node head;
        Node tail;
        head.x = pHead->now.y() / size;
        head.y = pHead->now.x() / size;
        head.num = maps[head.x][head.y];
        tail.x = pHead->pTail->now.y() / size;
        tail.y = pHead->pTail->now.x() / size;
        tail.num = maps[tail.x][tail.y];

//        if ( BFS(head, tail, maps) )
//        {

//        }

    }

    Snake* q;								//声明一个变量
    int dir,dir1;							//声明一个变量

    Judge_Direction(pHead);						//判断头方向，并移动
    maps[pHead->now.y()/size][pHead->now.x()/size] = -1;
    q = pHead->pNext;							//q指向p头结点的下一个节点
    dir = pHead->Direction;						//将头结点的方向记录下来，
    Snake * eatFood = new Snake;
    eatFood->Direction = pHead->pTail->Direction;
    eatFood->now = pHead->pTail->now;
    eatFood->former = pHead->pTail->former;
    eatFood->pNext = 0;
    eatFood->pTail = 0;
    while(q != 0)                           //q不为空
    {
        dir1 = dir;							//他的前一个节点的方向
        Judge_Direction(q);					//然后判断方向
        maps[q->now.y()/size][q->now.x()/size] = 1;
        maps[q->former.y()/size][q->former.x()/size] = 0;
        dir = q->Direction;					//dir把q节点的方向记录下来，便于他的后一个节点按着他的这个方向移动
        q->Direction = dir1;				//q节点的方向改为他的前一个的节点的方向
        q = q->pNext;						//执行完之后，将p指向他的下一个
    }

    if (eatFoodFlag == true)
    {
        pHead->length++;
        eatFoodFlag = false;
        maps[pHead->pTail->now.y()/size][pHead->pTail->now.x()/size] = 1;
        maps[pHead->pTail->former.y()/size][pHead->pTail->former.x()/size] = 0;
        pHead->pTail->pNext = eatFood;
        pHead->pTail = eatFood;
    }
    maps[pHead->pTail->now.y()/size][pHead->pTail->now.x()/size] = -2;
    maps[pHead->pTail->former.y()/size][pHead->pTail->former.x()/size] = 0;

    qDebug() << "finashedMove!!";
    qDebug() << pHead->length;
}

int Snake::random(int temp)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    return qrand() % temp;
}

QPoint Snake::drawSrandPoint(Snake* pHead)			//画任意点
{
    //方法3生成随机点
    int x = random(140-pHead->length) + 1;
    for (int i=1; i<=10; i++)
    {
        for (int j=1; j<=14; j++)
        {
            if (maps[i][j]==0)
            {
                x--;
                if (x == 0)
                {
                    srandPoint.setX(j * size);
                    srandPoint.setY(i * size);
                    return srandPoint;//返回任意点结构变量
                }
            }
        }
    }

    return srandPoint;
}

Snake* Snake::Judge_Direction(Snake* pHead)					//判断蛇运动的方向
{
    pHead->former.setX( pHead->now.x() );						//蛇移动之前的坐标X
    pHead->former.setY( pHead->now.y() );						//蛇移动之后的坐标Y

    switch(pHead->Direction)							//判断蛇运动的方向
    {
        case left_: 										//向左走，x坐标减一
            pHead->now.setX( pHead->now.x() - size);
            break;
        case right_:										//向右走，x坐标加一
            pHead->now.setX( pHead->now.x() + size );
            break;
        case up_:
            pHead->now.setY( pHead->now.y() - size );				//向上走，y坐标减一
            break;
        case down_:
            pHead->now.setY( pHead->now.y() + size );				//向下走，y坐标加一
            break;
    }
    return pHead;
}

void Snake::Judge_Wall(Snake* pHead)					//判断蛇运动是否撞到边界
{
    if( pHead->now.x()==(0) || pHead->now.x()==widthOfWindow-size ||
        pHead->now.y()==(0) || pHead->now.y()==heightOfWindow-size  )
        Game_Over();
}

void Snake::Judge_itself(Snake* pHead)					//判断蛇运动是否撞到自己
{
    Snake* p;									//声明一个新的结构体变量

    p = pHead->pNext;							//p指向下一个节点
    while(p != 0)							//p不为空
    {
        if( pHead->now.x()==p->now.x() &&
            pHead->now.y()==p->now.y() )	//不为空就比较蛇头的位置是否和身体的位置相等
        {
            Game_Over();						//相等  游戏结束
        }
        p = p->pNext;							//不相等就指向p的下一个
    }
}

void Snake::Eat_Point(Snake* pHead)					//吃食物
{
    if(	pHead->now.x()==srandPoint.x() &&
        pHead->now.y()==srandPoint.y())	//如果任意点的位置和头的位置相等
    {
//        Snake_Move(pHead);						//然后蛇向前移动一步
//        Create_Snake(pHead);					//先创建一个节点
        srandPoint = drawSrandPoint(pHead);		//生成一个任意点
        findFoodAndTail = 0;

        eatFoodFlag = true;
    }
}


void Snake::Game_Over(void)									//游戏结束
{
    qDebug() << "Game Over;";
    while(1);
}


bool Snake::isValid(Node Vw)
{
    if (Vw.x>=0 && Vw.x<12 && Vw.y>=0 && Vw.y<16
        && (Vw.num==0 || Vw.num==-1 || Vw.num==-2 || Vw.num==3) )//3只是测试
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * 广度优先搜索
 * @param Vs 起点
 * @param Vd 终点
 */
bool Snake::BFS(Node& Vs, Node& Vd, int maps[12][16])
{
    QQueue <Node> Q;
    Node Vn, Vw;
    int i;

    //用于标记颜色当visit[i][j]==true时，说明节点访问过，也就是黑色
    bool visit[12][16] = {0};//Qt中所有变量必须初始化，否则一定会出错

    //四个方向
    int dir[][2] =
    {
        {0, 1}, {1, 0},
        {0, -1}, {-1, 0}
    };

    //初始状态将起点放进队列Q
    qpath.clear();

    Q.push_front(Vs);
    visit[Vs.x][Vs.y] = true;//设置节点已经访问过了！

    while (!Q.empty())  //队列不为空，继续搜索！
    {
        //取出队列的头Vn
        Vn = Q.front();


        for (i=0; i<4; i++)
        {
//            Vw = Node(Vn.x+dir[i][0], Vn.y+dir[i][1]);//计算相邻节点
            Vw.x = Vn.x+dir[i][0];
            Vw.y = Vn.y+dir[i][1];
            Vw.num = maps[Vw.x][Vw.y];
            Vw.prex = Vn.x;
            Vw.prey = Vn.y;

            if (isValid(Vw) && (!visit[Vw.x][Vw.y]) )
            {
                qpath.push_back(Vw);
                //Vw是一个合法的节点并且为白色节点
                Q.push_back(Vw);//加入队列Q
                visit[Vw.x][Vw.y] = true;//设置节点颜色
            }

            if (isValid(Vw) && (Vw == Vd) )   //找到终点了！
            {
                //把路径记录，这里没给出解法
                Node qtemp = qpath[qpath.size()-1];
                for (int i=qpath.size()-2; i>=0; i--)
                {
                    if (qtemp.prex==qpath[i].x && qtemp.prey==qpath[i].y)
                    {
                        qtemp = qpath[i];
                    }
                    else
                    {
                        qpath.remove(i);
                    }
                }

                return true;//返回
            }
        }

        Q.pop_front();
    }

    return false;//无解
}



bool Snake::findPath(Node& Vs, Node& Vd) //路径为:(1,1)->(M,N)
{
    QVector <Node> Q;
    int minlen = 140;     //最短路径长度
    int maxlen = 0;

    static int i = 0;

    Node Vn, Vw;
    int nextfound = 0;  //Vn
    int index = 0;     //栈索引值，初始值为0

    int maptemp[12][16];
    for (int i=0; i<12; i++)
    {
        for (int j=0; j<16; j++)
        {
            maptemp[i][j] = maps[i][j];
        }
    }

    maxPath.clear();
    minPath.clear();

    Q.push_back(Vs);
//    Vs.num = 0;//   isValid(Node )中判断是否满足需要用到这句
    maptemp[Vs.x][Vs.y] = -11;	//-1表示该节点位置进栈过(最好用其他数组来标示)

    while( !Q.isEmpty() ) //栈不空时循环
    {
        i++;

        qDebug() << i;

        //1 -----获取索引值栈数据
        Vn = Q[index];
        Vw = Vn;

        //2 -----如果找到了出口（M,N），则输出查找路径，并退栈，用新的栈顶方向值取代当前的查找方向
        if (Vw.x==Vd.x && Vw.y==Vd.y /*&& isValid(Vw)*/ )
        {
            if(Q.size() < minlen) //比较输出最短路径
            {
                minPath.clear();

                for(int i=1; i<Q.size(); i++)
                {
                    minPath.push_back(Q[i]);//保存最短路径
                }
                minlen = Q.size();
            }

            if(Q.size() > maxlen) //比较输出最长路径
            {
                maxPath.clear();

                for(int i=1; i<Q.size(); i++)
                {
                    maxPath.push_back(Q[i]);//保存最长路径
                }
                maxlen = Q.size();
            }

            maptemp[Vn.x][Vn.y] = 0;   //0表示让该位置变为其他路径的可走结点

            //让元素出栈
            index--;
            Q.pop_back();
            Vw.di = Q[index].di;//记录下一次的查找方向为新索引值栈数据的查找方向

//            if (maxlen > minlen)
//                return true;

            if(!minPath.isEmpty() && !maxPath.isEmpty() && minlen>2)
                return true;
            else
                return false;
        }

        //3 -------在当前栈顶的基础上找到下一个可走节点，如果当前栈顶的4个方向都走完了，则退栈找新的栈顶
        nextfound = 0;
        while(Vw.di<4 && nextfound==0) //找下一个可走结点
        {
            Vw.di++;   		//下一个查找方向
            switch(Vw.di) 	//计算下一个查找方向的坐标
            {
                case 0: Vw.x = Vn.x-1;   Vw.y = Vn.y;     break;   //上面
                case 1: Vw.x = Vn.x;     Vw.y = Vn.y+1;   break;   //右边
                case 2: Vw.x = Vn.x+1;   Vw.y = Vn.y;     break;   //下面
                case 3: Vw.x = Vn.x;     Vw.y = Vn.y-1;   break;   //左边
            }
            if (maptemp[Vw.x][Vw.y]==0 || maptemp[Vw.x][Vw.y]==-1
                || maptemp[Vw.x][Vw.y]==-2 || maptemp[Vw.x][Vw.y]==3 )  //如果下一个可走节点没有进栈过，标记find为1表示找到了下一个可走节点
            {
                nextfound=1;
            }
        }

        if(nextfound == 1) //从当前栈顶上找到了下一个可走结点
        {
            Q[index].di = Vw.di;
            index++;
            //下一个可走结点进栈
            Node temp;
            temp.x = Vw.x;
            temp.y = Vw.y;
            temp.di = -1;

            //先不加这句，问题太多
            //            temp.num = 0;//   isValid(Node )中判断是否满足，需要用到判断.num这句
            Q.push_back(temp);
            maptemp[temp.x][temp.y] = -11;
        }
        else  //如果当前栈顶的4个方向都已经查找完
        {
//            maptemp[Q[index].x][Q[index].y] = 0;   //0表示让该位置变为其他路径的可走结点
            index--;
            Q.pop_back();			//当前栈顶已经查找完，让它出栈
        }
    }
}







