#include<iostream>
#include<queue>
#include<string>
#include<math.h>
using namespace std;

//1. 实现一个基于链表的二叉树，可以按照给定格式字符串生成对应二叉树。
//2. 删除二叉树第 K 层所有节点后重新调整成二叉树，调整方式不作要求。

struct node{
	int data;
	int id;
	int layer;
	node* left;
	node* right;
	node* father;
	node(int val,int new_id,int new_layer)
	{
		data=val;
		id=new_id;
		layer=new_layer;
		left=NULL;
		right=NULL;
	}
};

void Insert(node* root,int val)
{
	queue< node* >q;
	q.push(root);
	while(!q.empty())
	{
		node* tmp=q.front();
		q.pop();
		if(tmp->left==NULL)
		{
			int tmp_layer=tmp->layer;
			int tmp_id=tmp->id;
			tmp->left=new node(val,tmp_id*2,tmp_layer+1);
			tmp->left->father=tmp;
			return;
		}
		else
		{
			q.push(tmp->left);	
		}
		if(tmp->right==NULL)
		{
			int tmp_layer=tmp->layer;
			int tmp_id=tmp->id;
			tmp->right=new node(val,tmp_id*2+1,tmp_layer+1);
			tmp->right->father=tmp;
			return;
		}
	    else
		{
			q.push(tmp->right);
	    }
	}
	
}

void FrontOrder(node* now)
{
	if(now == NULL||now->data==-1)return;
	cout<<now->data<<" ";
	FrontOrder(now->left);
	FrontOrder(now->right);
			
}
void MiddleOrder(node* now)
{
	if(now == NULL||now->data==-1)return;
	MiddleOrder(now->left);
	cout<<now->data<<" ";
	MiddleOrder(now->right);
}
void BackOrder(node* now)
{
	if(now == NULL||now->data==-1)return;
	BackOrder(now->left);
	BackOrder(now->right);
	cout<<now->data<<" ";
}

int main()
{
	//任务一，根据层次遍历写出树，并进行前中后序遍历 
	int root_id=1;
	int root_layer=1;
	string s;
	getline(cin,s);
	int tarray[100]={};
	int len=0;;
	for(int i=0;i<s.size();i++)
	{
		if(s[i]=='['||s[i]==']'||s[i]==',') continue;
		int tmp=0;
		while(s[i]>='0'&&s[i]<='9')
		{
			tmp=tmp*10+s[i]-'0';
			i++;
		}
		if(s[i]=='n'&&s[i+1]=='u'&&s[i+2]=='l'&&s[i+3]=='l')
		{
			tmp=-1;
			i+=4;
		}
		tarray[len]=tmp;
		len++;
	}
	
	node* root=new node(tarray[0],root_id,root_layer);
	for(int i=1;i<len;i++)
	{
		Insert(root,tarray[i]);
	}
	FrontOrder(root);
	cout<<endl;
	MiddleOrder(root);
	cout<<endl;
	BackOrder(root);
	
	//任务二：根据层数来删减
	int k;
	cin>>k;
	node* new_root;
	if(k==1)
	{
		new_root=new node(tarray[1],root_id,root_layer);
		for(int i=2;i<len;i++)
	    {
    		Insert(new_root,tarray[i]);
	    }
	}
	else
	{
		new_root=new node(tarray[0],root_id,root_layer);
		int sta=pow(2,k-1)-1;
		cout<<sta<<" "<<tarray[sta]<<endl;
		int end=pow(2,k)-1;
		cout<<end<<" "<<tarray[end]<<endl;		
		for(int i=1;i<sta;i++)
		{
			Insert(new_root,tarray[i]);
		}
		for(int i=end;i<len;i++)
		{
			Insert(new_root,tarray[i]);
		}
	}
    FrontOrder(new_root);
	cout<<endl;
	MiddleOrder(new_root);
	cout<<endl;
	BackOrder(new_root);

	return 0;	
}
