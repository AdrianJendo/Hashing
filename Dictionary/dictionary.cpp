
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include "dictionary.hpp"

using namespace std;

Dictionary::Dictionary() {
	for (int i = 0; i < TABLESIZE; ++i) {
		table[i] = new LinkedList();
	}
	num_elements = 0;
}

void Dictionary::clean_up(LinkedList* cur_list) {
	Node* cur = cur_list->head;
	while (cur) {
		cur = cur->next;
		delete cur_list->head;
		cur_list->head = cur;
	}
	delete cur_list;
}

Dictionary::~Dictionary() {
	for (int i = 0; i < TABLESIZE; ++i) 
		clean_up(table[i]);
	num_elements = 0;
}

void Dictionary::print() const {
	for (int i = 0; i < TABLESIZE; ++i) {
		Node* cur = table[i]->head;
		if (!cur)
			cout << "NULL";
		while (cur) {
			cout << cur->value;
			if (cur->next)
				cout << " - ";
			cur = cur->next;
		}
		cout << endl;
	}
}

bool Dictionary::isEmpty() const{
	for (int i = 0; i < TABLESIZE; ++i) 
		if (table[i]->size != 0)
			return false;
	return true;
}

bool Dictionary::search(string key) const{
	auto start_time = chrono::steady_clock::now();
	int index = abs(key[0] - 'a');
	Node* cur = table[index]->head;

	if (cur) {
		while (cur && cur->value != key)
			cur = cur->next;
		if (!cur)
			return false;
	}
	else
		return false;

	string part_of_speech = cur->definition;
	int count = 1;

	while (part_of_speech[count] != '|' && part_of_speech[count -1] != '|') 
		++count;

	part_of_speech = cur->definition.substr(0, count);
	string def = cur->definition.substr(count+2, cur->definition.length());

	if(part_of_speech == "|")
		cout << key << " (" << "N/A" << "): " << def << endl;
	else
		cout <<  key << " (" << part_of_speech << "): " << def << endl;
	auto end_time = chrono::steady_clock::now();

	cout << "Time of lookup is " << chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << " microseconds" << endl;
	return true;
}

bool Dictionary::insertItem(string key, string definition) {
	int index = abs(key[0] - 'a');
	if (index >= TABLESIZE)
		return false;

	LinkedList* ptr = table[index];
	ptr->insertNode(key, definition);

	++num_elements;
	return true;
}

bool Dictionary::LinkedList::insertNode(string key, string definition) {
	Node* cur = this->head;
	if (!cur) 
		this->head = new Node(key, definition, nullptr);
	else 
		insertNode(key, definition, cur, 1);
	
	++size;
	return true;
}

bool Dictionary::LinkedList::insertNode(string& key, string& definition, Node*& cur, int i) {
	if (i >= cur->value.size()) 
		if (cur->next)
			return insertNode(key, definition, cur->next, 1);
		else
			cur->next = new Node(key, definition, nullptr);

	while (i < key.size() && cur->next && key[i] > cur->value[i]) {
		cur = cur->next;
		i = 1;
	}
	if (i < key.size() && key[i] == cur->value[i]) 
		return insertNode(key, definition, cur, ++i);
	else if (i < key.size() && key[i] < cur->value[i]) {
		Node* next = cur->next;
		string cur_value = cur->value;
		string cur_def = cur->definition;
		cur->value = key;
		cur->definition = definition;
		cur->next = new Node(cur_value, cur_def, next);

		if (cur->next->value == this->head->value)
			this->head = cur;
	}
	else //no cur->next (insert into last element)
		cur->next = new Node(key, definition, nullptr);

	return true;
}

bool Dictionary::removeItem(string key) {
	int index = abs(key[0] - 'a')%26;

	if (index >= TABLESIZE)
		return false;

	Node* cur = table[index]->head;

	if (!(cur))
		return false;
	else if (cur->value == key) {
		Node* temp = cur->next;
		delete cur;
		if (temp)
			table[index]->head = temp;
		else
			table[index]->head = nullptr;
	}
	else {
		while (cur->next && cur->next->value != key)
			cur = cur->next;
		if (!cur->next)
			return false;
		else {
			Node* temp = cur->next->next;
			delete cur->next;
			cur->next = temp;
		}	
	}
	--num_elements;
	return true;
}


bool Dictionary::insert_unsorted(string key, string definition) {
	int index = abs(key[0] - 'a')%26;
	if (index >= TABLESIZE)
		return false;

	LinkedList* ptr = table[index];
	Node* h = ptr->head;
	ptr->head = new Node(key, definition, h);

	++num_elements;
	return true;
}

bool Dictionary::load_dictionary(string file_name) {
	auto start_time = chrono::steady_clock::now();
	ifstream fin(file_name);
	if (!fin)
		return false;

	string word;
	string definition;

	while (getline(fin, word, ',')) {
		getline(fin, definition, '\n');
		insert_unsorted(word, definition);
	}

	auto end_time = chrono::steady_clock::now();
	cout << chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() << " seconds" << endl;
	fin.close();
	return true;
}

bool Dictionary::store_dictionary(string file_name) {
	auto start_time = chrono::steady_clock::now();
	ofstream fout(file_name);
	if (!fout)
		return false;

	for (int i = 0; i < 26; ++i) {
		LinkedList* ptr = table[i];
		Node* cur = ptr->head;
		while (cur) {
			fout << cur->value << "," << cur->definition << endl;
			cur = cur->next;
		}
	}

	auto end_time = chrono::steady_clock::now();
	cout << chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds" << endl;
	fout.close();
	return true;
}


//text format is "word {part_of_speech || definition}"
/*
bool Dictionary::load_dictionary(string file_name) {
	auto start_time = chrono::steady_clock::now();
	ifstream fin(file_name);
	if (!fin)
		return false;

	string word;
	string definition;
	string temp;

	while (fin >> word) {
		fin >> definition;
		definition = definition.substr(1);
		fin >> temp;
		while (temp[temp.size() - 1] != '}') {
			definition += " " + temp;
			fin >> temp;
		}

		definition += " " + temp.substr(0, temp.size() - 1);

		insert_unsorted(word, definition);
	}

	auto end_time = chrono::steady_clock::now();
	cout << chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() << " seconds" << endl;
	fin.close();
	return true;
}



bool Dictionary::store_dictionary(string file_name) {
	auto start_time = chrono::steady_clock::now();
	ofstream fout(file_name);
	if (!fout)
		return false;

	for (int i = 0; i < TABLESIZE; ++i) {
		LinkedList* ptr = table[i];
		Node* cur = ptr->head;
		while (cur) {
			fout << cur->value << " {" << cur->definition << "} ";
			cur = cur->next;
		}

		fout << endl << endl;
	}

	auto end_time = chrono::steady_clock::now();
	cout << chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds" << endl;
	fout.close();
	return true;
}

*/
