#include<iostream>
#include<queue>
using namespace std;
struct node
{
	int data;
	node* next;
	node()
	{
		next=NULL;
	}
	node(int val)
	{
		data=val;
		next=NULL;
	}
};
class linklist
{
	public:		
	    node* head;
	    int size;
	    linklist()
	    {
		    head=new node;
		    size=0;
	    }
	    ~linklist()
	    {
	 	    delete head;
	    }
	    void Insert(int val)
    	{
    		if(head==NULL) return;
    		node* insertnode=new node(val);
    		node* tmp=head->next;
    		head->next=insertnode;
    		insertnode->next=tmp;
			size++;
			return;
    	}
};
int main()
{
	int num;
	cin>>num;
	for(int i=1;i<=num;i++)
	{
		int cnt=1;
		int p_num,v_num;
		cin>>p_num>>v_num;
		linklist arr[5005];
		//入度值数组，dev_arr[i]表示节点i的入度 
		int dev_arr[5005];
		int data_arr[5005]; 
		for(int j=1;j<=p_num;j++)
		{
			dev_arr[j]=0;
		}
		for(int j=1;j<=v_num;j++)
		{
			int a,b; 
			//理论上若为<a,b>则有arr[a]链表链接数值为b的节点，此处利用反向链表，则反过来 
			cin>>a>>b;
			arr[b].Insert(a);
			dev_arr[a]++;
		}
		priority_queue<int> q;
		
		for(int j=1;j<=p_num;j++)
		{
			if(dev_arr[j]==0) q.push(j);
		}
		while(!q.empty())
		{
			int val=q.top();
			q.pop();
			data_arr[cnt]=val;
			cnt++;
			node* tmp=arr[val].head;
			for(int j=0;j<arr[val].size;j++)
			{
				tmp=tmp->next;
				dev_arr[tmp->data]--;
				if(dev_arr[tmp->data]==0) q.push(tmp->data);
			}			
		}
		cnt--;
		if(cnt!=p_num)
		{
			cout<<"impossible!"<<endl;
		}
        else
        {
			for(int j=p_num;j>=1;j--)
			{
				cout<<data_arr[j]<<" ";
			}
			cout<<endl;
	    }  
    }
}
