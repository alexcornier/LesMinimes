/***********************************************************************
**                       LesMinimesEnUnClic
**
** Programme de Gestion de Port simplifié, qui prend en charge
** les séjours ponctuels, avec un prix journalier, les abonnements
** et qui produit les données de facturation et enregistre les paiements
**
** Alexandre Cornier - 2020
************************************************************************/

//#include <iostream> // déjà dans maptexte.hpp
//#include <fstream>
//#include <iterator>
#include <iomanip>
#include <ctime>
#include <limits>
//#include <map> // déjà dans maptexte.hpp
#include "maptexte.hpp"
#include "GestionPort.hpp"
#include "Facturation.hpp"
#include "Tarifs.hpp"
#include "fonctions.hpp"
#include "sejour.hpp"
#include "place.hpp"
#include "facture.hpp"
#include "paiement.hpp"

// Noms des fichiers texte pour les map et class
std::string const fichier_usagers {"./database/usagers.txt"};
std::string const fichier_bateaux {"./database/bateaux.txt"};
std::string const fichier_places {"./config/places.txt"};
std::string const fichier_tarifs {"./config/tarifs.txt"};
std::string const fichier_sejours {"./database/sejours.txt"};
std::string const fichier_factures {"./database/factures.txt"};

// Definition des container map
std::map<std::string, int> usager {};
std::map<std::string, int> bateau {};
std::map<std::string, int> place {};
std::map<int, std::string> place_bateau {};
std::map<std::string, GestionPort*> sejour {};
std::map<std::string, Facturation*> facture {};

// Construction de l'nstance de la classe des Tarifs
Tarifs Prix {};

// =====================================================================
//                         MENU FACTURATION
// =====================================================================

void menu_facturation(int &continuer)
{
	int choix {0};
	
	std::cout << "********** Menu Facturation ********** \n\n";
	std::cout << "(1): Facturer un sejour passager\n";
	std::cout << "(2): Facturer un abonnement\n";
	std::cout << "(3): Facturer les sejours termines restants\n";
	std::cout << "(4): Facturer les abonnes pour le mois\n";
	std::cout << "\n(0): Retour >>\n\n";
    std::cout << "Choix> ";
	std::cin >> choix;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	
	switch(choix)
	{
        case 1:
            facture_unique(
                false, usager, bateau, place, place_bateau,
                sejour, facture, Prix
            );
            break;
        case 2:
            facture_unique(
                true, usager, bateau, place, place_bateau,
                sejour, facture, Prix
            );
            break;
        case 3:
            facture_batch(
                false, usager, bateau, place, place_bateau,
                sejour, facture, Prix
            );
            break;
        case 4:
            facture_batch(
                true, usager, bateau, place, place_bateau,
                sejour, facture, Prix
            );
            break;
        case 0:
            continuer = 0;
            break;
        default:
            continuer = 2;
            break;
	}
}

// =====================================================================
//                          MENU PAIEMENT
// =====================================================================

void menu_paiement(int &continuer)
{
	int choix {0};
	
	std::cout << "********** Menu Paiement ********** \n\n";
	std::cout << "(1): Enregistrer le paimeent d'une facture\n";
	std::cout << "(2): Lister les impayes\n";
	std::cout << "\n(0): Retour >>\n\n";
    std::cout << "Choix> ";
	std::cin >> choix;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	
	switch(choix)
	{
        case 1:
            paiement( facture );
            break;
        case 2:
            impayes( facture );
            break;
        case 0:
            continuer = 0;
            break;
        default:
            continuer = 2;
            break;
	}
}

// =====================================================================
//                         MENU PRINCIPAL
// =====================================================================

void menu_principal(int &continuer)
{
	int choix {0};
    int rester {1};
    int quitter {0};

	std::cout << "********** Menu Principal ********** \n\n";
	std::cout << "(1): Rechercher une place disponible\n";    
	std::cout << "(2): Enregistrer un nouveau sejour\n";
	std::cout << "(3): Enregistrer un nouvel abonnement\n";
	std::cout << "(4): Facturation >>\n";
	std::cout << "(5): Paiement >>\n";
	std::cout << "(6): Enregistrer les donnees\n";
	std::cout << "\n(0): Quitter >>\n\n";
    std::cout << "Choix> ";
	std::cin >> choix;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	switch(choix)
	{
        case 1:
            recherche_place(usager, place, sejour);
            break;
        case 2:
            ajout_sejour(0, usager, bateau, place, sejour);
            break;
        case 3:
            ajout_sejour(1, usager, bateau, place, sejour);
            break;
        case 4:
            rester = 1;
            while (rester)
            {
                clear_screen();
                if (rester == 2)
                {
                    std::cout << "ATTENTION : vous devez saisir une des options de la liste ! \n" << std::endl;
                }
                menu_facturation(rester);
            }            
            break;
        case 5:
            rester = 1;
            while (rester)
            {
                clear_screen();
                if (rester == 2)
                {
                    std::cout << "ATTENTION : vous devez saisir une des options de la liste ! \n" << std::endl;
                }
                menu_paiement(rester);
            }            
            break;
        case 6:
            // Enregistrement des map usagers, bateaux, places de port,
            if (! enregistrer(
                fichier_usagers, fichier_bateaux, fichier_places, fichier_sejours, fichier_factures,
                usager, bateau, place, sejour, facture
                )
            )
            {
                std::cout << "Les enregistrements ont ete effectues" << std::endl;
            }
            
            // Attendre une entrée console pour revenir au menu principal
            attente_console();

            break;
        case 0:
            continuer = 0;
            break;
        default:
            continuer = 2;
            break;
	}
}

// =====================================================================
//                               MAIN
// =====================================================================

int main ()
{
    // Bonjour et attendez le chargement des tables...
    std::cout << "\n********************************************";
    std::cout << "\n**          LesMinimesEnUnClic            **";
    std::cout << "\n**                                        **";
    std::cout << "\n**  Merci de patienter quelques instants  **";
    std::cout << "\n**    le temps de charger les donnees     **";
    std::cout << "\n********************************************";
    std::cout << std::endl;

    // =====================================================================
    //                         INITIALISATION
    // =====================================================================    

    int erreur {0};

    // Chargement des map usagers, bateaux, places de port
    if (texte_vers_map(fichier_usagers, usager))
    {
        erreur = 1;
    }
    if (texte_vers_map(fichier_bateaux, bateau))
    {
        erreur = 1;
    }
    if (texte_vers_map(fichier_places, place))
    {
        erreur = 1;
    }

    // Chargement de l'instance Tarifs
    if (texte_vers_prix(fichier_tarifs, Prix))
    {
        return 1;
    };

    // Chargement des sejours
    if (texte_vers_sejour(fichier_sejours, sejour))
    {
        erreur = 1;
    }

    // Création du container map d'indexation place, bateau pour les abonnés
    // Cet index facilite la création de la clé pour facturer un abonné
    if (! erreur)
    {
        creation_abonne(place_bateau, usager, bateau, sejour);
    }

    // Chargement des factures
    if (texte_vers_facture(fichier_factures, facture))
    {
        erreur = 1;
    }
    
    // =====================================================================
    //                               MENU
    // =====================================================================    

    if (! erreur)
    {
        int continuer = 1;

        while (continuer)
        {
            clear_screen();

            if (continuer == 2)
            {
                std::cout << "ATTENTION : vous devez saisir une des options de la liste ! \n" << std::endl;
            }

            menu_principal(continuer);
        }
    
        // =====================================================================
        //                      ENREGISTREMENT et CLOTURE
        // =====================================================================    

        // Enregistrement des map usagers, bateaux, places de port,
        // séjours et factures avant de quitter
        if (enregistrer
            (
                fichier_usagers, fichier_bateaux, fichier_places, fichier_sejours, fichier_factures,
                usager, bateau, place, sejour, facture
            )
        )
        {
            std::cout << ">>> Une ou plusieurs erreurs se sont produites lors des operations sur les fichiers\n" << std::endl;
        }  
    }
    else
    {
        std::cout << ">>> Une ou plusieurs erreurs se sont produites lors des operations sur les fichiers\n" << std::endl;
    }
    
    std::cout << "\n********************************************";
    std::cout << "\n**          LesMinimesEnUnClic            **";
    std::cout << "\n**              Au revoir !               **";
    std::cout << "\n********************************************";
    std::cout << std::endl;

    return erreur;
}