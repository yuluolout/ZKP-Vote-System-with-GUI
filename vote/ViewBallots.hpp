#ifndef VIEW_BALLOTS_HPP
#define VIEW_BALLOTS_HPP

#include <string>
#include <vector>

using namespace std;

class ViewBallots {
public:
    ViewBallots(const string &filename);
    void displayBallots() const; //Display the loaded ballots

private:
    string filename;
    vector<pair<string, string>> ballots;

    void loadBallots(); //Loading the ballots from the file
};
#endif 
