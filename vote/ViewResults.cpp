#include "ViewResults.hpp"
#include <iostream>
#include <fstream>

using namespace std;

ViewResults::ViewResults() {}

void ViewResults::displayResults() const {
    // Initialize the number of votes
    int votesAlice, votesBob;
    votesAlice = 0;
    votesBob = 0;
    
    // Display the voting reocrd
cout << "Current Voting Results:" <<endl;
ifstream file("voting_record.txt");
string EncryptedNickname, candidate;
    while (file >> EncryptedNickname >> candidate){
        if (candidate == "Alice"){
            votesAlice++;
        }
        else{
            votesBob++;
        }
    }
    // Display who win the vote
    if (votesAlice > votesBob){
        cout << "Alice Win!" <<endl;
    }
    else if (votesBob > votesAlice){
        cout << "Bob Win!" <<endl;
    }
    else {
        cout << "Tied!" <<endl;
    }
}
