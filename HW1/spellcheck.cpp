#include <fstream>
#include <iostream>
#include <locale>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>

#include "hash.h"

using namespace std;

const regex regex_val = regex("^[A-Za-z'-] + $");
// From cpp reference
string str_tolower(string s) {
    transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return tolower(c); });
    return s;
}
// Insert dictionary into hash table
hashTable* parse_dictionary(string filename) {
    //const regex regex_val = regex("^[A-Za-z'-] + $");
    ifstream inFile;
    inFile.open(filename);
    string line;
    // Error handling for a failed read
    if (!inFile.is_open()) {
        throw invalid_argument(
            "\n File for dictionary failed to open: \n Check permissions and "
            "existence?");
    }
    int fsize = 0;
    while (getline(inFile, line)) {
        fsize++;
    }
    inFile.close();
    const regex regex_val = regex("^[A-Za-z'-]+$");
    inFile.open(filename);
    hashTable* table = new hashTable(fsize);
    while (getline(inFile, line)) {
        if (line.size() <= 20 && regex_match(line, regex_val)) {
            string S = str_tolower(line);
            table->insert(S);
        }
    }
    inFile.close();
    return table;
}
// Check if word is in dictionary 
void spellcheck(string in_file, string out_file, hashTable& table) {
    ifstream inFile;
    inFile.open(in_file);
    ofstream outFile;
    outFile.open(out_file);
    string read_line;

    const regex regex_val = regex("[A-Za-z'-]");
    int line_number = 0;
    while (getline(inFile, read_line)) {
        line_number++;
        int i = 0;
        string word = "";
        bool shouldCheck = true;
        for (int i = 0; i < read_line.size(); i++) {
            string num = string(1, tolower(read_line[i]));
            if (regex_match(num, regex_val) > 0 || isdigit(read_line[i]) > 0) {
                if (isdigit(read_line[i])) {
                    shouldCheck = false;
                } else if (word.size() < 21) {
                    word += num;
                }
            } else {
                if (shouldCheck && word.size() > 20) {
                    word.pop_back();
                    outFile << "Long word at line " << to_string(line_number)
                            << ", starts: " << word << "\n";
                } else if (shouldCheck && word.size() > 0 && !(table.contains(word))) {
                    outFile << "Unknown word at line " << to_string(line_number) << ": "
                            << word << "\n";
                }
                shouldCheck = true;
                word = "";
            }
        }
    // Handling words that might've been missed at the end of the string
        if (shouldCheck && word.size() > 20) {
            word.pop_back();
            outFile << "Long word at line " << to_string(line_number)
                    << ", starts: " << word << "\n";
        } else if (shouldCheck && word.size() > 0 and !(table.contains(word))) {
            outFile << "Unknown word at line " << to_string(line_number) << ": "
                    << word << "\n";
        }
    }
}

int main() {
    string dict_name; // = "wordlist_small.txt";
    cout << "Enter dictionary file: ";
    cin >> dict_name;
    string spellcheck_name; // = "lyrics.txt";
    cout << "\nEnter name of file to spellcheck: ";
    cin >> spellcheck_name;
    string output; // = "output.txt";
    cout << "\nEnter output file: ";
    cin >> output;
    cout << "Parsing Dictionary.\n";
    clock_t t1 = clock();
    hashTable* table = parse_dictionary(dict_name);
    clock_t t2 = clock();
    double timeDiff = ((double)(t2 - t1)) / CLOCKS_PER_SEC;
    cout << "Hashmap Loaded. CPU time was " << timeDiff << " seconds.\n";
    t1 = clock();
    spellcheck(spellcheck_name, output, *table);
    t2 = clock();
    timeDiff = ((double)(t2 - t1)) / CLOCKS_PER_SEC;
    cout << "Spellcheck Complete. CPU time was " << timeDiff << " seconds.\n";

    return 0;
}