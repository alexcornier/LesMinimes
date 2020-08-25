#ifndef GESTION_PORT_
#define GESTION_PORT_

#include <iostream>
#include <ctime>

class GestionPort
{
    private:
        std::tm m_date_debut;
        std::tm m_date_fin;
        std::string m_usager;
        std::string m_bateau;
        int m_place;
        bool m_electricite;
        bool m_eau;

    public:
        GestionPort(); // constructeur par défault
        ~GestionPort(); // destructeur

        // Accesseurs GET
        std::tm get_debut() const;
        std::tm get_fin() const;
        std::string get_usager() const;
        std::string get_bateau() const;
        int get_place() const;
        bool get_electricite() const;
        bool get_eau() const;

        // Mutateurs SET
        void set_debut(const std::tm date_debut);
        void set_fin(const std::tm date_fin);
        void set_usager(const std::string usager);
        void set_bateau(const std::string bateau);
        void set_place(const int place);
        void set_electricite(const bool electricite);
        void set_eau(const bool eau);
        void set_all(
            const std::tm date_debut,
            const std::tm date_fin,
            const std::string usager,
            const std::string bateau,
            const int place,
            const bool electricite,
            const bool eau
        );
};

#endif