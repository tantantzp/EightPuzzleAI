#ifndef HEAP_IMPLEMENTATION_H
#define HEAP_IMPLEMENTATION_H

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include "heap.h"

using namespace std;

#define Parent(i) ((i-1)>>1)
#define LChild(i) (1+((i)<<1))
#define RChild(i) ((1+(i))<<1)

void Node::setData(int d[3][3]) 
{
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			data[i][j]=d[i][j];
	computeValue();
}
void Node::computeValue()
{
	if(parent==NULL)
		gvalue=0;
	else
		gvalue=parent->gvalue+1;

	hvalue=0;
	for(int i=0;i<3;i++)                 //启发函数,以每个位置到目标位置的曼哈顿距离作为hvalue.
		for(int j=0;j<3;j++)
		{
		    //if(data[i][j]!=target[i][j]) hvalue++;
			int dx,dy;
			dx=Target[data[i][j]][0]-i;
			if(dx<0) dx=-dx;
			dy=Target[data[i][j]][1]-j;
			if(dy<0) dy=-dy;
			hvalue+=dx+dy;
			
		}
	fvalue=gvalue+hvalue;
}
void Node::output()
{
	/*cout<<endl;
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++)
			cout<<data[i][j]<<' ';
		cout<<endl;
	}*/
	fout<<endl;
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++)
			fout<<data[i][j]<<' ';
		fout<<endl;
	}
}
bool Node::operator==(Node& node)
{
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			if(this->data[i][j]!=node.data[i][j])
			{
				return false;
			}
	return true;
}
Node& Node::operator=(Node& node)
{
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
		{
			data[i][j]=node.data[i][j];
		}
	parent=node.parent;
	fvalue=node.fvalue;
	gvalue=node.gvalue;
	hvalue=node.hvalue;
	index=node.index;
	used=node.used;
	return *this;
}

void Node::change(Node node)
{
	parent=node.parent;
	gvalue=node.gvalue;
	hvalue=node.hvalue;
	fvalue=node.fvalue;
}



void Node::generate(list<Node>* spring)    //产生后代节点，未考虑与祖先重合（将在主程序中通过哈希函数检查）
{
	int row=0; int col=0;
	for(int i=0;i<3;i++)     //查找0位置
		for(int j=0;j<3;j++)
			if(data[i][j]==0)
			{
				row = i;
				col = j;
				break;
			}
	int tdata[3][3];
	int dir[4][2]={0,1,0,-1,-1,0,1,0};     //定义向四个方向移动的x,y变化值
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++) tdata[i][j]=this->data[i][j];
    if(col!=2)                   //向右
		move(tdata,row,col,spring,dir[0][0],dir[0][1]);
	if(col!=0)                   //向左
        move(tdata,row,col,spring,dir[1][0],dir[1][1]);
	if(row!=0)                   //向上
	    move(tdata,row,col,spring,dir[2][0],dir[2][1]);
	if(row!=2)                   //向下
		move(tdata,row,col,spring,dir[3][0],dir[3][1]);

}

void Node::move(int tdata[3][3],int row,int col,list<Node>* spring,int dirx,int diry)  //移动函数,将新状态加入后代spring中
{
	swap(tdata[row][col],tdata[row+dirx][col+diry]);
	int h=hashCode(tdata);
		
	Node temp(tdata);
	temp.parent=this;
	temp.index=h;
	temp.computeValue();
	spring->push_back(temp);
		
	swap(tdata[row][col],tdata[row+dirx][col+diry]);
}

void Heap::insert(Node node)
{
	heap.push_back(node);
	int i=heap.size()-1;
	int j;
	while((j=Parent(i))>=0)          //上滤
	{
		if(heap[j].fvalue>heap[i].fvalue)
		{
		    Node temp=heap[j];
			heap[j]=heap[i];
			heap[i]=temp;
			i=j;
		}
		else break;
	}
}

void Heap::delFirst()
{
	if(heap.size()<=0) return;
	heap[0]=heap[heap.size()-1];
	vector<Node>::iterator itheap;
	itheap=heap.end();
	itheap--;
	heap.erase(itheap);        //删除末尾元素
    int j;
	int i=0;
	do{                                  //下滤
		if(RChild(i)<heap.size())        //选择合适的后代节点，处理特殊情况
		{
			if(heap[LChild(i)].fvalue<=heap[RChild(i)].fvalue)
		        j=LChild(i);
			else
			    j=RChild(i);
		}
		else
		{
			if(LChild(i)<heap.size())
				j=LChild(i);
			else j=-1;
		}
		if(j>0 && heap[i].fvalue>heap[j].fvalue)
		{
		    Node temp=heap[j];
			heap[j]=heap[i];
			heap[i]=temp;
			i=j;
		}
		else break;
	}while(true);
}

void Heap::output()
{
	for(int i=0;i<heap.size();i++)
	{
		heap[i].output();
	}
}

int Heap::find(Node node)
{
	for(int i=0;i<heap.size();i++)
	{
		if(heap[i]==node)
		{
		    return i;
		}
	}
	return -1;
}

void Heap::change(int place,Node newNode)
{
	//cout<<"change"<<' ';
	heap[place].change(newNode);
	int i=place;
	int j;
	while(j=Parent(i)>=0)          //上滤
	{
		if(heap[j].fvalue>heap[i].fvalue)
		{
		    Node temp=heap[j];
			heap[j]=heap[i];
			heap[i]=temp;
			i=j;
		}
		else break;
	}
}

#endif