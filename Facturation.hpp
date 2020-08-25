#ifndef FACTURATION_HPP_
#define FACTURATION_HPP_

#include <iostream>
// #include <ctime> // déjà dans GestionPort.hpp
#include <map>
#include "GestionPort.hpp"
#include "Tarifs.hpp"

class Facturation
{
    private:
        std::tm m_date_facture;
        std::tm m_date_debut;
        std::tm m_date_fin;
        int m_nb_jours;
        std::string m_usager;
        bool m_abonne;
        std::string m_bateau;
        int m_taille;
        int m_place;
        int m_type_place;
        float m_px_base;
        float m_px_place;
        float m_px_elec;
        float m_px_eau;
        float m_px_jour;
        float m_total_HT;
        float m_TVA;
        float m_total_TTC;
        std::tm m_date_paiement {};

    public:
        Facturation(); // constructeur par défault
        ~Facturation(); // destructeur

        // Accesseurs GET
        std::tm get_date_facture() const;
        std::tm get_date_debut() const;
        std::tm get_date_fin() const;
        int get_nb_jours() const;
        std::string get_usager() const;
        bool get_abonne() const;
        std::string get_bateau() const;
        int get_taille() const;
        int get_place() const;
        int get_type_place() const;
        float get_px_base() const;
        float get_px_place() const;
        float get_px_elec() const;
        float get_px_eau() const;
        float get_px_jour() const;
        float get_total_HT() const;
        float get_TVA() const;
        float get_total_TTC() const;
        std::tm get_date_paiement() const;

        // Mutateurs SET
        void set_date_paiement(const std::tm date_paiement);
        void set_all(
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
        );

        // Méthodes
        void calcul_facture(
            const bool abonne,
            const std::tm date_debut,
            const std::string cle_sejour,
            const std::map<std::string, GestionPort*> &sejour,
            const Tarifs &Prix,
            std::map<std::string, int> &bateau,
            std::map<std::string, int> &place
        );
};

#endif