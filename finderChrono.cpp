#include <iostream>
#include <string>
#include <chrono>      // pour mesurer le temps
using namespace std;
using namespace std::chrono;

int main() {
    // ============================================
    // 1. LES DONNEES
    // ============================================
    string texte    = "TESTEUR";   // longueur = 7
    string recherche = "TEST";     // longueur = 4
    
    // ============================================
    // 2. LES COMPTEURS (pour les calculs)
    // ============================================
    int nb_appels_find      = 0;   // combien de fois on appelle find()
    int nb_caracteres_vus   = 0;   // combien de caractères on regarde au total
    bool trouve = true;
    
    // ============================================
    // 3. DEMARRER LE CHRONOMETRE
    // ============================================
    auto debut = high_resolution_clock::now();
    
    // ============================================
    // 4. LA BOUCLE PRINCIPALE
    // ============================================
    for (int i = 0; i < (int)recherche.length(); i++) {
        char c = recherche[i];
        nb_appels_find++;
        
        cout << "Tour " << i+1 << " : on cherche le caractere '" << c << "'" << endl;
        
        size_t position = texte.find(c);
        
        // Compter combien de caracteres find a regarde
        int combien;
        if (position == string::npos) {
            combien = texte.length();     // il a tout regarde sans trouver
        } else {
            combien = position + 1;       // il s'est arrete a la position trouvee
        }
        nb_caracteres_vus += combien;
        
        cout << "   -> trouve a la position " << position 
             << " (find a regarde " << combien << " caracteres)" << endl;
        
        if (position == string::npos) {
            trouve = false;
            cout << "   -> PAS TROUVE, on arrete" << endl;
            break;
        }
    }
    
    // ============================================
    // 5. ARRETER LE CHRONOMETRE
    // ============================================
    auto fin = high_resolution_clock::now();
    auto duree_ns = duration_cast<nanoseconds>(fin - debut).count();
    
    // ============================================
    // 6. AFFICHER LE RESULTAT
    // ============================================
    cout << "\n--- RESULTAT ---" << endl;
    if (trouve) {
        cout << "\"" << recherche << "\" existe dans \"" << texte << "\"" << endl;
    } else {
        cout << "\"" << recherche << "\" n'existe pas dans \"" << texte << "\"" << endl;
    }
    
    // ============================================
    // 7. AFFICHER LES CALCULS DETAILLES
    // ============================================
    int N = texte.length();        // 7
    int M = recherche.length();    // 4
    int max_operations = N * M;    // 28
    
    cout << "\n--- DETAILS DES CALCULS ---" << endl;
    cout << "N = longueur du texte     = " << N << endl;
    cout << "M = longueur de recherche = " << M << endl;
    cout << "N x M (pire cas)          = " << max_operations << endl;
    
    cout << "\n--- CE QUI S'EST VRAIMENT PASSE ---" << endl;
    cout << "Appels a find()           = " << nb_appels_find << endl;
    cout << "Caracteres examines       = " << nb_caracteres_vus << endl;
    
    cout << "\n--- TEMPS D'EXECUTION ---" << endl;
    cout << "Temps = " << duree_ns << " nanosecondes" << endl;
    cout << "       = " << duree_ns / 1000.0 << " microsecondes" << endl;
    cout << "       = " << duree_ns / 1000000.0 << " millisecondes" << endl;
    
    cout << "\n--- COMPLEXITE ---" << endl;
    cout << "Pire cas  : O(N x M)  = O(" << N << " x " << M 
         << ") = O(" << max_operations << ")" << endl;
    cout << "Ici reel  : " << nb_caracteres_vus << " operations" << endl;
    
    return 0;
}