#ifndef PAIEMENT_HPP_
#define PAIEMENT_HPP_

#include <map>
#include "Facturation.hpp"

// ajout d'un paiement (ajout de la date dans l'instance facture)
void paiement( std::map<std::string, Facturation*> &facture );

// liste des factures non réglées
void impayes( std::map<std::string, Facturation*> &facture );

#endif