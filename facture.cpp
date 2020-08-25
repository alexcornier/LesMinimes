/***********************************************************************
** Gestion des factures
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
#include "facture.hpp"
#include "maptexte.hpp"
// #include "GestionPort.hpp"

// Path pour les fichiers texte des données de facturation
std::string const path_factures {"./factures/"};

// =====================================================================
// Saisie des données en console pour retrouver le sejour
// et préparation des information pour le calcul de la facture/
// Création d'une nouvelle instance dans la map facture si aucune
// facture correspondante n'a déjà été produite
// Appel de la méthode calcul_facture de la classe Facturation
// pour finaliser les calculs et charger les données dans l'instance
// =====================================================================

void facture_unique(
    bool abonne,
    std::map<std::string, int> &usager,
    std::map<std::string, int> &bateau,
    std::map<std::string, int> &place,
    std::map<int, std::string> &place_bateau,
    std::map<std::string, GestionPort*> &sejour,
    std::map<std::string, Facturation*> &facture,
    Tarifs &Prix
)
{
    std::tm dt_debut {};
    std::tm dt_fin {};
    std::string nom_usager {"0"};
    std::string nom_bateau {"0"};
    int num_place {0};

    clear_screen();

    if (abonne)
    {
        std::cout << "********** Facture mensuelle d'un abonnement ********** \n\n";

        std::cout << "Entrez le mois et l'annee de la facture mm/aaaa (ou 0 pour abandonner)\n";
        std::cout << "> ";
        std::cin >> std::get_time(&dt_debut, "%m/%Y");
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (dt_debut.tm_year > 99) // année 2000 minimum
        {
            // 1er jour du mois
            dt_debut.tm_mday = 1;
        }
    }
    else
    {
        std::cout << "********** Facture d'un Sejour Passager ********** \n\n";

        std::cout << "Entrez la date de debut du sejour jj/mm/aaaa (ou 0 pour abandonner)\n";
        std::cout << "> ";
        std::cin >> std::get_time(&dt_debut, "%d/%m/%Y");
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
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
        bool continuer {true};
        if (abonne)
        {
            if (usager.find(nom_usager)->second == 0)
            {
                continuer = false;
                std::cout << "\nPas d'abonnement trouvé pour l'usager " << nom_usager << std::endl;

                // Attendre une entrée console pour revenir au menu principal
                attente_console();
            }
        }
        else
        {
            if (usager.find(nom_usager)->second > 0)
            {
                continuer = false;
                std::cout << "\nL'usager " << nom_usager << " a un abonnement en cours" << std::endl;

                // Attendre une entrée console pour revenir au menu principal
                attente_console();
            }
        }

        // conditions remplies, création de la clé de facture
        if (continuer)
        {
            // Recherche de la place et du bateau
            num_place = usager.find(nom_usager)->second;

            if (abonne)
            {
                // utilisation du map place, bateau
                nom_bateau = place_bateau.find(num_place)->second;
            }
            else
            {
                // Demande du nom du bateau pour construire la clé du séjour correspondant
                std::cout << "Entrez le nom du bateau (ou 0 pour abandonner)\n";
                std::cout << "> ";
                std::cin >> nom_bateau;
                format_nom(nom_bateau);
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            
            if (nom_bateau != "0")
            {
                // Construction de la cle pour la facture
                std::string cle_facture {};
                {
                    std::ostringstream oss {}; // flux out pour construire la date au format aaaammjj
                    oss << std::put_time(&dt_debut, "%Y%m%d");
                    cle_facture = oss.str();
                }
                cle_facture = cle_facture + nom_usager + nom_bateau;

                // Recherche d'une facture identique, sinon création de la facture
                if ( facture.find(cle_facture) == facture.end() )
                {
                    // Si pas de facture, recherche du séjour (abonné au 1er janvier)
                    std::tm date_debut_sejour {dt_debut};
                    if (abonne)
                    {
                        date_debut_sejour.tm_mon = 0;
                    }
                    // Construction de la clé de séjour
                     std::string cle_sejour {};
                    {
                        std::ostringstream oss {}; // flux out pour construire la date au format aaaammjj
                        oss << std::put_time(&date_debut_sejour, "%Y%m%d");
                        cle_sejour = oss.str();
                    }

                    cle_sejour = cle_sejour + nom_usager + nom_bateau;

                    // Recherche du séjour correspondant
                    if (sejour.find(cle_sejour) != sejour.end() )
                    {
                        // Création de l'instance de facture dans le container map
                        facture.insert( std::make_pair( cle_facture, new Facturation() ) );

                        // Appel de la méthode de calcul des données de facturation (class Facturation)
                        facture.find(cle_facture)->second->calcul_facture(
                            abonne, dt_debut, cle_sejour, sejour, Prix, bateau, place
                        );

                        // Ajout de l'instance de facture mise à jour dans le container map temporaire
                        // pour générer le fichier de factuation en réutilisant la fonction d'enregistrement
                        std::map<std::string, Facturation*> tmp_facture;
                        tmp_facture.insert( std::make_pair( cle_facture, facture.find(cle_facture)->second ));

                        // Création du nom du fichier de facturation en utilisant une clé de facture pour garantir l'unicité
                        std::string fichier_facturation {path_factures};
                        if(abonne)
                        {
                            fichier_facturation = fichier_facturation + cle_facture + "_facture_abonne.txt";
                        }
                        else
                        {
                            fichier_facturation = fichier_facturation + cle_facture + "_facture_passager.txt";
                        }
                        
                        // Enregistrement du fichier de facturation au format CSV
                        if (facture_vers_texte(fichier_facturation, tmp_facture))
                        {
                            std::cout << "\nLe fichier de facturation " << fichier_facturation << "\n";
                            std::cout << "n'a pas pu etre enregistre, ou une erreur s'est produite" << std::endl;

                            // Attendre une entrée console pour revenir au menu principal
                            attente_console();
                        }
                        else
                        {
                            std::cout << "\nLe fichier de facturation " << fichier_facturation << "\n";
                            std::cout << "a ete enregistre" << std::endl;

                            // Attendre une entrée console pour revenir au menu principal
                            attente_console();                        
                        }
                        
                    }
                    else
                    {
                        std::cout << "\nPas d' abonnement ni sejour trouve pour l'usager " << nom_usager << "\n";
                        std::cout << "et la place " << num_place << "\n";
                        std::cout << "commencant le " << std::put_time(&date_debut_sejour, "%d%m%Y") << std::endl;

                        // Attendre une entrée console pour revenir au menu principal
                        attente_console();
                    }
                }
                else
                {
                    std::cout << "\nUne facture existe deja pour l'usager " << nom_usager << "\n";
                    std::cout << "et la place " << num_place << std::endl;

                    // Attendre une entrée console pour revenir au menu principal
                    attente_console();
                }
            }
        }
    }
}

// =====================================================================
// Batch de facturation pour les séjours non déjà facturés
// Pour les abonnés, on utilise les séjours commençant au 1er Janvier
// Pour les non abonnés, c'est la date de début du séjour
// Comme pour la facture_unique, génération des instances, puis
// appel de la méthode calcul_facture de la classe Facturation
// arguments : abonne, map usager, bateau, place, sejour
// =====================================================================

void facture_batch(
    bool abonne,
    std::map<std::string, int> &usager,
    std::map<std::string, int> &bateau,
    std::map<std::string, int> &place,
    std::map<int, std::string> &place_bateau,
    std::map<std::string, GestionPort*> &sejour,
    std::map<std::string, Facturation*> &facture,
    Tarifs &Prix
)
{
    std::tm date_du_jour {};
    std::map<std::string, Facturation*> tmp_facture;    

    clear_screen();

    // Chargement de la date du jour
    std::time_t t_courant = std::time(nullptr);
    date_du_jour = *std::localtime(&t_courant);

    // itération des séjours (la table la plus complexe en premier pour itérer une seule fois)
    std::map<std::string, GestionPort*>::iterator it_sejour {};
    for (it_sejour = sejour.begin(); it_sejour != sejour.end(); ++it_sejour)
    {
        // Chargement de l'usager, des dates de début et fin de séjour
        std::string nom_usager { it_sejour->second->get_usager() };
        std::string nom_bateau { it_sejour->second->get_bateau() };
        std::tm dt_debut_sejour { it_sejour->second->get_debut() };
        std::tm dt_fin_sejour { it_sejour->second->get_fin() };

        // Si le séjour est un séjour abonné en cours (1er janvier au 31 décembre, année courante)
        if (  abonne && ( dt_debut_sejour.tm_year == date_du_jour.tm_year )
            && ( dt_debut_sejour.tm_mday == 1 ) && ( dt_debut_sejour.tm_mon == 0 ) // 0 = janvier
            && ( dt_fin_sejour.tm_mday == 31 )  && ( dt_fin_sejour.tm_mon == 11 ) // 11 = décembre
        )
        {
            // Date de début de facture au 1er du mois en cours
            std::tm dt_debut_facture {};
            dt_debut_facture = dt_debut_sejour;
            dt_debut_facture.tm_mon = date_du_jour.tm_mon;

            // Construction de la cle pour la facture
            std::string cle_facture {};
            {
                // Réduction du scope du flux
                std::ostringstream oss {}; // flux out pour construire la date au format aaaammjj
                oss << std::put_time(&dt_debut_facture, "%Y%m%d");
                cle_facture = oss.str();
            }
            cle_facture = cle_facture + nom_usager + nom_bateau;

            // Recherche d'une facture identique, sinon création de la facture
            if ( facture.find(cle_facture) == facture.end() )
            {
                // Création de l'instance de facture dans le container map
                facture.insert( std::make_pair( cle_facture, new Facturation() ) );

                // Création de la clé de séjour, 1er Janvier pour un séjour abonné
                std::string cle_sejour {};
                {
                    // Réduction du scope du flux
                    std::ostringstream oss {};
                    oss << std::put_time(&dt_debut_sejour, "%Y%m%d");
                    cle_sejour = oss.str();
                }
                cle_sejour = cle_sejour + nom_usager + nom_bateau;

                // Appel de la méthode de calcul des données de facturation (class Facturation)
                bool abonne {true};
                facture.find(cle_facture)->second->calcul_facture(
                    abonne, dt_debut_facture, cle_sejour, sejour, Prix, bateau, place
                );

                // Ajout de l'instance de facture mise à jour dans le container map temporaire
                // pour générer le fichier de factuation en réutilisant la fonction d'enregistrement
                tmp_facture.insert( std::make_pair( cle_facture, facture.find(cle_facture)->second ));
            }
        }
        else
        {
            // Séjour passager (ou abonné années précédentes)
            // Evaluation de la fin du séjour par rapport à la date du jour
                std::time_t t_date_jour = std::mktime(&date_du_jour);
                std::time_t t_fin_sejour = std::mktime(&dt_fin_sejour);
                int ecart_jour {0};
                if ( t_date_jour != (std::time_t)(-1) && t_fin_sejour != (std::time_t)(-1) )
                {
                    ecart_jour = std::difftime(t_date_jour, t_fin_sejour) / (60 * 60 * 24);
                }

            // Si le séjour est terminé (écart en jour > 0)
            if (ecart_jour > 0)
            {
                // Construction de la cle pour la facture
                std::string cle_facture {};
                {
                    // Réduction du scope du flux
                    std::ostringstream oss {}; // flux out pour construire la date au format aaaammjj
                    oss << std::put_time(&dt_debut_sejour, "%Y%m%d");
                    cle_facture = oss.str();
                }
                cle_facture = cle_facture + nom_usager + nom_bateau;

                // Recherche d'une facture identique, sinon création de la facture
                if ( facture.find(cle_facture) == facture.end() )
                {
                    // Création de l'instance de facture dans le container map
                    facture.insert( std::make_pair( cle_facture, new Facturation() ) );

                    // Création de la clé de séjour, 1er Janvier pour un séjour abonné
                    std::string cle_sejour {};
                    {
                        // Réduction du scope du flux
                        std::ostringstream oss {};
                        oss << std::put_time(&dt_debut_sejour, "%Y%m%d");
                        cle_sejour = oss.str();
                    }
                    cle_sejour = cle_sejour + nom_usager + nom_bateau;

                    // Appel de la méthode de calcul des données de facturation (class Facturation)
                    bool abonne {true};
                    facture.find(cle_facture)->second->calcul_facture(
                        abonne, dt_debut_sejour, cle_sejour, sejour, Prix, bateau, place
                    );

                    // Ajout de l'instance de facture mise à jour dans le container map temporaire
                    // pour générer le fichier de factuation en réutilisant la fonction d'enregistrement
                    tmp_facture.insert( std::make_pair( cle_facture, facture.find(cle_facture)->second ));
                }
            }
        }
    }

    // Si au moins une facture a été créée alors création du fichier de facturation
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
        if (abonne)
        {
            fichier_facturation = fichier_facturation + str_dt_jour + "_batch_factures_abonnes.txt";
        }
        else
        {
            fichier_facturation = fichier_facturation + str_dt_jour + "_batch_factures_sejours.txt";
        }
        
        // Enregistrement du fichier de facturation au format CSV
        if (facture_vers_texte(fichier_facturation, tmp_facture))
        {
            std::cout << "\nLe fichier de facturation " << fichier_facturation << "\n";
            std::cout << "n'a pas pu etre enregistre, ou une erreur s'est produite" << std::endl;

            // Attendre une entrée console pour revenir au menu principal
            attente_console();
        }
        else
        {
            std::cout << "\nLe fichier de facturation " << fichier_facturation << "\n";
            std::cout << "a ete enregistre" << std::endl;

            // Attendre une entrée console pour revenir au menu principal
            attente_console();                        
        }
    }
    else
    {
        std::cout << "\nAucun abonnement ou sejour non facture trouve\n" << std::endl;

        // Attendre une entrée console pour revenir au menu principal
         attente_console();  
    }
    
}