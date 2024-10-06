#ifndef CLIENT_H
#define CLIENT_H

#include <string>

using namespace std;

class Client {
private:
    string cin;
    string nom;
    string prenom;
    string telephone;
    int codeSecret;

public:
    Client(string cin, string nom, string prenom, string telephone, int codeSecret);
    bool verifierCodeSecret(int code) const;
    string getNom() const;
    string getPrenom() const;
};

#endif
