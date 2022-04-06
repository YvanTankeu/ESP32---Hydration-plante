#ifndef __PANTE_HPP__
#define __PANTE_HPP__

#include <Arduino.h>

class Plante
{
    // Propriété privé de la classe Plante
    private:
        unsigned int humidite_;
        String date_;
        String heure_;

    // Methodes publiques de la classe Plante
    public:
        // Constructeur par defaut
        Plante();

        Plante(unsigned int humidite, String date, String heure);

        // Getters
        unsigned int humidite() const;
        String date() const;
        String heure() const;

        // setters
        void humidite(unsigned int);
        void date(String);
        void heure(String);

        // init date and time
        void initDateTime();

        int getSoilHumidityVal(int);

        //ouvrir la vane/relai pour laisser passer l'eau
        void hydrater(int);

        //fermer la vane/relai pour laisser passer l'eau
        void deshydrater(int);
};

#endif //__PANTE_HPP__
