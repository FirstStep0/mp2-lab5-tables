#pragma once
template<class K, class T>
class table {
protected:
	int eff = 0;
public:
	virtual void insert(const K&, const T&) = 0;
	virtual void erase(const K&) = 0;
	virtual T& find(const K&) = 0;
	int get_efficiency_last_operation() { return this->eff; }
};

enum access { owner, user };
enum state {
	clear, deleted, use
};

template<class K, class T>
struct record {
	K key;
	T value;
	state st;
	record(const K& key, const T& value) :key(key), value(value) { st = use; };
	record() :record(K(), T()) { st = clear; };
};