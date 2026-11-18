#include "pch.h"
#include "main.h"
#include "file.h"

#define SG_STYLE( style ) "\033[" style "m"
#define SG_STYLE_RESET      SG_STYLE( "0" )
#define STYLE_ERROR      SG_STYLE( "31" )

#define HELP_END "\n" \
    "[]  Required Argument"                                                                                        "\n"\
    "<>  Optional Argument"                                                                                        "\n"\
    "{}  Repeatable Argument"                                                                                      "\n"

#define HELP_MESSAGE \
    "Available Commands :"                                                                                         "\n"\
    "+-------------------------------------------------------------------------------------------------------------+\n"\
    "|                                                                                                             |\n"\
    "|  -help                                   | Alias : h                                                        |\n"\
    "|  -clone [File]                           | Alias : c                                                        |\n"\
    "|  -switchcolor [File]                     | Alias : sc                                                       |\n"\
    "|                                                                                                             |\n"\
    "|  -hide [File] [\"Message\"]                | Alias : hi                                                        |\n"\
    "|  -show [File]                            | Alias : s                                                        |\n"\
	"|  -resize [File] [Width] [Height]         | Alias : r                                                        |\n"\
    "|                                                                                                             |\n"\
    "+-------------------------------------------------------------------------------------------------------------+\n" HELP_END

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

	if (argc == 1) {
		PRINT_ERROR("Not enough arguments")
	}

	if (argv[1][0] != prefix) PRINT_ERROR("Commands starts with '" << prefix << "\'");

	std::string const& command = argv[1] + 1;

	if (command == "help" || command == "h") return Help(argc, argv); // -help
	if (command == "clone" || command == "c") { // -clone [File]
		if (argc < 3)
			PRINT_ERROR("File Directory is missing! Please use '" << prefix << "clone [File]'");

		const char* file_path = argv[2];

		return fm.HasClonedFile(file_path);
	}
	if (command == "switchcolor" || command == "sc") { // -switchcolor [File]
		if (argc < 3)
			PRINT_ERROR("File Directory is missing! Please use '" << prefix << "switchcolor [File]'");

		const char* file_path = argv[2];

		return fm.HasSwitchedColors(file_path);
	}
	if (command == "hide" || command == "hi") { // -hide [File] ["Message"]
		if (argc < 3)
			PRINT_ERROR("File Directory is missing! Please use '" << prefix << "hide [File] [Message]'");
		if (argc < 4)
			PRINT_ERROR("Hidden Message is missing! Please use '" << prefix << "hide [File] [Message]'");

		const char* file_path = argv[2];
		const char* message = argv[3];

		std::cout << message << std::endl;

		return fm.IsHiddenMessage(file_path, message);
	}
	if (command == "resize" || command == "r") {
		if (argc < 3)
			PRINT_ERROR("File Directory is missing! Please use '" << prefix << "resize [File] [Width] [Height]'");
		if (argc < 4)
			PRINT_ERROR("Width value is missing! Please use '" << prefix << "resize [File] [Width] [Height]'");
		if (argc < 5)
			PRINT_ERROR("Height value is missing! Please use '" << prefix << "resize [File] [Width] [Height]'");

		const char* file_path = argv[2];
		int width = atoi(argv[3]);
		int height = atoi(argv[4]);

		return fm.Resize(file_path, width, height);
	}

	PRINT_ERROR(<< command << " is not a valid command");

	return 0;
}
