#include "Voiture.hpp"
#include "Acceleration_Log.hpp"

Voiture::Voiture(int id, string nom, double accel, double dec,double vitesse,double capaciteReservoir,double consoMax)
    : id(id), nom(nom), accel(accel), dec(dec),vitesse(vitesse),capaciteReservoir(capaciteReservoir),niveauEssence(capaciteReservoir),consoMax(consoMax),distanceParcourue(0.0) {}

Voiture::~Voiture() {
    
}

void Voiture::afficherDetails(){
    cout << "Voiture ID: " << id << endl;
    cout << "Nom: " << nom << endl;
    cout << "Accélération: " << accel << " m/s²" << endl;
    cout << "Décélération: " << dec << " m/s²" << endl;
}

// double Voiture::calculDistanceParcouru(double temps,double accel){
//     double dist = distanceParcourue;
//     double form = (0.5*accel*temps*temps) + (this->vitesse * temps) + dist;
//     distanceParcourue = form;
//     return form;
// }

/*double Voiture::calculDistanceParcouru(double temps, double accel) {
    double accelMS2 = accel * 0.27778; // km/h/s en m/s²
    double vitesseMS = this->vitesse * 0.27778; // km/h en m/s
    double nouvelleDistance = (0.5 * accelMS2 * temps * temps) + (vitesseMS * temps); // distance pendant l'intervalle
    distanceParcourue += nouvelleDistance;
    return distanceParcourue;
}*/

double Voiture::calculDistanceParcouru(double temps, double accel) {
    double accelMS2 = accel * 0.27778; // km/h/s → m/s²
    double vitesseMS = this->vitesse * 0.27778; // vitesse de départ (km/h → m/s)
    double distance = (0.5 * accelMS2 * temps * temps) + (vitesseMS * temps); // Δx = v0.t + 0.5.a.t²
    distanceParcourue += distance; // On ajoute à la distance cumulée
    vitesse += accel * temps; // On met à jour la vitesse (km/h) si besoin
    return distance;
}



double Voiture::getConsoPourcentage(double pourcent){
    double consoMax = this->consoMax;
    double lany = pourcent * consoMax;
    return lany;
}

double Voiture::getFiryPourcent(double accel){
    // double result = this->getAccel() 
    double result = ( accel * 100 ) / this->getAccel();
    return result/100;
}

double Voiture::consommerEssence(double temps, double pourcent) {
    double pourcentageConso = getConsoPourcentage(pourcent);
    niveauEssence -= (pourcentageConso * temps);
    if (niveauEssence < 0) niveauEssence = 0;  // Pas de niveau négatif
    return niveauEssence;
}

// double Voiture::getCarburantConsommer(double temps,double pourcent) {

// }


double Voiture::acceleration(double temps,double pourcent) { // Passer temps en double
    double vitesseInitiale = this->vitesse;
    vitesseInitiale = (this->accel *pourcent * temps) + vitesseInitiale;
    this->setVitesse(vitesseInitiale);
    cout << "Sa vitesse maintenant est " << vitesseInitiale << " km/h" << endl;
    return vitesseInitiale;
}


double Voiture::decceleration(double temps,double pourcent) { // Passer temps en double
    double vitesseInitiale = this->vitesse;
    vitesseInitiale = (this->dec * -1 * temps * pourcent) + vitesseInitiale;
    if(vitesseInitiale < 0){
        vitesseInitiale = 0;
    }
    this->setVitesse(vitesseInitiale);
    cout << "Sa vitesse maintenant est " << vitesseInitiale << " km/h" << endl;
    return vitesseInitiale;
}

double Voiture::getAccelerationPourcent(double p){
    double acceleration = this->getAccel();
    acceleration = (acceleration * p);
    return acceleration;
}


vector<Voiture> Voiture::getAll(Connexion& conn){
    vector<Voiture> voitures;
    sql::Connection* con = conn.getConnection();
    
    // Exécution de la requête pour récupérer toutes les voitures
    string query = "SELECT * FROM voiture";
    vector<vector<string>> result = conn.executeQuery(query);
    
    // Traiter les résultats
    for (const auto& row : result) {
        int id = stoi(row[0]);
        string nom = row[1];
        double accel = stod(row[2]);
        double decel = stod(row[3]);
        double vitesse = stod(row[4]);
        double capacite = stod(row[5]);
        double consoMax = stod(row[6]);
        Voiture v(id, nom, accel,decel,vitesse,capacite,consoMax); 
        voitures.push_back(v);
    }
    
    return voitures;
}

/*Voiture* Voiture::getById(Connexion& conn,int idVoiture){
    for (Voiture& v : getAll(conn)){
        if(v.getId() == idVoiture){
            return &v;
        }
    }
    return nullptr;
}*/

Voiture* Voiture::getById(Connexion& conn, int idVoiture) {
    string query = "SELECT * FROM voiture WHERE id = " + to_string(idVoiture);
    vector<vector<string>> result = conn.executeQuery(query);
    
    if (result.empty()) {
        return nullptr;  // Si aucune voiture n'est trouvée
    }

    int id = stoi(result[0][0]);
    string nom = result[0][1];
    double accel = stod(result[0][2]);
    double decel = stod(result[0][3]);
    double vitesse = stod(result[0][4]);
    double capacite = stod(result[0][5]);
    double consoMax = stod(result[0][6]);

    Voiture* v = new Voiture(id, nom, accel, decel,vitesse,capacite,consoMax);
    return v;
}

void Voiture::create(Connexion& conn, string nom, double accel, double dec) {
    sql::Connection* con = conn.getConnection();
    
    string query = "INSERT INTO voiture (nom, accel,decel) VALUES ('" + nom + "', " + to_string(accel) + ", "+ to_string(dec)+")";
    
    // Exécution de la requête
    if (conn.executeUpdate(query)) {
        cout << "Voiture créée avec succès !" << endl;
    } else {
        cout << "Erreur lors de la création de la voiture." << endl;
    }
}

int Voiture::getId() { return id; }
string Voiture::getNom() { return nom; }
double Voiture::getAccel() { return accel; }
double Voiture::getDec() { return dec; }
double Voiture::getVitesse() {return vitesse; }
double Voiture::getConsoMax(){ return consoMax; }
double Voiture::getCapaciteReservoir(){ return capaciteReservoir; }
double Voiture::getNiveauEssence() { return niveauEssence; }
double Voiture::getDistanceParcouru(){return distanceParcourue; }

// double Voiture::getLastVitesse() {return vitesse; }

void Voiture::setId(int id) { this->id = id; }
void Voiture::setNom(string nom) { this->nom = nom; }
void Voiture::setAccel(double accel) { this->accel = accel; }
void Voiture::setDec(double dec) { this->dec = dec; }
void Voiture::setVitesse(double vitesse) { this->vitesse = vitesse; }
void Voiture::setConsoMax(double consoMax) { this->consoMax = vitesse; }
void Voiture::setCapaciteReservoir(double vitesse) { this->capaciteReservoir = vitesse; }
void Voiture::setNiveauEssence(double niveau) { this->niveauEssence = niveau; }
void Voiture::setDistanceParcourue(double diist){ this->distanceParcourue = diist;}

