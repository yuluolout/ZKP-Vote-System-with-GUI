#ifndef VOTERENCRYPTION_HPP
#define VOTERENCRYPTION_HPP

#include <string>
#include <openssl/sha.h>

using namespace std;

class VoterEncryption{
public:
    static string EncryptNickname(const string &nickname); //Encrypt the user's nickname
    static string sha256(const string str); //Use hash function sha256 to encrypt
};
#endif