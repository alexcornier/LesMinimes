/***********************************************************************
** Méthodes et accesseurs pour la Classe de gestion du port
************************************************************************/

#include "GestionPort.hpp"
#include <iostream>

// Constructeur
GestionPort::GestionPort():
    m_date_debut {},
    m_date_fin {},
    m_usager {},
    m_bateau {},
    m_place {},
    m_electricite {},
    m_eau {}
{}

// Destructeur
GestionPort::~GestionPort() {};

// Accesseurs GET
std::tm GestionPort::get_debut() const { return m_date_debut; }
std::tm GestionPort::get_fin() const { return m_date_fin; }
std::string GestionPort::get_usager() const { return m_usager; }
std::string GestionPort::get_bateau() const { return m_bateau; }
int GestionPort::get_place() const { return m_place; }
bool GestionPort::get_electricite() const { return m_electricite; }
bool GestionPort::get_eau() const { return m_eau; }

// Mutateurs SET
void GestionPort::set_debut(const std::tm date_debut) { m_date_debut = date_debut; }
void GestionPort::set_fin(const std::tm date_fin) { m_date_fin = date_fin; }
void GestionPort::set_usager(const std::string usager) { m_usager = usager; }
void GestionPort::set_bateau(const std::string bateau) { m_bateau = bateau; }
void GestionPort::set_place(const int place) { m_place = place; }
void GestionPort::set_electricite(const bool electricite) { m_electricite = electricite; }
void GestionPort::set_eau(const bool eau) { m_eau = eau; }
void GestionPort::set_all(
    const std::tm date_debut,
    const std::tm date_fin,
    const std::string usager,
    const std::string bateau,
    const int place,
    const bool electricite,
    const bool eau
)
{
    m_date_debut = date_debut;
    m_date_fin = date_fin;
    m_usager = usager;
    m_bateau = bateau;
    m_place = place;
    m_electricite = electricite;
    m_eau = eau;
}
