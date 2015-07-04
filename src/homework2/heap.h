#ifndef HEAD_H
#define HEAD_H

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <list>

using namespace std;

static ofstream fout;

#define Parent(i) ((i-1)>>1)
#define LChild(i) (1+((i)<<1))
#define RChild(i) ((1+(i))<<1)
#define HASH_SIZE 39999

static int target[3][3]={1,2,3,8,0,4,7,6,5};      //目标1 2 3 8 0 4 7 6 5
int Target[9][2]={1,1,0,0,0,1,0,2,1,2,2,2,2,1,2,0,1,0}; //记录每个标号对应的x和y的值。

//static int target[3][3]={1,2,3,4,5,6,7,8,0};      //目标 1 2 3 4 5 6 7 8 0
//int Target[9][2]={2,2,0,0,0,1,0,2,1,0,1,1,1,2,2,0,2,1};

int hashCode(int p[3][3]);            //通过哈希散列确定某个状态是否访问过,访问过的标记于hashList中

class Node       //节点类
{
public:
    int data[3][3];
	Node* parent;

    bool used;      //hashList中有用，标记状态是否访问过
	int fvalue;     //f、g、h值
	int gvalue;     
	int hvalue;
	int index;      //记录在hashList中的索引
public:
    void computeValue();         //计算启发值f,g,h
	Node(){ parent=NULL;used=false;};         //默认构造函数
	Node(int d[3][3],Node* p=NULL) { parent=p;used=false;setData(d);}
	void hash(){index=hashCode(data);}         //散列计算Index
	void generate(list<Node>* spring);        //扩展后代节点
	void move(int tdata[3][3],int row,int col,list<Node>* spring,int dirx,int diry);           
	void setData(int d[3][3]) ;	     //设置状态
	void change(Node node);          //更改启发值
    bool operator==(Node& node);
	Node& operator=(Node& node);
    void output();
};

Node hashList[HASH_SIZE];        //储存每个状态是否被访问过，hashList代替了Close表，减少了查找耗时。

bool eq(int a[3][3],int b[3][3] ){
   for(int i=0;i<3;i++)
   for(int j=0;j<3;j++) if( a[i][j]!=b[i][j] ) return false;
   return true;
}

int hashCode(int p[3][3])       //哈希函数，线性试探解决冲突
{
	int ret =( p[0][0]*7 + p[0][1]*17 + p[0][2]*47 + p[1][0]*117 + p[1][1] * 217 + p[1][2] 
               * 977 + p[2][0]*1299 + p[2][1]*5971 + p[2][2]*7779)% HASH_SIZE;
	while( hashList[ret].used && !eq(hashList[ret].data,p))
        ret= (ret+1) % HASH_SIZE;
    return ret;
}


class Heap             //小根堆，用于实现Open表
{
public:
	vector<Node> heap;       //内部用向量实现
	int size(){ return heap.size();}
	bool empty(){ return heap.empty();}
	void insert(Node node);            //插入最末，之后上滤
	inline Node& getFirst(){return heap[0];}
	inline void delFirst();           //删除第一个节点，之后下滤
	void output(); 
	inline int find(Node node);   
	inline void change(int place,Node newNode);          //改变place处的节点启发值，之后上滤
};


#include "heap_implementation.h"
#endif