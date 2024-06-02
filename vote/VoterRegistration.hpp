#ifndef VOTERREGISTRATION_HPP
#define VOTERREGISTRATION_HPP

#include <stdio.h>
#include <string>
#include <fstream>
#include <map>

using namespace std;

class VoterRegistration{
public:
    VoterRegistration(map<string, string> &users, map<string, int> &userVotes);
    bool registerVoter(const string &nickname, const string &password); //Check if the register success or fail

    private:
    map<string, string> &users;
    map<string, int> &userVotes;
};
#endif