// gramap.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <algorithm>
#include <deque>
#include <ctime>

using namespace std;

ifstream f("date.in");

int p; // nr productii;
	   // e=epsilon,lambda,alfa;
struct cstr
{
	char c;
	string s;

};

string neterm;
string term;
unordered_map <char, vector <string>> gramatica;
deque<char>nullables;
vector<cstr>nullfixneed;
vector<char> units;
vector<char> termrpl;
vector<string> added;
vector<cstr>::iterator nf;
unordered_map <char, vector <string>>::iterator ir;
unordered_map < char, vector<string>>::iterator iterz;
int istop, r_m, r_i, r_t;

int isNeterm(char X)
{
	for (unsigned int i = 0; i < neterm.size(); ++i)
	{
		if (neterm[i] == X)
			return 1;
		
	}
	return 0;
}

int isTerm(char X)
{
	for (unsigned int i = 0; i < term.size(); ++i)
	{
		if (term[i] == X)
			return 1;
		
	}
	return 0;
}

int isValidRhs(string rhs)
{
	int flag=0;
	for (unsigned int i = 0; i <rhs.size(); ++i)
	{
		if (isNeterm(rhs[i]) || isTerm(rhs[i]))
			flag = 1;
		else
		{
			flag = 0;
			break;
		}
		
	}
	return flag;
}

void addterm(char X)
{
	term = term + X;

}
void addrule(char X, string rhs)
{

	gramatica[X].push_back(rhs);
}
void read()
{
	char X;
	string rhs;
	f >> p;
	f >> term;
	f >> neterm;
	for (int i = 0; i < p; i++)
	{
		f >> X >> rhs;
		if (isTerm(X) && isValidRhs(rhs))
			addrule(X, rhs);
		
	}
}

void print()
{
	std::unordered_map <char, vector <string>>::iterator it;
	for (it = gramatica.begin(); it != gramatica.end(); ++it)
	{
		cout << it->first << ": ";
		for (std::vector<string>::iterator i = it->second.begin(); i != it->second.end(); ++i)
			cout << *i << "|";
		cout << endl;
	}
}
int IsInNullables(char X)
{
	for (unsigned int i = 0; i < nullables.size(); ++i)
			if (nullables[i] == X) return 1;

	return 0;
}
int IsInNullfix(cstr arg)
{
	for (unsigned int i = 0; i < nullfixneed.size();++i)
			if (nullfixneed[i].c == arg.c && nullfixneed[i].s==arg.s) return 1;
	
	return 0;

}
int IsInUnits(char X)
{
	for (unsigned int i = 0; i < units.size(); ++i)
	if (units[i] == X) return 1;

	return 0;

}
void isNullable()
{
	std::unordered_map <char, vector <string>>::iterator it;
	for (it = gramatica.begin(); it != gramatica.end(); ++it)
	{
		for (std::vector<string>::iterator i = it->second.begin(); i != it->second.end(); ++i)
		if ((*i) == "e")
		{
			cout << "regula cu lambda: ";
			cout << it->first << "->" << *i << endl;
			nullables.push_back(it->first);
			break;

		}
	}

}

bool Isnullable(char c)
{
	if (IsInNullables(c)) return true;
	return false;
}

int IsinRule(char c, string s)
{
	unordered_map <char, vector <string>>::iterator tp;
	tp=gramatica.find(c);
	if (tp == gramatica.end()) return 0;
	for (vector<string>::iterator i = tp->second.begin(); i != tp->second.end();++i)
		if (*i == s) return 1;
	return 0;

}

void spargestring(cstr param)
{
	std::string str1;
	
	str1 = param.s;
	for (unsigned int i = 0; param.s[i] != 0; ++i)
	{
		if (IsInNullables(param.s[i]))
		{

			param.s.erase(i, 1);
			if (param.s.empty())
				addrule(param.c, "e");
			else
			if (!(IsinRule(param.c,param.s)))
				addrule(param.c, param.s);
		}
		param.s=str1;
	}
	param.s.erase(remove_if(param.s.begin(), param.s.end(), &Isnullable), param.s.end());
	if ( !(param.s.size() == 1 && param.s[0] == (param.c)) )
		if (!(param.s.size() == 0))
			if ( !(IsinRule(param.c, param.s)) )
				addrule(param.c, param.s);
}

void erules()
{
	
	
	char c;
	isNullable(); // aflu regulile cu lambda si le pastrez in nullables
	do
	{
		//isNullable(); // aflu regulile cu lambda si le pastrez in nullables
		cout << endl;
		std::unordered_map <char, vector <string>>::iterator it;
		for (it = gramatica.begin(); it != gramatica.end(); ++it) // aflu regulile ce contin o regula nullable si le pastrez in nullfixneed
		{
			std::vector<char>::iterator n;

			for (std::vector<string>::iterator i = it->second.begin(); i != it->second.end(); ++i)
			{

				std::string::iterator s;
				for (s = (*i).begin(); s != (*i).end(); ++s)
				{
					c = (*s);
					cstr tmp;
					tmp.c = it->first;
					tmp.s = *i;

					if (IsInNullables(c))
					{
						if (!IsInNullfix(tmp))
						{
							cstr cs;
							cs.c = it->first;
							cs.s = *i;
							nullfixneed.push_back(cs);
						}

					}
				}

			}
		}


		for (nf = nullfixneed.begin(); nf != nullfixneed.end(); ++nf) // pentru fiecare regula ce contine o regula nullable, adauga o regula noua pentru fiecare varianta fara lambda
		{
			cstr tmp1;
			tmp1.c = nf->c;
			tmp1.s = nf->s;
			spargestring(tmp1);

		}
		
		std::unordered_map<char, vector<string>>::iterator de;
		for (unsigned int i = 0; i < nullables.size(); ++i)
		{

			de = gramatica.find(nullables[i]);
			for (vector<string>::iterator it = de->second.begin(); it != de->second.end(); ++it)
			{
				if ((*it) == "e")
				{
					de->second.erase(it);		// trebuie sa stearga toti e-ii, nu merge
					break;
				}
			}

		}
		nullables.pop_front();
		isNullable(); // verific daca au aparut alte reguli nullable
		
	} while (!nullables.empty());
	
}

void unitrulez()
{
	string c;
	unordered_map <char, vector <string>>::iterator it, is;
	for (it=gramatica.begin(); it != gramatica.end(); ++it)
	{
		
		if (it->second.size() == 1 && it->second[0].size()==1 && isTerm((it->second[0])[0]))
		{
			units.push_back(it->first);
			c = it->second[0]; // ='C'
			
			iterz = gramatica.find(c[0]); // caut regulile cu C 
			for (vector<string>::size_type i = 0; i != iterz->second.size(); ++i)
			if (iterz->second[i].size()>=1)
				addrule(it->first, iterz->second[i]);
			
		}
		else
		{
			
			for (vector<string>::size_type i = 0; i != it->second.size(); ++i)
			{
				if ((it->second)[i].size() == 1 && isTerm((it->second[i])[0]))
				{//cout <<"cAZde2"<< (it->second)[i];
					if (!IsInUnits(it->first))
					units.push_back(it->first);
					c = (it->second[i]); // ='C'

					iterz = gramatica.find(c[0]); // caut regulile cu C                | iterz->second.size()== istop|
					istop = iterz->second.size();								   //  ^	                          ^
					for (vector<string>::size_type i = 0; i < iterz->second.size() && !IsinRule(it->first,iterz->second[i]); ++i)
					{
						if (iterz->second[i].size() > 1)
							addrule(it->first, iterz->second[i]);
						if (iterz->second[i].size() == 1 && !isTerm(iterz->second[i][0]))
							addrule(it->first, iterz->second[i]);
					}
				}

			}
		}
		
	}
	for (unsigned int i = 0; i < units.size(); ++i)
	{
		unordered_map<char, vector<string>>::iterator f;
		
		f=gramatica.find(units[i]);
		for (int i = 1; i <= 3; i++)
		{
			for (vector<string>::iterator it = f->second.begin(); it != f->second.end(); ++it)
			{
				if ((*it).size() == 1 && isTerm((*it)[0]))
				{
					f->second.erase(it);
					break;
				}
			}
		}
	}
	
}

int isintermrpl(char X)
{
	for (vector<char>::size_type i = 0; i != termrpl.size();++i)
	if (termrpl[i] == X) return 1;
	return 0;
}

int IsAdded(string X)
{
	int j=0;
	for (unsigned int i = 0; i < added.size(); ++i)
	if (added[i] == X) j++;
	return j;

}
void replace()
{
	unordered_map <char, vector <string>>::iterator it, is;
																	// ma ocup de regulile gen ASB
	for (it = gramatica.begin(); it != gramatica.end(); ++it)
	{

		for (vector<string>::size_type i = 0; i != it->second.size(); ++i)
		{
			if (it->second[i].size() == 4)
			{
				string cm = "123456789";
				//r_m = (rand() % 8) + 1;
				string rp, r;
				
				rp.push_back(cm[r_m]);
				
				r.append(it->second[i], 1, 3);
				added.push_back(r);
				it->second[i].replace(1, 3, rp);
				addrule(cm[r_m], r);
				r_m++;
			}
		}
	}

	for (it = gramatica.begin(); it != gramatica.end(); ++it)
	{
		for (vector<string>::size_type i = 0; i != it->second.size(); ++i)
		{
			if (it->second[i].size() == 3)
			{
				string a_to_z = "5!@#$%";
				//r_i = (rand() % 9) + 1;
				string repl;
				string r;
			
				r.append(it->second[i], 1, 2);
				//added.push_back(r);
				
				repl.push_back(a_to_z[r_i]);
				addrule(a_to_z[r_i], r);
				it->second[i].replace(1, 2, repl);
				r_i++;
				/*if (added.size()==1)
					addrule(a_to_z[r_i], r);
				if (IsAdded(r)==1 && added.size()>1)
				 addrule(a_to_z[r_i], r);
				*/
			}
		}
		cout << endl;
	}
																	// ma ocup de problemele de genul aS unde fac XS
	for (it = gramatica.begin(); it != gramatica.end(); ++it)
	{
		for (vector<string>::size_type i = 0; i != it->second.size(); ++i)
		{
			for (string::size_type j = 0; j != it->second[i].size();++j)
			{

				if (isNeterm((it->second[i])[j]) && it->second[i].size()>=1 )
				{
					if ( !isintermrpl((it->second[i])[j]) ) 
					termrpl.push_back((it->second[i])[j]);
				}
				
			
			}
		} 
	}



	for (vector<char>::size_type  r= 0; r != termrpl.size(); ++r)
	{
		string a_to_z = "6789^&*{}";
		
		//r_t = (rand() % 8) + 1;
		string repl;
		//repl.push_back(a_to_z[r_t]);
		r_t++;
		string t;
		t = termrpl[r];
		repl.push_back(a_to_z[r_t]);
		
		for (it = gramatica.begin(); it != gramatica.end(); ++it)
		{
			for (vector<string>::size_type i = 0; i != it->second.size(); ++i)
			{
				for (string::size_type j = 0; j != it->second[i].size(); ++j)
				{

					if ((it->second[i])[j]==termrpl[r])
					{
						if ((it->second[i]).size()> 1)
						{
							string s;
							s.push_back(termrpl[r]);
							it->second[i].replace(it->second[i].find(s), it->second[i].find(s) + 1, repl);
						}
					}


				}
			}
		}
		addrule(repl[0], t);
	}


}
void replace2()
{
	unordered_map <char, vector <string>>::iterator it, is;
	// ma ocup de regulile gen ASB
	for (it = gramatica.begin(); it != gramatica.end(); ++it)
	{

		for (vector<string>::size_type i = 0; i != it->second.size(); ++i)
		{
			int rule_length = it->second[i].size();
			r_m = 0;
			string i_copy;
			i_copy = it->second[i];
			int start = 2;
			string ce = "123456789";
			string re;
			re.push_back(ce[r_m]);
			it->second[i].replace(1, rule_length - 1, re);
			r_m++;
			while (rule_length != 2)
			{
				string cm = "123456789";
				string rp, r;
				rp.push_back(cm[r_m]);

				r.append(i_copy, start, rule_length - 1);
				r.replace(1, rule_length - 1, rp);
				added.push_back(r);
				addrule(cm[r_m], r);
				r_m++;
				rule_length--;
				start++;
			}

		}
	}


	// ma ocup de problemele de genul aS unde fac XS
	for (it = gramatica.begin(); it != gramatica.end(); ++it)
	{
		for (vector<string>::size_type i = 0; i != it->second.size(); ++i)
		{
			for (string::size_type j = 0; j != it->second[i].size(); ++j)
			{

				if (isNeterm((it->second[i])[j]) && it->second[i].size() >= 1)
				{
					if (!isintermrpl((it->second[i])[j]))
						termrpl.push_back((it->second[i])[j]);
				}


			}
		}
	}



	for (vector<char>::size_type r = 0; r != termrpl.size(); ++r)
	{
		string a_to_z = "6789^&*{}";

		//r_t = (rand() % 8) + 1;
		string repl;
		//repl.push_back(a_to_z[r_t]);
		//r_t++;
		string t;
		t = termrpl[r];
		repl.push_back(toupper(termrpl[r]));

		for (it = gramatica.begin(); it != gramatica.end(); ++it)
		{
			for (vector<string>::size_type i = 0; i != it->second.size(); ++i)
			{
				for (string::size_type j = 0; j != it->second[i].size(); ++j)
				{

					if ((it->second[i])[j] == termrpl[r])
					{
						if ((it->second[i]).size()> 1)
						{
							string s;
							s.push_back(termrpl[r]);
							it->second[i].replace(it->second[i].find(s), it->second[i].find(s) + 1, repl);
						}
					}


				}
			}
		}
		addrule(repl[0], t);
	}


}
void transform()
{
	addterm('N');						//adaug un nou simbol initial
	gramatica['N'].push_back("S");
	erules();								//sterg regulile cu lambda
	unitrulez();							// sterg regulile de genu D->C
	replace();
}


int _tmain(int argc, _TCHAR* argv[])
{
	
	srand((unsigned)time(0));
	read();
	//addterm('N');						//adaug un nou simbol initial
	//gramatica['N'].push_back("S");
	print();
	cout << endl;
	erules();
	cout << endl;
	print();
	cout << endl;
	unitrulez();
	cout << endl;
	print();
	replace();
	//transform();
	print();
	cout << endl;
	
	system("pause");
	return 0;
}

