#pragma once
#include <vector>
#include <string>
#include "table.h"
using namespace std;

/*template<class K, class T>
struct record {
	K key;
	T value;
	record(const K& key, const T& value) :key(key), value(value) {};
	record() :record(K(), T()) {};
};*/

template<class K, class T>
class unsorted_table : public table<K, T> {
	vector<record<K, T*>> records;
public:
	unsorted_table() {}
	~unsorted_table() {
		int count = records.size();
		for (int i = 0; i < count; i++) {
			delete records[i].value;
		}
	}
	T& find(const K& key) {
		this->eff = 0;
		int index = _find(key);
		if (index >= 0) {
			return (*records[index].value);
		}
		throw string("records is not found");
	}
	int _find(const K& key) {
		int count = records.size();
		for (int i = 0; i < count; i++) {
			this->eff++;
			if (records[i].key == key)return i;
		}
		return -1;
	}
	void insert(const K& key, const T& value) {
		this->eff = 0;
		if (_find(key) == -1) {
			records.push_back(record<K, T*>(key, new T(value)));
		}
		else {
			throw string("record is not unique");
		}
	}
	/*void insert(string& key, T* value) {
		if (!isFull()) {
			records[count] = record<T>(key, value, user);
			count++;
			return;
		}
		throw string("array_table is full");
	}*/
	void erase(const K& key) {
		this->eff = 0;
		int index = _find(key);
		if (index >= 0) {
			delete records[index].value;
			records[index] = records.back();
			records.pop_back();
		}
	}
	/*size_t size() {
		return _size;
	}
	bool empty() {
		return !size();
	}
	bool isFull() {
		return count == size();
	}*/
};