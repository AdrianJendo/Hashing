
#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <iostream>
#include <string>
#include <iostream>

using namespace std;

//Stores approximately 100,000 words
//Able to access all data in less than 5 milliseconds
/*
Currently consists of:
80,234 entries
2,833,858 words
14,386,824 characters
*/
class Dictionary {
private:
	struct Node {
		//Constructors
		Node() : value(""), definition(""), next(nullptr) {}
		Node(string new_value, Node* new_next) : value(new_value), definition(""), next(new_next) {}
		Node(string new_value, string new_definition, Node* new_next) :value(new_value), definition(new_definition), next(new_next) {}
		Node(Node* new_node) : value(new_node->value), definition(new_node->definition), next(new_node->next) {}

		//Attributes
		string value;
		string definition;
		Node* next;
	};
	struct LinkedList {
		//Constructor
		LinkedList() : head(nullptr), size(0) {}

		//Attributes
		Node* head;
		int size;

		//Member functions
		bool insertNode(string key, string definition);
		bool insertNode(string& key, string& definition, Node*& cur, int i);
	};

	//Helper functions
	static const int TABLESIZE = 26;
	LinkedList* table[TABLESIZE]; //hash table as an array
	int num_elements;
	void clean_up(LinkedList* cur_list);
	/*
	For more data values: table is 2D array of linkedlist pointers of size 26x27 
	The rows represent the first letter of the word and the columns represent the 2nd letter (and the additional column is used for single letter words such as 'a')
	*/

public:
	//Constructor
	Dictionary();

	//Destructor
	~Dictionary();

	//functions
	bool isEmpty() const;
	bool search(string key) const;
	bool insertItem(string key, string definition); //linear time insert
	bool removeItem(string key);
	void print() const;

	bool insert_unsorted(string key, string definition); //constant time insert

	//File io
	bool load_dictionary(string file_name);
	bool store_dictionary(string file_name);
};


#endif