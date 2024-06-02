#include "VoterRegistration.hpp"

using namespace std;

//Constructor that initializes the VoterRegistration object and loads the existing user list from the file
VoterRegistration::VoterRegistration(map<string, string> &users, map<string, int> &userVotes) : users(users), userVotes(userVotes){}

// Check if voter nickname already exist
bool VoterRegistration::registerVoter(const string &nickname, const string &password){
    if (users.find(nickname) != users.end()){
        return false; // Nickname already exist
    }

    users[nickname] = password;
    userVotes[nickname] = 1; // Each new user has 1 vote

    return true; 
}
