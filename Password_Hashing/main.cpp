
#include "password_hashing.h"
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

int main() {
	Database d1;
	bool go = true;
	bool found = false;
	string username;
	string password;
	string confirm_password;
	string response;
	int ans;

	srand(time(NULL));

	cout << d1.isEmpty() << endl;
	cout << d1.isFull() << endl;

	//Load table
	d1.load_db("passwords.csv");

	cout << d1.isEmpty() << endl;
	cout << d1.isFull() << endl;

	while (go){
		cout << "Would you like to see the db? (y/n) ";
		cin >> response;
		if (response == "y" || response == "Y")
			d1.print();
		cout << "Enter username: ";
		cin >> username;
		if (d1.search(username)) {
			cout << "Enter password: ";
			cin >> password;
			ans = d1.search(username, password);

			if (ans == 1) {
				cout << "Login success. Would you like to delete your account? ";
				cin >> response;
				if (response == "y" || response == "Y")
					if (d1.removeItem(username))
						cout << "Success." << endl;
					else
						cout << "Fail. Try again." << endl;
			}
			else if (ans == 0)
				cout << "Invalid password. ";
			else
				cout << "ERROR" << endl;
		}
		else {
			cout << "Invalid username. Would you like to make an account? (y/n) ";
			cin >> response;
			if (response == "y" || response == "Y") {
				do{
					cout << "Enter username: ";
					cin >> username;
					found = d1.search(username);

					if (found) {
						cout << "Username already exists. Try again." << endl;
					}
					else {
						cout << "Valid. ";
					}
				} while (found);
				do {
					cout << "Enter password: ";
					cin >> password;
					cout << "Confirm password: ";
					cin >> confirm_password;

					if (password != confirm_password)
						cout << "Passwords do not match, try again." << endl;
					else
						cout << "Success." << endl;
				} while (password != confirm_password);
				
				d1.insertItem(username, password);
			}
		}


		cout << "Again? ";
		cin >> response;
		if (response == "n" || response == "N")
			go = false;
	}

	//Store table
	d1.store_db("passwords.csv");
}
