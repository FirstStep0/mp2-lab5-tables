#pragma once
typedef unsigned char uchar;
using namespace std;

template <class T>
class my_list {
protected:
	template<class T>
	class Node {
	public:
		Node<T>* next, * prev;
		T value;
		Node(const T& value) :value(value), next(this), prev(this) {}
		Node(T&& value) : value(value), next(this), prev(this) {}
		Node() :Node(T()) {};
	};
	template<class T>
	class iterator {
		friend my_list;
		Node<T>* node;
	public:
		iterator(Node<T>* _node) { node = _node; };
		T& operator*() { return (node->value); }
		Node<T>* operator->() { return node; };

		iterator<T>& operator++() { node = node->next; return *this; }
		iterator<T> operator++(int) {
			Node<T>* temp = node;
			node = node->next; return iterator<T>(temp);
		}

		iterator<T>& operator--() { node = node->prev; return *this; }
		iterator<T> operator--(int) {
			Node<T>* temp = node;
			node = node->prev; return iterator<T>(temp);
		}

		friend bool operator==(const iterator<T>& a, const iterator<T>& b) { return (a.node == b.node); }
		friend bool operator!=(const iterator<T>& a, const iterator<T>& b) { return (a.node != b.node); }
	};
	Node<T>* head;
	size_t count;
public:
	my_list() { head = new Node<T>(); count = 0; }
	my_list(const my_list<T>& l) :my_list() { operator=(l); }
	~my_list() { clear(); delete head; count = 0; }
	void clear();
	
	size_t size() { return count; }
	bool empty() { return !size(); }
	
	iterator<T> begin() { return iterator<T>(head->next); }
	iterator<T> end() { return iterator<T>(head); }
	iterator<T> middle(iterator<T> begin, iterator<T> end);
	
	T& front();
	T& back();

	void push_front(const T& value);
	void push_front(T&& value);
	void pop_front();

	void insert(iterator<T> _where, const T& value);
	void insert(iterator<T> _where, T&& value);
	void erase(iterator<T> _where);
	iterator<T> find(const T& key);

	void push_back(const T& value);
	void push_back(T&& value);
	void pop_back();

	
	my_list<T>& operator=(const my_list<T>& l);
	//merge
	template <class Compare>
	iterator<T> merge(iterator<T> fbegin, iterator<T> fend, iterator<T> sbegin, iterator<T> send, Compare less);
	template<class Compare>
	void merge(my_list<T>& l, Compare less);
	//default merge
	void merge(my_list<T>& l){
		merge(l, [](const T& a, const T& b) { return a < b; });
	}
	//sort
	template <class Compare>
	iterator<T> sort(iterator<T> begin, iterator<T> end, Compare less);
	template <class Compare>
	void sort(Compare less);
	//default sort
	void sort() {
		sort([](const T& a, const T& b) {return a < b; });
	}
protected:
	void attach_before(iterator<T> cur_node, iterator<T> att_node);
	void attach_after(iterator<T> cur_node, iterator<T> att_node);
	Node<T>* detach_node(iterator<T> node);
};
template<class T>
my_list<T>::iterator<T> my_list<T>::find(const T& key) {
	auto it = begin();
	for (; it != end(); ++it) {
		if (key == (*it)) {
			break;
		}
	}
	return it;
};


template<class T>
void my_list<T>::erase(iterator<T> _where) {
	if (_where != end()) {
		delete detach_node(_where);
		count--;
		return;
	}
	throw string("can't remove end");
}

template <class T>
my_list<T>& my_list<T>::operator=(const my_list<T>& l) {
	clear();
	my_list<T>* _l = const_cast<my_list<T>*>(&l);
	iterator<T> it = _l->begin();
	while (it != _l->end()) {
		this->push_back((*it));
		++it;
	}
	return *this;
};

template<class T>
void my_list<T>::clear() {
	iterator<T> temp(begin());
	while (temp != end()) {
		delete detach_node(temp++);
	}
	count = 0;
}
template<class T>
void my_list<T>::insert(iterator<T> _where, const T& value) {
	attach_after(_where, new Node<T>(value));
	count++;
}

template<class T>
void my_list<T>::insert(iterator<T> _where, T&& value) {
	attach_after(_where, new Node<T>(value));
	count++;
}

template<class T>
template <class Compare>
void my_list<T>::sort(Compare less) {
	sort(this->begin(), this->end(), less);
}

template <class T>
template <class Compare>
my_list<T>::iterator<T> my_list<T>::sort(iterator<T> begin, iterator<T> end, Compare less) {
	if ((begin->next) == end)return begin;
	iterator<T> mid = middle(begin, end);
	begin = sort(begin, mid, less);
	mid = sort(mid, end, less);
	return merge(begin, mid, mid, end, less);
}

template<class T>
void my_list<T>::push_front(const T& value) {
	attach_after(end(), new Node<T>(value));
	count++;
}

template<class T>
void my_list<T>::push_front(T&& value) {
	attach_after(end(), new Node<T>(value));
	count++;
}

template<class T>
void my_list<T>::pop_front() {
	if (!empty()) {
		delete detach_node(begin());
		count--;
		return;
	}
	throw string("my_list is empty");
}

template<class T>
void my_list<T>::push_back(const T& value) {
	attach_before(end(), new Node<T>(value));
	count++;
}

template<class T>
void my_list<T>::push_back(T&& value) {
	attach_before(end(), new Node<T>(value));
	count++;
};

template<class T>
void my_list<T>::pop_back() {
	if (!empty()) {
		iterator<T> bk(end());
		--bk;
		delete detach_node(bk);
		count--;
		return;
	}
	throw string("my_list is empty");
};

template<class T>
my_list<T>::iterator<T> my_list<T>::middle(iterator<T> begin, iterator<T> end) {
	iterator<T> mid = begin;
	while (begin != end && begin->next != end) {
		++begin;
		++begin;
		++mid;
	}
	return mid;
}

template<class T>
T& my_list<T>::front() {
	if (!empty()) {
		return (*begin());
	}
	throw string("my_list is empty");
}

template<class T>
T& my_list<T>::back() {
	if (!empty()) {
		iterator<T> bk(end());
		--bk;
		return *(bk);
	}
	throw string("my_list is empty");
}

template<class T>
template<class Compare>
void my_list<T>::merge(my_list<T>& l, Compare less) {
	merge(this->begin(), this->end(), l.begin(), l.end(), less);
}

template<class T>
template <class Compare>
my_list<T>::iterator<T> my_list<T>::merge(iterator<T> fbegin, iterator<T> fend, iterator<T> sbegin, iterator<T> send, Compare less) {
	Node<T>* temp = nullptr;
	Node<T> nend;
	attach_before(fend, &nend);
	iterator<T> new_first_node = less(*sbegin, *fbegin) ? sbegin : fbegin;
	while (sbegin != send) {
		if ((less(*sbegin, *fbegin)) || (fbegin == &nend)) {
			temp = sbegin.node;
			++sbegin;
			detach_node(temp);
			attach_before(fbegin, temp);
		}
		else {
			++fbegin;
		}
	}
	detach_node(&nend);
	return new_first_node;
}

template<class T>
my_list<T>::Node<T>* my_list<T>::detach_node(iterator<T> _node) {
	Node<T>* node = _node.node;
	node->prev->next = node->next;
	node->next->prev = node->prev;

	node->prev = nullptr;
	node->next = nullptr;
	return node;
}

template<class T>
void my_list<T>::attach_before(iterator<T> _cur_node, iterator<T> _att_node) {
	Node<T>* cur_node = _cur_node.node;
	Node<T>* att_node = _att_node.node;

	cur_node->prev->next = att_node;
	att_node->next = cur_node;

	att_node->prev = cur_node->prev;
	cur_node->prev = att_node;
}

template<class T>
void my_list<T>::attach_after(iterator<T> _cur_node, iterator<T> _att_node) {
	Node<T>* cur_node = _cur_node.node;
	Node<T>* att_node = _att_node.node;

	cur_node->next->prev = att_node;
	att_node->next = cur_node->next;

	cur_node->next = att_node;
	att_node->prev = cur_node;
}
