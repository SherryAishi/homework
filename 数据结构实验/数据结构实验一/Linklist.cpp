#include<bits/stdc++.h>
using namespace std;
int n,m,k;
struct node
{
	char data;
	node* next;
	node(char val)
	{
		data=val;
		next=NULL;
	}
	node()
	{
		next=NULL;
	}
};
class Linklist
{
	public:
		int size;
		node* head;
		Linklist();
		~Linklist();
		void Insert(int pos,char val);
		void Delete(int pos);
		void Print();
		bool Find(char val); 
};
Linklist::Linklist()
{
	size=0;
	head=new node();
}
Linklist::~Linklist()
{
	if(head==NULL) return;
	node* tmp=head;
	for(int i=0;i<size;i++)
	{
		head = tmp;
		tmp = head->next;
		delete head;
	}
    size=0;
}
void Linklist::Insert(int pos,char val)
{
	if(pos<0||pos>size) return;
	if(head==NULL) return;
	node* tmp=head;
	for(int i=0;i<pos;i++)
	{
		tmp=tmp->next;
	}
	node *insertnode=new node(val);
    insertnode->next=tmp->next;
    tmp->next=insertnode;
    size++;
}
void Linklist::Delete(int pos)
{
	if(pos<0||pos>=size) return;
	if(head==NULL) return;
	node* tmp=head;
	for(int i=0;i<pos;i++)
	{
		tmp=tmp->next;
	}
	node* DeletePoint = tmp->next;
	tmp->next = DeletePoint->next;
	delete DeletePoint; 
	size--;
}
void Linklist::Print()
{
	if(head==NULL) return;
	node* tmp=head->next;
	for(int i=0;i<size;i++)
	{
		cout<<tmp->data<<" ";
		tmp=tmp->next;
	}
	cout<<endl;
}
bool Linklist::Find(char val)
{
	if(head==NULL) return false;
	node* tmp=head->next;
	for(int i=0;i<size;i++)
	{
		if(tmp->data!=val)
		{
			tmp=tmp->next;
		}
		else
		return true;
	}
	return false;
}
int main()
{
	Linklist A,B;
	string s,S;
	getline(cin,s);
	getline(cin,S);
	for(int i=0;i<s.size();i++)
	{
		if(s[i]==' ')continue;
		A.Insert(A.size,s[i]);
		n++;
	}
	for(int i = 0;i < S.size();i++)
	{
		if(S[i]==' ')continue;
		B.Insert(B.size,S[i]);
		m++; 
	}
	A.Print();
	B.Print();
	
	
	 //交替合并 
	Linklist C;
	node* tmp1=A.head->next;
	node* tmp2=B.head->next;
	for(int i=0;i<min(n,m);i++)
	{
		C.Insert(C.size,tmp1->data);
		C.Insert(C.size,tmp2->data);
		tmp1=tmp1->next;
		tmp2=tmp2->next;
	}
	if(n<m)
	{
		while(tmp2!=NULL)
		{
			C.Insert(C.size,tmp2->data);
			tmp2=tmp2->next;
		}
	}
	if(n>m)
	{
		while(tmp1!=NULL)
		{
			C.Insert(C.size,tmp1->data);
			tmp1=tmp1->next;
		}
	}
	C.Print();
	//去重：
	Linklist D;
	int len=n+m;
	node* tmp=C.head->next;
	for(int i=0;i<len;i++)
	{
       if(!D.Find(tmp->data))
       {
       	 D.Insert(D.size,tmp->data);
	   }
	   tmp=tmp->next;
	}
	D.Print();
	//按k个反转 
	cin>>k;
	stack<int>p;
	Linklist E;
    if(D.size%k==0)
    {
    	node* tmp=D.head->next;
    	for(int i=0;i<D.size/k;i++)
    	{
    		for(int j=0;j<k;j++)
    		{
    			p.push(tmp->data);
    			tmp=tmp->next;
			}
			for(int q=0;q<k;q++)
			{
				E.Insert(E.size,p.top());
				p.pop();
			}
		}
	}
	else if(D.size%k!=0)
    {
    	node* tmp=D.head->next;
    	for(int i=0;i<D.size/k;i++)
    	{
    		for(int j=0;j<k;j++)
    		{
    			p.push(tmp->data);
    			tmp=tmp->next;
			}
			for(int q=0;q<k;q++)
			{
				E.Insert(E.size,p.top());
				p.pop();
			}
		}
        while(tmp!=NULL)
        {
        	E.Insert(E.size,tmp->data);
        	tmp=tmp->next;
		}
	}
	E.Print();
	return 0;
}



























