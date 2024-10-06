#ifndef COMPTE_H
#define COMPTE_H

#include "Client.h"
#include <mysql/mysql.h>

class Compte {
private:
    int rib;
    double solde;
    Client proprietaire;
    MYSQL* conn; // Connexion MySQL

public:
    Compte();  // Constructeur par défaut
    Compte(int rib, double solde, const Client& proprietaire, MYSQL* conn);
    // Autres méthodes
    void debiter(double montant);
    void crediter(double montant);
    void virement(Compte& destinataire, double montant);
    void consulterSolde() const;
    int getRIB() const;
    const Client& getProprietaire() const;
};

#endif
