#include<iostream>
using namespace std;

template<class T>
struct node{
	node* next;
	int keys;
	T data;
	node()
	{
		next=NULL;
	}
	node(T val)
	{
		data=val;
		next=NULL;
	}
	node(int k,T val)
	{
		keys=k;
		data=val;
		next=NULL;
	}
};

template<class T>
class Linklist{
	public:
		int size;
		node<T>* head;
		Linklist()
		{
			size=0;
			head=new node<T>();
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
				node<T>* tmp=head->next;
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
		void Insert(int keys,T val)
		{
			node<T>* Insertnode=new node<T>(keys,val);
			node<T>* tmp=head->next;
			Insertnode->next=tmp;
			head->next=Insertnode;
			size++;
		}
		bool Ifkeysin(int keys)
		{
			if(size==0) return false;
			node<T>* tmp=head->next;
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
			node<T>* tmp=head;
			for(int i=0;i<size;i++)
			{
				if(tmp->next->keys==keys)
				{
					node<T>* Dnode=tmp->next;
					tmp->next=Dnode->next;
					delete Dnode;
					size--;
					return;
				}
				else
				tmp=tmp->next;
			}
		}
		T Get(int keys)
		{
			node<T>* tmp=head->next;
			for(int i=0;i<size;i++)
			{
				if(tmp->keys==keys)
				return tmp->data;
				else
				tmp=tmp->next;
			}
		}
		void Modify(int keys,T val)
		{
			if(Ifkeysin(keys)==false) return;
			node<T>* tmp=head->next;
			for(int i=0;i<size;i++)
			{
				if(tmp->keys==keys)
				tmp->data=val;
				else
				tmp=tmp->next;
			}
		}
};


template<class T>
class Hash{
	private:
		int Maxsize;
		Linklist<T>* array[15];
	public:
		Hash()
		{
			Maxsize=11;
			for(int i=0;i<Maxsize;i++)
			{
				array[i]=new Linklist<T>();
			}
		}
		~Hash()
		{
			for(int i=0;i<Maxsize;i++)
			{
				delete array[i];
			}
		}
		void HashInsert(int keys,T val)
		{
			if(HashSearch(keys))
			{
				this->HashDelete(keys);
			}
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
		T Hashget(int keys)
		{
		    int pos=keys;
			while(pos<0)
			{
				pos+=Maxsize;
			}
			pos=pos%Maxsize;
			return array[pos]->Get(keys);
		}
		void HashModify(int keys,T val)
		{
			int pos=keys;
			while(pos<0)
			{
				pos+=Maxsize;
			}
			pos=pos%Maxsize;
			array[pos]->Modify(keys,val);
		}
};














