#include"HashTable.h"
#include<vector>
#include<string>
template<>//HashTable中设置其为类模板 ，在此使其变为特定数据类型的类 
struct node< vector<int> >
{
	node* next;
	int keys;
	vector<int> data;
	node()
	{
		next=NULL;
	}
	node(int key,int val)
	{
		keys=key;
		data.push_back(val);
		next=NULL;
	}
	
};

template<>
class Linklist< vector<int> >{
	public:
		int size;
		node< vector<int> >* head;
		Linklist()
		{
			size=0;
			head=new node< vector<int> >();
		}
		~Linklist()
		{
			if(IsEmpty())
			{
				delete head;
				return;
			}
			else
			{
				node< vector<int> >* tmp=head->next;
				while(tmp!=NULL)
				{
					delete head;
					head=tmp;
					tmp=tmp->next;
				}
				delete head;
			}	
		}
		bool IsEmpty()
		{
			return size==0;
		}
		int Size()
		{
			return size;
		}
		void Insert(int keys,int val)
		{
			if(Ifkeysin(keys)==false)
			{
			    node< vector<int> >* Insertnode=new node< vector<int> >(keys,val);
			    node< vector<int> >* tmp=head->next;
			    Insertnode->next=tmp;
			    head->next=Insertnode;
			    size++;
			}
			else
			{		
				this->GetPointer(keys)->push_back(val);
//				vector<int> m=this->Get(keys);
//				for(int i=0;i<m.size();i++)
//				{
//					cout<<m[i]<<" ";
//				}
//				cout<<endl;
			}
		}
		bool Ifkeysin(int keys)
		{
			if(size==0) return false;
			node< vector<int> >* tmp=head->next;
			for(int i=0;i<size;i++)
			{
				if(tmp->keys==keys) return true;
				else
				tmp=tmp->next;
			}
			return false;
		}
		void Delete(int keys)
		{
			if(Ifkeysin(keys)==false) return;
			node< vector<int> >* tmp=head;
			for(int i=0;i<size;i++)
			{
				if(tmp->next->keys==keys)
				{
					node< vector<int> >* Dnode=tmp->next;
					tmp->next=Dnode->next;
					delete Dnode;
					size--;
					return;
				}
				else
				tmp=tmp->next;
			}
		}
		vector<int> Get(int keys)
		{
			node< vector<int> >* tmp=head->next;
			for(int i=0;i<size;i++)
			{
				if(tmp->keys==keys)
				return tmp->data;
				else
				tmp=tmp->next;
			}
		}
		vector<int>* GetPointer(int keys)
		{
			node< vector<int> >* tmp=head->next;
			for(int i=0;i<size;i++)
			{
				if(tmp->keys==keys)
				return &(tmp->data);
				else
				tmp=tmp->next;
			}
		}
};

template<>
class Hash< vector<int> >{
	private:
		int Maxsize;
		Linklist< vector<int> >* array[13];
	public:
		Hash()
		{
			Maxsize=13;
			for(int i=0;i<Maxsize;i++)
			{
				array[i]=new Linklist< vector<int> >();
			}
		}
		~Hash()
		{
			for(int i=0;i<Maxsize;i++)
			{
				delete array[i];
			}
		}
		void HashInsert(int keys,int val)
		{
			    int pos=keys;
		        while(pos<0)
		        {
		            pos=pos+Maxsize;
			    }
			    pos=pos%Maxsize;
			    array[pos]->Insert(keys,val);
		}
		void HashDelete(int keys)
		{
			int pos=keys;
			while(pos<0)
			{
				pos+=Maxsize;
			}
			pos=pos%Maxsize;
			if(array[pos]->Ifkeysin(keys)==false) return;
			array[pos]->Delete(keys);
		}
		bool HashSearch(int keys)
		{
			int pos=keys;
			while(pos<0)
			{
				pos+=Maxsize;
			}
			pos=pos%Maxsize;
			if(array[pos]->Ifkeysin(keys)) return true;
			return false;
		}
		vector<int> Hashget(int keys)
		{
		    int pos=keys;
			while(pos<0)
			{
				pos+=Maxsize;
			}
			pos=pos%Maxsize;
			return array[pos]->Get(keys);
		}
};

int main()
{
	string s;
	getline(cin,s);
	int a[100]={0};
	int cnt=0;
	for(int i=0;i<s.size();i++)
	{
		int tmp=0;
		while(s[i]>='0'&&s[i]<='9')
		{
			tmp=tmp*10 +s[i]-'0';
			i++;
		}
		if(s[i]=='-')
		{
			i++;
		    while(s[i]>='0'&&s[i]<='9')
		    {
		    	tmp=tmp*10 +s[i]-'0';
		    	i++;
		    }
			tmp=-tmp; 
		}
		a[cnt]=tmp;
		cnt++;
	}
//	for(int i=0;i<cnt;i++)
//	{
//		cout<<a[i]<<" ";
//	}
//	cout<<endl;
	
	int b[100]={0};
	b[0]=a[0];
	for(int i=1;i<cnt;i++)
	{
		b[i]=b[i-1]+a[i];
	}
//	for(int i=0;i<cnt;i++)
//	{
//		cout<<b[i]<<" ";
//	}
//	cout<<endl;
	
	int k;
	cin>>k;
	Hash< vector<int> >h;
	Hash<int>value;	
    for(int i=0;i<cnt;i++)
	{
		h.HashInsert(b[i],i);
		int findkeys=b[i]-k;
		int fir=0;
		if(b[i]==k)
		{
			int va=0;
			for(int j=0;j<=i;j++)
			{
				va=va*61+a[j]+10;
			}
			if(value.HashSearch(va)==false)
			{ 
			    for(int j=0;j<=i;j++)
		    	{ 
		    		cout<<a[j]<<" ";
		    	}
		    	cout<<endl;
		    	value.HashInsert(va,0);
		    }
		}
		if(h.HashSearch(findkeys))
		{
			vector<int> p = h.Hashget(findkeys); 
			int si=p.size();
			if(si>1)
			{
				for(int j=0;j<si;j++)
				{
//					cout<<p[j]<<endl;
					fir=p[j]+1;
					int va=0;
					for(int sta=fir;sta<=i;sta++)
				    {
				    	va=va*61+a[sta]+10;
				    }
				    if(value.HashSearch(va)==false)
				    {
				    	for(int sta=fir;sta<=i;sta++)
				        {
				    	    cout<<a[sta]<<" ";
				        }
				        value.HashInsert(va,0);
				        cout<<endl;
					}
			    }
			}
			else
			{
				fir=p[0]+1;
				int va=0;
				for(int sta=fir;sta<=i;sta++)
				{
				    va=va*61+a[sta]+10;
				}
				if(value.HashSearch(va)==false)
				{
				    for(int sta=fir;sta<=i;sta++)
				    {
				    	cout<<a[sta]<<" ";
				    }
				    value.HashInsert(va,0);
				    cout<<endl;
				}
			}
		}
		else
		{
			continue;
		}
	}
	
}
