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


bool checkExpression(string s) //判断某个表达式是否合法
{   
    //判断s中出现的字符是否为合法字符(同样起到了判断是否有错误运算符如 @ 的情况)
    //只能为：运算符(+ - * /);操作数(数字和未知数(此处为字母));左右括号 
	for(int i=0;i<s.size();i++)
	{
		if(s[i]=='('||s[i]==')') continue;
		if( IsNumber(s[i]) ) continue;
		if( IsAlpha(s[i]) ) continue;
		if(IsOperator(s[i])) continue;
		cout<<"不为合法字符:"<<s[i]<<endl;
		return false;		
    }
    
    //括号不匹配
    LinkStack<char> t1;
    for(int i=0;i<s.size();i++)
    {
    	if(s[i]=='(') t1.Add(s[i]);
    	if(s[i]==')')
    	{
    		if(t1.GetTop()!='(')
    		{
    			//cout<<"括号不匹配：有右括号无左括号"<<endl;
    			return false;
			}
    		t1.Delete();
		}
	}
	if(!t1.IsEmpty())
	{
		//cout<<"括号不匹配：有左括号无右括号"<<endl;
		return false;
	}
	
	//运算符错误或语法错误 
	for(int i=1;i<=s.size();i++)
	{
		if(i==s.size()-1)
	    {
		    if(IsOperator(s[i]))
		    {
		    	//cout<<"结尾字符为运算符"<<endl;
				return false;
			}
	    }
		if( IsNumber(s[i]) )
		{
			if(!( IsNumber(s[i-1]) ))
			{
				if(s[i-1]=='(') continue;//若操作数前为(，为合法的，继续进行遍历
				if( IsOperator(s[i-1]) ) continue;//若操作数前为运算符也可 
				//cout<<"数字前面错误:"<<s[i]<<endl; 
				return false;//如果某个操作数前为)时，由于右括号后必须要有运算符才能后接操作数 
			}
		}
		if( IsOperator(s[i]) )
		{
			if(IsOperator(s[i-1]))
			{
				//cout<<"操作符前操作符: "<<s[i]<<endl; 
				return false;//如果运算符前一个也为运算符，为非法操作
			}
			if(s[i-1]=='(')
			{
				//cout<<"操作符前为左括号"<<endl;
				return false; 
			}
			continue;  //其余情况为合法操作 
		}
		if( IsAlpha(s[i]) )
		{
			if(IsOperator(s[i-1])) continue;//未知数前一个为操作数，是可行的 
			if(s[i-1]=='(') continue;
			//cout<<"未知数前错误: "<<s[i]<<endl;
			return false;
		}
		if(s[i]=='(')
		{
			if(IsOperator(s[i-1])) continue;
			if(s[i-1]=='(') continue;
			//cout<<"(前错误"<<endl; 
			return false;
		}
		if(s[i]==')')
		{
			if(IsAlpha(s[i-1])) continue;
			if(IsNumber(s[i-1])) continue;
			if(s[i-1]==')') continue;
			//cout<<")前错误"<<endl;
			return false;
		}
	}
	//以上均通过说明合法 
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
	//左值为tmp1 
	int tmp1=0;
	for(int i=0;i<l.size();i++)
	{
		tmp1=tmp1*10+l[i]-'0';
	}
	//右值为tmp2 
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
			while(!opr.IsEmpty()&&opr.GetTop()!='(')  //运算符栈中不为空且未遍历到左括号 
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
			opr.Delete();//由于(未进入到 while循环中，在最后应该删除 
		}
		else if(IsOperator(c))
		{
			if(opr.IsEmpty()) opr.Add(c);
			else if(pre(opr.GetTop()) < pre(c)) opr.Add(c);
			else
			{
			    while(pre(opr.GetTop()) >= pre(c)) //t的优先级比c高 
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
			while(!opr.IsEmpty()&&opr.GetTop()!='(')  //运算符栈中不为空且未遍历到左括号 
			{
				string r=number.GetTop();number.Delete();
				string l=number.GetTop();number.Delete();
				char oper=opr.GetTop();opr.Delete();
				if(string_IsAlpha(r)&&string_IsAlpha(l))  //两个字符进行运算 
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
				else if(string_IsAlpha(r)&&l==str0)//为：0 oper r 
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
				else if(string_IsAlpha(l)&&r==str0)//为：l oper 0 
				{
					if(oper=='+') number.Add(l);
				    else if(oper=='-') number.Add(l);
					else if(oper=='/') cout<<"不合法操作，不可以进行除以0操作";
					else if(oper=='*')
					{
						string tmp(1,0);
						number.Add(tmp);
					}
				}
				else if(string_IsAlpha(r)&&l==str1)//为：1 oper r 
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
				else if(string_IsAlpha(l)&&r==str1)//为：l oper 1 
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
				else  //两个数字进行运算 
				{
					string tmp=GetNewNumber(l,oper,r);
					number.Add(tmp);
				}
			}
			opr.Delete();//由于(未进入到 while循环中，在最后应该删除 
		}
		else if(IsOperator(c))
		{
			if(opr.IsEmpty()) opr.Add(c);
			else 
			while(pre(opr.GetTop()) >= pre(c)) //t的优先级比c高 
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
				else if(string_IsAlpha(r)&&l==str0)//为：0 oper r 
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
				else if(string_IsAlpha(l)&&r==str0)//为：l oper 0 
				{
					if(oper=='+') number.Add(l);
				    else if(oper=='-') number.Add(l);
					else if(oper=='/') cout<<"不合法操作，不可以进行除以0操作";
					else if(oper=='*')
					{
						string tmp(1,'0');
						number.Add(tmp);
					}
				}
				else if(string_IsAlpha(r)&&l==str1)//为：1 oper r 
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
				else if(string_IsAlpha(l)&&r==str1)//为：l oper 1 
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
		else if(string_IsAlpha(r)&&l==str0)//为：0 oper r 
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
		else if(string_IsAlpha(l)&&r==str0)//为：l oper 0 
		{
			if(oper=='+') number.Add(l);
			else if(oper=='-') number.Add(l);
			else if(oper=='/') cout<<"不合法操作，不可以进行除以0操作";
			else if(oper=='*')
			{
				string tmp(1,0);
				number.Add(tmp);
			}
		}
		else if(string_IsAlpha(r)&&l==str1)//为：1 oper r 
		{
			if(oper=='*')
			{
				number.Add(r);
			}
		}
		else if(string_IsAlpha(l)&&r==str1)//为：l oper 1 
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
