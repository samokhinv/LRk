// LRk.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <string>


using namespace std;

map <char, vector<string>> rules;
set<char> N, T;
int k;

map<char, set<string>> First_N;

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
	return rules;
}

//перегрузка оператора, чтобы получить + для множеств строк
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


//Реализация функции First_k, описанной подробно в Серебрякове
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
	char S;
	cin >> S;
	cin.ignore();
	cout << "Enter the number of rules: ";
	int n;
	cin >> n;
	cin.ignore();
	cout << "Enter the rules: \n";
	rules = ReadRules(n);

	First_N = First_NT();

	//множества lr ситуаций
	//автомат
	//таблица


	while (1);
    return 0;
}