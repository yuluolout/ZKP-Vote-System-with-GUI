#include "VoterEncryption.hpp"
#include <iomanip>
#include <sstream>

using namespace std;

//Use hash function sha256 in this vote system
string VoterEncryption::sha256(const string str){
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    stringstream ss;

    //The process for hash encryption sha256 in C++
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i){
        ss << hex << setw(2) << setfill('0') << (int) hash[i];
    }
    return ss.str();
    
}

//Encryption the user nickname
string VoterEncryption::EncryptNickname(const string &nickname){
    return sha256(nickname); //Directly encryption the nickname using sha256
}