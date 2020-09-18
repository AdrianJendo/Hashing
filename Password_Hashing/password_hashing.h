
#ifndef PASSWORD_HASHING_HPP
#define PASSWORD_HASHING_HPP

#include <iostream>
#include <string>
#include <iostream>

/*
brute force attack tries every combination of characters to get to the hashed password --tries every character combination
dictionary attack tries to hash a lot of passwords and compare their hash output to a username --tries common passwords

salting is not super effective against dictionary and brute force
peppering makes these attacks much more expensive to compute since the login has to cycle through all the peppers to find the one that hashes to the correct username
--peppering basically makes user login take longer, but it still takes a fraction of a second for a legit user

*/

/*
rainbow table requires the attacker to first get the database of username:password or username:hash and also the hash function
and then pre-compute a dictionary of password:hash - then they can work backwards to find the password that produces the hash stored in database

random salts and peppers make computing the raindbow table extremely expensive
- For salts, for each password they want to hash, they must add each salt value in the database before hashing
- For peppers, they must hash each combination of password + pepper as well
(when you use both salt and pepper, they must hash password+salt+pepper with each possible salt and pepper)
*/

using namespace std;

class Database {
private:
	struct User {
		//Constructor
		User() : username(""), salt(""), hash("") {}

		//Attributes
		string username;
		string salt;
		string hash;

		bool isEmpty() const; //checks if the current space is empty
	};

	//Helper functions
	static const int TABLESIZE = 1000;
	User* table[TABLESIZE]; //hash table as an array
	int num_elements;

	//hash functions
	string password_hash(char pepper, string salted_pass) const;
	int h1(string key) const;
	int h2(string key) const;
	string create_salt() const; //16 bytes
	bool insert(string u, string s, string h);


public:
	//Constructor
	Database();

	//Destructor
	~Database();

	//traversal functions
	int search(string key, string password) const;
	bool search(string key) const;
	bool insertItem(string key, string password); 
	bool removeItem(string key);
	void print() const;
	bool isFull() const;
	bool isEmpty() const;

	//File io
	bool load_db(string file_name);
	bool store_db(string file_name);
};


#endif
