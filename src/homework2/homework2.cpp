// homework2.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include "heap.h"

using namespace std;

ifstream fin;

int start[3][3];

Heap Open;          //Open表

bool haveSolution();
void Afind();

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc==3)
	{
		fin.open(argv[1]);
		fout.open(argv[2]);	
	}
	else
	{
		cout<<"homework1.exe input.txt output.txt";
		return 1;	
	} 
	for(int i=0;i<3;i++)
	    for(int j=0;j<3;j++)
			fin>>start[i][j];

	if(haveSolution())       //如果有解则执行A*算法查找解
	    Afind();
	else
		fout<<"no solution"<<endl;

	system("pause");
	return 0;
}
 
bool haveSolution()          //通过逆序对判断是否有解
{
    int sum1=0,sum2=0;
	int sdata[9],tdata[9];
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
		{
		    sdata[3*i+j]=start[i][j];
			tdata[3*i+j]=target[i][j];
		}
	for(int i=0;i<9;i++)
		for(int j=0;j<i;j++)
		{
			if(sdata[j]!=0)
			{
				if(sdata[j]<sdata[i])
					sum1++;
			}
			if(tdata[j]!=0)
			{
				if(tdata[j]<tdata[i])
					sum2++;
			}
		}
    if(sum1%2==sum2%2)
		return true;
	return false;

}

void Afind()        //A*算法
{
	Node* s=new Node(start);
    Open.insert(*s);

	while(Open.size()!=0)
	{
		Node* temp=new Node(Open.heap[0]);
        Open.delFirst();

		list<Node> spring;	  //扩展的后代节点

		if(temp->hvalue==0)      //找到解则根据parent指针回溯输出解
		{
			//cout<<Open.size()<<endl;
			//fout<<endl;
			fout<<temp->gvalue<<endl;
			list<Node> solution;
			solution.push_front(*temp);
			while(temp->parent!=NULL)
			{
				temp=temp->parent;
				solution.push_front(*temp);
			}
            auto i=solution.begin();
			i++;
			for(;i!=solution.end();i++)
			{
			    (*i).output();
			}
			return;
		}		    

		temp->generate(&spring);       //扩展后代节点
		
		list<Node>::iterator itlist;
		for(itlist=spring.begin();itlist!=spring.end();itlist++)
		{	
			Node tmpSpring=(*itlist);
            tmpSpring.used=true;
			if(!hashList[tmpSpring.index].used)        //通过查找哈希表hashList来判断该状态是否处理过，新状态直接进入Open表
			{
				hashList[tmpSpring.index]=tmpSpring;
				Open.insert(tmpSpring);
			}
			else if(tmpSpring.fvalue<hashList[tmpSpring.index].fvalue)  //如果状态出现过且比原状态更优，则
			{
				hashList[tmpSpring.index]=tmpSpring;
			    int oldPlace=0;
				oldPlace=Open.find(tmpSpring);               //判断是否在Open表中，是则更新Open表中节点的状态
				if(oldPlace>=0)         
				{
	                Open.change(oldPlace,tmpSpring);                           
				}
				else
				Open.insert(tmpSpring);                    //不在Open表中说明应该在Closed表中，重新放入Open表中
			}
		
		}
	}
	fout<<"no solution"<<endl;
}