#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
using namespace std;

class Plugin {
public:
    virtual string nom() const = 0;
    virtual string traiter(const string& texte) = 0;
    virtual ~Plugin() = default;

    static unique_ptr<Plugin> creerPlugin(const string& type);
};

class CorrecteurOrthographique : public Plugin {
public:
    string nom() const override { return "Correcteur Orthographique"; }

    string traiter(const string& texte) override {
        string res = texte;
        size_t pos;
        while ((pos = res.find("bonjor")) != string::npos) res.replace(pos, 6, "bonjour");
        while ((pos = res.find("merçi")) != string::npos) res.replace(pos, 5, "merci");
        while ((pos = res.find("sa")) != string::npos) {
            if (pos == 0 || res[pos - 1] == ' ') res.replace(pos, 2, "ça");
            else break;
        }
        return res;
    }
};

class TraducteurAnglais : public Plugin {
public:
    string nom() const override { return "Traducteur Anglais"; }

    string traiter(const string& texte) override {
        string res = texte;
        vector<pair<string, string>> dico = {
            {"bonjour", "hello"},
            {"merci", "thank you"},
            {"monde", "world"},
            {"ami", "friend"}
        };
        for (auto& mot : dico) {
            size_t pos;
            while ((pos = res.find(mot.first)) != string::npos)
                res.replace(pos, mot.first.size(), mot.second);
        }
        return res;
    }
};

class AnalyseurStyle : public Plugin {
public:
    string nom() const override { return "Analyseur de Style"; }

    string traiter(const string& texte) override {
        int longues = 0;
        size_t start = 0;
        while (true) {
            size_t pos = texte.find('.', start);
            if (pos == string::npos) break;
            string phrase = texte.substr(start, pos - start);
            int mots = count(phrase.begin(), phrase.end(), ' ') + 1;
            if (mots > 12) longues++;
            start = pos + 1;
        }
        string res = texte;
        if (longues > 0)
            res += "\n[AnalyseurStyle] Attention : " + to_string(longues) + " phrase(s) trop longue(s).";
        else
            res += "\n[AnalyseurStyle] Style clair et concis.";
        return res;
    }
};

unique_ptr<Plugin> Plugin::creerPlugin(const string& type) {
    if (type == "correcteur") return make_unique<CorrecteurOrthographique>();
    if (type == "traducteur") return make_unique<TraducteurAnglais>();
    if (type == "analyseur") return make_unique<AnalyseurStyle>();
    return nullptr;
}

int main() {
    vector<unique_ptr<Plugin>> plugins;
    plugins.push_back(Plugin::creerPlugin("correcteur"));
    plugins.push_back(Plugin::creerPlugin("traducteur"));
    plugins.push_back(Plugin::creerPlugin("analyseur"));

    string texte;
    cout << "Entrez un texte : ";
    getline(cin, texte);

    for (auto& plugin : plugins) {
        texte = plugin->traiter(texte);
        cout << "\n--- Plugin : " << plugin->nom() << " ---\n";
        cout << texte << endl;
    }

    return 0;
}
