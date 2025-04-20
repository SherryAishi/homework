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
	//放入数据置两个数组中 
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
	//依次进行选择
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
			//p为此时男性期望配对的女性 
			if(IsDated[p]==0)//若女性未处于配对状态 
			{
				couple[i]=p;
				IsDated[p]=1;
				WhoDate[p]=i;//女性现在配对的人为i 
				break;
			}
			else
			{
				int q=WhoDate[p];//q是p当前配对的对象
				int q_pos=FindPos(woman[p],num,q);
				int m_pos=FindPos(woman[p],num,i) ;
				if(q_pos>m_pos) //女性P的当前对象q不如i 
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
