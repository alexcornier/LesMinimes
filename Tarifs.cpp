/***********************************************************************
** Méthodes et accesseurs pour la Classe de Tarifs
************************************************************************/

#include "Tarifs.hpp"
#include <iostream>
#include <fstream>

// Constructeur
Tarifs::Tarifs():
    m_px_annuel {0},
    m_px_jour {0},
    m_coef_type1 {0},
    m_coef_type2 {0},
    m_coef_corpsmort {0},
    m_px_elec_jour {0},
    m_px_eau_jour {0},
    m_px_elec_mois {0},
    m_px_eau_mois {0},
    m_TVA {0}
{}

// Destructeur
Tarifs::~Tarifs() {}

// Accesseurs GET
float Tarifs::get_px_annuel() const { return m_px_annuel; }
float Tarifs::get_px_jour() const { return m_px_jour; }
float Tarifs::get_coef_type1() const { return m_coef_type1; }
float Tarifs::get_coef_type2() const { return m_coef_type2; }
float Tarifs::get_coef_corpsmort() const { return m_coef_corpsmort; }
float Tarifs::get_px_elec_jour() const { return m_px_elec_jour; }
float Tarifs::get_px_eau_jour() const { return m_px_eau_jour; }
float Tarifs::get_px_elec_mois() const { return m_px_elec_mois; }
float Tarifs::get_px_eau_mois() const { return m_px_eau_mois; }
float Tarifs::get_TVA() const { return m_TVA; }

// Mutateurs SET
void Tarifs::set_all(
    float px_annuel,
    float px_jour,
    float coef_type1,
    float coef_type2,
    float coef_corpsmort,
    float px_elec_jour,
    float px_eau_jour,
    float px_elec_mois,
    float px_eau_mois,
    float TVA
)
{
    m_px_annuel = px_annuel;
    m_px_jour = px_jour;
    m_coef_type1 = coef_type1;
    m_coef_type2 = coef_type2;
    m_coef_corpsmort = coef_corpsmort;
    m_px_elec_jour = px_elec_jour;
    m_px_eau_jour = px_eau_jour;
    m_px_elec_mois = px_elec_mois;
    m_px_eau_mois = px_eau_mois;
    m_TVA = TVA;
}