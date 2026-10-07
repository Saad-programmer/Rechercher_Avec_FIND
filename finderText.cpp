#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <cstdio>      // pour FILE, fopen, fclose
#include <cstdlib>     // pour exit

using namespace std;
using namespace std::chrono;

bool rechercher_mot(const vector<string>& lignes, const string& mot) {
    for (int i = 0; i < (int)lignes.size(); i++) {
        // lignes[i].find(mot) retourne la position si trouvé
        // sinon retourne string::npos
        if (lignes[i].find(mot) != string::npos) {
            cout << "   -> trouve a la ligne " << (i + 1) << endl;
            cout << "   -> contenu : \"" << lignes[i] << "\"" << endl;
            return true;
        }
    }
    return false;
}


int main() {
    FILE* myFile = fopen("roman.txt", "r");
    if (!myFile) {
        cout << "Erreur: Impossible de lire le fichier !!\n";
        exit(-1);
    }
    
    vector<string> lignes;
    char buffer[1024];       
    
    auto debut_lecture = high_resolution_clock::now();
    
    while (fgets(buffer, sizeof(buffer), myFile) != nullptr) {
        string ligne = buffer;
        
        // Enlever le '\n' à la fin (si present)
        if (!ligne.empty() && ligne.back() == '\n') {
            ligne.pop_back();
        }
        
        lignes.push_back(ligne);   // ajouter au vector
    }
    
    auto fin_lecture = high_resolution_clock::now();
    auto temps_lecture = duration_cast<microseconds>(fin_lecture - debut_lecture).count();
    
    fclose(myFile);   
    
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
    getline(cin, mot);   // getline pour pouvoir avoir des espaces
    
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
  
    long long L = lignes.size();            // nombre de lignes
    long long C = total_caracteres;         // total caractères
    long long M = mot.length();             // longueur du mot
    long long max_ops = C * M;              // pire cas : O(C x M)
    
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