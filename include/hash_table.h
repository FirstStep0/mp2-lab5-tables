#pragma once
#include "table.h"
#include <string>

template<class K, class T>
class hash_table : public table<K, T> {
	record<K, T>* data;
	const size_t _max_size;
	size_t _size;
	int(*hash)(const K&, int);
public:
	hash_table(size_t max_size, int(*_hash)(const K&, int)) : _max_size(max_size) {
		data = new record<K, T>[max_size];
		_size = 0;
		hash = _hash;
	}
	~hash_table() {
		delete[] data;
	}
	size_t size() {
		return _size;
	}
	size_t max_size() {
		return _max_size;
	}
	bool full() {
		return size() == max_size();
	}
	/*size_t hash(const string& k) {
		int s = k.size();
		long long sum = 0;
		for (int i = 0; i < s; i++) {
			sum = (sum + (i + 1) * k[i]) % max_size();
		}
		return sum;
	};*/
	bool empty() {
		return size() == 0;
	}
	/*void insert(string&& key, T&& value) {
		record<string, T> rec(key, value);
		insert(rec);
	}*/

	void insert(const K& key, const T& value) {
		this->eff = 0;
		record<K, T> rec(key, value);
		insert(rec);
	}
	void insert(const record<K, T>& rec) {
		if (_find(rec.key) >= 0) throw string("key of record is not unique");
		if (!full()) {
			size_t index = 0, numb, start;
			numb = start = hash(rec.key, index) % max_size();
			do {
				record<K, T>& cur = data[numb];
				if (cur.st != use) {
					cur = rec;
					cur.st = use;
					_size++;
					break;
				}
				this->eff++;
				numb = hash(rec.key, ++index) % max_size();
			} while (numb != start);
		}
		else {
			throw string("table is full");
		}
	}

	void erase(const K& key) {
		this->eff = 0;
		int index = _find(key);
		if (index >= 0) {
			data[index].st = deleted;
			_size--;
		}
	}

	T& find(const K& key) {
		this->eff = 0;
		int index = _find(key);
		if (index != -1) {
			return data[index].value;
		}
		else {
			throw string("record is not found");
		}
	}

	/*void show() {
		for (int i = 0; i < max_size(); i++) {
			cout << i << ": ";
			if (data[i].st == use) {
				cout<<"(" << data[i].key << ", " << data[i].value << ")" << "\n";
			}
			else {
				cout << "0\n";
			}
		}
	}*/

private:
	int _find(const K& key) {
		int res = -1;
		if (!empty()) {
			int index = 0, start, numb;
			numb = start = hash(key, index) % max_size();
			do {
				record<K, T>& cur = data[numb];
				if (key == cur.key && cur.st == use) {
					res = numb;
					break;
				}
				if (cur.st == clear)break;
				this->eff++;
				numb = hash(key, ++index) % max_size();
			} while (numb != start);
		}
		return res;
	}
};
