#ifndef PLACE_HPP_
#define PLACE_HPP_

#include <map>
#include <vector>
#include "GestionPort.hpp"

// liste des places disponibles
void places_disponibles(
    int place_type,
    std::tm date_debut,
    std::tm date_fin,
    std::vector<int> &places_dispo,
    std::map<std::string, int> place,
    std::map<std::string, int> usager,
    std::map<std::string, GestionPort*> sejour
);

// ajout d'un séjour
void recherche_place(
    std::map<std::string, int> &usager,
    std::map<std::string, int> &place,
    std::map<std::string, GestionPort*> &sejour
);

#endif