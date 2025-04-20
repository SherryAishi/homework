#include<iostream>
#include<string>
#include<math.h>
#include"bstTree.h" 
using namespace std;

struct node{
	int data;
	node* right;
	node* left;
	node* father;
	int bf;
	node()
	{
		right=NULL;
		left=NULL;
		father=NULL;
		bf=0;
	}
	node(int val)
	{
		right=NULL;
		left=NULL;
		father=NULL;
		bf=0;
		data=val;	
	}
};

node* root;
int fail=0;
int suce=0;
void PreOrder(node* now,int layer)
{
	if(now==NULL)
	{
		fail+=layer-1;
		return;
	}
	cout<<now->data<<" ";
	suce+=layer;
	PreOrder(now->left,layer+1);
	PreOrder(now->right,layer+1);
}
void MiddleOrder(node* now)
{
	if(now==NULL) return;
	MiddleOrder(now->left);
	cout<<now->data<<" ";
	MiddleOrder(now->right);
}
void Rotate_LL(node* tmp)
{
	//tmp为abs(bf)==2的结点；
	//进行的操作是：由于为LL型，则需要两个节点：分别为bf=2的tmp和bf=1的sonL
	//首先是tmp->father的孩子变为sonL，然后是sonL的原本的右孩子变成tmp的左孩子，现在的右孩子更新为tmp 
	node* fa=tmp->father;
	node* sonL=tmp->left;
	node* sonLR=sonL->right;
	if(fa==NULL)
	{
	    tmp->left= sonLR;
	    sonL->right=tmp;
	    sonL->father=NULL; 
	    if(sonLR!=NULL)sonLR->father=tmp;
	    tmp->father=sonL;
		root=sonL;		
	} 
	else
	if(fa->left==tmp)
	{
	    tmp->left= sonLR;
	    sonL->right=tmp;
	    fa->left=sonL;	    
	    //变化后需要更新每一个变化节点的父节点指针 
	    sonL->father=fa; 
	    if(sonLR!=NULL)sonLR->father=tmp;
	    tmp->father=sonL;
	}
	else
	{
	    tmp->left= sonLR;
	    sonL->right=tmp;
	    fa->right=sonL;
	    //变化后需要更新每一个变化节点的父节点指针
	    sonL->father=fa; 
	    if(sonLR!=NULL)sonLR->father=tmp;
	    tmp->father=sonL;
	}
	//更新bf
	sonL->bf=0;
	tmp->bf=0; 
}

void Rotate_RR(node* tmp)
{
	node* fa=tmp->father;
	node* sonR=tmp->right;
	node* sonRL=sonR->left;
	if(fa==NULL)//tmp就是树的root结点 
	{
	    tmp->right= sonRL;
	    sonR->left=tmp;
	    sonR->father=NULL; 
	    if(sonRL!=NULL)sonRL->father=tmp;
	    tmp->father=sonR;
	    root = sonR; 
	}
	else
	if(fa->left==tmp)
	{
	    tmp->right= sonRL;
	    sonR->left=tmp;
	    fa->left=sonR;	    
	    //变化后需要更新每一个变化节点的父节点指针 
	    sonR->father=fa; 
	    if(sonRL!=NULL)sonRL->father=tmp;
	    tmp->father=sonR;
	}
	else
	{
	    tmp->right= sonRL;
	    sonR->left=tmp;
	    fa->right=sonR;	    
	    //变化后需要更新每一个变化节点的父节点指针 
	    sonR->father=fa; 
	    if(sonRL!=NULL)sonRL->father=tmp;
	    tmp->father=sonR;
	}
	//更新bf
	sonR->bf=0;
	tmp->bf=0; 
}

void Rotate_LR(node* tmp)
{
	int s=tmp->left->right->bf;
	//LR型的旋转是：左孩子左旋，再整体右旋
	Rotate_RR(tmp->left);
	Rotate_LL(tmp);
	if(s==1)
	{
		tmp->left->bf=0;
		tmp->bf=-1;
	}
	else if(s==-1)
	{
		tmp->left->bf=1;
		tmp->bf=0;
	}
	else if(s=0)
	{
		tmp->left->bf=0;
		tmp->bf=0;		
	}
}

void Rotate_RL(node* tmp)
{
	int s=tmp->right->left->bf;
	Rotate_LL(tmp->right);
	Rotate_RR(tmp);
//	puts("");
//	PreOrder(root);
//	cout<<endl;
//	MiddleOrder(root);
//	puts(""); 
	if(s==1)
	{
		tmp->right->bf=-1;
		tmp->bf=0;
	}
	else if(s==-1)
	{
		tmp->right->bf=0;
		tmp->bf=1;
	}
	else if(s=0)
	{
		tmp->right->bf=0;
		tmp->bf=0;		
	}
}

void Update(node* cur)
{
	//插入新的结点cur后，父节点的更新状况 
	node* tmp=cur;
    while(tmp->father!=NULL)
    {
    	if(tmp->father->left==tmp)
    	{
    		tmp->father->bf+=1;
		}
		else
		{
			tmp->father->bf-=1;
		}
		if( abs(tmp->father->bf)==2 )
		{
			if(tmp->bf==1&&tmp->father->bf==2) //LL
			{
				Rotate_LL(tmp->father);
			}
			else if(tmp->bf==1&&tmp->father->bf==-2)//Rl
			{
				Rotate_RL(tmp->father);
			}
			else if(tmp->bf==-1&&tmp->father->bf==2)//LR
			{
				Rotate_LR(tmp->father);
			}
			else if(tmp->bf==-1&&tmp->father->bf==-2)//RR
			{
				Rotate_RR(tmp->father);
			}
			break;
		}
		else if( abs(tmp->father->bf)==1 )
		{
			tmp=tmp->father;
		}
		else
		{
			break;
		}
	}	
}

void Insert(int val,node* tmp)
{
	if(val<tmp->data)
	{
		if(tmp->left==NULL)
		{
			tmp->left=new node(val); 
			tmp->left->father = tmp;
			Update(tmp->left);
			return;
		}
		tmp=tmp->left;
		Insert(val,tmp);
	}
	else if(val>tmp->data)
	{
		if(tmp->right==NULL)
		{
			tmp->right=new node(val); 
			tmp->right->father = tmp;
			Update(tmp->right);
			return;
		}				
		tmp=tmp->right;
		Insert(val,tmp);
	}
	else
	return;
}
int main()
{
	string s;
	getline(cin,s);
	int a[100]={};
	int len=0;
	for(int i=0;i<s.size();i++)
	{
		int tmp=0;
		while(s[i]>='0'&&s[i]<='9')
		{
			tmp=tmp*10+s[i]-'0';
			i++;
		}
		a[len]=tmp;
		len++;		
	}
	//任务一
	bst<int>t1(a[0]);
	for(int i=1;i<len;i++)
	{
		t1.bst_Insert(a[i],t1.root);
	}
	cout<<"bst的前序遍历结果为：";
	t1.bst_PreOrder(t1.root,1);
	cout<<endl;
	cout<<"bst的中序遍历结果为：";
	t1.bst_MiddleOrder(t1.root);
	cout<<endl;
			
	//任务二 
	root=new node(a[0]);
	for(int i=1;i<len;i++)
	{
		Insert(a[i],root);
	}
	cout<<"avl的前序遍历结果为：";
	PreOrder(root,1);
	cout<<endl;
	cout<<"avl的中序遍历结果为：";
	MiddleOrder(root);
	cout<<endl;
	
	//任务三
	double bst_asl=1.0*t1.suc/len+1.0*t1.fai/(len+1);
	double avl_asl=1.0*suce/len+1.0*fail/(len+1);
	cout<<bst_asl-avl_asl<<endl;
	
	return 0;
}
