#include <iostream>
#include <string>
using namespace std;
/* je veux aussi calculer le temps d execution du programme en details vraies sur cette machine, et afficher a la fin temps d execution , complexite temporelle, etc*/
/* int main() {
    string texte = "TESTEUR";
    string recherche = "TEST";
    
    bool trouve = true;
    
    

    for (int i = 0; i < recherche.length(); i++) {
        if (texte.find(recherche[i]) == string::npos) {
            trouve = false;
            break;
        }
    }
    
    if (trouve) {
        cout << "\"" << recherche << "\" existe dans \"" << texte << "\"" << endl;
    } else {
        cout << "\"" << recherche << "\" n'existe pas dans \"" << texte << "\"" << endl;
    }
    

    return 0;
} */


#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace std::chrono;

int main() {
    string texte = "TESTEUR";
    string recherche = "TEST";
    
    bool trouve = true;
    
    long long nb_find_appels = 0;          // nombre d'appels à find()
    long long nb_comparaisons = 0;         // comparaisons caractère par caractère
    long long nb_caracteres_parcourus = 0; // total de caractères examinés
    
    auto debut = high_resolution_clock::now();
    
    for (int i = 0; i < (int)recherche.length(); i++) {
        char c = recherche[i];
        nb_find_appels++;
        
        // On appelle find et on mesure
        size_t position = texte.find(c);
        nb_caracteres_parcourus += (position == string::npos) 
                                    ? texte.length() 
                                    : position + 1;
        nb_comparaisons += (position == string::npos) 
                            ? texte.length() 
                            : position + 1;
        
        if (position == string::npos) {
            trouve = false;
            break;
        }
    }
    
    // ===== Fin du chrono =====
    auto fin = high_resolution_clock::now();
    auto duree = duration_cast<nanoseconds>(fin - debut).count();
    
    // ===== Affichage du résultat =====
    if (trouve) {
        cout << "\"" << recherche << "\" existe dans \"" << texte << "\"" << endl;
    } else {
        cout << "\"" << recherche << "\" n'existe pas dans \"" << texte << "\"" << endl;
    }
    
    // ===== Affichage des mesures =====
    cout << "\n========== MESURES REELLES ==========" << endl;
    cout << "Temps d'execution : " << duree << " ns" << endl;
    cout << "                    " << fixed << setprecision(6) 
         << (duree / 1'000'000.0) << " ms" << endl;
    
    cout << "\n========== STATISTIQUES ==========" << endl;
    cout << "Longueur texte    (N) : " << texte.length() << endl;
    cout << "Longueur recherche(M) : " << recherche.length() << endl;
    cout << "Appels a find()       : " << nb_find_appels << endl;
    cout << "Caracteres examines   : " << nb_caracteres_parcourus << endl;
    
    cout << "\n========== COMPLEXITE ==========" << endl;
    cout << "Algorithme utilise : recherche naive (find)" << endl;
    cout << "Complexite pire cas  : O(N x M)" << endl;
    cout << "  N = " << texte.length() 
         << ", M = " << recherche.length()
         << "  ->  N x M = " << (texte.length() * recherche.length()) << endl;
    cout << "Complexite ici       : O(M x N) theorique, "
         << nb_caracteres_parcourus << " operations reelles" << endl;
    
    return 0;
}
