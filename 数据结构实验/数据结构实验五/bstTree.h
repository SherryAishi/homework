#include<iostream>
using namespace std;

template<class T>
struct bst_node{
	T data;
	bst_node<T>* left;
	bst_node<T>* right;
	bst_node<T>* father;
	bst_node()
	{
		left=NULL;
		right=NULL;
		father=NULL;
	}
	bst_node(T val)
	{
		data=val;
		left=NULL;
		right=NULL;
		father=NULL;
	}
};

template<class T>
class bst{
	public:
		int suc;
		int fai;
		bst_node<T>* root;
		int size;
		bst()
		{
			root=new bst_node<T>();
		}
		bst(T val)
		{
			suc = 0;
			fai = 0;
			root=new bst_node<T>(val);
		}
		~bst()
		{
			delete root;
		}
		void bst_Insert(T val,bst_node<T>* tmp)
		{
			if(val<tmp->data)
			{
				if(tmp->left==NULL)
				{
					tmp->left=new bst_node<T>(val); 
					tmp->left->father = tmp;
					return;
				}
				tmp=tmp->left;
				bst_Insert(val,tmp);
			}
			else if(val>tmp->data)
			{
				if(tmp->right==NULL)
				{
					tmp->right=new bst_node<T>(val); 
					tmp->right->father = tmp;
					return;
				}				
				tmp=tmp->right;
				bst_Insert(val,tmp);
			}
			else
			return;
		}
		bst_node<T>* FindNode(T val,bst_node<T>* tmp)
		{
			if(val<tmp->data)
			{
				if(tmp->data==val)
				{
					return tmp;
				}
				else if(tmp==NULL)
				{
					return NULL;
				}
				else
				{
					tmp=tmp->left;
					return FindNode(val,tmp);
				}
			}
			else if(val>tmp->data)
			{
				if(tmp->data==val)
				{
					return tmp;
				}
				else if(tmp==NULL)
				{
					return NULL;
				}
				else
				{
					tmp=tmp->right;
					return FindNode(val,tmp);
				}				
			}
			else
			{
				return tmp;
			}
		}
		void bst_Delete(T val)
		{
			bst_node<T>* tmp=FindNode(val,root);
			if(tmp==NULL) //说明该节点不存在，直接return 
			{
				return;
			}
			else if(tmp->left!=NULL&&tmp->right!=NULL)
			{
	            bst_node<T>* now=tmp->right;//找右子树中的最小值，则要找左子树 
				while(now->left!=NULL)
				{
					now=now->left;
				}
				if(now->left==NULL&&now->right==NULL)
				{
					tmp->data=now->data;
					bst_node<T>* fnow=now->father;
					fnow->left=NULL;
				    delete now;
				}
				else
				{
					tmp->data=now->data;
					Delete(now->data);
				}					
			}
			else if(tmp->left==NULL&&tmp->right==NULL)
			{
				bst_node<T>* ftmp=tmp->father;
				if(tmp==ftmp->left)
				{
					ftmp->left=NULL;
					delete tmp;
				}
				else
				{
					ftmp->right=NULL;
					delete tmp;
				}
			}
			else
			{
				bst_node<T>* ftmp=tmp->father;
				if(tmp==ftmp->left)
				{
					if(tmp->left!=NULL&&tmp->right==NULL)
					{
						ftmp->left=tmp->left;
					    delete tmp;
					}
					else
					{
						ftmp->left=tmp->right;
					    delete tmp;
					}
				}
				else
				{
					if(tmp->left!=NULL&&tmp->right==NULL)
					{
						ftmp->right=tmp->left;
					    delete tmp;
					}
					else
					{
						ftmp->right=tmp->right;
					    delete tmp;
					}
				}
			}
		}
		void bst_PreOrder(bst_node<T>* now,int layer)
		{
			if(now==NULL)
			{
				fai += layer - 1;
				return;
			}
//			cout<<"test:"<<now->data<<endl;
			cout<<now->data<<" ";
			suc += layer;
			bst_PreOrder(now->left,layer+1);
			bst_PreOrder(now->right,layer+1);
		}
		void bst_MiddleOrder(bst_node<T>* now)
		{
			if(now==NULL) return;
			bst_MiddleOrder(now->left);
			cout<<now->data<<" ";
			bst_MiddleOrder(now->right);
		}		
};

