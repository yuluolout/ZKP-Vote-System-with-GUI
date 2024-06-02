#include "VoterLogin.hpp"
#include "VoterRegistration.hpp"
#include "ZKPVoting.hpp"
#include "ViewResults.hpp"
#include "ViewBallots.hpp"
#include "VoterEncryption.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

// Global variables for user data and voting records
map<string, string> users; // <nickname, password>
map<string, int> userVotes; // <nickname, votes>

// Function prototypes
void registerUser(VoterRegistration &registration);
void loginUser(const VoterLogin &login);
void castVote(ZKPVoting &zkpVoting);
void viewResults(ZKPVoting &zkpVoting);
void viewBallots(const ViewBallots &ballots);

int main() {
    VoterLogin voterLogin(users); // Voterlogin instance initialized with the users map
    VoterRegistration voterRegistration(users, userVotes); // VoterRegistration instance initialized with users and userVotes maps
    ZKPVoting zkpVoting; // ZKPVoting instance for handling voting with zero-knowledge proofs
    ViewResults viewResults; // ViewResults instance for displaying voting results
    ViewBallots viewBallots("voting_record.txt"); // ViewBallots instance for display voting reocrds
    int choice;

    // Load user data from file
    ifstream userFile("user_list.txt");
    if (userFile.is_open()) {
        string nickname, password, encryptedNickname;
        while (userFile >> nickname >> password) {
            users[nickname] = password;
            userVotes[nickname] = 1; // Each new user has 1 vote initially
            ifstream file("voting_record.txt");
            while (file >> encryptedNickname >> password){
                if (VoterEncryption::EncryptNickname(nickname) == encryptedNickname){
                userVotes[nickname] = 0;
                }
            }
            file.close();
        }
        userFile.close();
    }

    do {
        cout << "Please do not enter undefined choice!" << endl;
        cout << "1. Register\n2. Login\n3. Vote\n4. View Results\n5. View Voting Records\n6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                registerUser(voterRegistration);
                break;
            case 2:
                loginUser(voterLogin);
                break;
            case 3:
                castVote(zkpVoting);
                break;
            case 4:
                viewResults.displayResults();
                break;
            case 5:
                viewBallots.displayBallots();
                break;
            case 6:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 6);

    // Save user data to file
    ofstream outFile("user_list.txt");
    if (outFile.is_open()) {
        for (const auto &user : users) {
            outFile << user.first << " " << user.second << endl;
        }
        outFile.close();
    }

    return 0;
}

void registerUser(VoterRegistration &registration) {
    string nickname, password;
    cout << "Enter nickname: ";
    cin >> nickname;
    cout << "Enter password: ";
    cin >> password;

    // Attempt to register the user
    if (registration.registerVoter(nickname, password)) {
        cout << "Registration successful!" << endl;
    } else {
        cout << "Nickname already exists. Try a different one." << endl;
    }
}

void loginUser(const VoterLogin &login) {
    string nickname, password;
    cout << "Enter nickname: ";
    cin >> nickname;
    cout << "Enter password: ";
    cin >> password;

    if (login.login(nickname, password)) {
        cout << "Login successful. You have " << userVotes[nickname] << " votes remaining." << endl;
    } else {
        cout << "Invalid nickname or password." << endl;
    }
}

void castVote(ZKPVoting &zkpVoting) {
    string nickname, password;
    cout << "Enter nickname: ";
    cin >> nickname;
    cout << "Enter password: ";
    cin >> password;

    // Check if user can log in
    if (users.find(nickname) == users.end() || users[nickname] != password) {
        cout << "Login failed. Cannot cast vote." << endl;
        return;
    }
    // Check if user has votes reamining
    if (userVotes[nickname] == 0) {
        cout << "You have already voted." << endl;
        return;
    }

    int candidateChoice;
    string candidate;
    cout << "Enter 1 to vote for Alice or 2 to vote for Bob: ";
    cin >> candidateChoice;
    if (candidateChoice == 1) {
        candidate = "Alice";
    } else if (candidateChoice == 2) {
        candidate = "Bob";
    } else {
        cout << "Invalid choice." << endl;
        return;
    }

    //Generate and verify the proof
    zkpVoting.generate_and_OutputProof(nickname, candidate);
    if (zkpVoting.verify_Proof(nickname, candidate)) {
        zkpVoting.updateResults(candidate);
        userVotes[nickname] = 0;
        ofstream voteRecordFile("voting_record.txt", ios::app);
        voteRecordFile << VoterEncryption::EncryptNickname(nickname) << " " << candidate << endl;
        voteRecordFile.close();
        cout << "Vote cast successfully." << endl;
    } else {
        cout << "Vote verification failed." << endl;
    }
}
