/***********************************************************************
** Fonctions partagées par différentes fonctionnalités du logiciel
************************************************************************/

#include <iostream>
//#include <map> // already in fonctions.hpp
#include <iterator>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <limits>
//#include <vector> // already in fonctions.hpp
#include "fonctions.hpp"
#include "GestionPort.hpp"

// =====================================================================
// Effacement de la console
// CLS ou CLEAR selectionné par le preprocesseur en fonction
// du système d'exploitation sur lequel le code est compilé
// =====================================================================

void clear_screen()
{
#if defined(WIN32)||defined(_WIN32) // Windows
    system("cls");
#else
    system("clear"); // ux, mac
#endif
}

// =====================================================================
// Formattage d'un nom
// Première lettre en majuscule, le reste en minuscule
// argument : nom
// =====================================================================

void format_nom (std::string &nom)
{
    if( !nom.empty() )
    {
        nom[0] = std::toupper( nom[0] );

        for( std::size_t i = 1 ; i < nom.length() ; ++i )
            nom[i] = std::tolower( nom[i] );
    }
}

// =====================================================================
// Attente d'un entrée à la console pour sortir
// retour : integer contenant la réponse
// =====================================================================

int attente_console ()
{
    // Attendre une entrée console pour revenir au menu
    std::cout << "\nEntrez 0 pour revenir au menu)\n";
    std::cout << "> ";
    int retour;
    std::cin >> retour;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return retour;
}

// =====================================================================
// Conversion d'un float en string avec 2 décimales
// argument : float
// retour : string
// =====================================================================

std::string f_to_str (float f)
{
    std::stringstream oss {};
    oss << std::fixed << std::setprecision(2) << f;
    return oss.str();
}

// =====================================================================
// Calcul du type de place en fonction de la taille du bateau
// argument : taille
// retour : type de place
// =====================================================================

int get_place_type(int taille)
{
    if (taille == 0)
    {
        return 0;
    }
    else if (taille < 10)
    {
        return 1;
    }
    else if (taille <= 25)
    {
        return 2;
    }
    else
    {
        return 3;
    }
}

// =====================================================================
// Chargement d'un container map place, bateau pour les abonnés
// à partir des map usager pour trouver les abonnés et sejours
// pour trouver le bateau correspondant
// arguments : usager, bateau, sejour
// =====================================================================

void creation_abonne(
    std::map<int, std::string> &place_bateau,
    std::map<std::string, int> &usager,
    std::map<std::string, int> &bateau,
    std::map<std::string, GestionPort*> &sejour
)
{
    // création des itérateurs de map
    std::map<std::string, int>::iterator it_usager {};
    std::map<std::string, GestionPort*>::iterator it_sejour {};

    // itération des usagers pour lister les places des abonnés
    for (it_usager = usager.begin(); it_usager != usager.end(); ++it_usager)
    {
        // Si abonné, recherche du bateau correspondant
        if ( it_usager->second > 0 )
        {
            // itération des séjours
            for (it_sejour = sejour.begin(); it_sejour != sejour.end(); ++it_sejour)
            {
                // Si place trouvée, vérification de la validité du séjour (année en cours)
                if (it_sejour->second->get_place() == it_usager->second)
                {
                    // Get date courante
                    std::time_t t_courant = std::time(nullptr);
                    std::tm date_courante = *std::localtime(&t_courant);

                    // chargement de la date de début du séjour
                    std::tm date_debut {it_sejour->second->get_debut()};

                    // Si c'est le séjour en cours de l'abonné, enregistrement du couple place, bateau
                    if (date_courante.tm_year == date_debut.tm_year)
                    {
                        place_bateau.insert( std::make_pair( it_usager->second, it_sejour->second->get_bateau() ) );
                    }
                }                    
            }
        }
    }
}