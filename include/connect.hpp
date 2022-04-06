#ifndef __CONNECT_HPP__
#define __CONNECT_HPP__

class Connect
{
    // Propriété privé de la classe Connect
    private:
        char* nomWifi_;
        char* motDePasse_;

    // Methodes publiques de la classe Connect
    public:
        // Constructeur par defaut
        Connect();

        Connect(char* nomWifi, char* motDePasse);

        //Getters
        char* nomWifi() const;
        char* motDePasse() const;

        //setters
        void nomWifi(char*);
        void motDePasse(char*);

        //Se connecter au ssid
        void connecter(char* nomWifi, char* motDePasse);

};

#endif //__CONNECT_HPP___
