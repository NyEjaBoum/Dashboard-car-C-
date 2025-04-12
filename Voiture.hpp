#ifndef VOITURE_HPP
#define VOITURE_HPP

#include <string>
#include <iostream>
#include "Connexion.hpp"  // Inclure la classe Connexion
#include <vector>

using namespace std;

class Voiture {
public:
    // Constructeur
    Voiture(int id, string nom, double accel, double dec,double vitesse,double capaciteReservoir,double consoMax);

    // Destructeur
    ~Voiture();

    // Méthode pour afficher les détails de la voiture
    void afficherDetails();

    // Getters
    int getId();
    string getNom();
    double getAccel();
    double getDec();
    double getVitesse();
    double getConsoMax();
    double getDistanceParcouru();
    double getCapaciteReservoir();

    // Setters
    void setId(int id);
    void setNom(string nom);
    void setAccel(double accel);
    void setDec(double dec);
    void setVitesse(double vitesse);
    void setConsoMax(double consoMax);
    void setCapaciteReservoir(double capacite);
    void setDistanceParcourue(double dist);


    //fonction

    double acceleration(double temps,double pourcent);
    double decceleration(double temps,double pourcent);
    
    double getCarburantConsommer(double temps,double pourcent);

    double getAccelerationPourcent(double pourcent);
    double getFiryPourcent(double accel);
    double getConsoPourcentage(double pourcent);
    double consommerEssence(double temps,double pourcent);
    double getNiveauEssence();
    void setNiveauEssence(double niveau);
    double calculDistanceParcouru(double temps,double accel);
    // double getVitesse(Connexion& con);
    static vector<Voiture> getAll(Connexion& conn);
    static Voiture* getById(Connexion& conn, int idVoiture);
    static void create(Connexion& conn,string nom,double accel,double decel);

private:
    int id;
    string nom;
    double accel;
    double dec;
    double vitesse;
    double capaciteReservoir;
    double niveauEssence;
    double consoMax;
    double distanceParcourue;
};

#endif // VOITURE_HPP
