#include <gtest.h>
#include <iostream>
#include "../include/hash_table.h"
using namespace std;

TEST(hash_table, can_insert_element) {
	hash_table<int, int> s(997, [](const int& key, int index) {return (key + index * 17); });
	int key = 2, value = 3;
	EXPECT_NO_THROW(s.insert(key, value));
};

TEST(hash_table, throw_when_insert_element_twice) {
	hash_table<int, int> s(997, [](const int& key, int index) {return (key + index * 17); });
	int key = 2, value = 3;
	s.insert(key, value);
	EXPECT_THROW(s.insert(key, value), string);
}

TEST(hash_table, can_find_inserted_element) {
	hash_table<int, int> s(997, [](const int& key, int index) {return (key + index * 17); });
	int key = 2, value = 3;
	s.insert(key, value);
	EXPECT_EQ(s.find(key), value);
};

TEST(hash_table, throw_when_find_absent_element) {
	hash_table<int, int> s(997, [](const int& key, int index) {return (key + index * 17); });
	int key = 2, value = 3;
	EXPECT_THROW(s.find(key), string);
};

TEST(hash_table, can_erase_element) {
	hash_table<int, int> s(997, [](const int& key, int index) {return (key + index * 17); });
	int key = 2, value = 3;
	s.insert(key, value);
	EXPECT_NO_THROW(s.erase(key));
	EXPECT_THROW(s.find(key), string);
};

TEST(hash_table, no_throw_when_erase_absent_element) {
	hash_table<int, int> s(997, [](const int& key, int index) {return (key + index * 17); });
	int key = 2, value = 3;
	EXPECT_NO_THROW(s.erase(key));
};

TEST(hash_table, test1) {
	hash_table<int, int> s(997, [](const int& key, int index) {return (key + index * 17); });
	int key = 2, value = 3;
	const int _siz = 15;
	int arr[_siz];
	for (int i = 0; i < _siz; i++) arr[i] = rand();
	for (int i = 0; i < _siz; i++) s.insert(arr[i], i);
	for (int i = 0; i < _siz; i++) {
		EXPECT_EQ(s.find(arr[i]), i);
	}
	for (int i = 0; i < _siz; i++) {
		s.erase(arr[i]);
		EXPECT_THROW(s.find(arr[i]), string);
	}
	EXPECT_NO_THROW(s.erase(key));
};