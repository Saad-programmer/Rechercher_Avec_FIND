#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <cstdio>      // pour FILE, fopen, fclose, remove
#include <cstdlib>     // pour exit, system

using namespace std;
using namespace std::chrono;

bool rechercher_mot(const vector<string>& lignes, const string& mot) {
    for (int i = 0; i < (int)lignes.size(); i++) {
        if (lignes[i].find(mot) != string::npos) {
            cout << "   -> trouve a la ligne " << (i + 1) << endl;
            cout << "   -> contenu : \"" << lignes[i] << "\"" << endl;
            return true;
        }
    }
    return false;
}

int main() {
    // 1. CONVERSION DU PDF EN TEXTE BRUT
    cout << "Conversion du fichier PDF en texte..." << endl;
    
    // Cette commande extrait le texte de "roman.pdf" vers "roman_temp.txt"
    // Le paramètre "-layout" permet de préserver au mieux la structure des lignes
    string commande = "pdftotext -layout Feuillet-pauvre.pdf roman_temp.txt";
    int status = std::system(commande.c_str());
    
    if (status != 0) {
        cout << "Erreur: Impossible de convertir le fichier PDF ou 'pdftotext' n'est pas installe !!\n";
        exit(-1);
    }

    // 2. LECTURE DU FICHIER TEXTE GÉNÉRÉ
    FILE* myFile = fopen("roman_temp.txt", "r");
    if (!myFile) {
        cout << "Erreur: Impossible de lire le fichier temporaire !!\n";
        exit(-1);
    }
    
    vector<string> lignes;
    char buffer[1024];       
    
    auto debut_lecture = high_resolution_clock::now();
    
    while (fgets(buffer, sizeof(buffer), myFile) != nullptr) {
        string ligne = buffer;
        
        if (!ligne.empty() && ligne.back() == '\n') {
            ligne.pop_back();
        }
        
        lignes.push_back(ligne);   
    }
    
    auto fin_lecture = high_resolution_clock::now();
    auto temps_lecture = duration_cast<microseconds>(fin_lecture - debut_lecture).count();
    
    fclose(myFile);   
    
    // Nettoyage : Supprime le fichier temporaire du disque après lecture en mémoire
    std::remove("roman_temp.txt");

    // 3. TRAITEMENTS ET RECHERCHE (Le reste de votre code reste identique)
    long long total_caracteres = 0;
    for (int i = 0; i < (int)lignes.size(); i++) {
        total_caracteres += lignes[i].length();
    }
    
    cout << "========== FICHIER CHARGE ==========" << endl;
    cout << "Nombre de lignes      : " << lignes.size() << endl;
    cout << "Total caracteres      : " << total_caracteres << endl;
    cout << "Temps de lecture      : " << temps_lecture << " microsecondes" << endl;
    cout << "                       (" << temps_lecture / 1000.0 << " ms)" << endl;
    
    string mot;
    cout << "\nEntrez le mot a rechercher : ";
    getline(cin, mot);   
    
    cout << "\n========== RECHERCHE ==========" << endl;
    cout << "Mot recherche : \"" << mot << "\"" << endl;
    cout << "Longueur mot  : " << mot.length() << endl;
    
    auto debut_recherche = high_resolution_clock::now();
    bool trouve = rechercher_mot(lignes, mot);
    auto fin_recherche = high_resolution_clock::now();
    auto temps_recherche = duration_cast<nanoseconds>(fin_recherche - debut_recherche).count();
    

    cout << "\n--- RESULTAT ---" << endl;
    if (trouve) {
        cout << "\"" << mot << "\" EXISTE dans le fichier." << endl;
    } else {
        cout << "\"" << mot << "\" N'EXISTE PAS dans le fichier." << endl;
    }
  
    long long L = lignes.size();            
    long long C = total_caracteres;         
    long long M = mot.length();             
    long long max_ops = C * M;              
    
    cout << "\n========== DETAILS DES CALCULS ==========" << endl;
    cout << "L = nombre de lignes        = " << L << endl;
    cout << "C = total caracteres        = " << C << endl;
    cout << "M = longueur du mot cherche = " << M << endl;
    cout << "L x M (pire cas)            = " << (L * M) << endl;
    cout << "C x M (pire cas find)       = " << max_ops << endl;
    
    cout << "\n========== TEMPS D'EXECUTION ==========" << endl;
    cout << "Recherche : " << temps_recherche << " nanosecondes" << endl;
    cout << "          = " << temps_recherche / 1000.0 << " microsecondes" << endl;
    cout << "          = " << temps_recherche / 1000000.0 << " millisecondes" << endl;
    
    cout << "\n========== COMPLEXITE ==========" << endl;
    cout << "Pire cas : O(C x M)  (find naive sur chaque ligne)" << endl;
    cout << "          = O(" << C << " x " << M << ") = O(" << max_ops << ")" << endl;
    cout << "Meilleur cas : O(M)  (trouve sur la 1ere ligne)" << endl;
    
    return 0;
}
