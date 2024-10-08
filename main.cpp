#include <iostream>
#include "Client.h"
#include "Compte.h"
#include <mysql/mysql.h>

using namespace std;

// connexion à la base de donnée 

MYSQL* connectDB() {
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL) {
        cerr << "mysql_init() a echoue." << endl;
        exit(EXIT_FAILURE);
    }

    if (mysql_real_connect(conn, "192.168.137.68", "coly", "123456789", "banque", 0, NULL, 0) == NULL) {
        cerr << "mysql_real_connect() a echoue." << endl;
        cerr << "Erreur : " << mysql_error(conn) << endl;
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }
    cout << "Connexion a la base de donnees reussie !" << endl;
    return conn;
}

// Load accounts from the database
void chargerComptesDepuisBDD(MYSQL* conn, Compte comptes[], int& nombreComptes) {
    MYSQL_RES *res;
    MYSQL_ROW row;
    string query = "SELECT Compte.rib, Compte.solde, Client.cin, Client.nom, Client.prenom, Client.telephone, Client.code_secret FROM Compte JOIN Client ON Compte.client_cin = Client.cin";
    
    if (mysql_query(conn, query.c_str())) {
        cerr << "Échec de la requête : " << mysql_error(conn) << endl;
        return;
    }

    res = mysql_store_result(conn);
    if (res == NULL) {
        cerr << "Erreur de récupération des résultats : " << mysql_error(conn) << endl;
        return;
    }

    nombreComptes = 0;
    while ((row = mysql_fetch_row(res)) != NULL) {
        //Créer un client et un compte dans la base de donnée 
        Client client(row[2], row[3], row[4], row[5], stoi(row[6]));
        Compte compte(stoi(row[0]), stod(row[1]), client, conn); // Ajout de 'conn'
        comptes[nombreComptes++] = compte;
    }

    mysql_free_result(res);
}

// Fonctions pour presenter et permet de choisir dans le menu 
void afficherMenu(Compte& compte, Compte comptes[], int nombreComptes) {
    int choix;
    double montant;

    do {
        cout << "\nChoisissez une operation : " << endl;
        cout << "1. Debiter" << endl;
        cout << "2. Crediter" << endl;
        cout << "3. Virement" << endl;
        cout << "4. Consulter le solde" << endl;
        cout << "5. Se deconnecter" << endl;
        cin >> choix;

        switch (choix) {
            case 1:
                cout << "Montant a debiter : ";
                cin >> montant;
                compte.debiter(montant);
                break;
            case 2:
                cout << "Montant a crediter : ";
                cin >> montant;
                compte.crediter(montant);
                break;
            case 3: {
                cout << "RIB du beneficiaire : ";
                int ribDestinataire;
                cin >> ribDestinataire;

                Compte* destinataire = nullptr;
                for (int j = 0; j < nombreComptes; ++j) {
                    if (comptes[j].getRIB() == ribDestinataire) {
                        destinataire = &comptes[j];
                        break;
                    }
                }

                if (destinataire != nullptr) {
                    cout << "Montant à transferer : ";
                    cin >> montant;
                    compte.virement(*destinataire, montant);
                } else {
                    cout << "Beneficiaire introuvable." << endl;
                }
                break;
            }
            case 4:
                compte.consulterSolde();
                break;
            case 5:
                cout << "Deconnexion." << endl;
                break;
            default:
                cout << "Choix invalide." << endl;
        }
    } while (choix != 5);
}

int main() {
    // connexion à la base de donnée 
    MYSQL *conn = connectDB();

    // chargement des comptes dans la base de donnée 
    Compte comptes[10];  //le nombres max de comptes de 10 
    int nombreComptes;
    chargerComptesDepuisBDD(conn, comptes, nombreComptes);

    cout << "Bienvenue dans notre distributeur LCL !" << endl;

    bool fin = false;
    while (!fin) {
        int essaisRestants = 3;
        bool codeValide = false;

        //boucles pour le nombre de tentative qi est de 3 
        while (essaisRestants > 0) {
            cout << "\nVeuillez insérer votre carte bancaire et entrer votre code secret (4 chiffres) : " << endl;
            int codeSecret;
            cin >> codeSecret;

            //verification du code pour chacun des comptes 
            for (int i = 0; i < nombreComptes; ++i) {
                if (comptes[i].getProprietaire().verifierCodeSecret(codeSecret)) {
                    cout << "Bienvenue " << comptes[i].getProprietaire().getNom() << " "
                         << comptes[i].getProprietaire().getPrenom() << " !" << endl;

                    // Affichez le solde après la connexion
                    comptes[i].consulterSolde();

                    // Affichez le menu des opérations
                    afficherMenu(comptes[i], comptes, nombreComptes);
                    codeValide = true;
                    break;
                }
            }
            if (codeValide) {
                break;
            } else {
                essaisRestants--;
                cout << "Code incorrect. Il vous reste " << essaisRestants << " essai(s)." << endl;
            }
        }

        if (!codeValide) {
            cout << "Votre carte a ete retenue pour des raisons de securite. Veuillez contacter votre banque." << endl;
            break;
        }

        // demander le client s'il, veut quitter 
        cout << "\nSouhaitez-vous quitter ? (o/n) : ";
        char quitter;
        cin >> quitter;
        if (quitter == 'o' || quitter == 'O') {
            fin = true;
            cout << "Veuillez retirer votre carte bancaire." << endl;
        }
    }

    cout << "Merci de votre visite. A la prochaine !" << endl;

    // déconnexion à la base de donnée 
    mysql_close(conn);

    return 0;
}
