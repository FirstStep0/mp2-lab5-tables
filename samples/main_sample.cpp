#include <iostream>
#include <string>
#include "../include/polinom.h"
#include "../include/hash_table.h"
#include "../include/search_tree.h"
#include "../include/unsorted_table.h"

using namespace std;
typedef unsigned int uint;
const int Exit = 10;
polinom f, s, r;
table<string, polinom>* tables[3];
my_list<string> _list;
string name_of_table[3];

double parseToDouble(const string& str) {
	double res = 0.0;
	double pos = str.size();
	if (str.size() == 0)throw string("parseToDouble(): empty str");
	for (int i = 0; i < str.size(); i++)if (!(('0' <= str[i] && str[i] <= '9') || (str[i] == '.')))throw string("parseToDouble(): incorrect str");

	for (int i = 0; i < str.size(); i++) {
		if (str[i] == '.') {
			pos = i + 1;
		}
		else {
			res *= 10.0;
			res += (str[i] - '0');
		}
	}
	return res / pow(10.0, str.size() - pos);
}

struct error {
	uint pos;
	string message;
	error(uint pos, string message) :pos(pos), message(message) {};
};

void print_slash(int pos, string message) {
	for (int k = 0; k < pos; k++) {
		cout << " ";
	}
	cout << "^\n";
	if (message.size())
		cout << message << "\n";
}

unsigned int parseToUInt(const string& str) {
	uint res = 0;
	for (int i = 0; i < str.size(); i++) {
		if (!('0' <= str[i] && str[i] <= '9'))throw string("error: ParseToUInt()");
		res *= 10;
		res += (str[i] - '0');
	}
	return res;
}

void SetPolinom(string& input, polinom& p) {
	polinom test;
	input += '+';
	string temp = "";
	bool minus = false;
	for (int i = 0; i < input.size(); i++) {
		char& ch = input[i];
		if (ch == '+' || ch == '-') {
			if (temp.size() == 0) {
				minus = (minus ^ (ch == '-'));
				//minus = (ch == '-') ? true : false;
			}
			else /*(temp.size() > 0)*/ {
				bool current_minus = minus;
				minus = (ch == '-') ? true : false;

				bool was_point = false;
				int j = 0;
				string co = "";
				double coef = 1.0;
				temp += '#';
				for (; (j < temp.size()) && (('0' <= temp[j] && temp[j] <= '9') || temp[j] == '.'); j++) {
					if (temp[j] == '.') {
						if (was_point) {
							throw error(i - temp.size() + j + 1, "many dots");
						}
						was_point = true;
					}
					co += temp[j];
				}
				if (co.size()) coef = parseToDouble(co);

				uint u, x, y, z;
				string str_u = "";
				x = y = z = 0;
				char type_deg = 0;
				for (; j < temp.size(); j++) {
					if (('0' <= temp[j] && temp[j] <= '9')) {
						str_u += temp[j];
					}
					else {
						if (!str_u.empty())u = parseToUInt(str_u);
						switch (type_deg) {
						case 'x': {
							x = u;
							break;
						}
						case 'y': {
							y = u;
							break;
						}
						case 'z': {
							z = u;
							break;
						}
						}
						u = 1;
						str_u = "";

						switch (temp[j]) {
						case 'x': {
							type_deg = 'x';
							break;
						}
						case 'y': {
							type_deg = 'y';
							break;
						}
						case 'z': {
							type_deg = 'z';
							break;
						}
						case '#': {
							break;
						}
						default: {
							throw error(i - temp.size() + j + 1, "incorrect_input");
							return;
						}
						}
					}
				}
				if (current_minus)coef = -coef;
				test += monom(coef, x, y, z);
				temp = "";
			}
		}
		else {
			if (ch != ' ')
				temp += ch;
		}
	}
	p = test;
}

void SetPolinom(polinom& p) {
	cout << "Example input: -2.0x3y-2z2+4.5xy2z\n";
	cout << "the order of variables and monom can be any\n";
	cout << "maximum degree is 256\n";
	cout << "Input monom:\n";
	cout << ">";
	string input;
	getline(cin, input);
	SetPolinom(input, p);
}

void insert_into_tables(const string& key, const polinom& val) {
	for (int i = 0; i < 3; i++) {
		(*tables[i]).insert(key, val);
	}
}

polinom find_into_tables(const string& key) {
	polinom temp, res = (*tables[0]).find(key);
	for (int i = 1; i < 3; i++) {
		temp = (*tables[i]).find(key);
		if (res != temp)throw string("polinoms are not equal");
	}
	return res;
}

void erase_from_tables(const string& key) {
	for (int i = 0; i < 3; i++) {
		(*tables[i]).erase(key);
	}
}

void Initialize() {
	name_of_table[0] = "unsorted table";
	name_of_table[1] = "search tree";
	name_of_table[2] = "hash table";

	tables[0] = new unsorted_table<string, polinom>();
	tables[1] = new search_tree<string, polinom>();
	tables[2] = new hash_table<string, polinom>(997, [](const string& key,int index) {
		int sum = 0;
		for (int i = 0; i < key.size(); i++) {
			sum = (sum + (key[i] * (i + 1))) % 997;
		}
		sum = (sum + index * 17) % 997;
		return sum;
	});

	/*polinom p, q;

	_list.push_back("a");
	p += monom(3, 2, 2, 0);
	insert_into_tables("a", p);

	q += monom(0, 0, 0, 0);
	_list.push_back("zero");
	insert_into_tables("zero", q);*/
}

void set_name_of_key(string& key) {
	cout << "Input name of polinom:\n";
	cout << ">";
	cin >> key;
	cin.clear();
	cin.ignore(10000, '\n');
}

void get_efficiency_of_tables() {
	for (int i = 0; i < 3; i++) {
		cout << name_of_table[i] << ": " << (*tables[i]).get_efficiency_last_operation() << "\n";
	}
}

int main()
{
	Initialize();
	cout.precision(10);
	int code = 0;
	/*while (true) {
		hash_table<int, int> s(997, [](const int& key, int index) {return key + index * 17; });
		int count = 10000;
		for (int i = 0; i < count; i++)s.insert(code, code);
	}*/
	while (code != Exit) {
		system("cls");
		cout << "+-------------------------------+\n";
		cout << "|program for working with tables|\n";
		cout << "+-------------------------------+\n";
		cout << "[command for tables]\n";
		cout << "  1.Insert polinom into tables\n";
		cout << "  2.Erase polinom from tables\n";
		cout << "   List of available polinoms:\n";
		for (auto it = _list.begin(); it != _list.end(); it++) {
			cout << "    " << (*it) << "\n";
		}
		cout << "[command for polinoms]\n";
		cout << "  3.Set first polinom\n";
		cout << "   f: " << f << "\n";
		cout << "  4.Set second polinom\n";
		cout << "   s: " << s << "\n";
		cout << "  5.Insert operation result into tables:\n";
		cout << "   result: " << r << "\n";
		cout << "[arithmetic operations]\n";
		cout << "  6.Add\n";
		cout << "  7.Sub\n";
		cout << "  8.Multiply\n";
		cout << "\n";
		cout << "10.Exit\n";
		cout << ">";
		cin >> code;
		cin.clear();
		cin.ignore(10000, '\n');
		try {
			switch (code) {
			case 1: {
				string key;
				polinom value;
				set_name_of_key(key);
				SetPolinom(value);
				insert_into_tables(key, value);
				_list.push_back(key);
				get_efficiency_of_tables();
				break;
			}
			case 2: {
				string key;
				set_name_of_key(key);
				erase_from_tables(key);
				auto it = _list.find(key);
				_list.erase(_list.find(key));
				get_efficiency_of_tables();
				break;
			}
			case 3: {
				string key;
				polinom value;
				set_name_of_key(key);
				f = find_into_tables(key);
				get_efficiency_of_tables();
				break;
			}
			case 4: {
				string key;
				polinom value;
				set_name_of_key(key);
				s = find_into_tables(key);
				get_efficiency_of_tables();
				break;
			}
			case 5: {
				string key;
				set_name_of_key(key);
				insert_into_tables(key, r);
				_list.push_back(key);
				get_efficiency_of_tables();
				break;
			}
			case 6: {
				r = f + s;
				break;
			}
			case 7: {
				r = f - s;
				break;
			}
			case 8: {
				r = f * s;
				break;
			}
			case 10: {
				break;
			}
			default: {
				cout << "command not found\n";
			}
			}
		}
		catch (string s) {
			cout << s << "\n";
		}
		catch (error e) {
			print_slash(e.pos + 1, e.message);
		}
		catch (...) {
			cout << "undefined error\n";
		}
		system("pause");
	}
}
