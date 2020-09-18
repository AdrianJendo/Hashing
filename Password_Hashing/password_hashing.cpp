
#include "password_hashing.h"
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

using namespace std;


Database::Database() {
	for (int i = 0; i < TABLESIZE; ++i) {
		table[i] = new User();
	}
	num_elements = 0;
}


Database::~Database() {
	for (int i = 0; i < TABLESIZE; ++i) 
		delete table[i];
	num_elements = 0;
}

bool Database::User::isEmpty() const {
	if (username == "")
		return true;

	return false;
}

bool Database::isEmpty() const {
	for (int i = 0; i < TABLESIZE; ++i) 
		if (!table[i]->isEmpty())
			return false;

	return true;
}

bool Database::isFull() const {
	for (int i = 0; i < TABLESIZE; ++i)
		if (table[i]->isEmpty())
			return false;

	return true;
}

void Database::print() const {
	for (int i = 0; i < TABLESIZE; ++i) {
		if (table[i]->username != "") {
			cout << table[i]->username << "  :  " << table[i]->salt << "  :  " << table[i]->hash << endl;
		}
	}
}

bool Database::search(string key) const {
	auto start_time = chrono::steady_clock::now();
	int index = h1(key);
	int count = 0;

	while (table[index]->username != key && count < TABLESIZE) {
		index = (index + h2(key)) % TABLESIZE;
		++count;
	}

	if (count == TABLESIZE)
		return false;

	cout << chrono::duration_cast<std::chrono::microseconds>(chrono::steady_clock::now() - start_time).count() << " microseconds" << endl;
	return true;
}


int Database::search(string key, string password) const{
	auto start_time = chrono::steady_clock::now();
	int index = h1(key);
	int count = 0;
	int output = 0;

	while (table[index]->username != key && count < TABLESIZE) {
		index = (index + h2(key)) % TABLESIZE;
		++count;
	}

	if (count == TABLESIZE) //Username not found
		output = -1;
	else 
		for (int i = 0; i < 26; ++i) 
			if (password_hash('A' + i, password + table[index]->salt) == table[index]->hash)
				output = 1;	

	cout << chrono::duration_cast<std::chrono::microseconds>(chrono::steady_clock::now() - start_time).count() << " microseconds" << endl;
	return output;
}

bool Database::insert(string u, string s, string h) {
	//check if space available
	if (isFull())
		return false;

	int index = h1(u);

	//find the next empty spot
	while (!table[index]->isEmpty()) {
		index = (index + h2(u)) % TABLESIZE;
	}

	//insert into available spot
	table[index]->username = u;
	table[index]->salt = s;
	table[index]->hash = h;

	return true;
}

bool Database::insertItem(string key, string password) {
	string salt = create_salt();
	string hashed_password = password_hash('A' + rand()%26, password+salt);


	return insert(key, salt, hashed_password);
}


bool Database::removeItem(string key) {
	auto start_time = chrono::steady_clock::now();

	if (isEmpty())
		return false;

	int index = h1(key);
	int count = 0;

	//find the next empty spot
	while (table[index]->username != key && count < TABLESIZE) {
		index = (index + h2(key)) % TABLESIZE;
		++count;
	}

	if (count == TABLESIZE)
		return false;

	//remove
	table[index]->username = "";
	table[index]->salt = "";
	table[index]->hash = "";

	cout << chrono::duration_cast<std::chrono::microseconds>(chrono::steady_clock::now() - start_time).count() << " microseconds" << endl;
	return true;
}



bool Database::load_db(string file_name) {
	auto start_time = chrono::steady_clock::now();
	ifstream fin(file_name);
	if (!fin)
		return false;

	string username;
	string salt;
	string hash;


	while (getline(fin, username, ',')) {
		getline(fin, salt, ',');
		getline(fin, hash, '\n');
		insert(username, salt, hash);
	}

	cout << chrono::duration_cast<std::chrono::microseconds>(chrono::steady_clock::now() - start_time).count() << " microseconds" << endl;
	fin.close();
	return true;
}



bool Database::store_db(string file_name) {
	auto start_time = chrono::steady_clock::now();
	ofstream fout(file_name);
	if (!fout)
		return false;

	for (int i = 0; i < TABLESIZE; ++i) 
		if(table[i]->username != "")
			fout << table[i]->username << "," << table[i]->salt << "," << table[i]->hash << endl;
	
	cout << chrono::duration_cast<std::chrono::microseconds>(chrono::steady_clock::now() - start_time).count() << " microseconds" << endl;
	fout.close();
	return true;
}

//hash functions
string Database::password_hash(char pepper, string salted_pass) const {
	salted_pass = pepper + salted_pass;
	string hash = "";
	char temp1;
	char temp2;
	for (int i = 1; i < salted_pass.size(); i+=2) {
		temp1 = salted_pass[i] * 13 - 7;
		temp2 = salted_pass[i] - salted_pass[i - 1] + 101;


		while (temp1 < 33) {
			temp1 += 61;
		}

		while (temp2 < 33) {
			temp2+= 71;
		}

		hash += string(1, temp1) + string(1, temp2);
	}

	return hash;
}
int Database::h1(string key) const {
	return (8 * key[1] + 7) % TABLESIZE;
}
int Database::h2(string key) const {
	return (11 - (key[0] % 11)) % TABLESIZE;
}
string Database::create_salt() const {
	string salt = "";
	int min = 33; //ASCI 33 = !
	int max = 126; //ASCI 126 = ~

	for (int i = 0; i < 16; ++i) 
		salt += rand() % (max - min + 1) + min;

	return salt;
}
