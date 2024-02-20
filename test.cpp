#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>
#include <limits>
#include <algorithm>
#include <map>

struct Node {
    std::string id;
    std::string text;
    std::shared_ptr<Node> choix1;
    std::shared_ptr<Node> choix2;

    Node(const std::string& id, const std::string& text) : id(id), text(text) {}
};

class JeuAventure {
private:
    std::map<std::string, std::shared_ptr<Node>> nodes;
    std::shared_ptr<Node> startNode;

public:
    void chargerDepuisFichier(const std::string& filename);
    void jouer();
    void afficherChoix(const std::shared_ptr<Node>& node);
    void faireChoix(std::shared_ptr<Node>& currentNode, int choix);
    bool finDeLaventure(const std::shared_ptr<Node>& node);
};

void JeuAventure::chargerDepuisFichier(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream linestream(line);
        std::string id, text, choix1ID, choix2ID;

        // Lire l'ID du nœud
        std::getline(linestream, id, ';');

        // Lire et traiter le texte du nœud
        std::getline(linestream, text, ';');

        // Lire les ID des choix
        std::getline(linestream, choix1ID, ';');
        std::getline(linestream, choix2ID, ';');

        // Créer le nœud et l'ajouter à la map
        auto node = std::make_shared<Node>(id, text);
        nodes[id] = node;

        // Lier les nœuds aux choix
        if (nodes.find(choix1ID) != nodes.end()) {
            node->choix1 = nodes[choix1ID];
        }
        if (nodes.find(choix2ID) != nodes.end()) {
            node->choix2 = nodes[choix2ID];
        }

        // Assurez-vous que cela correspond à l'ID du nœud de départ de votre histoire
        if (id == "01") {
            startNode = node;
        }
    }
}

void JeuAventure::jouer() {
    auto currentNode = startNode;

    while (currentNode) {
        std::cout << currentNode->text << std::endl;

        if (finDeLaventure(currentNode)) {
            // Si nous avons atteint la fin de l'histoire
            break;
        }

        afficherChoix(currentNode);

        int choix;
        std::cout << "Faites votre choix (1 ou 2) : ";
        std::cin >> choix;

        faireChoix(currentNode, choix);
    }

    std::cout << "Fin de l'aventure. Merci d'avoir joué !" << std::endl;
}


void JeuAventure::afficherChoix(const std::shared_ptr<Node>& node) {
    if (node->choix1 != nullptr) std::cout << "Choix 1: " << node->choix1->text << std::endl;
    if (node->choix2 != nullptr) std::cout << "Choix 2: " << node->choix2->text << std::endl;
}

void JeuAventure::faireChoix(std::shared_ptr<Node>& currentNode, int choix) {
    if (choix == 1 && currentNode->choix1 != nullptr) {
        currentNode = currentNode->choix1;
    } else if (choix == 2 && currentNode->choix2 != nullptr) {
        currentNode = currentNode->choix2;
    } else {
        std::cout << "Choix invalide. Veuillez reessayer." << std::endl;
        return; // Retourne immédiatement si le choix est invalide
    }
}

bool JeuAventure::finDeLaventure(const std::shared_ptr<Node>& node) {
    return node->choix1 == nullptr && node->choix2 == nullptr;
}



int main() {
    JeuAventure jeu;
    jeu.chargerDepuisFichier("LireLivre.csv");
    jeu.jouer();
    return 0;
}
