#ifndef FONCTIONS_HPP_
#define FONCTIONS_HPP_

#include <map>
#include "GestionPort.hpp"

// CLear screen en fonction du system d'exploitation
void clear_screen();

// Formattage d'un nom (majuscule, minuscules)
void format_nom (std::string &nom);

// Attente d'une entrée pour sortir
int attente_console ();

// Conversion d'un float en string avec 2 décimales
std::string f_to_str (float f);

// Type de place en fonction de la taille du bateau
int get_place_type(int taille);

// Chargement d'un container map place, bateau
void creation_abonne(
    std::map<int, std::string> &place_bateau,
    std::map<std::string, int> &usager,
    std::map<std::string, int> &bateau,
    std::map<std::string, GestionPort*> &sejour
);

#endif