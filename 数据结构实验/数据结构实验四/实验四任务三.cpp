#include<iostream>
#include<queue>
#include<string>
#include<math.h>
using namespace std;

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
		father=NULL;
	}
};

int GetDistance(node* a,node* b)
{
	int aid[100]={};
	int bid[100]={};
	node* atmp=a;
	int alen=0;
	while(atmp!=NULL)
	{
		aid[alen]=atmp->id;
		atmp=atmp->father;
		alen++;
	}
	
	node* btmp=b;
	int blen=0;
	while(btmp!=NULL)
	{
		bid[blen]=btmp->id;
		btmp=btmp->father;
		blen++;
	}
	for(int i=0;i<alen;i++)
	{
		for(int j=0;j<blen;j++)
		{
			if(aid[i]==bid[j])
			{
				int d=i+j;
				return d;
			}
		}
	}
	return -1;	
}

void Insert(node* root,int val,node* vanode[],int& valen)
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
			vanode[valen]=tmp->left;
			valen++;
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
			vanode[valen]=tmp->right;
			valen++;
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
	
	int val;
	cin>>val;
	int k;
	cin>>k;
	node* root=new node(tarray[0],root_id,root_layer);
	node* vanode[100]={};
	int valen=1;
	vanode[0]=root;
	for(int i=1;i<len;i++)
	{
		Insert(root,tarray[i],vanode,valen);
	}
//	for(int i=0;i<valen;i++)
//	{
//		cout<<vanode[i]->id<<" "<<vanode[i]->data<<endl;
//	}
    node* val_arr[100]={};
    int val_len=0;
    for(int i=0;i<valen;i++)
    {
    	if(vanode[i]->data==val)
    	{
    		val_arr[val_len]=vanode[i];
    		val_len++; 
		}
	}
//	cout<<endl;
//	for(int i=0;i<val_len;i++)
//	{
//		cout<<val_arr[i]->id<<" "<<val_arr[i]->data<<endl;
//	}
	int flag[100]={};
	
	for(int i=0;i<val_len;i++)
	{
		for(int j=0;j<valen;j++)
		{
			if(GetDistance(val_arr[i],vanode[j])==k)
			{
				if(vanode[j]->data!=-1&&flag[j]==0)
				cout<<vanode[j]->data<<" ";
				flag[j]=1;
			}
		}
	}
	

	
}
