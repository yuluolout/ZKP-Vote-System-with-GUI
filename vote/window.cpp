#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>
#include "VoterLogin.hpp"
#include "VoterRegistration.hpp"
#include "ZKPVoting.hpp"
#include "ViewResults.hpp"
#include "ViewBallots.hpp"
#include "VoterEncryption.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

using namespace std;

// Global variables for user data and voting records
map<string, string> users; // <nickname, password>
map<string, int> userVotes; // <nickname, votes>

// Function prototypes for GUI callbacks
void registerUser(Fl_Widget*, void*);
void loginUser(Fl_Widget*, void*);
void castVote(Fl_Widget*, void*);
void viewResults(Fl_Widget*, void*);
void viewBallots(Fl_Widget*, void*);
void exitApp(Fl_Widget*, void*);
void showMessage(const string& title, const string& message);
void inputCallback(Fl_Widget*, void*);

// GUI Components
Fl_Input* inputNickname;
Fl_Input* inputPassword;
Fl_Window* window;
Fl_Button* registerBtn;
Fl_Button* loginBtn;
Fl_Button* voteBtn;
Fl_Button* resultsBtn;
Fl_Button* ballotsBtn;
Fl_Button* exitBtn;





int main() {
    // Initialize user data from file
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

    window = new Fl_Window(400, 300, "Voting System");

    inputNickname = new Fl_Input(150, 10, 230, 25, "Nickname:");
    inputPassword = new Fl_Input(150, 40, 230, 25, "Password:");

    registerBtn = new Fl_Button(10, 100, 120, 25, "Register");
    loginBtn = new Fl_Button(10, 130, 120, 25, "Login");
    voteBtn = new Fl_Button(10, 160, 120, 25, "Vote");
    resultsBtn = new Fl_Button(10, 190, 120, 25, "View Results");
    ballotsBtn = new Fl_Button(10, 220, 120, 25, "View Ballots");
    exitBtn = new Fl_Button(10, 250, 120, 25, "Exit");

    registerBtn->callback(registerUser);
    loginBtn->callback(loginUser);
    voteBtn->callback(castVote);
    resultsBtn->callback(viewResults);
    ballotsBtn->callback(viewBallots);
    exitBtn->callback(exitApp);

    inputNickname->callback(inputCallback);
    inputPassword->callback(inputCallback);

    window->end();
    window->show();

    // Initial state of buttons
    inputCallback(nullptr, nullptr);

    return Fl::run();
}





void showMessage(const string& title, const string& message) {
    fl_message_title(title.c_str());
    fl_message("%s", message.c_str());
}

void inputCallback(Fl_Widget*, void*) {
    const char* nickname = inputNickname->value();
    const char* password = inputPassword->value();

    bool enableButtons = nickname && *nickname && password && *password;

    registerBtn->activate();
    loginBtn->activate();
    voteBtn->activate();

    if (!enableButtons) {
        registerBtn->deactivate();
        loginBtn->deactivate();
        voteBtn->deactivate();
    }
}

void registerUser(Fl_Widget* widget, void* data) {
    const char* nickname = inputNickname->value();
    const char* password = inputPassword->value();

    if (!nickname || !password) {
        showMessage("Error", "Nickname and password cannot be empty.");
        return;
    }

    VoterRegistration voterRegistration(users, userVotes);
    if (voterRegistration.registerVoter(nickname, password)) {
        showMessage("Success", "Registration successful!");

        // Write the new user to the user_list.txt file
        ofstream outFile("user_list.txt", ios::app);
        if (outFile.is_open()) {
            outFile << nickname << " " << password << endl;
            outFile.close();
        }
    } else {
        showMessage("Error", "Nickname already exists. Try a different one.");
    }
}

void loginUser(Fl_Widget* widget, void* data) {
    const char* nickname = inputNickname->value();
    const char* password = inputPassword->value();

    if (!nickname || !password) {
        showMessage("Error", "Nickname and password cannot be empty.");
        return;
    }

    VoterLogin voterLogin(users);
    if (voterLogin.login(nickname, password)) {
        stringstream ss;
        ss << "Login successful. You have " << userVotes[nickname] << " votes remaining.";
        showMessage("Success", ss.str());
    } else {
        showMessage("Error", "Invalid nickname or password.");
    }
}

void castVote(Fl_Widget* widget, void* data) {
    const char* nickname = inputNickname->value();
    const char* password = inputPassword->value();

    if (!nickname || !password) {
        showMessage("Error", "Nickname and password cannot be empty.");
        return;
    }

    if (users.find(nickname) == users.end() || users[nickname] != password) {
        showMessage("Error", "Login failed. Cannot cast vote.");
        return;
    }

    if (userVotes[nickname] == 0) {
        showMessage("Error", "You have already voted.");
        return;
    }

    const char* choice = fl_input("Enter 1 to vote for Alice or 2 to vote for Bob:", "");
    if (!choice) return;

    int candidateChoice = atoi(choice);
    string candidate;
    if (candidateChoice == 1) {
        candidate = "Alice";
    } else if (candidateChoice == 2) {
        candidate = "Bob";
    } else {
        showMessage("Error", "Invalid choice.");
        return;
    }

    ZKPVoting zkpVoting;
    zkpVoting.generate_and_OutputProof(nickname, candidate);
    if (zkpVoting.verify_Proof(nickname, candidate)) {
        zkpVoting.updateResults(candidate);
        userVotes[nickname] = 0;
        ofstream voteRecordFile("voting_record.txt", ios::app);
        voteRecordFile << VoterEncryption::EncryptNickname(nickname) << " " << candidate << endl;
        voteRecordFile.close();
        showMessage("Success", "Vote cast successfully.");
    } else {
        showMessage("Error", "Vote verification failed.");
    }
}

void viewResults(Fl_Widget* widget, void* data) {
    ViewResults viewResults;
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    viewResults.displayResults();
    
    std::cout.rdbuf(old); 

    std::string results = buffer.str();
    showMessage("Current Voting Results", results);
}

void viewBallots(Fl_Widget* widget, void* data) {
    ViewBallots viewBallots("voting_record.txt");
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    viewBallots.displayBallots();
    
    std::cout.rdbuf(old); 

    std::string ballots = buffer.str();
    showMessage("Voting Records", ballots);
}

void exitApp(Fl_Widget* widget, void* data) {
    exit(0);
}