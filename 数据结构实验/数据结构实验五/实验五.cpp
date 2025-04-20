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
	//tmpΪabs(bf)==2�Ľ�㣻
	//���еĲ����ǣ�����ΪLL�ͣ�����Ҫ�����ڵ㣺�ֱ�Ϊbf=2��tmp��bf=1��sonL
	//������tmp->father�ĺ��ӱ�ΪsonL��Ȼ����sonL��ԭ�����Һ��ӱ��tmp�����ӣ����ڵ��Һ��Ӹ���Ϊtmp 
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
	    //�仯����Ҫ����ÿһ���仯�ڵ�ĸ��ڵ�ָ�� 
	    sonL->father=fa; 
	    if(sonLR!=NULL)sonLR->father=tmp;
	    tmp->father=sonL;
	}
	else
	{
	    tmp->left= sonLR;
	    sonL->right=tmp;
	    fa->right=sonL;
	    //�仯����Ҫ����ÿһ���仯�ڵ�ĸ��ڵ�ָ��
	    sonL->father=fa; 
	    if(sonLR!=NULL)sonLR->father=tmp;
	    tmp->father=sonL;
	}
	//����bf
	sonL->bf=0;
	tmp->bf=0; 
}

void Rotate_RR(node* tmp)
{
	node* fa=tmp->father;
	node* sonR=tmp->right;
	node* sonRL=sonR->left;
	if(fa==NULL)//tmp��������root��� 
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
	    //�仯����Ҫ����ÿһ���仯�ڵ�ĸ��ڵ�ָ�� 
	    sonR->father=fa; 
	    if(sonRL!=NULL)sonRL->father=tmp;
	    tmp->father=sonR;
	}
	else
	{
	    tmp->right= sonRL;
	    sonR->left=tmp;
	    fa->right=sonR;	    
	    //�仯����Ҫ����ÿһ���仯�ڵ�ĸ��ڵ�ָ�� 
	    sonR->father=fa; 
	    if(sonRL!=NULL)sonRL->father=tmp;
	    tmp->father=sonR;
	}
	//����bf
	sonR->bf=0;
	tmp->bf=0; 
}

void Rotate_LR(node* tmp)
{
	int s=tmp->left->right->bf;
	//LR�͵���ת�ǣ���������������������
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
	//�����µĽ��cur�󣬸��ڵ�ĸ���״�� 
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
	//����һ
	bst<int>t1(a[0]);
	for(int i=1;i<len;i++)
	{
		t1.bst_Insert(a[i],t1.root);
	}
	cout<<"bst��ǰ��������Ϊ��";
	t1.bst_PreOrder(t1.root,1);
	cout<<endl;
	cout<<"bst������������Ϊ��";
	t1.bst_MiddleOrder(t1.root);
	cout<<endl;
			
	//����� 
	root=new node(a[0]);
	for(int i=1;i<len;i++)
	{
		Insert(a[i],root);
	}
	cout<<"avl��ǰ��������Ϊ��";
	PreOrder(root,1);
	cout<<endl;
	cout<<"avl������������Ϊ��";
	MiddleOrder(root);
	cout<<endl;
	
	//������
	double bst_asl=1.0*t1.suc/len+1.0*t1.fai/(len+1);
	double avl_asl=1.0*suce/len+1.0*fail/(len+1);
	cout<<bst_asl-avl_asl<<endl;
	
	return 0;
}
