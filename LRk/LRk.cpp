// LRk.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <string>


using namespace std;


typedef struct {
	char N_terminal;
	string b_dot;
	string a_dot;
	string avan;
} LR_S;

map <char, vector<string>> rules;
set<char> N, T;
int k;
char Ax;

vector<pair<char, string>> rules_enum;

map<char, set<string>> First_nt;

map<int,map<char, int>> GoTo;
map<int, map<string, set<string>>> Action;

vector<set<LR_S>> States;

bool operator< (const LR_S arg1, const LR_S arg2) {
	if (arg1.N_terminal < arg2.N_terminal)
		return true;
	else {
		if (arg1.N_terminal > arg2.N_terminal)
			return false;
		else {
			if (arg1.b_dot < arg2.b_dot)
				return true;
			else {
				if (arg1.b_dot > arg2.b_dot)
					return false;
				else {
					if (arg1.a_dot < arg2.a_dot)
						return true;
					else {
						if (arg1.a_dot > arg2.a_dot)
							return false;
						else {
							if (arg1.avan < arg2.avan)
								return true;
							else
								return false;
						}
					}
				}
			}

		}
	}
}

bool operator== (const LR_S arg1, const LR_S arg2) {
	if (arg1.N_terminal == arg2.N_terminal) {
		if (arg1.b_dot == arg2.b_dot) {
			if (arg1.a_dot == arg2.a_dot) {
				return arg1.avan == arg2.avan;
			}
		}
	}
	return false;
}


set<char> GetSet() {
	set<char> result;
	while (cin.peek() != '\n') {
		string t;
		cin >> t;
		result.insert(t[0]);
	}
	return result;
}

map<char, vector<string>> ReadRules(int n) {
	map<char, vector<string>> rules;
	for (int i = 0; i < n; i++) {
		char N;
		string r;
		cin >> N;
		cin.ignore(2);
		cin >> r;
		rules[N].push_back(r);
		cin.ignore();
	}

	rules_enum.push_back({ 'Z', string(1, Ax) });
	for (auto k : rules) {
		for (auto j : k.second) {
			rules_enum.push_back({ k.first, j });
		}
	}
	return rules;
}

//overloading operator to implement "+ in circle"
set<string> operator+(set<string> s1, set<string> s2) {
	set<string> result;
	for (auto i : s1) {
		
		for (auto l : s2) {
			string w;
			if (i == "e") {
				w = l;
			}
			else 
				if (l == "e"){
					w = i;
				}
				else {
					w = i + l;
				}
			result.insert(w.substr(0, k));
		}
		if (s2.empty()) {
			result.insert(i.substr(0, k));
		}
	}
	if (s1.empty()) {
		for (auto l : s2) {
			result.insert(l.substr(0, k));
		}
	}
	return result;
}


//First_k, described in Serebryakov
map<char,set<string>> First_NT() {
	map<char, set<string>> result0;
	map<char, set<string>> result1;
	for (auto i : T) {
		result1[i].insert(string( 1, i));
	}
	for (auto i : N) {
		for (auto l : rules[i]) {
			string l1 = l.substr(0, k);
			bool check = false;
			for (int m = 0; m < l1.length(); m++){
				if (N.find(l1[m]) != N.end()) {
					check = true;
					break;
				}
			}
			if (!check) {
				result1[i].insert(l1);
			}
		}
	}
	do {
		result0 = result1;
		for (auto i : N) {
			for (auto l : rules[i]) {
				set<string> r_sum;
				for (int m = 0; i < l.length(); i++) {
					r_sum = r_sum + result0[l[m]];
				}
				result1[i].insert(r_sum.begin(), r_sum.end());
			}
		}
	} while (result0 != result1);

	for (auto i : result1) {
		cout << i.first << ": ";
		for (auto m : i.second) {
			cout << m << " ";
		}
		cout << endl;
	}
	return result1;
}

set<string> First_S(string s) {
	set<string> result;
	for (auto i : s) {
		result = result + First_nt[i];
	}
	return result;
}

set<string> EFF1(char n) { 
	set<string> result;

	for (auto i : rules[n]) {
		set<string> temp;
		if (N.find(i[0]) == N.end() && i != "e") {
			temp = First_S(i);
		}
		else {
			if (N.find(i[0]) != N.end()) {
				temp = EFF1(i[0]) + First_S(i.substr(1, i.length()));
			}
		}
		result.insert(temp.begin(), temp.end());
	}

	return result;
}


set<string> EFF(string s) {
	set<string> result;
	if (N.find(s[0]) == N.end()) {
		return First_S(s);
	}
	else 
	{
		if (s != "e") {
			return EFF1(s[0]) + First_S(s.substr(1, s.length()));
		}
	}
}



set<LR_S> Closure(LR_S init) {
	set<LR_S> res1;
	set<LR_S> res0;
	res1.insert(init);
	do {
		res0 = res1;
		for (auto i : res0) {
			if (N.find(i.a_dot[0]) != N.end()) {

				for (auto l : First_S((i.a_dot + i.avan).substr(1, (i.a_dot + i.avan).length()))) {
					for (auto m: rules[i.a_dot[0]]){
						res1.insert({ i.a_dot[0], string() , m, l });
						}
				}
			}
		}
	} while (res0 != res1);
	return res1;
}


vector<set<LR_S>> Automata() {
	int current = 0;
	vector<set<LR_S>> result;
	map<char, set<LR_S>> calculations;
	
	result.push_back(Closure({ 'Z', "", string(1,Ax), string(1,'$') }));
	
	set<char> NT;
	NT.insert(N.begin(), N.end());
	NT.insert(T.begin(), T.end());
	while (current <= result.size() - 1) {
		for (auto n : NT) {
			for (auto s : result[current]) {
				if (s.a_dot[0] == n) {
					set<LR_S> temp = Closure({ s.N_terminal, s.b_dot + string(1, s.a_dot[0]), s.avan });
					calculations[n].insert(temp.begin(), temp.end());
				}
			}
		}

		for (auto c : calculations) {
			ptrdiff_t pos = find(result.begin(), result.end(), c.second) - result.begin();
			if (pos >= result.size()) {
				result.push_back(c.second);
				GoTo[current][c.first] = find(result.begin(), result.end(), c.second) - result.begin();
			}
			else {
				GoTo[current][c.first] = pos;
			}
		}
		current++;
	}

	return result;
}

map<int, map<string, set<string>>> CreateAction() {
	map<int, map<string, set<string>>> result;
	for (int i = 0; i < States.size(); i++) {
		for (auto l : States[i]) {
			if (l.a_dot != "") {
				for (auto u : EFF(l.a_dot + l.avan)) {
					result[i][u].insert("Shift");
				}
			}
			else {
				if (l.a_dot == "" && l.avan != "$") {
					int n = 0;
					pair<char, string> cur_rule = { l.N_terminal, l.b_dot };
					while (1) {
						if (cur_rule == rules_enum[n]) {
							break;
						}
						else {
							n++;
						}
					}
					result[i][l.avan].insert("Reduce" + to_string(n));
				}
				else {
					if (l.a_dot == "" && l.avan == "$") {
						result[i][l.avan].insert("Accept");
					}
				}

			}
		}
	}
	return result;
}


bool Check() {
	for (auto i : Action) {
		for (auto l : i.second) {
			if (l.second.size() > 1) {
				return false;
			}
		}
	}
	return true;
}


int main()
{

	cout << "Enter k: ";
	cin >> k;
	cin.ignore();
	cout << "Enter the nonterminal symbols: ";
	N = GetSet();
	cin.ignore();
	cout << "Enter the terminal symbols: ";
	T = GetSet();
	cin.ignore();
	cout << "Enter the start symbol: ";
	cin >> Ax;
	cin.ignore();
	cout << "Enter the number of rules: ";
	int n;
	cin >> n;
	cin.ignore();
	cout << "Enter the rules: \n";
	rules = ReadRules(n);

	

	First_nt = First_NT();

	States = Automata();
	Action = CreateAction();

	if (Check()) {
		cout << "This is LR(k) grammar\n";
	}
	else {
		cout << "This is not LR(k) grammar\n";
	}



	while (1);
    return 0;
}