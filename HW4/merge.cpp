// Dylan Qiu ME '27
// ECE365 Fall 2025 HW4 Merge Question

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cctype>
using namespace std;

bool M[1001][1001]; // Check if a[i] and b[j] match

string solve(const string& a, const string& b, const string& c) {
    int lenA = a.length();
    int lenB = b.length();
    int lenC = c.length();
    
    // Basic length check
    if (lenA + lenB != lenC) return "*** NOT A MERGE ***";
    
    // Initialize as False
    for (int i = 0; i <= lenA; i++) {
        for (int j = 0; j <= lenB; j++) {
            M[i][j] = false;
        }
    }
    // Start at -1,-1 
    M[0][0] = true;

    // Bottom-up
    for (int i = 0; i <= lenA; i++) {
        for (int j = 0; j <= lenB; j++) {
            // Check if a path exists
            if (M[i][j]){
                // Taking next char from A
                if (i < lenA && a[i] == c[i + j]) {
                    M[i + 1][j] = true;
                }
                // Taking next char from B
                if (j < lenB && b[j] == c[i + j]) {
                    M[i][j + 1] = true;
                }
            }
            else{
                continue;
            }
        }
    }
    
    // If the bottom-right corner is false, we couldn't merge them
    if (!M[lenA][lenB]) return "*** NOT A MERGE ***";
    string result = c;
    int i = lenA;
    int j = lenB;
    
    while (i > 0 || j > 0) {
        if ((j > 0) && (M[i][j - 1])) {
            j--; // Taken from B
        } else {
            // Taken from A
            result[i + j - 1] = toupper(a[i - 1]);
            i--;
        }
    }
    return result;
}

int main() {
    string inputFile, outputFile;
    cout << "Enter name of input file: ";
    cin >> inputFile;
    cout << "Enter name of output file: ";
    cin >> outputFile;
    
    ifstream fin(inputFile);
    ofstream fout(outputFile);
    
    string a, b, c;
    while (getline(fin, a) && getline(fin, b) && getline(fin, c)) {
        fout << solve(a, b, c) << "\n";
    }
    
    fin.close();
    fout.close();
    
    return 0;
}