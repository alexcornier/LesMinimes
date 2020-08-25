/***********************************************************************
** Gestion des séjours passagers et des abonnements
************************************************************************/

#include <iostream>
// #include <map>
#include <iterator>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>
#include "fonctions.hpp"
#include "place.hpp"
// #include "GestionPort.hpp"

// =====================================================================
// Saisie des données en console pour le sejour, création de la clé
// contrôle si existe, puis construction d'une nouvelle instance
// de GestionPort dans le container map sejour
// Ajout usager, bateau si nouveaux
// arguments : map usager, bateau, place, sejour
// =====================================================================

void ajout_sejour(
    int abonne,
    std::map<std::string, int> &usager,
    std::map<std::string, int> &bateau,
    std::map<std::string, int> &place,
    std::map<std::string, GestionPort*> &sejour
)
{
    int place_type {99};
    bool place_ok {};
    std::vector<int> places_dispo {};

    std::tm dt_debut {};
    std::tm dt_recherche {};
    std::tm dt_fin {};
    std::string nom_usager {"0"};
    std::string nom_bateau {"0"};
    int taille_bateau {0};
    bool corps_mort {0};
    int choix_place {0};
    bool elec {};
    bool eau {};

    clear_screen();

    if (abonne)
    {
        std::cout << "********** Ajout d'un nouvel abonnement ********** \n\n";

        // Chargement de la date système (courante)
        std::time_t t_courant = std::time(nullptr);
        std::tm date_courante = *std::localtime(&t_courant);

        date_courante.tm_hour = 0;
        date_courante.tm_min = 0;
        date_courante.tm_sec = 0;

        // pour démarrer la recherche de place à aujourd'hui pour un abonné
        dt_recherche = date_courante;

        // 1er Janvier au 31 Décembre de l'année en cours pour un abonné
        dt_debut.tm_mday = 1;
        dt_debut.tm_mon = 0;
        dt_debut.tm_year = date_courante.tm_year;

        dt_fin.tm_mday = 31;
        dt_fin.tm_mon = 11;
        dt_fin.tm_year = date_courante.tm_year;
    }
    else
    {
        std::cout << "********** Ajout d'un Sejour Passager ********** \n\n";

        std::cout << "Entrez la date de debut du sejour jj/mm/aaaa (ou 0 pour abandonner)\n";
        std::cout << "> ";
        std::cin >> std::get_time(&dt_debut, "%d/%m/%Y");
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (dt_debut.tm_year > 99) // année 2000 minimum
        {
            std::cout << "Entrez la date de fin du sejour jj/mm/aaaa (ou 0 pour abandonner)\n";
            std::cout << "> ";
            std::cin >> std::get_time(&dt_fin, "%d/%m/%Y");
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    
    if (dt_fin.tm_year > 99) // année 2000 minimum
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
        std::cout << "Entrez la taille du bateau (ou 0 pour abandonner)\n";
        std::cout << "> ";
        std::cin >> taille_bateau;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    if (taille_bateau != 0)
    {
        std::cout << "Preferez-vous une place a quai ou un corps-mort ? (0 place a quai, 1 corps-mort)\n";
        std::cout << "> ";
        std::cin >> corps_mort;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // Récupération du type en fonction de la taille
        if (corps_mort)
        {
            place_type = get_place_type(0);
        }
        else
        {
            place_type = get_place_type(taille_bateau);
        }

        // liste des places disponibles
        if (abonne)
        {
            places_disponibles(place_type, dt_recherche, dt_fin, places_dispo, place, usager, sejour);
        }
        else
        {
            places_disponibles(place_type, dt_debut, dt_fin, places_dispo, place, usager, sejour);
        }
        if (places_dispo.empty())
        {
            std::cout << "\nAucune place de type " << place_type << " n'est disponible du ";
            std::cout << std::put_time(&dt_debut, "%d/%m/%Y") << " au ";
            std::cout << std::put_time(&dt_fin, "%d/%m/%Y") << std::endl;
            do
            {
                choix_place = attente_console();
            } while (choix_place != 0);
        }
        else
        {
            std::cout << "Places disponibles de type " << place_type << "\n";
            for (int i = 0; i < places_dispo.size(); i++)
            {
                std::cout << places_dispo[i] << ", ";
            }
            std::cout << std::endl;
            do
            {
                // Choix de la place
                std::cout << "Entrez le numero de place de port (ou 0 pour abandonner)\n";
                std::cout << "> ";
                std::cin >> choix_place;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                place_ok = false;
                for (int j = 0; j < places_dispo.size(); j++)
                {
                    if (choix_place == places_dispo[j])
                    {
                        place_ok = true;
                        break;
                    }
                    
                }
            } while (! place_ok && choix_place > 0);
        }            
    }

    if (choix_place != 0)
    {
        if (corps_mort || place_type == 1)
        {
            elec = false;
            eau = false;
        }
        else
        {
            std::cout << "Utilisez-vous l'electricite ? (0 pour non, 1 pour oui)\n";
            std::cout << "> ";
            std::cin >> elec;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Utilisez-vous l'eau ? (0 pour non, 1 pour oui)\n";
            std::cout << "> ";
            std::cin >> eau;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        // Construction de la cle pour le sejour
        std::string cle_sejour {};
        {
            std::ostringstream oss {}; // flux out pour construire la date au format aaaammjj
            oss << std::put_time(&dt_debut, "%Y%m%d");
            cle_sejour = oss.str();
        }
        cle_sejour = cle_sejour + nom_usager + nom_bateau;

        // Vérifier qu'un séjour identique n'existe pas déjà
        if ( sejour.find(cle_sejour) == sejour.end() )
        {
            // Enregistrer l'usager si non trouve, mettre à jour si abonne
            if ( usager.find(nom_usager) == usager.end() )
            {
                if (abonne)
                {
                    usager[nom_usager] = choix_place;
                }
                else
                {
                    usager[nom_usager] = 0;
                }
            }
            else
            {
                if (abonne)
                {
                    usager[nom_usager] = choix_place;
                }
            }

            // Enregistrer le bateau si non trouve
            if ( bateau.find(nom_bateau) == bateau.end() )
            {
                bateau[nom_bateau] = taille_bateau;
            }

            // Enregistrer le sejour
            sejour.insert( std::make_pair( cle_sejour, new GestionPort() ) );

            sejour.find(cle_sejour)->second->set_debut(dt_debut);
            sejour.find(cle_sejour)->second->set_fin(dt_fin);
            sejour.find(cle_sejour)->second->set_usager(nom_usager);
            sejour.find(cle_sejour)->second->set_bateau(nom_bateau);
            sejour.find(cle_sejour)->second->set_place(choix_place);
            sejour.find(cle_sejour)->second->set_electricite(elec);
            sejour.find(cle_sejour)->second->set_eau(eau);

            clear_screen();

            // Résumer les données prises en compte
            if (abonne)
            {
                std::cout << "\nAbonnement enregistre : \n\n";
            }
            else
            {
                std::cout << "\nSejour enregistre : \n\n";
            }
            
            std::cout << "Du " << std::put_time(&dt_debut, "%d/%m/%Y");
            std::cout << " au : " << std::put_time(&dt_fin, "%d/%m/%Y") << "\n";
            std::cout << "Usager : " << nom_usager << "\n";
            std::cout << "Bateau : " << nom_bateau << " , taille : " << taille_bateau << "\n";
            std::cout << "Place  : " << choix_place << " , Type : " << place_type << "\n";
            std::cout << "Electricite : " << elec << " , eau : " << eau << std::endl;

            // Attendre une entrée console pour revenir au menu principal
            attente_console();
        }
        else
        {
            std::cout << "\nUn sejour debutant le " << std::put_time(&dt_debut, "%d/%m/%Y") << "\n";
            std::cout << "existe deja pour " << nom_usager << " avec le bateau " << nom_bateau << std::endl;

            // Attendre une entrée console pour revenir au menu principal
            attente_console();
        }
    }
}