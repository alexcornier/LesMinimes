/***********************************************************************
** Gestion des factures
************************************************************************/

#include <iostream>
#include <map>
//#include <iterator>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
//#include <vector>
#include <limits>
#include "fonctions.hpp"
//#include "facture.hpp"
#include "paiement.hpp"
#include "maptexte.hpp"

// Path pour les fichiers texte des données de facturation
std::string const path_factures {"./factures/"};

// =====================================================================
// Paiement d'une facture à partir de sa clé date, usager bateau
// Enregistrement dans l'instance facture correspondante à la date
// du jour
// arguments : facture
// =====================================================================

void paiement( std::map<std::string, Facturation*> &facture )
{
    std::tm dt_debut {};
    std::string nom_usager {"0"};
    std::string nom_bateau {"0"};

    clear_screen();

    std::cout << "********** Enregistrement d'un paiement ********** \n\n";

    std::cout << "Entrez la date de debut du sejour jj/mm/aaaa (ou 0 pour abandonner)\n";
    std::cout << "> ";
    std::cin >> std::get_time(&dt_debut, "%d/%m/%Y");
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (dt_debut.tm_year > 99) // année 2000 minimum
    {
        std::cout << "Entrez le nom de l'usager (ou 0 pour abandonner)\n";
        std::cout << "> ";
        std::cin >> nom_usager;
        format_nom(nom_usager);
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    if (nom_usager != "0")
    {
        std::cout << "Entrez le nom du bateau (ou 0 pour abandonner)\n";
        std::cout << "> ";
        std::cin >> nom_bateau;
        format_nom(nom_bateau);
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    if (nom_bateau != "0")
    {
        std::string cle_facture {};   
        {
            // Construction de la cle pour la facture
            std::ostringstream oss {}; // flux out pour construire la date au format aaaammjj
            oss << std::put_time(&dt_debut, "%Y%m%d");
            cle_facture = oss.str();
            cle_facture = cle_facture + nom_usager + nom_bateau;
        }
        std::string str_debut {};
        {
            // Enregistrement de la date de début au format dd/mm/aaaa pour affichage si facture non trouvée
            std::ostringstream oss {};
            oss << std::put_time(&dt_debut, "%d/%m/%Y");
            str_debut = oss.str();
        }

        // Recherche d'une facture identique, sinon création de la facture
        if ( facture.find(cle_facture) != facture.end() )
        {
            // Vérification que la facture n'est pas déjà réglée
            if ( facture.find(cle_facture)->second->get_date_paiement().tm_year < 99 )
            {
                // Chargement du montant TTC pour demande de contrôle
                float montant {0};
                montant = facture.find(cle_facture)->second->get_total_TTC();

                // Validation
                std::string confirmation {};
                std::cout << "Confirmez-vous le montant de ";
                std::cout << std::fixed << std::setprecision(2) << montant << " TTC ?";
                std::cout << "> ";
                std::cin >> confirmation;
                format_nom(confirmation);
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (confirmation.front() == 'O')
                {
                    // Chargement de la date courante
                    std::tm date_paiement {};
                    std::time_t t_courant = std::time(nullptr);
                    date_paiement = *std::localtime(&t_courant);

                    // Mise à jour 
                    facture.find(cle_facture)->second->set_date_paiement(date_paiement);

                    std::cout << "\nPaiement enregistre pour l'usager " << nom_usager << "\n";
                    std::cout << "et le bateau " << nom_bateau << "\n";
                    std::cout << "pour le sejour commencant le " << str_debut << std::endl;

                    // Attendre une entrée console pour revenir au menu principal
                    attente_console();
                }
            }
            else
            {
                std::tm date_paiement {};
                date_paiement = facture.find(cle_facture)->second->get_date_paiement();
                std::cout << "\nLa facture trouvee pour l'usager " << nom_usager << "\n";
                std::cout << "et le bateau " << nom_bateau << "\n";
                std::cout << "pour un sejour commencant le " << str_debut << "\n";
                std::cout << "a deja ete reglee le ";
                std::cout << std::put_time(&date_paiement, "%d/%m/%Y") << std::endl;

                // Attendre une entrée console pour revenir au menu principal
                attente_console();
            }
        }
        else
        {
            std::cout << "\nAucune facture trouvee pour l'usager " << nom_usager << "\n";
            std::cout << "et le bateau " << nom_bateau << "\n";
            std::cout << "pour un sejour commencant le " << str_debut << std::endl;

            // Attendre une entrée console pour revenir au menu principal
            attente_console();
        }
    }
}

// =====================================================================
// Recherche et liste des factures impayées
// arguments : facture
// =====================================================================

void impayes( std::map<std::string, Facturation*> &facture )
{
    std::tm date_du_jour {};
    std::map<std::string, Facturation*> tmp_facture;    

    clear_screen();

    // Chargement de la date du jour
    std::time_t t_courant = std::time(nullptr);
    date_du_jour = *std::localtime(&t_courant);

    // Itération sur la table des factures
    std::map<std::string, Facturation*>::iterator it_facture {};
    for (it_facture = facture.begin(); it_facture != facture.end(); ++it_facture)
    {
        // Si la date de paiement n'est pas positionnée, i.e. 01/01/1900
        if ( it_facture->second->get_date_paiement().tm_year <= 99 )
        {
            // clé de facture
            std::string cle_facture { it_facture->first };

            // Ajout de l'instance de facture impayée dans le container map temporaire
            // pour générer le fichier de factures impayées en réutilisant la fonction d'enregistrement
            tmp_facture.insert( std::make_pair( cle_facture, facture.find(cle_facture)->second ));
        }
    }

    // Si au moins une facture impayée a été troucée alors création du fichier de facturation
    if ( ! tmp_facture.empty() )
    {
        // Date du jour au format string
        std::string str_dt_jour {};
        {
            std::ostringstream oss {};
            oss << std::put_time(&date_du_jour, "%Y%m%d");
            str_dt_jour = oss.str();
        }

        // Création du nom du fichier de facturation en utilisant une clé de facture pour garantir l'unicité
        std::string fichier_facturation {path_factures};
        fichier_facturation = fichier_facturation + str_dt_jour + "_factures_impayees.txt";
        
        // Enregistrement du fichier de facturation au format CSV
        if (facture_vers_texte(fichier_facturation, tmp_facture))
        {
            std::cout << "\nLe fichier des factures impayees " << fichier_facturation << "\n";
            std::cout << "n'a pas pu etre enregistre, ou une erreur s'est produite" << std::endl;

            // Attendre une entrée console pour revenir au menu principal
            attente_console();
        }
        else
        {
            std::cout << "\nLe fichier des factures impayees " << fichier_facturation << "\n";
            std::cout << "a ete enregistre" << std::endl;

            // Attendre une entrée console pour revenir au menu principal
            attente_console();                        
        }
    }
    else
    {
        std::cout << "\nAucune facture impayee trouve\n" << std::endl;

        // Attendre une entrée console pour revenir au menu principal
         attente_console();  
    }
}