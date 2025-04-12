#ifndef FENETRE_HPP
#define FENETRE_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <chrono>
#include "Voiture.hpp"
#include "Connexion.hpp"
#include "Acceleration_Log.hpp"
#include <ctime>

class Fenetre {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text vitesseText;
    sf::Text chronoText;
    sf::Text essenceText;
    sf::Text distanceText;
    sf::Clock chronoClock;
    std::chrono::high_resolution_clock::time_point lastTime;
    bool spacePressed;
    bool controlPressed;
    bool isPressed[11] = {false};
    sf::ConvexShape cadran;
    sf::RectangleShape aiguille;
    std::vector<sf::Text> kmMarkings;
    sf::RectangleShape jaugeEssenceFond;
    sf::RectangleShape jaugeEssenceRempli;
    Voiture* selectedVoiture; // Pointer to the selected vehicle
    bool isSelecting;         // Flag for selection phase
    std::vector<Voiture> voitures; // List of available vehicles
    std::vector<sf::Text> vehicleTexts; // Texts for vehicle selection
    int selectedIndex;        // Index of the currently highlighted vehicle
    sf::RectangleShape replayButton;
    sf::Text replayText;

public:
    Fenetre(int width, int height, std::string titre, Connexion& conn);
    ~Fenetre();
    bool isOpen() const;
    void evenement(Connexion& conn);
    void update();
    void render();
    float getDeltaTime();
    // void replay(Connexion& con, float deltaTime); // Nouvelle méthode ajoutée
    void replay(Connexion& con, float deltaTime); // Nouvelle méthode ajoutée

    std::string convertTimeIntoString(time_t* now);
    bool loadResources();
};

#endif // FENETRE_HPP