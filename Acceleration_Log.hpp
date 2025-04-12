#ifndef ACCELERATION_LOG_HPP
#define ACCELERATION_LOG_HPP

#include <string>
#include <iostream>
#include "Connexion.hpp"  // Inclure la classe Connexion
#include <vector>
#include "Voiture.hpp"


using namespace std;

class Acceleration_Log{
    public:
        Acceleration_Log();
        Acceleration_Log(int id,int voiture_id,double vitesseI,double acceleration,string heureAcceleration);
        ~Acceleration_Log();
// get
        int getId();
        int getVoitureId();
        double getVitesseInitiale();
        double getAcceleration();
        string getHeureAcceleration();
//set     
        void setVoitureId(int voiture_id);
        void setVitesseInitiale(double vitesseInitiale);
        void setAcceleration(double acceleration);
        void setHeureAcceleration(string heure_acceleration);
        static double calculerConsoMoyenne(Connexion& con, Voiture* voiture);

//fonction
        static void create(Connexion& con, int voiture_id,double vitesseI,double acceleration,string heure);
        static Acceleration_Log* getLastAction(Connexion& con, int voiture);
        static vector<Acceleration_Log> getAllActionForVoiture(Connexion& con, Voiture* voiture);
        static void replay(Connexion& con, Voiture* voiture);

    private:
        int id;
        int voiture_id;
        double vitesseInitiale;
        double acceleration;
        string heureAcceleration;

};



#endif