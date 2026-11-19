#include "pch.h"
#include "CommandManager.h"
#include "file.h"

int CommandManager::Help(int argc, char* argv[]) {
	if (argc >= 2) {
		std::cout << HELP_MESSAGE;
		return 0;
	}

	return 0;
}

int CommandManager::Clone(int argc, char* argv[]) {
	file fm;
	if (argc < 3) 
		PRINT_ERROR("File Directory is missing! Please use '" << PREFIX << "clone [File]'");

	const char* file_path = argv[2];

	return fm.HasClonedFile(file_path);
}

int CommandManager::SwitchColor(int argc, char* argv[]) {
	file fm;
	if (argc < 3)
		PRINT_ERROR("File Directory is missing! Please use '" << PREFIX << "switchcolor [File]'");

	const char* file_path = argv[2];

	return fm.HasSwitchedColors(file_path);
}

int CommandManager::Hide(int argc, char* argv[]) {
	file fm;
	if (argc < 3)
		PRINT_ERROR("File Directory is missing! Please use '" << PREFIX << "hide [File] [Message]'");
	if (argc < 4)
		PRINT_ERROR("Hidden Message is missing! Please use '" << PREFIX << "hide [File] [Message]'");

	const char* file_path = argv[2];
	const char* message = argv[3];

	std::cout << message << std::endl;

	return fm.IsHiddenMessage(file_path, message);
}

int CommandManager::Show(int argc, char* argv[]) {
	file fm;
	if (argc < 3)
		PRINT_ERROR("File Directory is missing! Please use '" << PREFIX << "show [File]'");

	const char* file_path = argv[2];

	return fm.ExtractMessage(file_path);
}

int CommandManager::Resize(int argc, char* argv[]) {
	file fm;
	if (argc < 3)
		PRINT_ERROR("File Directory is missing! Please use '" << PREFIX << "resize [File] [Width] [Height]'");
	if (argc < 4)
		PRINT_ERROR("Width value is missing! Please use '" << PREFIX << "resize [File] [Width] [Height]'");
	if (argc < 5)
		PRINT_ERROR("Height value is missing! Please use '" << PREFIX << "resize [File] [Width] [Height]'");

	const char* file_path = argv[2];
	int width = atoi(argv[3]);
	int height = atoi(argv[4]);

	return fm.Resize(file_path, width, height);
}

int CommandManager::Start(int argc, char* argv[]) {
	std::cout << "-=-=- Projet BMP-Modifier (x64 Debug Mode) -=-=-" << std::endl << std::endl;

	if (argc == 1) {
		PRINT_ERROR("Not enough arguments")
	}

	if (argv[1][0] != PREFIX) PRINT_ERROR("Commands starts with '" << PREFIX << "\'");

	std::string const& command = argv[1] + 1;

	if (command == "help" || command == "h") return Help(argc, argv); // -help
	if (command == "clone" || command == "c") return Clone(argc, argv); // -clone [File]
	if (command == "switchcolor" || command == "sc") return SwitchColor(argc, argv); // -switchcolor [File]
	if (command == "hide" || command == "hi") return Hide(argc, argv); // -hide [File] ["Message"]
	if (command == "show" || command == "s") return Show(argc, argv);
	if (command == "resize" || command == "r") return Resize(argc, argv);

	PRINT_ERROR(<< command << " is not a valid command")

	return 0;
}