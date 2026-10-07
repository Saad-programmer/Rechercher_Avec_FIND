# 📚 README.md — Projet `Finder` : Recherche de motifs dans du texte et des PDF

![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Status](https://img.shields.io/badge/status-educational-orange.svg)

> **Collection de programmes C++ illustrant la recherche de caractères, de sous-chaînes, et de mots dans du texte brut ou des fichiers PDF, avec analyse fine de la complexité temporelle et mesures de performance réelles.**

---

## 📖 Table des matières

1. [Présentation](#-présentation)
2. [Structure du projet](#-structure-du-projet)
3. [Détail des fichiers](#-détail-des-fichiers)
   - [finder.cpp](#-findercpp)
   - [finderS.cpp](#-finderscpp)
   - [finderChrono.cpp](#-finderchronocpp)
   - [finderText.cpp](#-findertextcpp)
   - [finderPDF.cpp](#-finderpdfcpp)
   - [finderPdfDetails.cpp](#-finderpdfdetailscpp)
4. [Complexité temporelle — Comparaison](#-complexité-temporelle--comparaison)
5. [Compilation et exécution](#-compilation-et-exécution)
6. [Prérequis](#-prérequis)
7. [Concepts pédagogiques abordés](#-concepts-pédagogiques-abordés)
8. [Améliorations possibles](#-améliorations-possibles)
9. [Auteur et licence](#-auteur-et-licence)

---

## 🎯 Présentation

Ce dépôt regroupe **six programmes C++ indépendants** qui explorent, à des niveaux de plus en plus avancés, la problématique de **recherche de motifs (pattern matching)** dans du texte :

- Recherche de **caractères isolés** dans n'importe quel ordre
- Recherche de **sous-chaînes exactes et contiguës**
- **Remplacement** de sous-chaînes
- **Mesure du temps d'exécution** (chronomètre haute résolution)
- **Analyse de la complexité temporelle** (pire cas, meilleur cas, cas moyen)
- **Extraction de texte depuis un PDF** puis recherche avec **contexte** (page, paragraphe, ligne)

C'est un projet **pédagogique** idéal pour comprendre :
- La fonction `std::string::find`
- La mesure de performance en C++ (`std::chrono`)
- Les différences entre complexité théorique et comportement réel
- L'interaction C++ ↔ outils système (`pdftotext`, `system()`)

---

## 🗂 Structure du projet

```
.
├── Feuillet-pauvre.pdf         # Fichier PDF d'exemple (roman) utilisé pour les tests
├── finder.cpp                  # Démonstration de base de string::find
├── finderS.cpp                 # Recherche dans le désordre + mesures simples
├── finderChrono.cpp            # Recherche détaillée avec compteurs et chrono
├── finderText.cpp              # Recherche de mots dans un fichier texte
├── finderPDF.cpp               # Recherche dans un PDF converti en texte
├── finderPdfDetails.cpp        # Recherche avancée avec contexte (page, paragraphe)
│
├── finderChrono                # Binaire compilé (finderChrono)
├── finderPDF                   # Binaire compilé (finderPDF)
├── finderPdfDetails            # Binaire compilé (finderPdfDetails)
├── finderS                     # Binaire compilé (finderS)
└── finderText                  # Binaire compilé (finderText)
```

> ⚠️ Les binaires listés sont des **artefacts de compilation** ; ils ne devraient généralement pas être versionnés (ajoutez-les à `.gitignore`).

---

## 🔍 Détail des fichiers

### 📄 `finder.cpp`

**Rôle :** Démonstration pédagogique de la fonction `std::string::find` dans **trois scénarios distincts**.

| Fonction | Description | Complexité |
|---|---|---|
| `contientTousLesCaracteres(texte, recherche)` | Vérifie que **chaque caractère** de `recherche` existe dans `texte`, **peu importe l'ordre** | O(M × N) |
| `contientSousChaineExacte(texte, recherche)` | Vérifie que `recherche` apparaît comme **sous-chaîne contiguë** | O(N × M) pire cas |
| `remplacerSousChaine(texte, aRemplacer, remplacement)` | Remplace **toutes** les occurrences | O(N × M) |

**Exemple de sortie :**
```
========== 1. RECHERCHE DANS LE DESORDRE ==========
Texte 1: computer
Texte 2: cote
-> (Desordre) Toutes les lettres de "cote" sont dans "computer"

========== 2. RECHERCHE DE SOUS-CHAINE EXACTE ==========
-> (Exact) "cote" N'EST PAS une sous-chaine exacte de "computer"

========== 3. REMPLACEMENT DE SOUS-CHAINE ==========
Texte original : J'aime le langage C. Le C est un langage puissant.
Texte modifie  : J'aime le langage C++. Le C++ est un langage puissant.
```

**Points clés :**
- Utilisation de `string::npos` pour détecter l'absence
- Gestion du cas `aRemplacer.empty()` pour éviter une boucle infinie
- Recherche successive avec `find(aRemplacer, position + remplacement.length())`

**Intérêt pédagogique :** Montre la différence cruciale entre *« tous les caractères existent »* et *« la sous-chaîne exacte existe »*.

---

### 📄 `finderS.cpp`

**Rôle :** Version enrichie de la recherche « dans le désordre », avec **mesure du temps** et **statistiques d'opérations**.

**Fonctionnalités :**
- Comptage du **nombre d'appels à `find()`**
- Comptage des **caractères réellement parcourus** (basé sur la position retournée)
- Chronométrage en **nanosecondes** via `high_resolution_clock`
- Affichage de la **complexité théorique** vs **réelle**

**Extrait de sortie :**
```
"TEST" existe dans "TESTEUR"

========== MESURES REELLES ==========
Temps d'execution : 18500 ns
                    0.018500 ms

========== STATISTIQUES ==========
Longueur texte    (N) : 7
Longueur recherche(M) : 4
Appels a find()       : 4
Caracteres examines   : 16

========== COMPLEXITE ==========
Complexite pire cas  : O(N x M)  ->  N x M = 28
Complexite ici       : 16 operations reelles
```

**Apport par rapport à `finder.cpp` :** instrumentation complète pour comprendre où le temps est réellement passé.

---

### 📄 `finderChrono.cpp`

**Rôle :** Version **ultra-détaillée** de `finderS.cpp` avec affichage tour par tour de la recherche.

**Ce que le programme affiche :**
- À chaque itération, quel **caractère** est cherché
- La **position trouvée** et le **nombre de caractères examinés** par `find()`
- Le **temps total** en ns / µs / ms
- Le **pire cas théorique** (`N × M`) et le **nombre d'opérations réelles**

**Exemple de sortie :**
```
Tour 1 : on cherche le caractere 'T'
   -> trouve a la position 0 (find a regarde 1 caracteres)
Tour 2 : on cherche le caractere 'E'
   -> trouve a la position 1 (find a regarde 2 caracteres)
...
--- CE QUI S'EST VRAIMENT PASSE ---
Appels a find()           = 4
Caracteres examines       = 10

--- COMPLEXITE ---
Pire cas  : O(N x M)  = O(7 x 4) = O(28)
Ici reel  : 10 operations
```

**Intérêt pédagogique :** Visualisation concrète de la différence entre **complexité asymptotique** et **coût réel**.

---

### 📄 `finderText.cpp`

**Rôle :** Recherche d'un mot dans un **fichier texte** (`roman.txt`).

**Pipeline :**
1. Ouverture du fichier avec `fopen`
2. Lecture ligne par ligne via `fgets`
3. Stockage dans un `vector<string>`
4. Mesure du **temps de lecture** (I/O)
5. Recherche du mot saisi par l'utilisateur
6. Mesure du **temps de recherche** (CPU)
7. Calcul de la **complexité** : `O(C × M)` où `C` = total caractères

**Extrait de sortie :**
```
========== FICHIER CHARGE ==========
Nombre de lignes      : 1247
Total caracteres      : 58392
Temps de lecture      : 4210 microsecondes (4.21 ms)

Entrez le mot a rechercher : chevalier

========== RECHERCHE ==========
-> trouve a la ligne 342
-> contenu : "Le chevalier s'avanca vers la tour..."

========== COMPLEXITE ==========
Pire cas : O(C x M)  = O(58392 x 9) = O(525528)
Meilleur cas : O(M)  (trouve sur la 1ere ligne)
```

**Différence clé :** introduit la **lecture de fichier** et la distinction entre **temps I/O** et **temps CPU**.

---

### 📄 `finderPDF.cpp`

**Rôle :** Recherche d'un mot dans un **PDF** (`Feuillet-pauvre.pdf`).

**Pipeline :**
1. Conversion du PDF en texte via `pdftotext -layout` (appel système)
2. Lecture du fichier temporaire généré
3. Suppression du fichier temporaire (`remove`)
4. Recherche du mot
5. Mesure des temps de **lecture** et de **recherche**

**Extrait de code clé :**
```cpp
string commande = "pdftotext -layout Feuillet-pauvre.pdf roman_temp.txt";
int status = std::system(commande.c_str());
if (status != 0) { /* erreur : pdftotext absent */ }
```

**Complexité :** identique à `finderText.cpp` (`O(C × M)`), mais avec un **coût de prétraitement I/O** non négligeable (conversion PDF).

**Prérequis système :** `pdftotext` (paquet `poppler-utils`).

---

### 📄 `finderPdfDetails.cpp`

**Rôle :** **Version la plus avancée** — recherche avec **contexte structurel complet**.

**Nouveautés majeures :**

| Fonctionnalité | Description |
|---|---|
| **Struct `Occurrence`** | Stocke `page`, `paragraphe`, `ligneGlobale`, `contexte` |
| **Détection des pages** | Utilise le caractère `\f` (Form Feed) généré par `pdftotext` |
| **Détection des paragraphes** | Compte les lignes vides consécutives |
| **Mode de recherche** | `1` = première occurrence, `2` = toutes les occurrences |
| **Compteur d'opérations** | Simule le coût réel de `find()` sur chaque ligne |
| **Analyse comparative** | Compare pire cas théorique vs opérations réelles |

**Extrait de sortie :**
```
Choisissez le mode de recherche :
  1. Premiere occurrence uniquement (Sortie rapide)
  2. Toutes les occurrences du roman
Votre choix (1 ou 2) : 2

========== RECHERCHE EN COURS ==========
Mot recherche : "chevalier" (9 caracteres)
Mode choisi    : Toutes les occurrences

--- CONTEXTE DES OCCURRENCES TROUVEES ---

[Match #1] Page 12 | Paragraphe 3 (Ligne globale : 342)
   Contexte -> "Le chevalier s'avanca vers la tour..."

[Match #2] Page 47 | Paragraphe 1 (Ligne globale : 1203)
   Contexte -> "Un chevalier sans armure apparut..."

========== COMPARAISON DE COMPLEXITE ==========
Cas 2 (Toutes les occurrences) :
  • Complexite Strictement Fixe : O(C x M).
  • Explication : Le programme est oblige de scanner l'integralite du roman.
  • Efficacite constatée : Toujours egal au pire cas = O(525528) operations.
```

**Intérêt pédagogique :** Montre comment **enrichir une recherche** avec des **métadonnées structurelles** (pages, paragraphes) tout en conservant une analyse de complexité rigoureuse.

---

## 📊 Complexité temporelle — Comparaison

| Fichier | Algorithme | Pire cas | Meilleur cas | Espace |
|---|---|---|---|---|
| `finder.cpp` (caractères) | Recherche indépendante | **O(M × N)** | O(M) | O(1) |
| `finder.cpp` (sous-chaîne) | `find` naïf | **O(N × M)** | O(M) | O(1) |
| `finder.cpp` (remplacement) | `find` + `replace` itératif | **O(N × M × K)** | O(N) | O(N) |
| `finderS.cpp` | `find` par caractère | **O(M × N)** | O(M) | O(1) |
| `finderChrono.cpp` | `find` par caractère instrumenté | **O(M × N)** | O(M) | O(1) |
| `finderText.cpp` | `find` ligne par ligne | **O(C × M)** | O(M) | O(L) |
| `finderPDF.cpp` | `find` ligne par ligne | **O(C × M)** | O(M) | O(L) |
| `finderPdfDetails.cpp` | `find` avec contexte | **O(C × M)** | O(M) | O(L + occ) |

**Légende :**
- `N` = longueur du texte d'une ligne
- `M` = longueur du motif recherché
- `C` = nombre total de caractères du fichier
- `L` = nombre de lignes
- `K` = nombre d'occurrences remplacées

> 💡 **À noter :** Tous ces algorithmes utilisent la recherche **naïve**. Pour des performances supérieures, on utiliserait **KMP** (`O(N + M)`), **Boyer-Moore** (`O(N/M)` en pratique), ou **Rabin-Karp** (hachage).

---

## ⚙️ Compilation et exécution

### Compilation individuelle

```bash
# Programme de base
g++ -std=c++17 -O2 finder.cpp -o finder

# Version instrumentée simple
g++ -std=c++17 -O2 finderS.cpp -o finderS

# Version chronométrée détaillée
g++ -std=c++17 -O2 finderChrono.cpp -o finderChrono

# Recherche dans un fichier texte
g++ -std=c++17 -O2 finderText.cpp -o finderText

# Recherche dans un PDF
g++ -std=c++17 -O2 finderPDF.cpp -o finderPDF

# Version avancée avec contexte
g++ -std=c++17 -O2 finderPdfDetails.cpp -o finderPdfDetails
```

### Exécution

```bash
./finder
./finderS
./finderChrono
./finderText          # nécessite roman.txt
./finderPDF           # nécessite Feuillet-pauvre.pdf + pdftotext
./finderPdfDetails    # nécessite Feuillet-pauvre.pdf + pdftotext
```

---

## 📦 Prérequis

| Outil | Version minimale | Usage |
|---|---|---|
| **g++** | 7.0+ (C++17) | Compilation |
| **poppler-utils** | — | Fournit `pdftotext` pour les programmes PDF |
| **Fichier `roman.txt`** | — | Requis par `finderText.cpp` |
| **Fichier `Feuillet-pauvre.pdf`** | — | Requis par `finderPDF.cpp` et `finderPdfDetails.cpp` |

### Installation de `pdftotext`

```bash
# Debian / Ubuntu
sudo apt-get install poppler-utils

# Fedora / RHEL
sudo dnf install poppler-utils

# macOS (Homebrew)
brew install poppler

# Windows (via MSYS2)
pacman -S mingw-w64-x86_64-poppler
```

---

## 🎓 Concepts pédagogiques abordés

- ✅ Utilisation avancée de `std::string::find` et `std::string::replace`
- ✅ Différence entre **recherche désordonnée** et **sous-chaîne contiguë**
- ✅ Mesure de performance avec `std::chrono::high_resolution_clock`
- ✅ Conversion d'unités : ns → µs → ms → s
- ✅ Analyse de **complexité temporelle** (pire cas, meilleur cas)
- ✅ Comparaison **complexité théorique** vs **opérations réelles**
- ✅ Lecture de fichiers avec `FILE*` / `fgets` (C-style) et `std::vector`
- ✅ Appels système (`std::system`) et interaction avec des outils externes
- ✅ Parsing structurel : détection de pages (`\f`) et paragraphes (lignes vides)
- ✅ Séparation des coûts **I/O** vs **CPU**

---

## 🚀 Améliorations possibles

| Idée | Bénéfice |
|---|---|
| Implémenter **KMP** ou **Boyer-Moore** | Complexité `O(N + M)` au lieu de `O(N × M)` |
| Utiliser `std::ifstream` au lieu de `FILE*` | Code plus idiomatique C++ |
| Ajouter une **recherche insensible à la casse** | Robustesse |
| Support des **expressions régulières** (`std::regex`) | Recherche avancée |
| Ajouter des **tests unitaires** (Catch2, Google Test) | Fiabilité |
| Remplacer `system()` par une **API PDF** (Poppler, PDFium) | Pas de dépendance externe |
| Ajouter un **Makefile** ou **CMakeLists.txt** | Build automatisé |
| Mode **multi-fichiers** (recherche dans un dossier) | Utilité |
| **Parallélisation** avec OpenMP ou `std::thread` | Performance sur gros volumes |

---

## 👤 Auteur et licence

- **Auteur :** *Saad AIT YAHIA / Saad-programmer
- **Année :** 2026

> 📝 Ce projet est fourni à des fins **pédagogiques**. N'hésitez pas à forker, modifier et expérimenter !

---

## ⭐ Contribution

Les contributions sont les bienvenues ! Pour proposer une amélioration :

1. Forkez le dépôt
2. Créez une branche (`git checkout -b feature/amelioration`)
3. Committez vos changements (`git commit -m 'Ajout de KMP'`)
4. Pushez (`git push origin feature/amelioration`)
5. Ouvrez une **Pull Request**

---

## 📚 Ressources complémentaires

- [cppreference — `std::string::find`](https://en.cppreference.com/w/cpp/string/basic_string/find)
- [cppreference — `std::chrono`](https://en.cppreference.com/w/cpp/chrono)
- [Poppler / pdftotext](https://poppler.freedesktop.org/)
- [Algorithme de Knuth-Morris-Pratt](https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm)
- [Algorithme de Boyer-Moore](https://en.wikipedia.org/wiki/Boyer%E2%80%93Moore_string-search_algorithm)

---

<p align="center">
  <b>⭐ Si ce projet vous a aidé, n'oubliez pas de lui donner une étoile ! ⭐</b>
</p>
