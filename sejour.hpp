#ifndef SEJOUR_HPP_
#define SEJOUR_HPP_

#include <map>
#include "GestionPort.hpp"

// ajout d'un séjour
void ajout_sejour(
    int abonne,
    std::map<std::string, int> &usager,
    std::map<std::string, int> &bateau,
    std::map<std::string, int> &place,
    std::map<std::string, GestionPort*> &sejour
);

#endif