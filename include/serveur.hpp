#ifndef __SERVEUR_HPP__
#define __SERVEUR_HPP__
#include <Arduino.h>

class Serveur
{
    // Propriété privé de la classe Serveur
    private:
        String startIndex_;
        String updatePage_;

    // Methodes publiques de la classe Serveur
    public:
        // Constructeur par defaut
        Serveur();

        Serveur( String startIndex, String updatePage);

        // Getters
        String startIndex() const;
        String updatePage() const;

        // setters
        void startIndex(String);
        void updatePage(String);

        // Page à envoyer au client
        void sendPage(String pageToSend) const;
};

#endif //__SERVEUR_HPP__
