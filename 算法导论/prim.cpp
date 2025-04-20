#include<iostream>
#include<vector>
#include<queue>
using namespace std;
const long int maxdata=1e5+5;

struct node{
	int point;
	int data;  
	int number;
	
	friend bool operator < (const node &x,const node &y)
	{
		if(x.data==y.data) return x.number>y.number;
		return x.data>y.data;
	}
	node()
	{
		data=maxdata;
	}
	node(int p,int d,int n)
	{
		point=p;
		data=d;
		number=n; 
	}
};
vector<node> a[maxdata];
    //创建一个数组a,a[i]代表着：与节点i相连的所有node组合成的vector
    //(由于用二维数组会有很多空余空间)使用vector可以节省空间

priority_queue< node > minheap;
	//集合G代表所有已经相连的节点 minheap中记录与集合G所有相连的边
	//在每一次循环中，G增加相邻最近的节点p，随之更新minheap，增加所有与 p相连的边 
	//由于要生成最小字典序的最小生成树，则若权重相同，选取边较小的；若不同，选权重较小的
		
bool vis[maxdata];
    //用vis记录某个点是否已经被访问，即是否已经在集合G中 
int main()
{
	int v_num,e_num;
	cin>>v_num>>e_num;
	
	for(int i=1;i<=e_num;i++)
	{
		int p1,p2,val;
		cin>>p1>>p2>>val;
		a[p1].push_back(node(p2,val,i));
		a[p2].push_back(node(p1,val,i));
	}  	
		
	for(int i=0;i<a[1].size();i++)
	{
		minheap.push(a[1][i]);
	}
	int cnt=1;
	vis[1]=1;
	
	long long sum=0;
	long long xor_sum=0;
	while(!minheap.empty())
	{
		node tmp=minheap.top();
		minheap.pop();
		if(vis[tmp.point])continue;
		cnt++;
		vis[tmp.point]=1;
		for(int i=0;i<a[tmp.point].size();i++)
		{
			if(vis[a[tmp.point][i].point])continue;
			minheap.push(a[tmp.point][i]);
		}
		//cout<<tmp.data<<" "<<tmp.number<<endl; 
		sum+=tmp.data;
		xor_sum^=tmp.number;
	}
	
	if(cnt!=v_num)
	{
		cout<<"-1";
	}
	else
	{
		cout<<sum<<" "<<xor_sum<<endl;
	}
	return 0;	
}
