#include "Compte.h"
#include <iostream>

// Constructeur par défaut
Compte::Compte() : rib(0), solde(0.0), proprietaire(Client("", "", "", "", 0)), conn(nullptr) {}

Compte::Compte(int rib, double solde, const Client& proprietaire, MYSQL* conn)
    : rib(rib), solde(solde), proprietaire(proprietaire), conn(conn) {}
//autres methodes
void Compte::debiter(double montant) {
    if (montant <= solde) {
        solde -= montant;
        cout << montant << " EUR debite. Nouveau solde : " << solde << " EUR." << endl;

        // Requête SQL pour mettre à jour le solde dans la base de données
        string query = "UPDATE Compte SET solde = " + to_string(solde) + " WHERE rib = " + to_string(rib);
        if (mysql_query(conn, query.c_str())) {
            cerr << "Erreur lors de la mise à jour du solde : " << mysql_error(conn) << endl;
        } else {
            cout << "Solde mis à jour dans la base de donnees." << endl;
        }
    } else {
        cout << "Solde insuffisant pour debiter." << endl;
    }
}

void Compte::crediter(double montant) {
    solde += montant;
    cout << montant << " EUR credite. Nouveau solde : " << solde << " EUR." << endl;

    // Requête SQL pour mettre à jour le solde dans la base de donnees
    string query = "UPDATE Compte SET solde = " + to_string(solde) + " WHERE rib = " + to_string(rib);
    if (mysql_query(conn, query.c_str())) {
        cerr << "Erreur lors de la mise à jour du solde : " << mysql_error(conn) << endl;
    } else {
        cout << "Solde mis à jour dans la base de donnees." << endl;
    }
}

void Compte::virement(Compte& destinataire, double montant) {
    if (montant <= solde) {
        debiter(montant);
        destinataire.crediter(montant);
        cout << "Virement de " << montant << " EUR effectue." << endl;
    } else {
        cout << "Solde insuffisant pour le virement." << endl;
    }
}

void Compte::consulterSolde() const {
    cout << "Solde actuel : " << solde << " EUR." << endl;
}

int Compte::getRIB() const {
    return rib;
}

const Client& Compte::getProprietaire() const {
    return proprietaire;
}
