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

static int target[3][3]={1,2,3,8,0,4,7,6,5};      //Ŀ��1 2 3 8 0 4 7 6 5
int Target[9][2]={1,1,0,0,0,1,0,2,1,2,2,2,2,1,2,0,1,0}; //��¼ÿ����Ŷ�Ӧ��x��y��ֵ��

//static int target[3][3]={1,2,3,4,5,6,7,8,0};      //Ŀ�� 1 2 3 4 5 6 7 8 0
//int Target[9][2]={2,2,0,0,0,1,0,2,1,0,1,1,1,2,2,0,2,1};

int hashCode(int p[3][3]);            //ͨ����ϣɢ��ȷ��ĳ��״̬�Ƿ���ʹ�,���ʹ��ı����hashList��

class Node       //�ڵ���
{
public:
    int data[3][3];
	Node* parent;

    bool used;      //hashList�����ã����״̬�Ƿ���ʹ�
	int fvalue;     //f��g��hֵ
	int gvalue;     
	int hvalue;
	int index;      //��¼��hashList�е�����
public:
    void computeValue();         //��������ֵf,g,h
	Node(){ parent=NULL;used=false;};         //Ĭ�Ϲ��캯��
	Node(int d[3][3],Node* p=NULL) { parent=p;used=false;setData(d);}
	void hash(){index=hashCode(data);}         //ɢ�м���Index
	void generate(list<Node>* spring);        //��չ����ڵ�
	void move(int tdata[3][3],int row,int col,list<Node>* spring,int dirx,int diry);           
	void setData(int d[3][3]) ;	     //����״̬
	void change(Node node);          //��������ֵ
    bool operator==(Node& node);
	Node& operator=(Node& node);
    void output();
};

Node hashList[HASH_SIZE];        //����ÿ��״̬�Ƿ񱻷��ʹ���hashList������Close�������˲��Һ�ʱ��

bool eq(int a[3][3],int b[3][3] ){
   for(int i=0;i<3;i++)
   for(int j=0;j<3;j++) if( a[i][j]!=b[i][j] ) return false;
   return true;
}

int hashCode(int p[3][3])       //��ϣ������������̽�����ͻ
{
	int ret =( p[0][0]*7 + p[0][1]*17 + p[0][2]*47 + p[1][0]*117 + p[1][1] * 217 + p[1][2] 
               * 977 + p[2][0]*1299 + p[2][1]*5971 + p[2][2]*7779)% HASH_SIZE;
	while( hashList[ret].used && !eq(hashList[ret].data,p))
        ret= (ret+1) % HASH_SIZE;
    return ret;
}


class Heap             //С���ѣ�����ʵ��Open��
{
public:
	vector<Node> heap;       //�ڲ�������ʵ��
	int size(){ return heap.size();}
	bool empty(){ return heap.empty();}
	void insert(Node node);            //������ĩ��֮������
	inline Node& getFirst(){return heap[0];}
	inline void delFirst();           //ɾ����һ���ڵ㣬֮������
	void output(); 
	inline int find(Node node);   
	inline void change(int place,Node newNode);          //�ı�place���Ľڵ�����ֵ��֮������
};


#include "heap_implementation.h"
#endif