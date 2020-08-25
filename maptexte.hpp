#ifndef MAPTEXTE_HPP_
#define MAPTEXTE_HPP_

#include <map>
#include "GestionPort.hpp"
#include "Facturation.hpp"
#include "Tarifs.hpp"

// lecture d'un fichier texte mot par mot et enregistrement dans un map
int texte_vers_map(std::string fichier_texte, std::map<std::string, int> &mapptr);

// écriture d'un fichier texte à partir d'un container map : clé ; valeur
int map_vers_texte(std::string fichier_texte, std::map<std::string, int> &mapptr);

// lecture d'un fichier texte mot par mot et chargement dans un array passé en paramètre
int texte_vers_prix(std::string fichier_texte, Tarifs &Prix);

// écriture d'un fichier texte à partir du container map des instances de GestionPort
int sejour_vers_texte(std::string fichier_texte, std::map<std::string, GestionPort*> &mapptr);

// lecture d'un fichier texte mot par mot, création d'une instance de GestionPort avec les attributs
int texte_vers_sejour(std::string fichier_texte, std::map<std::string, GestionPort*> &mapptr);

// Lecture d'un map d'instances de facturation et écriture des attributs dans un fichier texte
int facture_vers_texte(std::string fichier_texte, std::map<std::string, Facturation*> &mapptr);

// lecture d'un fichier texte mot par mot, création d'une instance de Facturation avec les attributs
int texte_vers_facture(std::string fichier_texte, std::map<std::string, Facturation*> &mapptr);

// Enregistrement des map usagers, bateaux, places de port, séjours et factures
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
);

#endif