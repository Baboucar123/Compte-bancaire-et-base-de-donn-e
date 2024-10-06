#include "Client.h"

Client::Client(string cin, string nom, string prenom, string telephone, int codeSecret)
    : cin(cin), nom(nom), prenom(prenom), telephone(telephone), codeSecret(codeSecret) {}

bool Client::verifierCodeSecret(int code) const {
    return code == codeSecret;
}

string Client::getNom() const {
    return nom;
}

string Client::getPrenom() const {
    return prenom;
}
