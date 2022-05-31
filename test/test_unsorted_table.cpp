#include <gtest.h>
#include <iostream>
#include "../include/unsorted_table.h"
using namespace std;

TEST(unsorted_table, can_insert_element) {
	unsorted_table<int, int> s;
	int key = 2, value = 3;
	EXPECT_NO_THROW(s.insert(key, value));
};

TEST(unsorted_table, throw_when_insert_element_twice) {
	unsorted_table<int, int> s;
	int key = 2, value = 3;
	s.insert(key, value);
	EXPECT_THROW(s.insert(key, value), string);
}

TEST(unsorted_table, can_find_inserted_element) {
	unsorted_table<int, int> s;
	int key = 2, value = 3;
	s.insert(key, value);
	EXPECT_EQ(s.find(key), value);
};

TEST(unsorted_table, throw_when_find_absent_element) {
	unsorted_table<int, int> s;
	int key = 2, value = 3;
	EXPECT_THROW(s.find(key), string);
};

TEST(unsorted_table, can_erase_element) {
	unsorted_table<int, int> s;
	int key = 2, value = 3;
	s.insert(key, value);
	EXPECT_NO_THROW(s.erase(key));
	EXPECT_THROW(s.find(key), string);
};

TEST(unsorted_table, no_throw_when_erase_absent_element) {
	unsorted_table<int, int> s;
	int key = 2, value = 3;
	EXPECT_NO_THROW(s.erase(key));
};

TEST(unsorted_table, test1) {
	unsorted_table<int, int> s;
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