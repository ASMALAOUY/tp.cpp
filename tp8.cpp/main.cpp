#include <iostream>
#include <string>
using namespace std;

class SortieAudio {
public:
    virtual void ouvrir() = 0;
    virtual void jouer(const string& son) = 0;
    virtual void fermer() = 0;
    virtual ~SortieAudio() {
        cout << "Destruction SortieAudio" << endl;
    }
};

class HautParleur : public SortieAudio {
public:
    void ouvrir() override {
        cout << "[HautParleur] Ouverture du haut-parleur." << endl;
    }
    void jouer(const string& son) override {
        cout << "[HautParleur] Lecture du son : " << son << endl;
    }
    void fermer() override {
        cout << "[HautParleur] Fermeture du haut-parleur." << endl;
    }
    ~HautParleur() {
        cout << "Destruction HautParleur" << endl;
    }
};

class CasqueBT : public SortieAudio {
public:
    void ouvrir() override {
        cout << "[CasqueBT] Connexion Bluetooth en cours..." << endl;
    }
    void jouer(const string& son) override {
        cout << "[CasqueBT] Lecture du son via Bluetooth : " << son << endl;
    }
    void fermer() override {
        cout << "[CasqueBT] Déconnexion Bluetooth." << endl;
    }
    ~CasqueBT() {
        cout << "Destruction CasqueBT" << endl;
    }
};

class SortieHDMI : public SortieAudio {
public:
    void ouvrir() override {
        cout << "[SortieHDMI] Initialisation du port HDMI..." << endl;
    }
    void jouer(const string& son) override {
        cout << "[SortieHDMI] Lecture du son via HDMI : " << son << endl;
    }
    void fermer() override {
        cout << "[SortieHDMI] Fermeture du port HDMI." << endl;
    }
    ~SortieHDMI() {
        cout << "Destruction SortieHDMI" << endl;
    }
};

void tester(SortieAudio* sortie) {
    sortie->ouvrir();
    sortie->jouer("Musique classique.mp3");
    sortie->fermer();
    cout << "----------------------------------------" << endl;
}

int main() {
    SortieAudio* peripheriques[3];
    peripheriques[0] = new HautParleur();
    peripheriques[1] = new CasqueBT();
    peripheriques[2] = new SortieHDMI();

    for (int i = 0; i < 3; ++i) {
        tester(peripheriques[i]);
    }

    for (int i = 0; i < 3; ++i) {
        delete peripheriques[i];
    }

    return 0;
}
