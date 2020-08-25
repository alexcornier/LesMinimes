/***********************************************************************
** Méthodes et accesseurs pour la Classe de facturation
************************************************************************/

#include <string>
#include "Facturation.hpp"
#include "fonctions.hpp"

// Constructeur
Facturation::Facturation():
    m_date_facture {},
    m_date_debut {},
    m_date_fin {},
    m_nb_jours {},
    m_usager {},
    m_abonne {},
    m_bateau {},
    m_taille {},
    m_place {},
    m_type_place {},
    m_px_base {},
    m_px_place {},
    m_px_elec {},
    m_px_eau {},
    m_px_jour {},
    m_total_HT {},
    m_TVA {},
    m_total_TTC {},
    m_date_paiement {}
{}

// Destructeur
Facturation::~Facturation() {}

// Accesseurs GET
std::tm Facturation::get_date_facture() const { return m_date_facture; }
std::tm Facturation::get_date_debut() const { return m_date_debut; }
std::tm Facturation::get_date_fin() const { return m_date_fin; }
int Facturation::get_nb_jours() const { return m_nb_jours; }
std::string Facturation::get_usager() const { return m_usager; }
bool Facturation::get_abonne() const { return m_abonne; }
std::string Facturation::get_bateau() const { return m_bateau; }
int Facturation::get_taille() const { return m_taille; }
int Facturation::get_place() const { return m_place; }
int Facturation::get_type_place() const { return m_type_place; }
float Facturation::get_px_base() const { return m_px_base; }
float Facturation::get_px_place() const { return m_px_place; }
float Facturation::get_px_elec() const { return m_px_elec; }
float Facturation::get_px_eau() const { return m_px_eau; }
float Facturation::get_px_jour() const { return m_px_jour; }
float Facturation::get_total_HT() const { return m_total_HT; }
float Facturation::get_TVA() const { return m_TVA; }
float Facturation::get_total_TTC() const { return m_total_TTC; }
std::tm Facturation::get_date_paiement() const { return m_date_paiement; }

// Mutateurs SET
void Facturation::set_date_paiement(const std::tm date_paiement) { m_date_paiement = date_paiement; }
void Facturation::set_all(
    const std::tm date_facture,
    const std::tm date_debut,
    const std::tm date_fin,
    const int nb_jours,
    const std::string usager,
    const bool abonne,
    const std::string bateau,
    const int taille,
    const int place,
    const int type_place,
    const float px_base,
    const float px_place,
    const float px_elec,
    const float px_eau,
    const float px_jour,
    const float total_HT,
    const float TVA,
    const float total_TTC,
    const std::tm date_paiement
)
{
    m_date_facture = date_facture;
    m_date_debut = date_debut;
    m_date_fin = date_fin;
    m_nb_jours = nb_jours;
    m_usager = usager;
    m_abonne = abonne;
    m_bateau = bateau;
    m_taille = taille;
    m_place = place;
    m_type_place = type_place;
    m_px_base = px_base;
    m_px_place = px_place;
    m_px_elec = px_elec;
    m_px_eau = px_eau;
    m_px_jour = px_jour;
    m_total_HT = total_HT;
    m_TVA = TVA;
    m_total_TTC = total_TTC;
    m_date_paiement = date_paiement;
}

// Méthodes

// =====================================================================
// Calcul des données de facturation à partir du séjour
// et des tarifs, et mise à jour de l'instance de la classe
// arguments : abonne, cle_facture, cle_sejour, sejour, Prix
// =====================================================================

void Facturation::calcul_facture(
    const bool abonne,
    const std::tm date_debut,
    const std::string cle_sejour,
    const std::map<std::string, GestionPort*> &sejour,
    const Tarifs &Prix,
    std::map<std::string, int> &bateau,
    std::map<std::string, int> &place
)
{
    // Date de facture = date et heure actuelles pour un sejour passager
    // 1er du mois pour les abonnés
    if (abonne)
    {
        m_date_facture = date_debut;
    }
    else
    {
        std::time_t t_courant = std::time(nullptr);
        m_date_facture = *std::localtime(&t_courant);
    }

    // Date de début = Date de début du séjour ou du mois pour les abonnés
    m_date_debut = date_debut;

    // Date de fin = fin de séjour ou du mois pour les abonnés
    if (abonne)
    {
        // Dernier jour du mois en calculant le 1er du mois suivant -1 jour
        m_date_fin = date_debut;

        if (m_date_fin.tm_mon == 11)
        {
            m_date_fin.tm_mon = 0;
            m_date_fin.tm_year += 1;
        }
        else
        {
            m_date_fin.tm_mon += 1;
        }

        const time_t oneDay = 24 *60 *60; // valeur d'un jour
        time_t t_dernier_jour = mktime(&m_date_fin) + (-1 * oneDay);
        m_date_fin = *std::localtime(&t_dernier_jour);
    }
    else
    {
        m_date_fin = sejour.find(cle_sejour)->second->get_fin();
    }

    // nombre de jours
    // Différence en nb de jours
    std::time_t t1 = std::mktime(&m_date_debut);
    std::time_t t2 = std::mktime(&m_date_fin);

    // t1...t2 difftime(t2, t1) effectue t2-t1,
    if ( t1 != (std::time_t)(-1) && t2 != (std::time_t)(-1) )
    {
        m_nb_jours = ( std::difftime(t2, t1) / (60 * 60 * 24) );

        // ajout du premier jour si abonne, pour compter tous les jours du mois
        // sinon on compte en nuitées pour un séjour passager
        if (abonne)
        {
            m_nb_jours += 1;
        }
    }
    else
    {
        m_nb_jours = 0;
    }
    
    // Chargement des données depuis les maps sejour, bateau et place
    m_usager = sejour.find(cle_sejour)->second->get_usager();
    m_abonne = abonne;
    m_bateau = sejour.find(cle_sejour)->second->get_bateau();
    m_taille = bateau.find(m_bateau)->second;
    m_place = sejour.find(cle_sejour)->second->get_place();
    m_type_place = place.find( std::to_string(m_place) )->second;

    // Choix du coefficient en fonction du type de place
    float coef {1.0};
    switch (m_type_place)
    {
    case 0:
        coef = Prix.get_coef_corpsmort();
        break;
    case 1:
        coef = 1;
        break;
    case 2:
        coef = Prix.get_coef_type1();
        break;
    case 3:
        coef = Prix.get_coef_type2();
        break;   
    default:
        coef = 1;
        break;
    }

    // Calcul du prix en fonction du status abonne
    if (abonne)
    {
        // Prix de base du Tarif
        m_px_base = Prix.get_px_annuel() / 12;
        m_px_place = m_px_base * coef;
        m_px_elec = Prix.get_px_elec_mois() * sejour.find(cle_sejour)->second->get_electricite();
        m_px_eau = Prix.get_px_eau_mois() * sejour.find(cle_sejour)->second->get_eau();
        // peu d'intérêt pour un abonné, calculé pour information
        m_px_jour = ( ( m_px_place + m_px_elec + m_px_eau ) * 12 ) / 365;
        m_total_HT = m_px_place + m_px_elec + m_px_eau;
    }
    else
    {
        // Prix de base du Tarif
        m_px_base = Prix.get_px_jour();
        m_px_place = m_px_base * coef;
        m_px_elec = Prix.get_px_elec_jour() * sejour.find(cle_sejour)->second->get_electricite();
        m_px_eau = Prix.get_px_eau_jour() * sejour.find(cle_sejour)->second->get_eau();
        m_px_jour = m_px_place + m_px_elec + m_px_eau;
        m_total_HT = m_px_jour * m_nb_jours;
    }
    // Calcul TVA et montant TTC
    m_TVA = m_total_HT * Prix.get_TVA();
    m_total_TTC = m_total_HT + m_TVA;

    // Date de paiement au 01/01/1900
    m_date_paiement.tm_mday = 1;
}