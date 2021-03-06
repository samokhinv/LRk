// LRk.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <stack>
#include <sstream>
#include <exception>
#include <algorithm>

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

map<int, set<string>>  Action0;

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
	N.insert('Z');
	T.insert('e');
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
				for (int m = 0; m < l.length(); m++) {
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






set<LR_S> Closure0(LR_S init) {
	set<LR_S> res1;
	set<LR_S> res0;
	res1.insert(init);
	do {
		res0 = res1;
		for (auto i : res0) {
			if (N.find(i.a_dot[0]) != N.end()) {
				for (auto m : rules[i.a_dot[0]]) {
					res1.insert({ i.a_dot[0], string() , m, "e" });
				}
			}
		}
	} while (res0 != res1);
	return res1;
}

set<LR_S> Closure(LR_S init) {
	if (k == 0) {
		return Closure0(init);
	}
	set<LR_S> res1;
	set<LR_S> res0;
	res1.insert(init);
	do {
		res0 = res1;
		for (auto i : res0) {
			if (N.find(i.a_dot[0]) != N.end()) {
				for (auto l : First_S((i.a_dot + i.avan).substr(1))) {
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

	N.insert('Z');
	result.push_back(Closure({ 'Z', "", string(1,Ax), string(1,'e') }));
	
	set<char> NT;
	NT.insert(N.begin(), N.end());
	NT.insert(T.begin(), T.end());
	while (current <= result.size() - 1) {
		map<char, set<LR_S>> calculations;
		for (auto n : NT) {
			for (auto s : result[current]) {
				if (s.a_dot[0] == n && s.a_dot!="e") {
					set<LR_S> temp = Closure({ s.N_terminal, s.b_dot + string(1, s.a_dot[0]), s.a_dot.substr(1), s.avan });
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


	for (int i = 0; i < result.size(); i++) {
		cout << i << endl;
		for (auto j : result[i]) {
			cout << ((j.N_terminal == 'Z')? "S'" : string(1,j.N_terminal)) << "->" << j.b_dot << "." << j.a_dot << "," << j.avan << endl;
		}
		cout << endl;
	}
	return result;
}

map<int, map<string, set<string>>> CreateAction() {
	map<int, map<string, set<string>>> result;
	for (int i = 0; i < States.size(); i++) {
		for (auto l : States[i]) {
			if (l.a_dot != "" && l.a_dot != "e") {
				//for (auto u : EFF(l.a_dot + l.avan)) {
				//result[i][u].insert("Shift");
				//}
				if (N.find(l.a_dot[0]) == N.end()) {
					for (auto u : First_S(l.a_dot + l.avan)) {
						result[i][u].insert("Shift");
					}
				}
			}
			else {
				if ((l.a_dot == "" || l.a_dot == "e") && l.N_terminal!='Z') {
					int n = 0;
					pair<char, string> cur_rule = { l.N_terminal, (l.b_dot == "" ? "e" : l.b_dot)};
					while (1) {
						if (rules_enum.size() == n || cur_rule == rules_enum[n]) {
							break;
						}
						else {
							n++;
						}
					}
					result[i][l.avan].insert("Reduce" + to_string(n));
				}
				else {
					if (l.N_terminal == 'Z' && l.a_dot == "" && l.avan == "e") {
						result[i][l.avan].insert("Accept");
					}
				}

			}
		}
	}
	return result;
}
map<int, set<string>> CreateAction0() {
	map<int, set<string>> result;
	for (int i = 0; i < States.size(); i++) {
		for (auto l : States[i]) {
			if (l.a_dot != "" && l.a_dot != "e") {
				if (N.find(l.a_dot[0]) == N.end()) {
					result[i].insert("Shift");
				}
			}
			else {
				if ((l.a_dot == "" || l.a_dot == "e") && l.N_terminal != 'Z') {
					int n = 0;
					pair<char, string> cur_rule = { l.N_terminal, (l.b_dot == "" ? "e" : l.b_dot) };
					while (1) {
						if (rules_enum.size() == n || cur_rule == rules_enum[n]) {
							break;
						}
						else {
							n++;
						}
					}
					result[i].insert("Reduce" + to_string(n));
				}
				else {
					if (l.N_terminal == 'Z' && l.a_dot == "" ) {
						result[i].insert("Accept");
					}
				}

			}
		}
	}
	return result;
}

void Print_Goto() {
	cout << endl << "GoTo table" << endl;

	set<char> NT;
	auto GT_copy = GoTo;
	NT.insert(N.begin(), N.end());
	NT.insert(T.begin(), T.end());
	cout << setw(4) << "";
	for (auto i : NT) {
		cout << setw(4) << i;
	}
	cout << endl;
	for (auto i : GoTo) {
		cout << setw(4) << i.first;
		for (auto m : NT) {
			cout << setw(4) << ((i.second.count(m) != 0) ? to_string(i.second[m]) : "");
		}
		cout << endl;
	}
}

void PrintAction0() {
	cout << endl << "Action table" << endl;

	auto A_copy = Action0;
	int maxlen = 7;
	for (auto i : A_copy) {
		for (auto j : i.second) {
			if (j.length() > maxlen) {
				maxlen = j.length();
			}
		}
	}
	cout << setw(4) << "";
	cout << setw(maxlen + 3) << "";
	cout << endl;
	for (auto i : A_copy) {
		cout << setw(4) << i.first;
		if (!i.second.empty()) {
			for (auto k : i.second) {
				cout << setw(maxlen + 3) << k;
				break;
			}
		}
		else {
			cout << setw(maxlen + 3) << "";
		}
		cout << endl;
	}

}

void PrintAction() {
	if (k == 0) {
		PrintAction0();
		return;
	}
	cout << endl << "Action table" << endl;

	auto A_copy = Action;
	set<string> avan;
	int maxlen = 7;
	for (auto i : A_copy) {
		for (auto j : i.second) {
			avan.insert(j.first);
			if (j.first.length() > maxlen) {
				maxlen = j.first.length();
			}
		}
	}
	cout << setw(4) << "";
	for (auto i : avan) {
		cout << setw(maxlen + 3) << i;
	}
	cout << endl;
	for (auto i : A_copy) {
		cout << setw(4) << i.first;
		for (auto j : avan) {
			if (i.second.count(j) != 0) {
				for (auto k : i.second[j]) {
					cout << setw(maxlen + 3) << k;
					break;
				}
			}
			else {
				cout << setw(maxlen + 3) << "";
			}
		}
		cout << endl;
	}
}


bool Check0() {
	for (auto i : Action0) {
		if (i.second.size() > 1) {
			return false;
		}
	}
	return true;
}

bool Check() {
	if (k == 0)
		return Check0();
	for (auto i : Action) {
		for (auto l : i.second) {
			if (l.second.size() > 1) {
				return false;
			}
		}
	}
	return true;
}



//Not connected with the topic of the programm
ostream& operator<< (ostream& os, const stack<string>& state) {
	stack<string> copy = state;
	vector<string> copies;
	while (!copy.empty()) {
		copies.push_back(copy.top());
		copy.pop();
	}
	for (int i = 0; i < copies.size(); i++) {
		cout << copies[copies.size() - 1 - i];
		if (i != copies.size() - 1) {
			cout << "-";
		}
	}
	return os;
}

string Analyze0(string w) {
	stack<string> state;
	state.push("0");
	string steps;
	bool error = false;
	while (1) {
		cout << state << '\t' << w << endl;
		char avan = w[0];
		int t = stoi(state.top(), nullptr);
		if (Action0[t].empty()) {
			error = true;
			break;
		}
		else {
			if (Action0[t].count("Shift") != 0) {
				if (GoTo[t].count(avan) == 0) {
					error = true;
					break;
				}
				else {
					int gt = GoTo[t][avan];
					state.push(string(1, avan));
					state.push(to_string(gt));
					w = w.substr(1);
				}
			}
			else {
				if (Action0[t].count("Accept") != 0) {
					break;
				}
				else {
					//every set should have only one action since the grammar is lr(k)
					for (auto r : Action0[t]) {
						steps += r.substr(6); //six because there is a number of used rule after the word "reduce"
						steps += ' ';
						int rule_number = stoi(r.substr(6));
						int s = 2 * (rules_enum[rule_number].second == "e" ? 0 : rules_enum[rule_number].second.length());
						for (int i = 0; i < s; i++) {
							state.pop();
						}
						int new_t = stoi(state.top(), nullptr);
						if (GoTo[new_t].count(rules_enum[rule_number].first) == 0) {
							error = true;
							break;
						}
						else {
							int gt = GoTo[new_t][rules_enum[rule_number].first];
							state.push(string(1, rules_enum[rule_number].first));
							state.push(to_string(gt));
						}
						break;
					}
				}
			}
		}
	}
	if (error) {
		return "error";
	}
	else
		return steps;
}

string Analyze(string w){
	if (k == 0)
		return Analyze0(w);
	stack<string> state;
	state.push("0");
	string steps;
	bool error = false;
	while (1) {
		cout << state << '\t' << w << endl;
		string avan = w.substr(0, k);
		int t = stoi(state.top(), nullptr);
		if (Action[t].count(avan) == 0) {
			error = true;
			break;
		}
		else {
			if (Action[t][avan].count("Shift") != 0) {
				if (GoTo[t].count(avan[0]) == 0) {
					error = true;
					break;
				}
				else {
					int gt = GoTo[t][avan[0]];
					state.push(string(1, avan[0]));
					state.push(to_string(gt));
					w = w.substr(1);
				}
			}
			else {
				if (Action[t][avan].count("Accept") != 0) {
					break;
				}
				else {
					//every set should have only one action since the grammar is lr(k)
					for (auto r : Action[t][avan]) {
						steps += r.substr(6);
						steps += ' ';
						int rule_number = stoi(r.substr(6));
						int s = 2 * (rules_enum[rule_number].second == "e" ? 0 :rules_enum[rule_number].second.length());
						for (int i = 0; i < s; i++) {
							state.pop();
						}
						int new_t = stoi(state.top(), nullptr);
						if (GoTo[new_t].count(rules_enum[rule_number].first) == 0) {
							error = true;
							break;
						}
						else {
							int gt = GoTo[new_t][rules_enum[rule_number].first];
							state.push(string(1, rules_enum[rule_number].first));
							state.push(to_string(gt));
						}
						break;
					}
				}
			}
		}
	}
	if (error) {
		return "error";
	}
	else
		return steps;
}




void Print_Used_Rules(string rules) {
	if (rules == "error") {
		cout << rules << endl;
		return;
	}
	cout << endl << "Used rules " << endl;
	stringstream ss(rules);
	stack<int> numbers;
	while (ss.peek() != EOF) {
		int n;
		ss >> n;
		numbers.push(n);
		ss.ignore();
	}
	while (!numbers.empty()) {
		cout << rules_enum[numbers.top()].first << "->" << rules_enum[numbers.top()].second << endl;
		numbers.pop();
	}
}

int main()
{
	string(1,'b').substr(0, 1);
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
	cin.ignore();
	if (k == 0) {
		Action0 = CreateAction0();
	}
	else {
		Action = CreateAction();
	}
	Print_Goto();

	cout << endl;

	for (int i = 0; i < rules_enum.size(); i++) {
		cout << i << " " << rules_enum[i].first << " " << rules_enum[i].second << endl;
	}
	cout << endl;

	cin.ignore();
	if (Check()) {
		cout << "This is LR(k) grammar\n";
		PrintAction();
		cout << endl;
		while (1) {
			cout << "Enter the word: ";
			string word;
			cin >> word;
			cout << endl;
			Print_Used_Rules(Analyze(word + "e"));
		}
	}
	else {
		cout << "This is not LR(k) grammar\n";
	}



	while (1);
    return 0;
}
