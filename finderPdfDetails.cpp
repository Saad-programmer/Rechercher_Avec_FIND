#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <cstdio>      // pour FILE, fopen, fclose, remove
#include <cstdlib>     // pour exit, system

using namespace std;
using namespace std::chrono;

// Structure pour stocker le contexte complet d'une occurrence trouvée
struct Occurrence {
    int page;
    int paragraphe;
    int ligneGlobale;
    string contexte;
};

// Analyse le texte brut généré pour découper le livre en pages, paragraphes et lignes
void charger_livre_pdf(const string& fichierTxt, vector<string>& toutesLesLignes, 
                       vector<int>& correspondancePages, vector<int>& correspondanceParagraphes) {
    FILE* myFile = fopen(fichierTxt.c_str(), "r");
    if (!myFile) {
        cout << "Erreur: Impossible de lire le fichier temporaire !!\n";
        exit(-1);
    }

    char buffer[2048];
    int pageActuelle = 1;
    int paragrapheActuel = 1;
    bool nouvelleLigneVide = true;
    int indexLigne = 0;

    while (fgets(buffer, sizeof(buffer), myFile) != nullptr) {
        string ligne = buffer;

        // Détection d'un saut de page physique (caractère Form Feed '\f' généré par pdftotext)
        size_t ff_pos = ligne.find('\f');
        if (ff_pos != string::npos) {
            pageActuelle++;
            paragrapheActuel = 1; // Réinitialise les paragraphes à chaque nouvelle page
            // Nettoie le caractère de saut de page de la ligne
            ligne.erase(ff_pos, 1);
        }

        if (!ligne.empty() && ligne.back() == '\n') {
            ligne.pop_back();
        }

        // Détection des paragraphes (séparés par des lignes vides ou d'espaces)
        if (ligne.empty() || ligne.find_first_not_of(" \t\r") == string::npos) {
            nouvelleLigneVide = true;
        } else {
            if (nouvelleLigneVide) {
                if (indexLigne > 0) paragrapheActuel++; 
                nouvelleLigneVide = false;
            }
        }

        toutesLesLignes.push_back(ligne);
        correspondancePages.push_back(pageActuelle);
        correspondanceParagraphes.push_back(paragrapheActuel);
        indexLigne++;
    }

    fclose(myFile);
}

// Fonction de recherche prenant en charge les deux modes demandés
void rechercher_mot_pdf(const vector<string>& lignes, const vector<int>& pages, 
                         const vector<int>& paragraphes, const string& mot, 
                         bool chercherTout, vector<Occurrence>& resultats, long long& operationsEffectuees) {
    operationsEffectuees = 0;
    
    for (size_t i = 0; i < lignes.size(); i++) {
        // Simulation du coût théorique de l'opération find() sur cette ligne
        operationsEffectuees += (lignes[i].length() * mot.length());

        size_t pos = lignes[i].find(mot);
        if (pos != string::npos) {
            Occurrence occ;
            occ.page = pages[i];
            occ.paragraphe = paragraphes[i];
            occ.ligneGlobale = i + 1;
            occ.contexte = lignes[i];
            resultats.push_back(occ);

            // Si l'utilisateur a choisi la première occurrence uniquement, on quitte immédiatement
            if (!chercherTout) {
                return;
            }
        }
    }
}

int main() {
    // 1. CONVERSION DU PDF AVEC INJECTION DES SAUTS DE PAGES
    cout << "Extraction du contenu de \"roman.pdf\" en cours..." << endl;
    
    // On utilise -layout pour préserver la structure visuelle des paragraphes
    string commande = "pdftotext -layout Feuillet-pauvre.pdf roman_temp.txt";
    int status = std::system(commande.c_str());
    
    if (status != 0) {
        cout << "Erreur: 'pdftotext' a echoue. Verifiez qu'il est installe au meme emplacement ou dans votre PATH.\n";
        exit(-1);
    }

    // Structures de données pour indexer le PDF
    vector<string> lignes;
    vector<int> correspondancePages;
    vector<int> correspondanceParagraphes;
    
    auto debut_lecture = high_resolution_clock::now();
    charger_livre_pdf("roman_temp.txt", lignes, correspondancePages, correspondanceParagraphes);
    auto fin_lecture = high_resolution_clock::now();
    auto temps_lecture = duration_cast<microseconds>(fin_lecture - debut_lecture).count();
    
    // Nettoyage du disque apres la mise en mem
    std::remove("roman_temp.txt");

    // metric globale du livre
    long long total_caracteres = 0;
    for (const auto& l : lignes) total_caracteres += l.length();
    int total_pages = correspondancePages.empty() ? 0 : correspondancePages.back();

    cout << "\n========== ROMAN PDF CHARGE ==========" << endl;
    cout << "Nombre de pages estimees : " << total_pages << endl;
    cout << "Nombre total de lignes   : " << lignes.size() << endl;
    cout << "Total des caracteres     : " << total_caracteres << endl;
    cout << "Temps de traitement I/O  : " << temps_lecture << " microsecondes (" << temps_lecture / 1000.0 << " ms)" << endl;
    
    // amelioration
    string mot;
    cout << "\nEntrez le mot ou l'expression a rechercher : ";
    getline(cin, mot);//cin>>mot;// pour permettre espaces
    // choix: premiere ou toutes occurence
    cout << "\nChoisissez le mode de recherche :\n";
    cout << "  1. Premiere occurrence uniquement (Sortie rapide)\n";
    cout << "  2. Toutes les occurrences du roman\n";
    cout << "Votre choix (1 ou 2) : ";
    string choix;
    getline(cin, choix);
    bool chercherTout = (choix == "2");

    cout << "\n========== RECHERCHE EN COURS ==========" << endl;
    cout << "Mot recherche : \"" << mot << "\" (" << mot.length() << " caracteres)" << endl;
    cout << "Mode choisi    : " << (chercherTout ? "Toutes les occurrences" : "Premiere occurrence") << endl;
    
    //  chronometrage
    vector<Occurrence> occurrencesTrouvees;
    long long operationsSimulees = 0;
    
    auto debut_recherche = high_resolution_clock::now();
    rechercher_mot_pdf(lignes, correspondancePages, correspondanceParagraphes, mot, chercherTout, occurrencesTrouvees, operationsSimulees);
    auto fin_recherche = high_resolution_clock::now();
    
    auto temps_recherche_ns = duration_cast<nanoseconds>(fin_recherche - debut_recherche).count();

    cout << "\n--- CONTEXTE DES OCCURRENCES TROUVEES ---" << endl;
    if (occurrencesTrouvees.empty()) {
        cout << "Aucune occurrence trouvee pour le terme \"" << mot << "\"." << endl;
    } else {
        for (size_t idx = 0; idx < occurrencesTrouvees.size(); idx++) {
            const auto& occ = occurrencesTrouvees[idx];
            cout << "\n[Match #" << (idx + 1) << "] Page " << occ.page 
                 << " | Paragraphe " << occ.paragraphe 
                 << " (Ligne globale : " << occ.ligneGlobale << ")" << endl;
            cout << "   Contexte -> \"" << occ.contexte << "\"" << endl;
        }
    }

    // 5. BILAN ÉVOLUTIF DES PERFORMANCES ET DE LA COMPLEXITÉ
    long long L = lignes.size();            
    long long C = total_caracteres;         
    long long M = mot.length();             
    long long pire_cas_th = C * M;              

    cout << "\n========== DETAILS COMPORTEMENTAUX ==========" << endl;
    cout << "L (Nombre total de lignes)      = " << L << endl;
    cout << "C (Nombre total de caracteres)  = " << C << endl;
    cout << "M (Longueur du filtre cherche)  = " << M << endl;
    cout << "Operations algorithmiques reelles : " << operationsSimulees << " calculs scalaires." << endl;
    
    cout << "\n========== TEMPS D'EXECUTION REEL ==========" << endl;
    cout << "Duree stricte de la recherche : " << temps_recherche_ns << " nanosecondes" << endl;
    cout << "                              = " << temps_recherche_ns / 1000.0 << " microsecondes" << endl;
    cout << "                              = " << temps_recherche_ns / 1000000.0 << " millisecondes" << endl;
    
    cout << "\n========== COMPARAISON DE COMPLEXITE ==========" << endl;
    if (!chercherTout) {
        cout << "Cas 1 (Premiere occurrence) :\n";
        cout << "  • Complexite Pire Cas : O(C x M) si absent ou positionne a la derniere ligne.\n";
        cout << "  • Complexite Meilleur Cas : O(M) si trouve des le debut du PDF.\n";
        cout << "  • Efficacite constatée : O(" << operationsSimulees << ") sur un plafond max de O(" << pire_cas_th << ").\n";
    } else {
        cout << "Cas 2 (Toutes les occurrences) :\n";
        cout << "  • Complexite Strictement Fixe : O(C x M).\n";
        cout << "  • Explication : Le programme est oblige de scanner l'integralite du roman sans interruption.\n";
        cout << "  • Efficacite constatée : Toujours egal au pire cas = O(" << operationsSimulees << ") operations.\n";
    }
    
    return 0;
}
