#include<iostream>
#include<string>
using namespace std;

struct node{
	int data;
	node* left;
	node* right;
	node(int val)
	{
	    data=val;
	    left=NULL;
	    right=NULL;
	}
};

void build(int pre[],int mid[],node* root,int len,char s)
{
	if(len == 0)return;
	node* new_root=new node(pre[0]);
	///cout<<new_root->data<<endl;
	if(s=='l') root->left=new_root;
	else 
	root->right=new_root; 
	
	if(len==1)  
	{
		return;
	}
	int pre_left[50]={};
	int pre_right[50]={};
	int mid_left[50]={};
	int mid_right[50]={};
	
	int root_val=pre[0];
	int len_left=0;
	int len_right=0;
	//先构建左子树中的前序和中序遍历结果 
	for(int i=0;i<len;i++)
	{
		if(mid[i]!=root_val)
		{
			mid_left[len_left]=mid[i];
			len_left++;
		}
		else
		{
			break;
		}
	}
	for(int i=1;i<=len_left;i++)
	{
		pre_left[i-1]=pre[i];
	}
	build(pre_left,mid_left,new_root,len_left,'l');
	//再构建右子树中的前序和中序遍历结果
	for(int i=len_left+1;i<len;i++)
	{
		mid_right[len_right]=mid[i];
		len_right++;
	}
	len_right=0;
	for(int i=len_left+1;i<len;i++)
	{
		pre_right[len_right]=pre[i];
		len_right++;
	}
	build(pre_right,mid_right,new_root,len_right,'r');
}

void BackOrder(node* tmp)
{
	if(tmp==NULL)return;
	BackOrder(tmp->left);
	BackOrder(tmp->right);
	cout<<tmp->data<<" ";
}

int main()
{
	//part1.将前序和中序的字符串放到数组中，肯定是等长的 
	string pre;
	string mid;
	getline(cin,pre);
	getline(cin,mid);
	int p[100]={};
	int m[100]={};
	int plen=0;
	int mlen=0;
	for(int i=0;i<pre.size();i++)
	{
		int tmp=0;
		while(pre[i]>='0'&&pre[i]<='9')
		{
			tmp=tmp*10+pre[i]-'0';
			i++;
		}
		p[plen]=tmp;
		plen++;
	}
	for(int i=0;i<mid.size();i++)
	{
		int tmp=0;
		while(mid[i]>='0'&&mid[i]<='9')
		{
			tmp=tmp*10+mid[i]-'0';
			i++;
		}
		m[mlen]=tmp;
		mlen++;
	}	
	
	
//	1 2 3 4 5 6 7
//	2 3 4 1 5 6 7
	//part2.进行递归
	node* root=new node(p[0]);
	int root_val=p[0];
	
	int pre_left[50]={};
	int pre_right[50]={};
	int mid_left[50]={};
	int mid_right[50]={};
	
	int len=mlen;

	int len_left=0;
	int len_right=0;
	//先构建左子树中的前序和中序遍历结果 
	for(int i=0;i<len;i++)
	{
		if(m[i]!=root_val)
		{
			mid_left[len_left]=m[i];
//			cout<<m[i]<<" "<<mid_left[len_left]<<" "<<len_left<<endl;			
			len_left++;
		}
		else
		{
			break;
		}
	}
	for(int i=1;i<=len_left;i++)
	{
		pre_left[i-1]=p[i];
	}		
	build(pre_left,mid_left,root,len_left,'l');
	//再构建右子树中的前序和中序遍历结果
	for(int i=len_left+1;i<len;i++)
	{
		mid_right[len_right]=m[i];
		len_right++;
	}
	len_right=0;
	for(int i=len_left+1;i<len;i++)
	{
		pre_right[len_right]=p[i];
		len_right++;
	}
	build(pre_right,mid_right,root,len_right,'r');
	
	//part3;
	BackOrder(root);
	
	return 0;
}
