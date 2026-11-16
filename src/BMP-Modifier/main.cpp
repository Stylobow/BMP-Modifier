#include "pch.h"
#include "main.h"

/*
Liste d'arguments : 
    -help ( -h ) : Donne la liste de tout les arguments et comment les utiliser.
*/

int main(int argc, char* argv[])
// argc -> Nombre d'arguments
// argv -> List d'arguments

{
    if (argc < 2) {
        std::cout << "Aucun argument. Veuillez mettre un argument '-help' pour obtenir la liste des arguments." << std::endl;
        system("pause"); // Evite de fermer le programme instantanément
        return 0; // Si la liste d'arguments est vide, on ferme le programme (sécurité)
    }
    std::cout << "-=-=- Projet BMP-Modifier (x64 Debug Mode) -=-=-" << std::endl;
    std::cout << "Nombre d'arguments: (argc) -> " << argc << std::endl;

    if (std::string(argv[1]) == "-help" || std::string(argv[1]) == "-h") { // Si le premier argument est "-help" ou "-h"
        std::cout << "Argument help appele." << std::endl;
    }

    return 0;
}
