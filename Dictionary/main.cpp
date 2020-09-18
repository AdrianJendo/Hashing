
#include <iostream>
#include <string>
#include "dictionary.hpp"

using namespace std;

int main() {
	Dictionary d1;

	d1.load_dictionary("dictionary.csv");

	bool go = true;
	string word;
	string def;
	string temp;
	while (go) {
		//d1.print();	
		cout << "Enter the word you would like to look up: ";
		cin >> word;
		if (!d1.search(word)) {
			cout << "Invalid word. Would you like to make a new entry? (y/n) ";
			cin >> temp;
			if (temp[0] == 'y' || temp[0] == 'Y') {
				cout << "Enter part of speech: ";
				cin.ignore();
				getline(cin, def);
				cout << "Enter definition (or type 'abort' to exit): ";
				getline(cin, temp);
				if (temp != "abort") {
					def += "|| " + temp;
					int count = 1;
					string temp2;
					do {
						cout << "Another definition? (y/n) ";
						getline(cin, temp2);
						if (temp2[0] == 'y' || temp2[0] == 'Y') {
							cout << "Enter definition (or type 'abort' to exit): ";
							getline(cin, temp);
							if (temp != "abort") {
								def += " / " + temp;
							}
							else
								temp2 = 'n';
						}
					} while (++count < 3 && (temp2[0] == 'y' || temp2[0] == 'Y'));

					d1.insert_unsorted(word, def);
				}
			}
		}
		
		cout << "Again? ";
		cin >> word;
		if (word[0] == 'n' || word[0] == 'N')
			go = false;
	}

	d1.store_dictionary("dictionary.csv");
}