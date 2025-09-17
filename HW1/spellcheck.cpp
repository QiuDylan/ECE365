#include <string>
#include <locale>
#include <regex>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "hash.h"

using namespace std;

regex word_re("[A-Za-z0-9\\-']+");

int main() {
    string dict_name = "wordlist_small";
    cout << "Enter dictionary file: ";
    cin >> dict_name;
    string spellcheck_name = "lyrics.txt";
    cout << "\nEnter name of file to spellcheck: ";
    cin >> spellcheck_name;
    string output = "output.txt";
    cout << "\nEnter output file: ";
    cin >> output;
    cout << "Parsing Dictionary.\n";
    clock_t t1 = clock();
    //hashTable* table = parse_dictionary(dict_name);
    clock_t t2 = clock();
    double timeDiff = ((double)(t2 - t1)) / CLOCKS_PER_SEC;
    cout << "Hashmap Loaded. CPU time was " << timeDiff << " seconds.\n";
    t1 = clock();
    //spellcheck(spellcheck_name, output, *table);
    t2 = clock();
    timeDiff = ((double)(t2 - t1)) / CLOCKS_PER_SEC;
    cout << "Spellcheck Complete. CPU time was " << timeDiff << " seconds.\n";

    return 0;
}