/***********************************************************************
** Fonctions de gestion des fichiers texte pour sauvegarder le
** contenu des containers map pour les usagers, bateaux et places
** de port.
************************************************************************/

#include <iostream>
// #include <map> // déjà dans maptexte.hpp
#include <iterator>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include "maptexte.hpp"
#include "fonctions.hpp"
// #include "GestionPort.hpp" // déjà dans maptexte.hpp
// #include "Facturation.hpp" // déjà dans maptexte.hpp

// =====================================================================
// lecture d'un fichier texte mot par mot et enregistrement dans un map
// arguments : nom complet du fichier, pointeur vers le container map
// sortie : 0 si ok, 1 si erreur
// =====================================================================

int texte_vers_map(std::string fichier_texte, std::map<std::string, int> &mapptr)
{
    // création du stream en lecture du fichier texte
    std::ifstream rd_texte {fichier_texte.c_str()};

    if (rd_texte)
    {
        std::string ligne {};
        std::string key_texte {};
        std::string separateur {};
        int attribut_texte {};

        // saut de la première ligne
        std::getline(rd_texte, ligne);

        while ( std::getline(rd_texte, ligne) )
        {
            // création d'un flux string pour charger les données de la ligne
            std::istringstream iss {ligne};
            iss >> key_texte >> separateur >> attribut_texte;

            iss.clear();

            mapptr[key_texte] = attribut_texte;
        }

        rd_texte.close();
        return 0;
    }
    else {
        std::cout << "ERREUR: fichier " << fichier_texte << " introuvable" << std::endl;

        return 1;
    }
}

// =====================================================================
// lecture d'un map et écriture du contenu dans un fichier texte
// arguments : nom complet du fichier, pointeur vers le container map
// sortie : 0 si ok, 1 si erreur
// =====================================================================

int map_vers_texte(std::string fichier_texte, std::map<std::string, int> &mapptr)
{
    // création du stream en écriture du fichier texte
    std::ofstream wr_texte {fichier_texte.c_str()};

    // création de l'itérateur du map
    std::map<std::string, int>::iterator it_mapptr {};

    if (wr_texte)
    {
        // ligne d'entête de colonnes
        wr_texte << "cle ; attribut" << std::endl;

        for (it_mapptr = mapptr.begin(); it_mapptr != mapptr.end(); ++it_mapptr)
        {
            wr_texte << it_mapptr->first << " ; " << it_mapptr->second << std::endl;
        }

        wr_texte.close();

        return 0;
    }
    else {
        std::cout << "ERREUR: fichier" << fichier_texte << " impossible à ouvrir en eciture" << std::endl;

        return 1;
    }
}

// =====================================================================
// lecture d'un fichier texte mot par mot et chargement du tableau
// des valeurs
// arguments : nom complet du fichier, pointeur vers le tableau
// sortie : 0 si ok, 1 si non trouvé, 2 si incomplet
// =====================================================================

int texte_vers_prix(std::string fichier_texte, Tarifs &Prix)
{
    // création du stream en lecture du fichier texte
    std::ifstream rd_texte {fichier_texte.c_str()};

    if (rd_texte)
    {
        std::string ligne {};
        std::string key_texte {};
        std::string separateur {};

        float px_annuel {};
        float px_jour {};
        float coef_type1 {};
        float coef_type2 {};
        float coef_corpsmort {};
        float px_elec_jour {};
        float px_eau_jour {};
        float px_elec_mois {};
        float px_eau_mois {};
        float coef_TVA {0};

        // saut de la première ligne
        std::getline(rd_texte, ligne);

        while ( std::getline(rd_texte, ligne) )
        {
            // création d'un flux string pour charger les données de la ligne
            std::istringstream iss {ligne};

            // Charge le nom de l'item, premier mot de la ligne dans le fichier
            iss >> key_texte >> separateur;

            // Charge la valeur dans l'item correspondant
            if ( key_texte == "pxAnnuel" ) { iss >> px_annuel; }
            if ( key_texte == "pxJour" ) { iss >> px_jour; }
            if ( key_texte == "coefType1" ) { iss >> coef_type1; }
            if ( key_texte == "coefType2" ) { iss >> coef_type2; }
            if ( key_texte == "coefCorpsMort" ) { iss >> coef_corpsmort; }
            if ( key_texte == "pxElecJour" ) { iss >> px_elec_jour; }
            if ( key_texte == "pxEauJour" ) { iss >> px_eau_jour; }
            if ( key_texte == "pxElecMois" ) { iss >> px_elec_mois; }
            if ( key_texte == "pxEauMois" ) { iss >> px_eau_mois; }
            if ( key_texte == "TVA" ) { iss >> coef_TVA; }

            iss.clear();
        }
        rd_texte.close();

        // Controle du nombre de valeurs et chargement de l'instance
        if ( coef_TVA == 0 )
        {
            std::cout << "ERREUR: fichier " << fichier_texte << " incomplet" << std::endl;

            return 2; // fichier incomplet
        }
        
        // CHarge les tarifs dans l'instance Prix
        Prix.set_all(
            px_annuel, px_jour, coef_type1, coef_type2, coef_corpsmort,
            px_elec_jour, px_eau_jour, px_elec_mois, px_eau_mois, coef_TVA
        );

        return 0;
    }
    else {
        std::cout << "ERREUR: fichier " << fichier_texte << " introuvable" << std::endl;

        return 1;
    }
}

// =====================================================================
// lecture d'un map d'instances de GestionPort et écriture des attributs
// dans un fichier texte
// arguments : nom complet du fichier, pointeur vers le container map
// sortie : 0 si ok, 1 si erreur
// =====================================================================

int sejour_vers_texte(std::string fichier_texte, std::map<std::string, GestionPort*> &mapptr)
{
    // création du stream en écriture du fichier texte
    std::ofstream wr_texte {fichier_texte.c_str()};

    // création de l'itérateur du map
    std::map<std::string, GestionPort*>::iterator it_mapptr {};

    std::tm date_debut {};
    std::tm date_fin {};


    if (wr_texte)
    {
        // ligne d'entête de colonnes
        wr_texte << "date de debut ; date de fin ; usager ; bateau ; place ; ";
        wr_texte << "electricite ; eau" << std::endl;

        for (it_mapptr = mapptr.begin(); it_mapptr != mapptr.end(); ++it_mapptr)
        {
            date_debut = it_mapptr->second->get_debut();
            wr_texte << std::put_time(&date_debut, "%d/%m/%Y") <<  " ; ";
            date_fin = it_mapptr->second->get_fin();
            wr_texte << std::put_time(&date_fin, "%d/%m/%Y") <<  " ; ";
            wr_texte << it_mapptr->second->get_usager() <<  " ; ";
            wr_texte << it_mapptr->second->get_bateau() <<  " ; ";
            wr_texte << it_mapptr->second->get_place() <<  " ; ";
            wr_texte << it_mapptr->second->get_electricite() <<  " ; ";
            wr_texte << it_mapptr->second->get_eau() << std::endl;
        }

        wr_texte.close();

        return 0;
    }
    else {
        std::cout << "ERREUR: fichier" << fichier_texte << " impossible à ouvrir en ecriture" << std::endl;

        return 1;
    }
}

// =====================================================================
// lecture d'un fichier texte mot par mot, construction et chargement
// des instances de la classe GestionPort
// arguments : nom complet du fichier, pointeur vers le container map
// sortie : 0 si ok, 1 si non trouvé
// =====================================================================

int texte_vers_sejour(std::string fichier_texte, std::map<std::string, GestionPort*> &mapptr)
{
    // création du stream en lecture du fichier texte
    std::ifstream rd_texte {fichier_texte.c_str()};

    if (rd_texte)
    {
        std::string ligne {};
        std::string key_texte {};
        std::string separateur {};

        std::tm date_debut {};
        std::tm date_fin {};
        std::string usager {};
        std::string bateau {};
        int place {};
        bool electricite {};
        bool eau {};
       
        // saut de la première ligne
        std::getline(rd_texte, ligne);

        while ( std::getline(rd_texte, ligne) )
        {
            // création d'un flux string pour charger les données de la ligne
            std::istringstream iss {ligne};

            iss >> std::get_time(&date_debut, "%d/%m/%Y") >> separateur;
            iss >> std::get_time(&date_fin, "%d/%m/%Y") >> separateur;
            iss >> usager >> separateur;
            iss >> bateau >> separateur;
            iss >> place >> separateur;
            iss >> electricite >> separateur;
            iss >> eau;

            iss.clear();

            if ( place > 0)
            {
                std::ostringstream oss {};
                oss << std::put_time( &date_debut, "%Y%m%d" );
                key_texte = oss.str() + usager + bateau;

                if ( mapptr.find(key_texte) == mapptr.end())
                {
                    mapptr.insert( std::make_pair( key_texte, new GestionPort() ) );

                    mapptr.find(key_texte)->second->set_all(
                        date_debut, date_fin, usager, bateau, place, electricite, eau
                    );
                }
            }

        }

        rd_texte.close();
        return 0;
    }
    else {
        std::cout << "ERREUR: fichier " << fichier_texte << " introuvable" << std::endl;

        return 1;
    }
}

// =====================================================================
// lecture d'un map d'instances de Facturation et écriture des attributs
// dans un fichier texte
// arguments : nom complet du fichier, pointeur vers le container map
// sortie : 0 si ok, 1 si erreur
// =====================================================================

int facture_vers_texte(std::string fichier_texte, std::map<std::string, Facturation*> &mapptr)
{
    // création du stream en écriture du fichier texte
    std::ofstream wr_texte {fichier_texte.c_str()};

    // création de l'itérateur du map
    std::map<std::string, Facturation*>::iterator it_mapptr {};

    std::tm date_facture {};
    std::tm date_debut {};
    std::tm date_fin {};
    std::tm date_paiement {};

    if (wr_texte)
    {
        // ligne d'entête de colonnes
        wr_texte << "date de facture ; date de debut ; date de fin ; nb de jours ; ";
        wr_texte << "usager ; abonne ; bateau ; taille ; place ; type de place ; ";
        wr_texte << "prix de base ; prix de la place ; prix elec ; prix eau ; ";
        wr_texte << "prix jour ; total HT ; TVA ; total TTC ; date paiement" << std::endl;

        for (it_mapptr = mapptr.begin(); it_mapptr != mapptr.end(); ++it_mapptr)
        {
            date_facture = it_mapptr->second->get_date_facture();
            wr_texte << std::put_time(&date_facture, "%d/%m/%Y") <<  " ; ";
            date_debut = it_mapptr->second->get_date_debut();
            wr_texte << std::put_time(&date_debut, "%d/%m/%Y") <<  " ; ";
            date_fin = it_mapptr->second->get_date_fin();
            wr_texte << std::put_time(&date_fin, "%d/%m/%Y") <<  " ; ";
            wr_texte << it_mapptr->second->get_nb_jours() <<  " ; ";
            wr_texte << it_mapptr->second->get_usager() <<  " ; ";
            wr_texte << it_mapptr->second->get_abonne() <<  " ; ";
            wr_texte << it_mapptr->second->get_bateau() <<  " ; ";
            wr_texte << it_mapptr->second->get_taille() <<  " ; ";
            wr_texte << it_mapptr->second->get_place() <<  " ; ";
            wr_texte << it_mapptr->second->get_type_place() <<  " ; ";
            wr_texte << std::fixed << std::setprecision(2) << it_mapptr->second->get_px_base() <<  " ; ";
            wr_texte << std::fixed << std::setprecision(2) << it_mapptr->second->get_px_place() <<  " ; ";
            wr_texte << std::fixed << std::setprecision(2) << it_mapptr->second->get_px_elec() <<  " ; ";
            wr_texte << std::fixed << std::setprecision(2) << it_mapptr->second->get_px_eau() <<  " ; ";
            wr_texte << std::fixed << std::setprecision(2) << it_mapptr->second->get_px_jour() <<  " ; ";
            wr_texte << std::fixed << std::setprecision(2) << it_mapptr->second->get_total_HT() <<  " ; ";
            wr_texte << std::fixed << std::setprecision(2) << it_mapptr->second->get_TVA() <<  " ; ";
            wr_texte << std::fixed << std::setprecision(2) << it_mapptr->second->get_total_TTC() <<  " ; ";
            date_paiement = it_mapptr->second->get_date_paiement();
            // Si la valeur est null, alors 1er du mois pour enregistrer une date complète 01011900 (sinon 011900)
            if (date_paiement.tm_mday == 0) { date_paiement.tm_mday = 1; }
            wr_texte << std::put_time(&date_paiement, "%d/%m/%Y") << std::endl;
        }

        wr_texte.close();

        return 0;
    }
    else {
        std::cout << "ERREUR: fichier" << fichier_texte << " impossible à ouvrir en ecriture" << std::endl;

        return 1;
    }
}

// =====================================================================
// lecture d'un fichier texte mot par mot, construction et chargement
// des instances de la classe Facturation
// arguments : nom complet du fichier, pointeur vers le container map
// sortie : 0 si ok, 1 si non trouvé
// =====================================================================

int texte_vers_facture(std::string fichier_texte, std::map<std::string, Facturation*> &mapptr)
{
    // création du stream en lecture du fichier texte
    std::ifstream rd_texte {fichier_texte.c_str()};

    if (rd_texte)
    {
        std::string ligne {};
        std::string key_texte {};
        std::string separateur {};

        std::tm date_facture {};
        std::tm date_debut {};
        std::tm date_fin {};
        int nb_jours {};
        std::string usager {};
        bool abonne {};
        std::string bateau {};
        int taille {};
        int place {};
        int type_place {};
        float px_base {};
        float px_place {};
        float px_elec {};
        float px_eau {};
        float px_jour {};
        float total_HT {};
        float TVA {};
        float total_TTC {};
        std::tm date_paiement {};

        // saut de la première ligne
        std::getline(rd_texte, ligne);

        while ( std::getline(rd_texte, ligne) )
        {
            // création d'un flux string pour charger les données de la ligne
            std::istringstream iss {ligne};

            iss >> std::get_time(&date_facture, "%d/%m/%Y") >> separateur;
            iss >> std::get_time(&date_debut, "%d/%m/%Y") >> separateur;
            iss >> std::get_time(&date_fin, "%d/%m/%Y") >> separateur;
            iss >> nb_jours >> separateur;
            iss >> usager >> separateur;
            iss >> abonne >> separateur;
            iss >> bateau >> separateur;
            iss >> taille >> separateur;
            iss >> place >> separateur;
            iss >> type_place >> separateur;
            iss >> px_base >> separateur;
            iss >> px_place >> separateur;
            iss >> px_elec >> separateur;
            iss >> px_eau >> separateur;
            iss >> px_jour >> separateur;
            iss >> total_HT >> separateur;
            iss >> TVA >> separateur;
            iss >> total_TTC >> separateur;
            iss >> std::get_time(&date_paiement, "%d/%m/%Y");

            iss.clear();

            if (total_TTC > 0)
            {
                std::ostringstream oss {};
                oss << std::put_time( &date_debut, "%Y%m%d" );
                key_texte = oss.str() + usager + bateau;

                if ( mapptr.find(key_texte) == mapptr.end())
                {
                    mapptr.insert( std::make_pair( key_texte, new Facturation() ) );

                    mapptr.find(key_texte)->second->set_all(
                        date_facture, date_debut, date_fin, nb_jours,
                        usager, abonne, bateau, taille, place, type_place,
                        px_base, px_place, px_elec, px_eau, px_jour,
                        total_HT, TVA, total_TTC, date_paiement
                    );
                }
            }
        }

        rd_texte.close();
        return 0;
    }
    else {
        std::cout << "ERREUR: fichier " << fichier_texte << " introuvable" << std::endl;

        return 1;
    }
}

// =====================================================================
// Enregistrement des map usagers, bateaux, places de port,
// séjours et factures
// arguments : les noms des fichiers et les map
// sortie : 0 si ok, 1 si erreur sur un ou plusieurs fichiers
// =====================================================================

int enregistrer(
    std::string const &fichier_usagers,
    std::string const &fichier_bateaux,
    std::string const &fichier_places,
    std::string const &fichier_sejours,
    std::string const &fichier_factures,
    std::map<std::string, int> &usager,
    std::map<std::string, int> &bateau,
    std::map<std::string, int> &place,
    std::map<std::string, GestionPort*> &sejour,
    std::map<std::string, Facturation*> &facture
)
{
    int resultat {0};

    if (map_vers_texte(fichier_usagers, usager))
    {
        resultat = 1;
    }
    if (map_vers_texte(fichier_bateaux, bateau))
    {
        resultat = 1;
    }
    if (map_vers_texte(fichier_places, place))
    {
        resultat = 1;
    }
    if (sejour_vers_texte(fichier_sejours, sejour))
    {
        resultat = 1;
    }
    if (facture_vers_texte(fichier_factures, facture))
    {
        resultat = 1;
    }

    return resultat;
}