#include <SFML/Graphics.hpp>
#include <iostream>
#include "Connexion.hpp"
#include "Voiture.hpp"
#include "Fenetre.hpp"

using namespace std;

int main() {
    // Connexion à la base de données
    Connexion conn("root", "", "voiture");
    conn.connecter();
  
    // Création de la fenêtre avec la nouvelle classe
    Fenetre fenetre(800, 600, "Vitesse Voiture",conn);
    
    // Création d'une voiture pour le test
    // Voiture voiture1(1, "Audi A4", 20, 20);  // Exemple de voiture avec accélération et décélération
    Voiture* voiture = Voiture::getById(conn,6);
    // cout << voiture->acceleration(10,0.5) << endl;
    //     std::cout << voiture->getVitesse() << std::endl;

    // voiture->acceleration(10);
    // printf(voiture->getAccel());
    // std::cout << voiture->getId() << std::endl;
    // std::cout << voiture->getAccel() << std::endl;
    // std::cout << voiture->getVitesse() << std::endl;
    // std::cout << voiture->getDec() << std::endl;    // Boucle principale

    // std::cout << "acceleration avec pourcentage " << voiture->getAccelerationPourcent(0.9) << "km/h/s" << std::endl;    // Boucle principale
    // std::cout << "conso avec pourcentage " << voiture->getConsoPourcentage(0.20) << "km/h/s" << std::endl;    // Boucle principale
    // std::cout << "conso avec pourcentage " << voiture->consommerEssence(10,1) << "km/h/s" << std::endl;    // Boucle principale

    // Acceleration_Log* accel = Acceleration_Log::getLastAction(conn,1);
    // std::cout << "last action " << voiture->getVitesse(conn) << std::endl; calculDistanceParcouru
    // std::cout << "Capacite reservoir " << voiture->getCapaciteReservoir() << std::endl;
    // std::cout << "Conso max " << voiture->getConsoMax() << std::endl;
    // std::cout << "Distance parcouru " << voiture->calculDistanceParcouru(10,20) << std::endl;

    // std::cout << "essence lany " << voiture->consommerEssence(10,0.5) << std::endl;
    //         std::cout << voiture->getNiveauEssence() << std::endl;

    // Acceleration_Log::replay(conn,voiture);

    // cout << "vitesse ny amzao " << voiture->getVitesse() << endl;

    // while (fenetre.isOpen()) {
    //     // Gestion des événements
    //     fenetre.evenement(conn);
        
    //     // Mise à jour des éléments
    //     fenetre.update();
        
    //     // Rendu
    //     fenetre.render();
    // }

    std::cout << "Distance parcouru " << voiture->calculDistanceParcouru(10,25) << std::endl;
    std::cout << "Distance parcouru " << voiture->calculDistanceParcouru(5,0) << std::endl;
    std::cout << "Distance parcouru " << voiture->calculDistanceParcouru(5,-4) << std::endl;

    // std::cout << "distance total parcourue " << voiture->getDistanceParcouru() << std::endl;

    double consoMoyenne = Acceleration_Log::calculerConsoMoyenne(conn, voiture);
    cout << "La consommation moyenne de la voiture " << voiture->getId() 
         << " est de " << consoMoyenne << " L/100 km" << endl;




    delete(voiture);
    
    return 0;
}