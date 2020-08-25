/***********************************************************************
** Gestion des places de port
************************************************************************/

#include <iostream>
// #include <map> // déjà dans place.hpp
#include <iterator>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
// #include <vector> // déjà dans fonctions.hpp
#include <limits>
#include "place.hpp"
#include "fonctions.hpp"
// #include "GestionPort.hpp" // déjà dans fonctions.hpp

// =====================================================================
// Liste des places disponibles pour un séjour, en supprimant d'abord
// les places des abonnés, puis en vérifiant les séjours en cours
// arguments : type de place, dispo, place, sejour
// =====================================================================

void places_disponibles(
    int place_type,
    std::tm date_debut,
    std::tm date_fin,
    std::vector<int> &places_dispo,
    std::map<std::string, int> place,
    std::map<std::string, int> usager,
    std::map<std::string, GestionPort*> sejour
)
{
    std::vector<int> liste_places_abonnes {};

    // création des itérateurs de map
    std::map<std::string, int>::iterator it_place {};
    std::map<std::string, int>::iterator it_usager {};
    std::map<std::string, GestionPort*>::iterator it_sejour {};

    // itération des usagers pour lister les places des abonnés
    for (it_usager = usager.begin(); it_usager != usager.end(); ++it_usager)
    {
        // Flag la place si elle appartient à un abonne
        if ( it_usager->second > 0 )
        {
            liste_places_abonnes.push_back(it_usager->second);
        }
    }

    // itération des places du port
    for (it_place = place.begin(); it_place != place.end(); ++it_place)
    {
        // filtre sur le type, 99 pour tous les types
        if ( (it_place->second == place_type) || (place_type == 99) )
        {
            int place_int {stoi(it_place->first)};
            bool place_abonne {false};

            // itération des places des abonnés
            for (int i {0}; i < liste_places_abonnes.size(); ++i)
            {
                // Flag la place si elle appartient à un abonne
                if ( place_int == liste_places_abonnes[i] )
                {
                    place_abonne = true;
                }
            }

            // contrôle des dates de séjour pour les places non abonné
            if ( ! place_abonne )
            {
                bool place_occupe {false};
                // itération des séjours
                for (it_sejour = sejour.begin(); it_sejour != sejour.end(); ++it_sejour)
                {
                    // filtre sur les places restantes, non utilisees aux dates demandees
                    if (it_sejour->second->get_place() == place_int)
                    {
                        std::tm dt_buffer;

                        std::time_t t_debut_demande {std::mktime(&date_debut)};
                        std::time_t t_fin_demande {std::mktime(&date_fin)};

                        dt_buffer = it_sejour->second->get_debut();
                        std::time_t t_debut_trouve {std::mktime(&dt_buffer)};                     
                        
                        dt_buffer = it_sejour->second->get_fin();
                        std::time_t t_fin_trouve {std::mktime(&dt_buffer)};

                        // flag la place occupe si elle est attribuée pour un séjour trouvé
                        // soit : la date de début ou la date de fin demandé se trouve
                        // entre le début et la fin du séjour trouvé
                        // place occupee si :  ( dt   DD...ft ) ou ( DD   dt...ft   FD ) ou ( dt...FD   ft)

                        if ( ( std::difftime(t_debut_demande, t_debut_trouve) > 0 && std::difftime(t_fin_trouve, t_debut_demande) > 0 )
                            || ( std::difftime(t_debut_trouve, t_debut_demande) > 0 && std ::difftime(t_fin_demande, t_fin_trouve) > 0 )
                            || ( std::difftime(t_fin_demande, t_debut_trouve) > 0 && std ::difftime(t_fin_trouve, t_fin_demande) > 0 ) )
                        {
                            place_occupe = true;
                            break;
                        }                        
                    }                    
                }

                // enregistrement de la place si non occupee par un sejour
                if (! place_occupe)
                {
                    places_dispo.push_back(stoi(it_place->first));
                }     
            }            
        }
    }
}

// =====================================================================
// Saisie des données en console pour rechercher une place de port
// disponible pour un séjour ou abonne et un type donné
// arguments : map usager, place, sejour
// =====================================================================

void recherche_place(
    std::map<std::string, int> &usager,
    std::map<std::string, int> &place,
    std::map<std::string, GestionPort*> &sejour
)
{
    int place_type {99};
    int abonne {};
    std::vector<int> places_dispo {};
    std::tm dt_debut {};
    std::tm dt_recherche {};
    std::tm dt_fin {};

    clear_screen();

    std::cout << "********** Recherche de places disponibles ********** \n\n";

    std::cout << "Entrez 0 pour un sejour passager ou 1 pour un abonnement\n";
    std::cout << "> ";
    std::cin >> abonne;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (abonne)
    {
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
        std::cout << "Entrez le type de place : 0 corps mort, 1 < 10m, 2 <= 25m, 3 > 25m, 99 tous les types\n";
        std::cout << "> ";
        std::cin >> place_type;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    if (dt_fin.tm_year > 99)
    {
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
            std::cout << std::put_time(&dt_fin, "%d/%m/%Y") << "\n\n";
        }
        else
        {
            std::cout << "Places disponibles de type " << place_type << "\n";
            for (int i = 0; i < places_dispo.size(); i++)
            {
                std::cout << places_dispo[i] << ", ";
            }
            std::cout << std::endl;
        }            
        attente_console();
    }
}