#ifndef FACTURE_HPP_
#define FACTURE_HPP_

#include <map>
#include "GestionPort.hpp"
#include "Facturation.hpp"
#include "Tarifs.hpp"

// creation d'un facture pour un séjour ou un abonné
void facture_unique(
    bool abonne,
    std::map<std::string, int> &usager,
    std::map<std::string, int> &bateau,
    std::map<std::string, int> &place,
    std::map<int, std::string> &place_bateau,
    std::map<std::string, GestionPort*> &sejour,
    std::map<std::string, Facturation*> &facture,
    Tarifs &Prix
);

// creation d'un facture pour un séjour ou un abonné
void facture_batch(
    bool abonne,
    std::map<std::string, int> &usager,
    std::map<std::string, int> &bateau,
    std::map<std::string, int> &place,
    std::map<int, std::string> &place_bateau,
    std::map<std::string, GestionPort*> &sejour,
    std::map<std::string, Facturation*> &facture,
    Tarifs &Prix
);

#endif