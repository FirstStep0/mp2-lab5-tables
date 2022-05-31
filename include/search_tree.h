#pragma once
#include "table.h"
#include <string>
using namespace std;

template<class K, class T>
class search_tree :public table <K, T> {
public:
	struct Node {
		Node* left, * right;
		record<K, T> p;
		int h;
		Node() {
			left = right = nullptr;
			h = 0;
		}
		Node(const record<K, T>& p) :Node() {
			this->p = p;
		}
	};
	Node* root;
	search_tree() {
		root = nullptr;
	}

	~search_tree() {
		if (root != nullptr) {
			clear(root);
		}
	}

	void clear(Node* node) {
		if (node->left != nullptr)clear(node->left);
		if (node->right != nullptr)clear(node->right);
		delete node;
	}

	/*void show() {
		if (root != nullptr)
		show(root);
	};*/

	/*bool check() {
		bool flag = false;
		if (root != nullptr) {
			check(root, flag);
		}
		return flag;
	}*/

	/*int check(Node* node, bool& flag) {
		int hl, hr, h;
		h = hl = hr = 0;
		if (node->left != nullptr) {
			hl = check(node->left,flag);
		}
		if (node->right != nullptr) {
			hr = check(node->right,flag);
		}
		if (abs(hl - hr) > 1)flag = true;
		h = max(hl, hr) + 1;
		return h;
	};*/

	void insert(const K& key, const T& value) {
		this->eff = 0;
		record<K, T> p;
		p.key = key;
		p.value = value;
		insert(p);
	}

	/*void insert(K&& key, T&& value) {
		record<K, T> p;
		p.key = key;
		p.value = value;
		insert(p);
	}*/

	void insert(const record<K, T>& p) {
		if (root != nullptr) {
			root = insert(root, p);
		}
		else {
			root = new Node(p);
		}
	}

	T& find(const K& key) {
		this->eff = 0;
		Node* node = find(root, key);
		if (node != nullptr) {
			return node->p.value;
		}
		else {
			throw string("record is not found");
		}
	}

	Node* find(Node* start, const K& key) {
		Node* node = start;
		while (node != nullptr) {
			this->eff++;
			if (key < node->p.key) {
				node = node->left;
			}
			else if (node->p.key < key) {
				node = node->right;
			}
			else {
				break;
			}
		}
		return node;
	}

	Node* insert(Node* node, const record<K, T>& p) {
		if (p.key < node->p.key) {
			this->eff++;
			if (node->left != nullptr) {
				node->left = insert(node->left, p);
			}
			else {
				node->left = new Node(p);
			}
		}
		else if (node->p.key < p.key) {
			this->eff++;
			if (node->right != nullptr) {
				node->right = insert(node->right, p);
			}
			else {
				node->right = new Node(p);
			}
		}
		else {
			throw string("record is not unique");
		}
		node = balance(node);
		return node;
	}

	void erase(const K& key) {
		this->eff = 0;
		if (root != nullptr) {
			root = erase(root, key);
		}
	}

	Node* erase(Node* node, const K& key) {
		this->eff++;
		if (key < node->p.key) {
			if (node->left != nullptr) {
				node->left = erase(node->left, key);
			}
		}
		else if (node->p.key < key) {
			if (node->right != nullptr) {
				node->right = erase(node->right, key);
			}
		}
		else {
			Node* temp = node;
			node = detach_node(node);
			delete temp;
			return node;
		}
		node = balance(node);
		return node;
	}

	Node* detach_node(Node* node) {
		if (node->right != nullptr) {
			this->eff++;
			Node* ret_node;
			node->right = down_left(node->right, ret_node);

			ret_node->left = node->left;
			ret_node->right = node->right;
			ret_node = balance(ret_node);
			return ret_node;
		}
		else if (node->left != nullptr) {
			this->eff++;
			Node* ret_node;
			node->left = down_right(node->left, ret_node);

			ret_node->left = node->left;
			ret_node->right = node->right;
			ret_node = balance(ret_node);
			return ret_node;
		}
		else {
			return nullptr;
		}
	}

	Node* down_left(Node* node, Node*& ret_node) {
		this->eff++;
		if (node->left != nullptr) {
			node->left = down_left(node->left, ret_node);
			node = balance(node);
			return node;
		}
		else {
			ret_node = node;
			return detach_node(node);
		}
	}

	Node* down_right(Node* node, Node*& ret_node) {
		this->eff++;
		if (node->right != nullptr) {
			node->right = down_right(node->right, ret_node);
			node = balance(node);
			return node;
		}
		else {
			ret_node = node;
			return detach_node(node);
		}
	}

	Node* balance(Node* node) {
		int hl, hr;
		hl = hr = 0;
		if (node->left != nullptr) {
			hl = node->left->h + 1;
		}
		if (node->right != nullptr) {
			hr = node->right->h + 1;
		}
		int delta = hl - hr;
		delta = (delta >= 0) ? delta : -delta; //abs(hl-hr);

		if (delta >= 2) {
			this->eff++;
			if (hl > hr) {
				Node* b = node->left;
				int _hl, _hr;
				_hl = _hr = 0;
				if (b->left != nullptr) _hl = b->left->h + 1;
				if (b->right != nullptr) _hr = b->right->h + 1;

				if (_hl < _hr) {
					node->left = left_rotate(b);
					node = right_rotate(node);
				}
				else {
					node = right_rotate(node);
				}
			}
			else {
				Node* b = node->right;
				int _hl, _hr;
				_hl = _hr = 0;
				if (b->left != nullptr) _hl = b->left->h + 1;
				if (b->right != nullptr) _hr = b->right->h + 1;

				if (_hl > _hr) {
					node->right = right_rotate(b);
					node = left_rotate(node);
				}
				else {
					node = left_rotate(node);
				}
			}
		}
		update_height(node);
		return node;
	}

	void update_height(Node* node) {
		int hl, hr;
		hl = hr = 0;
		if (node->left != nullptr) hl = node->left->h + 1;
		if (node->right != nullptr) hr = node->right->h + 1;
		node->h = max(hl, hr);
	}

	Node* left_rotate(Node* a) {
		Node* b = a->right;
		a->right = b->left;
		b->left = a;

		update_height(a);
		update_height(b);

		return b;
	}
	Node* right_rotate(Node* a) {
		Node* b = a->left;
		a->left = b->right;
		b->right = a;

		update_height(a);
		update_height(b);

		return b;
	}
};