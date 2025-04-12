#include "Acceleration_Log.hpp"
#include <thread>
#include <chrono>
#include <sstream>

Acceleration_Log::Acceleration_Log(int id, int voiture, double vitesse, double acc, string heure)
    : id(id), voiture_id(voiture), vitesseInitiale(vitesse), acceleration(acc), heureAcceleration(heure) {}

Acceleration_Log::~Acceleration_Log() {}


// Création d'un nouvel enregistrement
void Acceleration_Log::create(Connexion& conn, int voiture, double vitesse, double acc, string heure) {
    string query = "INSERT INTO acceleration_log (voiture_id, vitesseInitiale, acceleration, heure_acceleration) VALUES (" + 
                    to_string(voiture) + "," + to_string(vitesse) + "," + to_string(acc) + ",'" + heure + "')";
    if (conn.executeUpdate(query)) {
        cout << "Historique acceleration créée avec succès !" << endl;
    } else {
        cout << "Erreur lors de la création de l'historique acceleration." << endl;
    }
}


// Acceleration_Log* Acceleration_Log::getLastAction(Connexion& con, int voiture) {
//     string query = "SELECT * FROM acceleration_log where voiture_id = "+to_string(voiture)+"";


//     vector<vector<string>> result = con.executeQuery(query);

//     if (result.empty()) return nullptr;  // Vérifie s'il y a un résultat

//     const auto& row = result[0];
//     int id = stoi(row[0]);
//     int voiture_id = stoi(row[1]);
//     double vitesse = stod(row[2]);
//     double acceleration = stod(row[3]);
//     double essence = stod(row[4]);
//     string time = row[5];

//     return new Acceleration_Log(id, voiture_id, vitesse, acceleration,essence, time);
// }


Acceleration_Log* Acceleration_Log::getLastAction(Connexion& con, int voiture) {
    string query = "SELECT * FROM acceleration_log where voiture_id = "+to_string(voiture)+"";


    vector<vector<string>> result = con.executeQuery(query);

    if (result.empty()) return nullptr;  // Vérifie s'il y a un résultat

    const auto& row = result[0];
    int id = stoi(row[0]);
    int voiture_id = stoi(row[1]);
    double vitesse = stod(row[2]);
    double acceleration = stod(row[3]);
    string time = row[4];

    return new Acceleration_Log(id, voiture_id, vitesse, acceleration,time);
}

vector<Acceleration_Log> Acceleration_Log::getAllActionForVoiture(Connexion& con, Voiture* voiture) {
    vector<Acceleration_Log> all;
    string query = "SELECT * FROM acceleration_log WHERE voiture_id = " + to_string(voiture->getId());
    vector<vector<string>> result = con.executeQuery(query);
    for (const auto& row : result) {
        int id = stoi(row[0]);
        int voiture_id = stoi(row[1]);
        double vitesse = stod(row[2]);
        double acceleration = stod(row[3]);
        string time = row[4]; // Format attendu : HH:MM:SS.sss (ex. "08:00:05.237")
        Acceleration_Log a(id, voiture_id, vitesse, acceleration, time);
        all.push_back(a);
    }
    return all;
}

// double Acceleration_Log::calculerConsoMoyenne(Connexion& con, Voiture* voiture) {
//     vector<Acceleration_Log> historique = Acceleration_Log::getAllActionForVoiture(con, voiture);
//     if (historique.empty()) {
//         cout << "Aucune donnée pour calculer la consommation." << endl;
//         return 0.0;
//     }

//     double consoTotale = 0.0; // Consommation totale en litres
//     double distanceTotale = 0.0; // Distance totale en km
//     double niveauEssenceInitial = voiture->getNiveauEssence();

//     for (size_t i = 0; i + 1 < historique.size(); i += 2) {
//         Acceleration_Log current = historique[i];
//         Acceleration_Log next = historique[i + 1];

//         // Calcul du temps écoulé en secondes
//         int h1, m1, s1, ms1, h2, m2, s2, ms2;
//         sscanf(current.getHeureAcceleration().c_str(), "%d:%d:%d.%d", &h1, &m1, &s1, &ms1);
//         sscanf(next.getHeureAcceleration().c_str(), "%d:%d:%d.%d", &h2, &m2, &s2, &ms2);
//         double tempsSecondes = (h2 * 3600 + m2 * 60 + s2 + ms2/1000.0) - 
//                               (h1 * 3600 + m1 * 60 + s1 + ms1/1000.0);
//         double tempsHeures = tempsSecondes / 3600.0; // Convertir en heures
//         if (tempsSecondes <= 0) continue;

//         // Vitesse initiale (km/h) et accélération (km/h/s)
//         double vitesseInitiale = current.getVitesseInitiale(); // km/h
//         double acceleration = current.getAcceleration();       // km/h/s

//         // Calcul de la distance en km - Formule corrigée
//         // Pour une vitesse en km/h et un temps en heures
//         double distanceVitesseInitiale = vitesseInitiale * tempsHeures;
        
//         // Pour une accélération en km/h/s, on doit la convertir en km/h²
//         // 1 km/h/s = 3600 km/h²
//         double distanceAcceleration = 0.5 * acceleration * tempsSecondes * tempsSecondes / 3600.0;
        
//         double distance = distanceVitesseInitiale + distanceAcceleration;
//         distanceTotale += distance;

//         // Calcul de la consommation
//         double pourcent = voiture->getFiryPourcent(acceleration);
//         double essenceAvant = voiture->getNiveauEssence();
//         voiture->consommerEssence(tempsSecondes, pourcent); // Temps en secondes pour la conso
//         double essenceApres = voiture->getNiveauEssence();
//         double consommation = essenceAvant - essenceApres;
//         consoTotale += consommation;

//         // Affichage des détails du segment
//         cout << "Segment " << i/2 + 1 << " : " << current.getHeureAcceleration() << " à " 
//              << next.getHeureAcceleration() << endl;
//         cout << "  Temps : " << tempsSecondes << " s" << endl;
//         cout << "  Vitesse initiale : " << vitesseInitiale << " km/h" << endl;
//         cout << "  Accélération : " << acceleration << " km/h/s" << endl;
//         cout << "  Distance : " << distance << " km" << endl;
//         cout << "  Conso : " << consommation << " L" << endl;
//     }

//     voiture->setNiveauEssence(niveauEssenceInitial);

//     if (distanceTotale <= 0.0001) {  // Éviter division par un nombre très petit
//         cout << "Distance totale trop faible, calcul impossible ou non pertinent." << endl;
//         return 0.0;
//     }

//     double consoMoyenne = (consoTotale / distanceTotale) * 100.0;
//     cout << "Consommation totale : " << consoTotale << " L" << endl;
//     cout << "Distance totale : " << distanceTotale << " km" << endl;
//     cout << "Consommation moyenne : " << consoMoyenne << " L/100 km" << endl;

//     return consoMoyenne;
// }

double Acceleration_Log::calculerConsoMoyenne(Connexion& con, Voiture* voiture) {
    vector<Acceleration_Log> historique = Acceleration_Log::getAllActionForVoiture(con, voiture);
    if (historique.size() < 2) {
        cout << "Données insuffisantes pour calculer la consommation." << endl;
        return 0.0;
    }

    double consoTotale = 0.0;
    double distanceTotale = 0.0;
    double niveauEssenceInitial = voiture->getNiveauEssence();

    // Calcul de la consommation et de la distance pour chaque log d'accélération
    for (size_t i = 0; i < historique.size() - 1; ++i) {
        Acceleration_Log current = historique[i];
        Acceleration_Log next = historique[i + 1];

        int h1, m1, s1, ms1, h2, m2, s2, ms2;
        sscanf(current.getHeureAcceleration().c_str(), "%d:%d:%d.%d", &h1, &m1, &s1, &ms1);
        sscanf(next.getHeureAcceleration().c_str(), "%d:%d:%d.%d", &h2, &m2, &s2, &ms2);

        double temps1 = h1 * 3600 + m1 * 60 + s1 + ms1 / 1000.0;
        double temps2 = h2 * 3600 + m2 * 60 + s2 + ms2 / 1000.0;
        double tempsSecondes = temps2 - temps1;

        if (tempsSecondes <= 0.0) continue;  // Ignore les logs avec des temps invalides

        // Accélération du log courant
        double acceleration = current.getAcceleration();

        // Calcul de la distance parcourue pendant cette période
        double distance = voiture->calculDistanceParcouru(tempsSecondes, acceleration) / 1000.0; // en km
        distanceTotale += distance;

        // Calcul de la consommation d'essence pour ce segment
        double pourcent = voiture->getFiryPourcent(acceleration);
        double essenceAvant = voiture->getNiveauEssence();
        voiture->consommerEssence(tempsSecondes, pourcent);
        double essenceApres = voiture->getNiveauEssence();
        double consommation = essenceAvant - essenceApres;
        consoTotale += consommation;

        // Affichage pour chaque segment
        cout << "Segment " << i + 1 << " : " << current.getHeureAcceleration() << " à "
             << next.getHeureAcceleration() << endl;
        cout << "  Temps : " << tempsSecondes << " s" << endl;
        cout << "  Accélération : " << acceleration << " km/h/s" << endl;
        cout << "  Distance : " << distance << " km" << endl;
        cout << "  Conso : " << consommation << " L" << endl;
    }

    // Remettre le niveau d’essence initial
    voiture->setNiveauEssence(niveauEssenceInitial);

    // Si la distance totale est trop faible, retourner 0.0
    if (distanceTotale <= 0.0001) {
        cout << "Distance trop faible pour un calcul fiable." << endl;
        return 0.0;
    }

    // Calcul de la consommation moyenne
    double consoMoyenne = (consoTotale / distanceTotale) * 100.0;

    // Affichage final
    cout << "Consommation totale : " << consoTotale << " L" << endl;
    cout << "Distance totale : " << distanceTotale << " km" << endl;
    cout << "Consommation moyenne : " << consoMoyenne << " L/100 km" << endl;

    return consoMoyenne;
}





// void Acceleration_Log::replay(Connexion& con, Voiture* voiture) {
//     vector<Acceleration_Log> historique = getAllActionForVoiture(con, voiture);

//     if (historique.empty()) {
//         cout << "Aucune donnée d'accélération pour cette voiture." << endl;
//         return;
//     }

//     for (size_t i = 0; i + 1 < historique.size(); i += 2) {  // On avance de 2 en 2
//         Acceleration_Log current = historique[i];
//         Acceleration_Log next = historique[i + 1];

//         // Extraction des heures sous forme HH:MM:SS
//         int h1, m1, s1, h2, m2, s2;
//         sscanf(current.getHeureAcceleration().c_str(), "%d:%d:%d", &h1, &m1, &s1);
//         sscanf(next.getHeureAcceleration().c_str(), "%d:%d:%d", &h2, &m2, &s2);

//         // Conversion en secondes et calcul du temps écoulé
//         int time1 = h1 * 3600 + m1 * 60 + s1;
//         int time2 = h2 * 3600 + m2 * 60 + s2;
//         int elapsed_seconds = time2 - time1;

//         if (elapsed_seconds <= 0) continue; // Éviter les erreurs si les logs sont mal enregistrés

//         double vitesse = current.getVitesseInitiale();
//         double acceleration = current.getAcceleration();
        
//         cout << "Début à " << current.getHeureAcceleration() << " - Vitesse initiale: " 
//              << vitesse << " m/s, Accélération: " << acceleration << " m/s²" << endl;

//         for (int t = 0; t < elapsed_seconds; t++) {
//             vitesse += acceleration;  // Mise à jour de la vitesse
//             if (vitesse < 0) vitesse = 0; // La vitesse ne peut pas être négative

//             cout << "Temps: " << (t + 1) << "s - Vitesse: " << vitesse << " m/s" << endl;
//             this_thread::sleep_for(chrono::seconds(1)); // Simulation en temps réel
//         }

//         cout << "Fin à " << next.getHeureAcceleration() << " - Vitesse atteinte: " << vitesse << " m/s\n" << endl;
//     }
// }

/*void Acceleration_Log::replay(Connexion& con, Voiture* voiture) {
    vector<Acceleration_Log> historique = getAllActionForVoiture(con, voiture);

    if (historique.empty()) {
        cout << "Aucune donnée d'accélération pour cette voiture." << endl;
        return;
    }

    for (size_t i = 0; i + 1 < historique.size(); i += 2) {  // On avance de 2 en 2
        Acceleration_Log current = historique[i];
        Acceleration_Log next = historique[i + 1];

        // Extraction des heures sous forme HH:MM:SS
        int h1, m1, s1, h2, m2, s2;
        sscanf(current.getHeureAcceleration().c_str(), "%d:%d:%d", &h1, &m1, &s1);
        sscanf(next.getHeureAcceleration().c_str(), "%d:%d:%d", &h2, &m2, &s2);

        // Conversion en secondes et calcul du temps écoulé
        int time1 = h1 * 3600 + m1 * 60 + s1;
        int time2 = h2 * 3600 + m2 * 60 + s2;
        int elapsed_seconds = time2 - time1;

        if (elapsed_seconds <= 0) continue; // Éviter les erreurs si les logs sont mal enregistrés

        double vitesse = current.getVitesseInitiale();
        double acceleration = current.getAcceleration();
        double pourcent = voiture->getFiryPourcent(acceleration);

        cout << "vitesse courante est " << vitesse << endl;
        cout << "acceleration courante est " << acceleration << endl;
        cout << "heure courante courante est " << current.getHeureAcceleration() << endl;
        cout << "elsaped temps   est " << elapsed_seconds << endl;
        cout << "pourcent accel   est " << pourcent << endl;

        // cout << "Début à " << current.getHeureAcceleration() << " - Vitesse initiale: " 
        //      << vitesse << " m/s, Accélération: " << acceleration << " m/s²" << endl;

        if(acceleration > 0){
            voiture->acceleration(elapsed_seconds,pourcent);
            voiture->consommerEssence(elapsed_seconds,pourcent);
        }else{
            voiture->decceleration(elapsed_seconds,-pourcent);
        }

        // cout << "Fin à " << next.getHeureAcceleration() << " - Vitesse atteinte: " << vitesse << " m/s\n" << endl;
    }
}*/

// void Acceleration_Log::replay(Connexion& con, Voiture* voiture) {
//     vector<Acceleration_Log> historique = getAllActionForVoiture(con, voiture);

//     if (historique.empty()) {
//         cout << "Aucune donnée d'accélération pour cette voiture." << endl;
//         return;
//     }

//     // Pas de temps fixe (en secondes), comme dans Fenetre::evenement
//     const float fixedTimeStep = 0.016f; // ~60 FPS, ajustable

//     for (size_t i = 0; i + 1 < historique.size(); i += 2) { // On avance de 2 en 2
//         Acceleration_Log current = historique[i];
//         Acceleration_Log next = historique[i + 1];

//         // Extraction des heures sous forme HH:MM:SS
//         int h1, m1, s1, h2, m2, s2;
//         sscanf(current.getHeureAcceleration().c_str(), "%d:%d:%d", &h1, &m1, &s1);
//         sscanf(next.getHeureAcceleration().c_str(), "%d:%d:%d", &h2, &m2, &s2);

//         // Conversion en secondes et calcul du temps écoulé
//         int time1 = h1 * 3600 + m1 * 60 + s1;
//         int time2 = h2 * 3600 + m2 * 60 + s2;
//         float elapsed_seconds = static_cast<float>(time2 - time1);

//         if (elapsed_seconds <= 0) continue; // Éviter les erreurs

//         double vitesse = current.getVitesseInitiale();
//         double acceleration = current.getAcceleration();
//         double pourcent = voiture->getFiryPourcent(acceleration);

//         // cout << "Début à " << current.getHeureAcceleration() << " - Vitesse initiale: " 
//         //      << voiture->getVitesse() << " km/h, Accélération: " << acceleration << " km/h/s" << endl;
//         // cout << "Durée de l'action : " << elapsed_seconds << " secondes" << endl;
//         // cout << "Pourcentage : " << pourcent << endl;

//         cout << "vitesse courante est " << vitesse << endl;
//         cout << "acceleration courante est " << acceleration << endl;
//         cout << "heure courante courante est " << current.getHeureAcceleration() << endl;
//         cout << "elsaped temps   est " << elapsed_seconds << endl;
//         cout << "pourcent accel   est " << pourcent << endl;

//         // Simuler la progression sur elapsed_seconds avec fixedTimeStep
//         float timeAccumulator = 0.0f;
//         while (timeAccumulator < elapsed_seconds) {
//             // Appliquer l'accélération ou la décélération par petit pas
//             if (acceleration > 0) {
//                 voiture->acceleration(fixedTimeStep, pourcent);
//                 voiture->consommerEssence(fixedTimeStep, pourcent);
//             } else {
//                 voiture->decceleration(fixedTimeStep, -pourcent);
//             }

//             timeAccumulator += fixedTimeStep;

//             cout << "Temps: " << timeAccumulator << "s - Vitesse: " << voiture->getVitesse() << " km/h" << endl;

//             this_thread::sleep_for(chrono::milliseconds(static_cast<int>(fixedTimeStep * 1000)));
//         }

//         cout << "Fin à " << next.getHeureAcceleration() << " - Vitesse atteinte: " 
//              << voiture->getVitesse() << " km/h\n" << endl;
//     }
// }


/*void Acceleration_Log::replay(Connexion& con, Voiture* voiture) {
    vector<Acceleration_Log> historique = getAllActionForVoiture(con, voiture);
    if (historique.empty()) {
        cout << "Aucune donnée d'accélération pour cette voiture." << endl;
        return;
    }

    // Pas de temps fixe pour la simulation (en secondes)
    const float fixedTimeStep = 0.016f; // ~60 FPS, pour une simulation fluide

    for (size_t i = 0; i + 1 < historique.size(); i += 2) { // On avance de 2 en 2
        Acceleration_Log current = historique[i];
        Acceleration_Log next = historique[i + 1];

        // Extraction des heures sous forme HH:MM:SS
        int h1, m1, s1, h2, m2, s2;
        sscanf(current.getHeureAcceleration().c_str(), "%d:%d:%d", &h1, &m1, &s1);
        sscanf(next.getHeureAcceleration().c_str(), "%d:%d:%d", &h2, &m2, &s2);

        // Conversion en secondes et calcul du temps écoulé
        int time1 = h1 * 3600 + m1 * 60 + s1;
        int time2 = h2 * 3600 + m2 * 60 + s2;
        float elapsed_seconds = static_cast<float>(time2 - time1);

        if (elapsed_seconds <= 0) continue; // Éviter les erreurs

        // Récupération des données du log
        double vitesseInitiale = current.getVitesseInitiale();
        double acceleration = current.getAcceleration();
        double pourcent = voiture->getFiryPourcent(acceleration);

        // Mise à jour de la vitesse de la voiture AVANT la simulation
        voiture->setVitesse(vitesseInitiale);

        cout << "Début à " << current.getHeureAcceleration() << endl;
        cout << "Vitesse initiale: " << voiture->getVitesse() << " km/h" << endl;
        cout << "Accélération: " << acceleration << " km/h/s" << endl;
        cout << "Durée de l'action : " << elapsed_seconds << " secondes" << endl;
        cout << "Pourcentage d'accélération : " << pourcent << endl;

        // Simuler la progression sur elapsed_seconds petit à petit en temps réel
        float timeAccumulator = 0.0f;
        while (timeAccumulator < elapsed_seconds) {
            // Appliquer l'accélération ou la décélération par petit pas
            if (acceleration > 0) {
                voiture->acceleration(fixedTimeStep, pourcent);
                voiture->consommerEssence(fixedTimeStep, pourcent);
            } else {
                voiture->decceleration(fixedTimeStep, -pourcent);
            }

            timeAccumulator += fixedTimeStep;

            cout << "Temps: " << timeAccumulator << "s - Vitesse: "
                 << voiture->getVitesse() << " km/h" << endl;

            // Pause pour simuler en temps réel
            this_thread::sleep_for(chrono::milliseconds(static_cast<int>(fixedTimeStep * 1000)));
        }

        cout << "Fin à " << next.getHeureAcceleration() << " - Vitesse atteinte: "
             << voiture->getVitesse() << " km/h\n" << endl;

        // Respecter l'écart réel avant de passer au segment suivant
        if (i + 2 < historique.size()) { // Vérifier s'il y a un prochain segment
            Acceleration_Log nextSegment = historique[i + 2];
            sscanf(nextSegment.getHeureAcceleration().c_str(), "%d:%d:%d", &h1, &m1, &s1);
            int nextTime = h1 * 3600 + m1 * 60 + s1;
            float delayToNext = static_cast<float>(nextTime - time2);
            if (delayToNext > 0) {
                cout << "Attente avant le prochain segment : " << delayToNext << " secondes" << endl;
                this_thread::sleep_for(chrono::milliseconds(static_cast<int>(delayToNext * 1000)));
            }
        }
    }
}*/

// void Acceleration_Log::replay(Connexion& con, Voiture* voiture){
//     vector<Acceleration_Log>historique = getAllActionForVoiture(con,voiture);
//     for()
// }


// Getters
int Acceleration_Log::getId() { return id; }
int Acceleration_Log::getVoitureId() { return voiture_id; }
double Acceleration_Log::getVitesseInitiale() { return vitesseInitiale; }
double Acceleration_Log::getAcceleration() { return acceleration; }
string Acceleration_Log::getHeureAcceleration() { return heureAcceleration; }

// Setters
void Acceleration_Log::setVoitureId(int voiture_id) { this->voiture_id = voiture_id; }
void Acceleration_Log::setVitesseInitiale(double vitesseInitiale) { this->vitesseInitiale = vitesseInitiale; }
void Acceleration_Log::setAcceleration(double acceleration) { this->acceleration = acceleration; }
void Acceleration_Log::setHeureAcceleration(string heure_acceleration) { this->heureAcceleration = heure_acceleration; }

