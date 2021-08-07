# Hashing
Separate Chaining and Open Addressing Hash Table Implementations

The Dictionary folder contains a separate chaining implementation of a hash-table for constant-time read and write.
The csv file contains almost 100,000 words and their definitions in a format that the program can understand and organize.
The csv file was compiled using a python script with the bs4 library to webscrape data from an online dictionary.

The Password_Hashing folder contains a hash table that uses open-addressing (with double hashing) to store users in a 'database'.
When creating a new user, the user's password is appended with a random 16-byte salt value, peppered, and hashed.
Each object in the hash table contains the user's username, salt, and hash value.
