# NSI 1ère - Project 1

## La Logique de `CRUDS` en utilisant C++, dans un fichier de `.json`

### Si vous voulez avoir tous les informations de ce project Taper [ICI](https://netricc.github.io/nsi-1-project1/)

### Explication du Makefile

Ce projet utilise un **Makefile** pour automatiser la compilation et le nettoyage des fichiers. Voici une explication de chaque partie :

---

#### Variables

* **CXX = g++**
  Définit le compilateur utilisé (ici `g++`).

* **CXXFLAGS = -Wall -Wextra -std=c++17**
  Options passées au compilateur :

  * `-Wall` : active les avertissements de compilation les plus courants.
  * `-Wextra` : active des avertissements supplémentaires.
  * `-std=c++17` : utilise le standard C++17.

* **TARGET = main**
  Nom de l’exécutable final.

* **SRCS = main.cpp**
  Liste des fichiers sources du projet.

* **OBJS = $(SRCS:.cpp=.o)**
  Règle de substitution qui transforme chaque fichier `.cpp` en son équivalent `.o` (fichier objet).

---

#### Règles de compilation

* **all: $(TARGET)**
  La règle par défaut. Elle construit le projet en appelant la cible `$(TARGET)`.

* **$(TARGET): $(OBJS)**
  Lie les fichiers objets pour produire l’exécutable.

  ```
  g++ -Wall -Wextra -std=c++17 -o main main.o
  ```

* **%.o: %.cpp**
  Règle générique qui compile chaque fichier source `.cpp` en un fichier objet `.o`.

  ```
  g++ -Wall -Wextra -std=c++17 -c fichier.cpp -o fichier.o
  ```

---

#### Nettoyage

* **clean:**
  Supprime les fichiers générés (exécutable et fichiers objets).

  ```
  rm -f *.o main
  ```

---

#### Cibles factices (Phony)

* **.PHONY: all clean**
  Indique que `all` et `clean` ne sont pas de vrais fichiers mais seulement des règles. Cela évite des conflits si un fichier portant le même nom existe.

---

### Utilisation

* **`make`** : compile le projet et génère l’exécutable `main`.
* **`make clean`** : supprime les fichiers objets et l’exécutable.
