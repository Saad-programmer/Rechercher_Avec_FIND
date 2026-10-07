/*
 Un programme utilisant la fonction find de la classe string :
 1. Pour vérifier si des caractères individuels existent (même dans le désordre)
 2. Pour rechercher une sous-chaîne exacte et contiguë
 3. Pour remplacer une sous-chaîne par une autre
*/

#include <iostream>
#include <string>
using namespace std;

// vérifie si TOUS les caractères individuels existent 
bool contientTousLesCaracteres(string texte, string recherche) {
    for (size_t i = 0; i < recherche.length(); i++) {
        char c = recherche[i];
        size_t position = texte.find(c);
        if (position == string::npos) {
            return false;
        }
    }
    return true;
}

// cherche une sous-chaîne EXACTE et contigue
bool contientSousChaineExacte(string texte, string recherche) {
    // find cherche ici la séquence complète et ordonnée "recherche" dans "texte"
    size_t position = texte.find(recherche);
    if (position != string::npos) {
        return true;
    }
    return false;
}

// remplace TOUTES les occurrences d'une sous-chaîne par une autre
string remplacerSousChaine(string texte, string aRemplacer, string remplacement) {
    if (aRemplacer.empty()) return texte; // Évite une boucle infinie si la chaîne à chercher est vide
    
    size_t position = texte.find(aRemplacer);
    
    while (position != string::npos) {
        texte.replace(position, aRemplacer.length(), remplacement);
        
        position = texte.find(aRemplacer, position + remplacement.length());
    }
    return texte;
}

int main() {
    // Recherche de caractères dans le desordre
    cout << "========== 1. RECHERCHE DANS LE DESORDRE ==========" << endl;
    string texte1 = "computer";
    string texte2 = "cote";
    
    cout << "Texte 1: " << texte1 << "\nTexte 2: " << texte2 << endl;
    if (contientTousLesCaracteres(texte1, texte2)) {
        cout << "-> (Desordre) Toutes les lettres de \"" << texte2 << "\" sont dans \"" << texte1 << "\"" << endl;
    }
    
    //  Recherche de sous-chaîne exacte
    cout << "\n========== 2. RECHERCHE DE SOUS-CHAINE EXACTE ==========" << endl;
    if (contientSousChaineExacte(texte1, texte2)) {
        cout << "-> (Exact) \"" << texte2 << "\" forme une sous-chaine de \"" << texte1 << "\"" << endl;
    } else {
        cout << "-> (Exact) \"" << texte2 << "\" N'EST PAS une sous-chaine exacte de \"" << texte1 << "\" (les lettres ne se suivent pas)." << endl;
    }

    // la sous-chaîne exacte existe
    string phrase = "Programmer en langage C++ est formidable.";
    string motCherche = "langage";
    cout << "\nPhrase : " << phrase << endl;
    if (contientSousChaineExacte(phrase, motCherche)) {
        cout << "-> \"" << motCherche << "\" existe bien en tant que sous-chaine exacte." << endl;
    }

    // Remplacement de sous-chaîne 
    cout << "\n========== 3. REMPLACEMENT DE SOUS-CHAINE ==========" << endl;
    string texteOriginal = "J'aime le langage C. Le C est un langage puissant.";
    string cible = "C";
    string nouveau = "C++";
    
    cout << "Texte original : " << texteOriginal << endl;
    cout << "Remplacer \"" << cible << "\" par \"" << nouveau << "\"" << endl;
    
    string texteModifie = remplacerSousChaine(texteOriginal, cible, nouveau);
    cout << "Texte modifie  : " << texteModifie << endl;
    
    return 0;
}
