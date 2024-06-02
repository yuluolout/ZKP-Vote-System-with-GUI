#include "VoterLogin.hpp"

using namespace std;

VoterLogin::VoterLogin(const map<string, string> &users) : users(users){}

bool VoterLogin::login(const string &nickname, const string &password) const {
    auto it = users.find(nickname);
    return (it != users.end() && it -> second == password);
}