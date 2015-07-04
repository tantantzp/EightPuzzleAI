// homework2.cpp : �������̨Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include "heap.h"

using namespace std;

ifstream fin;

int start[3][3];

Heap Open;          //Open��

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

	if(haveSolution())       //����н���ִ��A*�㷨���ҽ�
	    Afind();
	else
		fout<<"no solution"<<endl;

	system("pause");
	return 0;
}
 
bool haveSolution()          //ͨ��������ж��Ƿ��н�
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

void Afind()        //A*�㷨
{
	Node* s=new Node(start);
    Open.insert(*s);

	while(Open.size()!=0)
	{
		Node* temp=new Node(Open.heap[0]);
        Open.delFirst();

		list<Node> spring;	  //��չ�ĺ���ڵ�

		if(temp->hvalue==0)      //�ҵ��������parentָ����������
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

		temp->generate(&spring);       //��չ����ڵ�
		
		list<Node>::iterator itlist;
		for(itlist=spring.begin();itlist!=spring.end();itlist++)
		{	
			Node tmpSpring=(*itlist);
            tmpSpring.used=true;
			if(!hashList[tmpSpring.index].used)        //ͨ�����ҹ�ϣ��hashList���жϸ�״̬�Ƿ��������״ֱ̬�ӽ���Open��
			{
				hashList[tmpSpring.index]=tmpSpring;
				Open.insert(tmpSpring);
			}
			else if(tmpSpring.fvalue<hashList[tmpSpring.index].fvalue)  //���״̬���ֹ��ұ�ԭ״̬���ţ���
			{
				hashList[tmpSpring.index]=tmpSpring;
			    int oldPlace=0;
				oldPlace=Open.find(tmpSpring);               //�ж��Ƿ���Open���У��������Open���нڵ��״̬
				if(oldPlace>=0)         
				{
	                Open.change(oldPlace,tmpSpring);                           
				}
				else
				Open.insert(tmpSpring);                    //����Open����˵��Ӧ����Closed���У����·���Open����
			}
		
		}
	}
	fout<<"no solution"<<endl;
}