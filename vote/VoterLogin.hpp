#ifndef VOTERLOGIN_HPP
#define VOTERLOGIN_HPP

#include <string>
#include <map>

using namespace std;

class VoterLogin{
public:
    VoterLogin(const map<string, string> &users); // User login constructor
    bool login(const string &nickname, const string &password) const; //Check if user existed

private:
    const map<string, string> &users; // User map
};
#endif