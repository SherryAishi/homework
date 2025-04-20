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
    //����һ������a,a[i]�����ţ���ڵ�i����������node��ϳɵ�vector
    //(�����ö�ά������кܶ����ռ�)ʹ��vector���Խ�ʡ�ռ�

priority_queue< node > minheap;
	//����G���������Ѿ������Ľڵ� minheap�м�¼�뼯��G���������ı�
	//��ÿһ��ѭ���У�G������������Ľڵ�p����֮����minheap������������ p�����ı� 
	//����Ҫ������С�ֵ������С������������Ȩ����ͬ��ѡȡ�߽�С�ģ�����ͬ��ѡȨ�ؽ�С��
		
bool vis[maxdata];
    //��vis��¼ĳ�����Ƿ��Ѿ������ʣ����Ƿ��Ѿ��ڼ���G�� 
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
