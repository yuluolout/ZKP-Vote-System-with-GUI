#include "ViewBallots.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>

using namespace std;

ViewBallots::ViewBallots(const string &filename) : filename(filename) {
    loadBallots();
}

void ViewBallots::loadBallots() {
    //Open the file for reading
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string nickname, candidate;
        if (!(iss >> nickname >> candidate)) {
            cerr << "Error reading line: " << line << endl;
            continue;
        }
        //Adding the nickname(this will replace to encrypted nickname in main fucntion) and candidate to the ballots vector
        ballots.emplace_back(nickname, candidate);
    }
}

//Display the loaded ballots
void ViewBallots::displayBallots() const {
    ifstream file("voting_record.txt");
    string EncryptedNickname, candidate;
    cout << "Voting Records:" << endl;

    while (file >> EncryptedNickname >> candidate){
        cout << EncryptedNickname << " " << candidate << endl;
    }
    
}
