#ifndef TARIFS_HPP_
#define TARIFS_HPP_

#include <iostream>

class Tarifs
{
    private:
        float m_px_annuel; // 500
        float m_px_jour; // 20
        float m_coef_type1; // 30% = 0.3
        float m_coef_type2; // 60% = 0.6
        float m_coef_corpsmort; // 50% = 0.5
        int m_px_elec_jour; // 5
        int m_px_eau_jour; // 5
        int m_px_elec_mois; // 10
        int m_px_eau_mois; // 10
        float m_TVA; // 20% = 0.2

    public:
        // constructeur
        Tarifs();
        ~Tarifs(); // destructeur

        // Accesseurs GET
        float get_px_annuel() const;
        float get_px_jour() const;
        float get_coef_type1() const;
        float get_coef_type2() const;
        float get_coef_corpsmort() const;
        float get_px_elec_jour() const;
        float get_px_eau_jour() const;
        float get_px_elec_mois() const;
        float get_px_eau_mois() const;
        float get_TVA() const;

        // Mutateurs SET
        void set_all(
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
        );
};

#endif