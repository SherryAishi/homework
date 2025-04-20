#include<bits/stdc++.h>
using namespace std;

string str0(1,'0');
string str1(1,'1');
template<class T>
struct node
{
	public:
	T data;
	node* next;
	node()
	{
		next=NULL;
	}
	node(T val)
	{
		data=val;
		next=NULL;
	}
};

template<class T>
class LinkStack
{
private:
	node<T>* head;
	int size;
public:
	LinkStack()
	{
		head=new node<T>();
		size=0; 
	}
	~LinkStack()
	{
		if(head==NULL) return;
		node<T>* tmp=head->next;
		while(tmp!=NULL)
		{
			delete head;
			head=tmp;
			tmp=tmp->next;
		}
		delete head;
	}
	void Add(T val)
	{
		node<T>* Insertnode=new node<T>(val);
		node<T>*tmp=head->next;
		head->next=Insertnode;
		Insertnode->next=tmp;
		size++;
	}
	void Delete()
	{
		if(IsEmpty()) return;	
		node<T>* Deletenode=head->next;
		head->next=Deletenode->next;
		size--;
		delete Deletenode;
	}
	T GetTop()
	{
	    node<T>* tmp=head->next;
		T var=tmp->data;
		return var;	
	}
	bool IsEmpty()
	{
		return size==0;
	}
	int GetSize()
	{
		return size;
	}
};

bool IsAlpha(char s)
{
	if(s>='a'&&s<='z') return 1;
	if(s>='A'&&s<='Z') return 1;
	return 0;
}

bool string_IsAlpha(string a)
{
   for(int i = 0;i < a.size();i++)
   {
   	if(a[i] >= 'a' && a[i] <= 'z')return 1;
   	if(a[i] >= 'A' && a[i] <= 'Z')return 1;
   }
    return 0;
}

bool IsOperator(char s)
{
	if(s=='+'||s=='-'||s=='*'||s=='/') return true;
	return false;
}

bool IsNumber(char s)
{
	if(s>='0'&&s<='9') return true;
	return false;
}


bool checkExpression(string s) //�ж�ĳ�����ʽ�Ƿ�Ϸ�
{   
    //�ж�s�г��ֵ��ַ��Ƿ�Ϊ�Ϸ��ַ�(ͬ�������ж��Ƿ��д���������� @ �����)
    //ֻ��Ϊ�������(+ - * /);������(���ֺ�δ֪��(�˴�Ϊ��ĸ));�������� 
	for(int i=0;i<s.size();i++)
	{
		if(s[i]=='('||s[i]==')') continue;
		if( IsNumber(s[i]) ) continue;
		if( IsAlpha(s[i]) ) continue;
		if(IsOperator(s[i])) continue;
		cout<<"��Ϊ�Ϸ��ַ�:"<<s[i]<<endl;
		return false;		
    }
    
    //���Ų�ƥ��
    LinkStack<char> t1;
    for(int i=0;i<s.size();i++)
    {
    	if(s[i]=='(') t1.Add(s[i]);
    	if(s[i]==')')
    	{
    		if(t1.GetTop()!='(')
    		{
    			//cout<<"���Ų�ƥ�䣺����������������"<<endl;
    			return false;
			}
    		t1.Delete();
		}
	}
	if(!t1.IsEmpty())
	{
		//cout<<"���Ų�ƥ�䣺����������������"<<endl;
		return false;
	}
	
	//�����������﷨���� 
	for(int i=1;i<=s.size();i++)
	{
		if(i==s.size()-1)
	    {
		    if(IsOperator(s[i]))
		    {
		    	//cout<<"��β�ַ�Ϊ�����"<<endl;
				return false;
			}
	    }
		if( IsNumber(s[i]) )
		{
			if(!( IsNumber(s[i-1]) ))
			{
				if(s[i-1]=='(') continue;//��������ǰΪ(��Ϊ�Ϸ��ģ��������б���
				if( IsOperator(s[i-1]) ) continue;//��������ǰΪ�����Ҳ�� 
				//cout<<"����ǰ�����:"<<s[i]<<endl; 
				return false;//���ĳ��������ǰΪ)ʱ�����������ź����Ҫ����������ܺ�Ӳ����� 
			}
		}
		if( IsOperator(s[i]) )
		{
			if(IsOperator(s[i-1]))
			{
				//cout<<"������ǰ������: "<<s[i]<<endl; 
				return false;//��������ǰһ��ҲΪ�������Ϊ�Ƿ�����
			}
			if(s[i-1]=='(')
			{
				//cout<<"������ǰΪ������"<<endl;
				return false; 
			}
			continue;  //�������Ϊ�Ϸ����� 
		}
		if( IsAlpha(s[i]) )
		{
			if(IsOperator(s[i-1])) continue;//δ֪��ǰһ��Ϊ���������ǿ��е� 
			if(s[i-1]=='(') continue;
			//cout<<"δ֪��ǰ����: "<<s[i]<<endl;
			return false;
		}
		if(s[i]=='(')
		{
			if(IsOperator(s[i-1])) continue;
			if(s[i-1]=='(') continue;
			//cout<<"(ǰ����"<<endl; 
			return false;
		}
		if(s[i]==')')
		{
			if(IsAlpha(s[i-1])) continue;
			if(IsNumber(s[i-1])) continue;
			if(s[i-1]==')') continue;
			//cout<<")ǰ����"<<endl;
			return false;
		}
	}
	//���Ͼ�ͨ��˵���Ϸ� 
	return true; 	
}

int pre(char s)
{
	if(s=='+'||s=='-') return 1;
	if(s=='*'||s=='/') return 2;
	return 0;
}


string GetNewNumber(string l,char opr,string r)
{
	//��ֵΪtmp1 
	int tmp1=0;
	for(int i=0;i<l.size();i++)
	{
		tmp1=tmp1*10+l[i]-'0';
	}
	//��ֵΪtmp2 
	int tmp2=0;
	for(int i=0;i<r.size();i++)
	{
		tmp2=tmp2*10+r[i]-'0';
	}
	double val=0;
	if(opr=='+') val=1.0*tmp1+1.0*tmp2;
	if(opr=='-') val=1.0*tmp1-1.0*tmp2;
	if(opr=='*') val=1.0*(tmp1*tmp2);
	if(opr=='/') val=1.0*tmp1/(1.0*tmp2);
	
    std::ostringstream oss;
    oss << val;  
    std::string str = oss.str();
	return str;
}

string GetAnswer(string s)
{
	LinkStack<string> number;
	LinkStack<char> opr;
	for(int i=0;i<s.size();i++)
	{
		char c=s[i];
		if(IsNumber(c))
		{
			string num;
            while(i < s.size()&&(IsNumber(s[i])))
            {
                num = num + s[i++];
            }
            i--;
            number.Add(num);			
		}
		else if(IsAlpha(c))
		{
			string tmp(1,c);
			number.Add(tmp);
		}
		else if(c=='(')
		{
			opr.Add(c);
		}
		else if(c==')')
		{
			while(!opr.IsEmpty()&&opr.GetTop()!='(')  //�����ջ�в�Ϊ����δ������������ 
			{
				string r=number.GetTop();number.Delete();
				string l=number.GetTop();number.Delete();
				char oper=opr.GetTop();opr.Delete();
				if(string_IsAlpha(r)||string_IsAlpha(l)) 
				{
					string tmp=l+oper+r;
					number.Add(tmp);
				}
				else
				{
					string tmp=GetNewNumber(l,oper,r);
					number.Add(tmp);
				}
			}
			opr.Delete();//����(δ���뵽 whileѭ���У������Ӧ��ɾ�� 
		}
		else if(IsOperator(c))
		{
			if(opr.IsEmpty()) opr.Add(c);
			else if(pre(opr.GetTop()) < pre(c)) opr.Add(c);
			else
			{
			    while(pre(opr.GetTop()) >= pre(c)) //t�����ȼ���c�� 
	    		{
	    			char oper=opr.GetTop();
		    		opr.Delete();
		    		string r=number.GetTop();number.Delete();
				    string l=number.GetTop();number.Delete();
				    if(string_IsAlpha(r)||string_IsAlpha(l)) 
				    {
					    string tmp=l+oper+r;
					    number.Add(tmp);
				    }
				    else
				    {
					    string tmp=GetNewNumber(l,oper,r);
					    number.Add(tmp);
				    }				
		       	}
			    opr.Add(c);	
		    }
		}
	}
	while(!opr.IsEmpty())
	{
		char oper=opr.GetTop();
		opr.Delete();
		string r=number.GetTop();number.Delete();
		string l=number.GetTop();number.Delete();
		if(string_IsAlpha(r)||string_IsAlpha(l)) 
		{
			string tmp=l+oper+r;
			number.Add(tmp);
		}
		else
		{
			string tmp=GetNewNumber(l,oper,r);
			number.Add(tmp);
		}				
	}
	return number.GetTop();
}

bool IsUnknowed(string s)
{
	for(int i=0;i<s.size();i++)
	{
		if(IsAlpha(s[i]))
		{
			return true;
		}
	}
	return false;
}

//
string New_GetAnswer(string s)
{
	LinkStack<string> number;
	LinkStack<char> opr;
	for(int i=0;i<s.size();i++)
	{
		char c=s[i];
		if(IsNumber(c))
		{
			string num;
            while(i < s.size()&&(IsNumber(s[i])))
            {
                num = num + s[i++];
            }
            i--;
            number.Add(num);			
		}
		else if(IsAlpha(c))
		{
			string tmp(1,c);
			number.Add(tmp);
		}
		else if(c=='(')
		{
			opr.Add(c);
		}
		else if(c==')')
		{
			while(!opr.IsEmpty()&&opr.GetTop()!='(')  //�����ջ�в�Ϊ����δ������������ 
			{
				string r=number.GetTop();number.Delete();
				string l=number.GetTop();number.Delete();
				char oper=opr.GetTop();opr.Delete();
				if(string_IsAlpha(r)&&string_IsAlpha(l))  //�����ַ��������� 
				{
					string tmp=l+oper+r;
					number.Add(tmp);
				}
				else if( (string_IsAlpha(r)) && l!=str0 && l!=str1 ) 
				{
					string tmp=l+oper+r;
					number.Add(tmp);
				}
				else if( (string_IsAlpha(l)) && r!=str0 && r!=str1 )
				{
					string tmp=l+oper+r;
					number.Add(tmp);
				}
				else if(string_IsAlpha(r)&&l==str0)//Ϊ��0 oper r 
				{
					if(oper=='+') number.Add(r);
					else if(oper=='/')
					{
						string tmp(1,'0');
						number.Add(tmp);
					}
					else if(oper=='*')
					{
						string tmp(1,'0');
						number.Add(tmp);
					}
					else
					{
						string tmp=l+oper+r;
					    number.Add(tmp);
					}
				}
				else if(string_IsAlpha(l)&&r==str0)//Ϊ��l oper 0 
				{
					if(oper=='+') number.Add(l);
				    else if(oper=='-') number.Add(l);
					else if(oper=='/') cout<<"���Ϸ������������Խ��г���0����";
					else if(oper=='*')
					{
						string tmp(1,0);
						number.Add(tmp);
					}
				}
				else if(string_IsAlpha(r)&&l==str1)//Ϊ��1 oper r 
				{
					if(oper=='*')
					{
						number.Add(r);
					}
					else
					{
						string tmp=l+oper+r;
				    	number.Add(tmp);
					}
				}
				else if(string_IsAlpha(l)&&r==str1)//Ϊ��l oper 1 
				{
					if(oper=='*')
					{
						number.Add(l);
					}
					else
					{
						string tmp=l+oper+r;
						number.Add(tmp);
					}
				}
				else  //�������ֽ������� 
				{
					string tmp=GetNewNumber(l,oper,r);
					number.Add(tmp);
				}
			}
			opr.Delete();//����(δ���뵽 whileѭ���У������Ӧ��ɾ�� 
		}
		else if(IsOperator(c))
		{
			if(opr.IsEmpty()) opr.Add(c);
			else 
			while(pre(opr.GetTop()) >= pre(c)) //t�����ȼ���c�� 
			{
				char oper=opr.GetTop();
				opr.Delete();
				string r=number.GetTop();number.Delete();
				string l=number.GetTop();number.Delete();
				if(string_IsAlpha(r)&&string_IsAlpha(l)) 
				{
					string tmp=l+oper+r;
					number.Add(tmp);
				}
				else if( (string_IsAlpha(r)) && l!=str0 && l!=str1 ) 
				{
					string tmp=l+oper+r;
					number.Add(tmp);
				}
				else if( (string_IsAlpha(l)) && r!=str0 && r!=str1 )
				{
					string tmp=l+oper+r;
					number.Add(tmp);
				}
				else if(string_IsAlpha(r)&&l==str0)//Ϊ��0 oper r 
				{
					if(oper=='+') number.Add(r);
					else if(oper=='/')
					{
						string tmp(1,'0');
						number.Add(tmp);
					}
					else if(oper=='*')
					{
						string tmp(1,'0');
						number.Add(tmp);
					}
					else
					{
						string tmp=l+oper+r;
						number.Add(tmp);
					}
				}
				else if(string_IsAlpha(l)&&r==str0)//Ϊ��l oper 0 
				{
					if(oper=='+') number.Add(l);
				    else if(oper=='-') number.Add(l);
					else if(oper=='/') cout<<"���Ϸ������������Խ��г���0����";
					else if(oper=='*')
					{
						string tmp(1,'0');
						number.Add(tmp);
					}
				}
				else if(string_IsAlpha(r)&&l==str1)//Ϊ��1 oper r 
				{
					if(oper=='*')
					{
						number.Add(r);
					}
					else
					{
						string tmp=l+oper+r;
						number.Add(tmp);
					}
				}
				else if(string_IsAlpha(l)&&r==str1)//Ϊ��l oper 1 
				{
					if(oper=='*')
					{
						number.Add(l);
					}
					else
					{
						string tmp=l+oper+r;
						number.Add(tmp);
					}
				}
				else
				{
					string tmp=GetNewNumber(l,oper,r);
					number.Add(tmp);
				}				
			}
			opr.Add(c);	
		}
	}
	while(!opr.IsEmpty())
	{
		char oper=opr.GetTop();
		opr.Delete();
		string r=number.GetTop();number.Delete();
		string l=number.GetTop();number.Delete();
		if(string_IsAlpha(r)||string_IsAlpha(l)) 
		{
			string tmp=l+oper+r;
			number.Add(tmp);
		}
		else if(string_IsAlpha(r)&&l==str0)//Ϊ��0 oper r 
	    {
		if(oper=='+') number.Add(r);
		else if(oper=='/')
		{
			string tmp(1,0);
			number.Add(tmp);
		}
		else if(oper=='*')
		{
	        string tmp(1,0);
	    	number.Add(tmp);
		}
        }
		else if(string_IsAlpha(l)&&r==str0)//Ϊ��l oper 0 
		{
			if(oper=='+') number.Add(l);
			else if(oper=='-') number.Add(l);
			else if(oper=='/') cout<<"���Ϸ������������Խ��г���0����";
			else if(oper=='*')
			{
				string tmp(1,0);
				number.Add(tmp);
			}
		}
		else if(string_IsAlpha(r)&&l==str1)//Ϊ��1 oper r 
		{
			if(oper=='*')
			{
				number.Add(r);
			}
		}
		else if(string_IsAlpha(l)&&r==str1)//Ϊ��l oper 1 
		{
			if(oper=='*')
			{
				number.Add(l);
			}
		}
		else
		{
			string tmp=GetNewNumber(l,oper,r);
			number.Add(tmp);
		}				
	}
	return number.GetTop();
}


//
bool Is01Exit(string s)
{
	for(int i=0;i<s.size();i++)
	{
		if(s[i]=='0'||s[i]=='1') return true;
	}
	return false;
}


int main()
{
	string expression;
	getline(cin,expression);
	if(checkExpression(expression)) cout<<"True"<<endl;
    else
    {
    	cout<<"False"<<endl;
    	return 0;
	}
	string tmp=GetAnswer(expression);
	for(int i=0;i<tmp.size();i++)
	{
		cout<<tmp[i];
	}
	cout<<endl;
	string tmp2="";
	if(IsUnknowed(tmp))
    {
		if(!Is01Exit(tmp))
		{
			tmp2=tmp;
		}
		else
		{
			tmp2=New_GetAnswer(expression);
		}
	}
	else tmp2=tmp;
	cout<<tmp2;		
}
//((3+4*(2-1))/5+x+0*y)
//3+4*(2-1)+
