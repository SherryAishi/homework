#include<iostream>
using namespace std;

int FindPos(int *a,int len,int data)
{
	for(int i=1;i<=len;i++)
	{
		if(a[i]==data) return i;
	}
	return -1;
}

bool IsFull(int* a,int len)
{
	for(int i=1;i<=len;i++)
	{
		if(a[i]==0) return false;
	}
	return true;
}

int main()
{
	int num;
	cin>>num;
	//�������������������� 
	int man[100][100]={};
	int woman[100][100]={};
	for(int i=1;i<=num;i++)
	{
		for(int j=1;j<=num;j++)
		{
			int m;
			cin>>m;
			man[i][j]=m;
		}
	}
	for(int i=1;i<=num;i++)
	{
		for(int j=1;j<=num;j++)
		{
			int m;
			cin>>m;
			woman[i][j]=m;
		}
	}
	//���ν���ѡ��
	int couple[100]={0};
	int IsDated[100]={0};
	int WhoDate[100]={0};
	
	
	while(!IsFull(couple,num))
	{		 
	for(int i=1;i<=num;i++)
	{
		if(couple[i]!=0)
		{
			continue;
		}
		for(int j=1;j<=num;j++)
		{
			int p=man[i][j];
			//pΪ��ʱ����������Ե�Ů�� 
			if(IsDated[p]==0)//��Ů��δ�������״̬ 
			{
				couple[i]=p;
				IsDated[p]=1;
				WhoDate[p]=i;//Ů��������Ե���Ϊi 
				break;
			}
			else
			{
				int q=WhoDate[p];//q��p��ǰ��ԵĶ���
				int q_pos=FindPos(woman[p],num,q);
				int m_pos=FindPos(woman[p],num,i) ;
				if(q_pos>m_pos) //Ů��P�ĵ�ǰ����q����i 
				{
					couple[q]=0;
					couple[i]=p;
					WhoDate[p]=i;
					break;
				}
				else
				{
					continue;
				}			 
			}			
		}
	} 	
    }
    
	for(int i=1;i<=num;i++)
	{
		cout<<couple[i]<<" ";
	}	
	
	return 0;	
}
