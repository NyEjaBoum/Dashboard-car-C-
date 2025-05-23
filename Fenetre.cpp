#include "Fenetre.hpp"
#include <iostream>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <thread>      // Ajouté pour std::this_thread

Fenetre::Fenetre(int width, int height, std::string titre, Connexion& conn)
    : window(sf::VideoMode(width, height), titre), 
      spacePressed(false), 
      controlPressed(false), 
      lastTime(std::chrono::high_resolution_clock::now()),
      selectedVoiture(nullptr),
      isSelecting(true), // Start in selection mode
      selectedIndex(0) {

    // Fetch all vehicles from the database
    voitures = Voiture::getAll(conn);

    if (voitures.empty()) {
        std::cout << "Aucune voiture disponible dans la base de données." << std::endl;
        window.close();
        return;
    }

    // Load resources first (font is needed for selection menu)
    if (!loadResources()) {
        window.close();
        return;
    }

    // Initialize vehicle selection texts
    for (size_t i = 0; i < voitures.size(); ++i) {
        sf::Text vehicleText;
        vehicleText.setFont(font);
        vehicleText.setCharacterSize(24);
        vehicleText.setFillColor(sf::Color::White);
        vehicleText.setString(std::to_string(i + 1) + ". " + voitures[i].getNom() + 
                              " (ID: " + std::to_string(voitures[i].getId()) + ")");
        vehicleText.setPosition(50.f, 50.f + i * 40.f);
        vehicleTexts.push_back(vehicleText);
    }

    // Initialize simulation UI elements only after selection
}

/*void Fenetre::replay(Connexion& con, float deltaTime) {
    if (!selectedVoiture) {
        std::cout << "Aucune voiture sélectionnée pour le replay." << std::endl;
        return;
    }

    vector<Acceleration_Log> historique = Acceleration_Log::getAllActionForVoiture(con, selectedVoiture);
    if (historique.empty()) {
        std::cout << "Aucune donnée d'accélération pour cette voiture." << std::endl;
        return;
    }

    const float fixedTimeStep = 0.016f; // Pas de temps fixe (~60 FPS), remplace deltaTime

    for (size_t i = 0; i + 1 < historique.size(); i += 2) {
        Acceleration_Log current = historique[i];
        Acceleration_Log next = historique[i + 1];

        int h1, m1, s1, h2, m2, s2;
        float ms1, ms2; // Pour stocker les millisecondes

        // Parser avec millisecondes (format HH:MM:SS.sss)
        sscanf(current.getHeureAcceleration().c_str(), "%d:%d:%d.%f", &h1, &m1, &s1, &ms1);
        sscanf(next.getHeureAcceleration().c_str(), "%d:%d:%d.%f", &h2, &m2, &s2, &ms2);

        // Calculer le temps en secondes avec précision des millisecondes
        float time1 = h1 * 3600 + m1 * 60 + s1 + ms1 / 1000.0f;
        float time2 = h2 * 3600 + m2 * 60 + s2 + ms2 / 1000.0f;
        float elapsed_seconds = time2 - time1;

        if (elapsed_seconds <= 0) continue;

        double vitesseInitiale = current.getVitesseInitiale();
        double acceleration = current.getAcceleration();
        double pourcent = selectedVoiture->getFiryPourcent(acceleration);

        selectedVoiture->setVitesse(vitesseInitiale);

        std::cout << "Début à " << current.getHeureAcceleration() << std::endl;
        std::cout << "Vitesse initiale: " << selectedVoiture->getVitesse() << " km/h" << std::endl;
        std::cout << "Accélération: " << acceleration << " km/h/s" << std::endl;
        std::cout << "Durée de l'action : " << elapsed_seconds << " secondes" << std::endl;
        std::cout << "Pourcentage d'accélération : " << pourcent << std::endl;

        float timeAccumulator = 0.0f;
        while (timeAccumulator <= elapsed_seconds) {
            if (acceleration > 0) {
                selectedVoiture->acceleration(fixedTimeStep, pourcent);
                selectedVoiture->consommerEssence(fixedTimeStep, pourcent);
            } else {
                selectedVoiture->decceleration(fixedTimeStep, -pourcent);
            }

            selectedVoiture->calculDistanceParcouru(fixedTimeStep, selectedVoiture->getAccel() * pourcent);

            timeAccumulator += fixedTimeStep;

            update();
            render();

            std::cout << "Temps: " << timeAccumulator << "s - Vitesse: "
                      << selectedVoiture->getVitesse() << " km/h" << std::endl;

            this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(fixedTimeStep * 1000)));
        }

        std::cout << "Fin à " << next.getHeureAcceleration() << " - Vitesse atteinte: "
                  << selectedVoiture->getVitesse() << " km/h" << std::endl;
        std::cout << "Distance parcourue : " << selectedVoiture->getDistanceParcouru() << " m\n" << std::endl;

        update();
        render();

        // Attente avant le prochain segment (optionnel)
        if (i + 2 < historique.size()) {
            Acceleration_Log nextSegment = historique[i + 2];
            sscanf(nextSegment.getHeureAcceleration().c_str(), "%d:%d:%d.%f", &h1, &m1, &s1, &ms1);
            float nextTime = h1 * 3600 + m1 * 60 + s1 + ms1 / 1000.0f;
            float delayToNext = nextTime - time2;
            if (delayToNext > 0) {
                std::cout << "Attente avant le prochain segment : " << delayToNext << " secondes" << std::endl;
                this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delayToNext * 1000)));
            }
        }
    }
}*/

void Fenetre::replay(Connexion& con, float deltaTime) {
    if (!selectedVoiture) {
        std::cout << "Aucune voiture sélectionnée pour le replay." << std::endl;
        return;
    }

    vector<Acceleration_Log> historique = Acceleration_Log::getAllActionForVoiture(con, selectedVoiture);
    if (historique.empty()) {
        std::cout << "Aucune donnée d'accélération pour cette voiture." << std::endl;
        return;
    }

    const float fixedTimeStep = 0.016f; // Pas de temps fixe (~60 FPS)

    for (size_t i = 0; i + 1 < historique.size(); i += 2) {
        Acceleration_Log current = historique[i];
        Acceleration_Log next = historique[i + 1];

        int h1, m1, s1, h2, m2, s2;
        float ms1, ms2; // Pour stocker les millisecondes

        // Parser avec millisecondes (format HH:MM:SS.sss)
        sscanf(current.getHeureAcceleration().c_str(), "%d:%d:%d.%f", &h1, &m1, &s1, &ms1);
        sscanf(next.getHeureAcceleration().c_str(), "%d:%d:%d.%f", &h2, &m2, &s2, &ms2);

        // Calculer le temps en secondes avec précision des millisecondes
        float time1 = h1 * 3600 + m1 * 60 + s1 + ms1 / 1000.0f;
        float time2 = h2 * 3600 + m2 * 60 + s2 + ms2 / 1000.0f;
        float elapsed_seconds = time2 - time1;

        if (elapsed_seconds <= 0) continue;

        double vitesseInitiale = current.getVitesseInitiale();
        double vitesseFinaleDB = next.getVitesseInitiale(); // Vitesse finale en base de données
        double acceleration = current.getAcceleration();
        double pourcent = selectedVoiture->getFiryPourcent(acceleration);

        selectedVoiture->setVitesse(vitesseInitiale);
        // selectedVoiture->setNiveauEssence(selectedVoiture->getCapaciteReservoir());

        std::cout << "Début à " << current.getHeureAcceleration() << std::endl;
        std::cout << "Vitesse initiale: " << selectedVoiture->getVitesse() << " km/h" << std::endl;
        std::cout << "Accélération: " << acceleration << " km/h/s" << std::endl;
        std::cout << "Durée de l'action : " << elapsed_seconds << " secondes" << std::endl;
        std::cout << "Pourcentage d'accélération : " << pourcent << std::endl;

        float timeAccumulator = 0.0f;
        double vitesseCalculee = 0.0; // Pour stocker la vitesse calculée à la fin
        
        while (timeAccumulator < elapsed_seconds) {
            float remainingTime = elapsed_seconds - timeAccumulator;
            float stepTime = std::min(fixedTimeStep, remainingTime);
            
            if (acceleration > 0) {
                selectedVoiture->acceleration(stepTime, pourcent);
                selectedVoiture->consommerEssence(stepTime, pourcent);
            } else {
                selectedVoiture->decceleration(stepTime, -pourcent);
            }

            selectedVoiture->calculDistanceParcouru(stepTime, selectedVoiture->getAccel() * pourcent);

            timeAccumulator += stepTime;

            // Débogage intermédiaire (optionnel)
            if (timeAccumulator >= elapsed_seconds - fixedTimeStep) {
                std::cout << "Juste avant la fin - Vitesse calculée: " 
                          << selectedVoiture->getVitesse() << " km/h" << std::endl;
            }

            update();
            render();

            // Afficher le temps et la vitesse actuelle
            std::cout << "Temps: " << timeAccumulator << "s - Vitesse: "
                      << selectedVoiture->getVitesse() << " km/h" << std::endl;

            this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(stepTime * 1000)));
        }
        
        // Sauvegarder la vitesse calculée avant de l'écraser
        // vitesseCalculee = selectedVoiture->getVitesse();
        // selectedVoiture->setNiveauEssence(selectedVoiture->getCapaciteReservoir());

        // Afficher la comparaison entre vitesse calculée et vitesse en base
        std::cout << "Fin à " << next.getHeureAcceleration() << std::endl;
        std::cout << "Vitesse calculée: " << vitesseCalculee << " km/h" << std::endl;
        std::cout << "Vitesse en base: " << vitesseFinaleDB << " km/h" << std::endl;
        
        // Si vous voulez afficher la vitesse de la base mais utiliser la vitesse calculée
        // pour la physique, commentez la ligne suivante:
        // selectedVoiture->setVitesse(vitesseFinaleDB);
        
        std::cout << "Distance parcourue : " << selectedVoiture->getDistanceParcouru() << " m\n" << std::endl;

        update();
        render();

        // Attente avant le prochain segment (optionnel)
        if (i + 2 < historique.size()) {
            Acceleration_Log nextSegment = historique[i + 2];
            sscanf(nextSegment.getHeureAcceleration().c_str(), "%d:%d:%d.%f", &h1, &m1, &s1, &ms1);
            float nextTime = h1 * 3600 + m1 * 60 + s1 + ms1 / 1000.0f;
            float delayToNext = nextTime - time2;
            if (delayToNext > 0) {
                std::cout << "Attente avant le prochain segment : " << delayToNext << " secondes" << std::endl;
                this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delayToNext * 1000)));
            }
        }
    }
    double consoMoyenne = Acceleration_Log::calculerConsoMoyenne(con, selectedVoiture);
    cout << "La consommation moyenne de la voiture " << selectedVoiture->getId() 
         << " est de " << consoMoyenne << " L/100 km" << endl;
}

Fenetre::~Fenetre() {
    if (selectedVoiture) {
        delete selectedVoiture;
    }
    if (window.isOpen()) {
        window.close();
    }
}

bool Fenetre::isOpen() const {
    return window.isOpen();
}

float Fenetre::getDeltaTime() {
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = now - lastTime;
    lastTime = now;
    return elapsed.count();
}

std::string Fenetre::convertTimeIntoString(time_t* now) {
    tm* ltm = localtime(now);
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << ltm->tm_hour << ":"
        << std::setfill('0') << std::setw(2) << ltm->tm_min << ":"
        << std::setfill('0') << std::setw(2) << ltm->tm_sec;
    return oss.str();
}

void Fenetre::evenement(Connexion& conn) {
    sf::Event event;
    float deltaTime = getDeltaTime();

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (isSelecting && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) {
                selectedIndex = (selectedIndex > 0) ? selectedIndex - 1 : voitures.size() - 1;
            } else if (event.key.code == sf::Keyboard::Down) {
                selectedIndex = (selectedIndex < static_cast<int>(voitures.size()) - 1) ? selectedIndex + 1 : 0;
            } else if (event.key.code == sf::Keyboard::Enter) {
                // Select the vehicle and transition to simulation
                selectedVoiture = Voiture::getById(conn, voitures[selectedIndex].getId());
                if (!selectedVoiture) {
                    std::cout << "Erreur lors de la récupération du véhicule sélectionné." << std::endl;
                    window.close();
                    return;
                }
                isSelecting = false;

                // Initialize simulation UI elements here (unchanged)
                vitesseText.setFont(font);
                vitesseText.setCharacterSize(24);
                vitesseText.setFillColor(sf::Color::White);
                vitesseText.setPosition(10.f, 10.f);

                chronoText.setFont(font);
                chronoText.setCharacterSize(24);
                chronoText.setFillColor(sf::Color::White);
                chronoText.setPosition(10.f, 50.f);

                essenceText.setFont(font);
                essenceText.setCharacterSize(24);
                essenceText.setFillColor(sf::Color::White);
                essenceText.setPosition(10.f, 90.f);

                jaugeEssenceFond.setSize(sf::Vector2f(200.f, 20.f));
                jaugeEssenceFond.setFillColor(sf::Color(50, 50, 50));
                jaugeEssenceFond.setOutlineThickness(2.f);
                jaugeEssenceFond.setOutlineColor(sf::Color::White);
                jaugeEssenceFond.setPosition(10.f, 120.f);

                jaugeEssenceRempli.setSize(sf::Vector2f(200.f, 20.f));
                jaugeEssenceRempli.setFillColor(sf::Color::Green);
                jaugeEssenceRempli.setPosition(10.f, 120.f);

                distanceText.setFont(font);
                distanceText.setCharacterSize(24);
                distanceText.setFillColor(sf::Color::White);
                distanceText.setPosition(10.f, 150.f);

                cadran.setPointCount(182);
                cadran.setFillColor(sf::Color::Black);
                cadran.setOutlineThickness(5.f);
                cadran.setOutlineColor(sf::Color::White);

                // Configurer le bouton Replay
                replayButton.setSize(sf::Vector2f(120.f, 40.f));
                replayButton.setFillColor(sf::Color::Blue);
                replayButton.setPosition(window.getSize().x - 140.f, 20.f);

                replayText.setFont(font);
                replayText.setCharacterSize(20);
                replayText.setFillColor(sf::Color::White);
                replayText.setString("Replay");
                sf::FloatRect textBounds = replayText.getLocalBounds();
                replayText.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
                replayText.setPosition(replayButton.getPosition().x + replayButton.getSize().x / 2.f,
                                       replayButton.getPosition().y + replayButton.getSize().y / 2.f);

                float radius = 100.f;
                float centerX = window.getSize().x / 2.f;
                float centerY = window.getSize().y / 2.f;

                std::vector<sf::Vector2f> points;
                points.push_back(sf::Vector2f(0.f, 0.f));
                for (int i = 0; i <= 180; i++) {
                    float angle = i * 3.14159f / 180.f;
                    float x = cos(angle) * radius;
                    float y = -sin(angle) * radius;
                    points.push_back(sf::Vector2f(x, y));
                }
                points.push_back(sf::Vector2f(0.f, 0.f));

                cadran.setPointCount(points.size());
                for (size_t i = 0; i < points.size(); i++) {
                    cadran.setPoint(i, points[i]);
                }
                cadran.setPosition(centerX, centerY);

                aiguille.setSize(sf::Vector2f(radius - 10.f, 3.f));
                aiguille.setFillColor(sf::Color::Red);
                aiguille.setOrigin(0.f, 1.5f);
                aiguille.setPosition(centerX, centerY);
                aiguille.setRotation(-90.f);

                for (int i = 0; i <= 6; i++) {
                    sf::Text kmMarker;
                    kmMarker.setFont(font);
                    kmMarker.setCharacterSize(16);
                    kmMarker.setFillColor(sf::Color::White);
                    int vitesseMarquage = i * 40;
                    kmMarker.setString(std::to_string(vitesseMarquage));
                    float angle = 180.f - (vitesseMarquage / 240.f) * 180.f;
                    float radAngle = angle * 3.14159f / 180.f;
                    float markerRadius = radius + 20.f;
                    float x = centerX + cos(radAngle) * markerRadius;
                    float y = centerY - sin(radAngle) * markerRadius;
                    sf::FloatRect textBounds = kmMarker.getLocalBounds();
                    kmMarker.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
                    kmMarker.setPosition(x, y);
                    kmMarkings.push_back(kmMarker);
                }
            }
        }

        // Handle replay button click when not in selection mode
        if (!isSelecting && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
            if (replayButton.getGlobalBounds().contains(mousePos)) {
                std::cout << "Replay button clicked!" << std::endl;
                if (selectedVoiture) {
                    std::cout << "Lancement du replay pour la voiture sélectionnée." << std::endl;
                    replay(conn, deltaTime); // Appel de la nouvelle méthode avec deltaTime
                }
            }
        }
    } 

    if (!isSelecting && selectedVoiture) {
        // Simulation event handling code using deltaTime
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        auto tt = std::chrono::system_clock::to_time_t(now);
        std::tm* ptm = std::localtime(&tt);
        char buffer[32];
        sprintf(buffer, "%02d:%02d:%02d.%03ld", ptm->tm_hour, ptm->tm_min, ptm->tm_sec, ms.count());
        std::string heure = buffer; // Ex. "08:00:05.237"

        sf::Keyboard::Key touches[] = {
            sf::Keyboard::Num1, sf::Keyboard::Num2, sf::Keyboard::Num3,
            sf::Keyboard::Num4, sf::Keyboard::Num5, sf::Keyboard::Num6,
            sf::Keyboard::Num7, sf::Keyboard::Num8, sf::Keyboard::Num9,
            sf::Keyboard::Num0, sf::Keyboard::A
        };

        double pourcentages[] = {
            0.10, 0.20, 0.30, 0.40, 0.50, 0.60, 0.70, 0.80, 0.90, 1.00, 1.00
        };

        bool toucheNumeriquePressee = false;
        double accelPourcent = 0;
        double accelle = 0;

        for (int i = 0; i < 11; ++i) {
            if (sf::Keyboard::isKeyPressed(touches[i])) {
                toucheNumeriquePressee = true;
                accelPourcent = pourcentages[i];
                if (!isPressed[i] && spacePressed) {
                    isPressed[i] = true;
                    std::cout << "heureeee" << heure << std::endl;
                    Acceleration_Log::create(conn, selectedVoiture->getId(), selectedVoiture->getVitesse(), 
                        selectedVoiture->getAccel() * accelPourcent, heure);
                }
            } else {
                isPressed[i] = false;
            }
        }

        // Handle acceleration
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (!spacePressed && toucheNumeriquePressee) {
                chronoClock.restart();
                spacePressed = true;
                accelle = selectedVoiture->getAccel() * accelPourcent;
            }
            if (selectedVoiture->getNiveauEssence() > 0) {
                selectedVoiture->acceleration(deltaTime, accelPourcent);
                selectedVoiture->consommerEssence(deltaTime, accelPourcent);
            }
        } else {
            if (spacePressed) {
                float pressedDuration = chronoClock.getElapsedTime().asSeconds();
                std::cout << "heureeee final" << heure << std::endl;

                Acceleration_Log::create(conn, selectedVoiture->getId(), selectedVoiture->getVitesse(), 0, heure);
                std::cout << "Touche Espace pressée pendant : " << pressedDuration << " secondes." << std::endl;
                std::cout << "Distance parcourue : " << selectedVoiture->getDistanceParcouru() << " m" << std::endl;
                spacePressed = false;
            }
        }

        // Update distance based on current speed and acceleration
        selectedVoiture->calculDistanceParcouru(deltaTime, accelle);

        // Handle deceleration
        for (int i = 0; i < 11; ++i) {
            if (sf::Keyboard::isKeyPressed(touches[i])) {
                toucheNumeriquePressee = true;
                accelPourcent = pourcentages[i];
                if (!isPressed[i] && controlPressed) {
                    isPressed[i] = true;
                    Acceleration_Log::create(conn, selectedVoiture->getId(), selectedVoiture->getVitesse(), 
                        -selectedVoiture->getDec() * accelPourcent, heure);
                }
            } else {
                isPressed[i] = false;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
            if (!controlPressed && toucheNumeriquePressee) {
                chronoClock.restart();
                controlPressed = true;
            }
            if (selectedVoiture->getVitesse() > 0 && toucheNumeriquePressee && selectedVoiture->getNiveauEssence() > 0) {
                selectedVoiture->decceleration(deltaTime, accelPourcent);
            }
        } else {
            if (controlPressed) {
                float pressedDuration = chronoClock.getElapsedTime().asSeconds();
                Acceleration_Log::create(conn, selectedVoiture->getId(), selectedVoiture->getVitesse(), 0, heure);
                std::cout << "Touche Ctrl pressée pendant : " << pressedDuration << " secondes." << std::endl;
                controlPressed = false;
            }
        }

    // double consoMoyenne = Acceleration_Log::calculerConsoMoyenne(conn, selectedVoiture);
    // cout << "La consommation moyenne de la voiture " << selectedVoiture->getId() 
    //      << " est de " << consoMoyenne << " L/100 km" << endl;
    }
}

void Fenetre::update() {
    if (isSelecting) {
        // Highlight the selected vehicle in the menu
        for (size_t i = 0; i < vehicleTexts.size(); ++i) {
            if (i == static_cast<size_t>(selectedIndex)) {
                vehicleTexts[i].setFillColor(sf::Color::Yellow);
            } else {
                vehicleTexts[i].setFillColor(sf::Color::White);
            }
        }
    } else if (selectedVoiture) {
        // Existing simulation update code
        // vitesseText.setString("Vitesse : " + std::to_string(static_cast<int>(selectedVoiture->getVitesse())) + " km/h");
        vitesseText.setString("Vitesse : " + std::to_string(selectedVoiture->getVitesse()) + " km/h"); // Sans cast int

        float pressedDuration = chronoClock.getElapsedTime().asSeconds();
        int minutes = static_cast<int>(pressedDuration) / 60;
        int seconds = static_cast<int>(pressedDuration) % 60;
        chronoText.setString("Temps Espace: " + std::to_string(minutes) + "m " + std::to_string(seconds) + "s");

        double niveau = selectedVoiture->getNiveauEssence();
        double capaciteMax = selectedVoiture->getCapaciteReservoir();
        essenceText.setString("Essence : " + std::to_string(static_cast<double>(niveau)) + "L / " + 
                             std::to_string(static_cast<double>(capaciteMax)) + "L");

        // distanceText.setString("Distance : " + std::to_string(static_cast<int>(selectedVoiture->getDistanceParcouru())) + " m");
        distanceText.setString("Distance : " + std::to_string(static_cast<int>(selectedVoiture->getDistanceParcouru())) + " m");

        float pourcentageEssence = static_cast<float>(niveau / capaciteMax);
        if (pourcentageEssence < 0) pourcentageEssence = 0;
        jaugeEssenceRempli.setSize(sf::Vector2f(200.f * pourcentageEssence, 20.f));
        
        if (pourcentageEssence > 0.5) {
            jaugeEssenceRempli.setFillColor(sf::Color::Green);
        } else if (pourcentageEssence > 0.25) {
            jaugeEssenceRempli.setFillColor(sf::Color::Yellow);
        } else {
            jaugeEssenceRempli.setFillColor(sf::Color::Red);
        }

        float vitesse = selectedVoiture->getVitesse();
        float maxSpeed = 240.f;
        float angle = (vitesse / maxSpeed) * 180.f;
        if (angle > 180.f) angle = 180.f;
        aiguille.setRotation(angle - 180.f);
    }
}

void Fenetre::render() {
    window.clear();

    if (isSelecting) {
        // Render the selection menu
        for (const auto& text : vehicleTexts) {
            window.draw(text);
        }
    } else {
        // Render the simulation
        window.draw(cadran);
        for (auto& marker : kmMarkings) {
            window.draw(marker);
        }
        window.draw(aiguille);
        window.draw(vitesseText);
        window.draw(chronoText);
        window.draw(essenceText);
        window.draw(distanceText);
        window.draw(jaugeEssenceFond);
        window.draw(jaugeEssenceRempli);

        // Draw the replay button and its text
        window.draw(replayButton);
        window.draw(replayText);
    }

    window.display();
}

bool Fenetre::loadResources() {
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
        std::cout << "Erreur de chargement de la police !" << std::endl;
        return false;
    }
    return true;
}