#include "pch.h"
#include "main.h"
#include "file.h"

#define HELP_END "\n" \
    "[]  Required Argument"                                                                                        "\n"\
    "<>  Optional Argument"                                                                                        "\n"\
    "{}  Repeatable Argument"                                                                                      "\n"

#define HELP_MESSAGE \
    "Available Commands :"                                                                                         "\n"\
    "+-------------------------------------------------------------------------------------------------------------+\n"\
    "|                                                                                                             |\n"\
    "|  -help ( -h )                                                                                               |\n"\
    "|                                                                                                             |\n"\
    "|  -clone ( -c )                                                                                              |\n"\
    "+-------------------------------------------------------------------------------------------------------------+\n" HELP_END SG_STYLE_RESET

#define SG_STYLE( style ) "\033[" style "m"
#define SG_STYLE_RESET      SG_STYLE( "0" )
#define STYLE_ERROR      SG_STYLE( "31" )

#define PRINT_ERROR( message ) { std::cout << STYLE_ERROR message "\n" SG_STYLE_RESET; return 1; }

int Help(int argc, char* argv[]) {
    if (argc == 2) {
        std::cout << HELP_MESSAGE;
        return 0;
    }

    return 0;
}

int main(int argc, char* argv[])
{
// argc -> Nombre d'arguments
// argv -> List d'arguments
    file fm;

    const char prefix = '-'; // Prefix a utiliser pour les commandes

    std::cout << "-=-=- Projet BMP-Modifier (x64 Debug Mode) -=-=-" << std::endl << std::endl;

    if (argc == 1) PRINT_ERROR("Not enough arguments"); // Si aucun argument est donné.

    if (argv[1][0] != prefix) PRINT_ERROR("Commands starts with '" << prefix << "\'");

    std::string const& command = argv[1] + 1;

    if (command == "help" || command == "h") return Help(argc, argv);
    if (command == "clone" || command == "c") return 0; // Commande pas encore disponible

    return 0;
}
