#include <gtest.h>
#include <iostream>
#include "../include/search_tree.h"
using namespace std;

TEST(search_tree, can_insert_element) {
	search_tree<int, int> s;
	int key = 2, value = 3;
	EXPECT_NO_THROW(s.insert(key, value));
};

TEST(search_tree, throw_when_insert_element_twice) {
	search_tree<int, int> s;
	int key = 2, value = 3;
	s.insert(key, value);
	EXPECT_THROW(s.insert(key, value), string);
}

TEST(search_tree, can_find_inserted_element) {
	search_tree<int, int> s;
	int key = 2, value = 3;
	s.insert(key, value);
	EXPECT_EQ(s.find(key), value);
};

TEST(search_tree, throw_when_find_absent_element) {
	search_tree<int, int> s;
	int key = 2, value = 3;
	EXPECT_THROW(s.find(key), string);
};

TEST(search_tree, can_erase_element) {
	search_tree<int, int> s;
	int key = 2, value = 3;
	s.insert(key, value);
	EXPECT_NO_THROW(s.erase(key));
	EXPECT_THROW(s.find(key), string);
};

TEST(search_tree, no_throw_when_erase_absent_element) {
	search_tree<int, int> s;
	int key = 2, value = 3;
	EXPECT_NO_THROW(s.erase(key));
};

TEST(search_tree, test1) {
	search_tree<int, int> s;
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