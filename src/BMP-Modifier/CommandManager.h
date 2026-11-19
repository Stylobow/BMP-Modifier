#pragma once

#define PREFIX '-'

#define SG_STYLE( style ) "\033[" style "m"
#define SG_STYLE_RESET      SG_STYLE( "0" )
#define STYLE_ERROR      SG_STYLE( "31" )
#define STYLE_SUCCESS      SG_STYLE( "42" )

#define HELP_END "\n" \
    "[]  Required Argument\n"

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
#define PRINT_SUCCESS( message ) { std::cout << STYLE_SUCCESS message "\n" SG_STYLE_RESET; return 1; }

class CommandManager
{
public:
    int Start(int argc, char* argv[]);

private:
    int Help(int argc, char* argv[]);
    int Clone(int argc, char* argv[]);
    int SwitchColor(int argc, char* argv[]);
    int Hide(int argc, char* argv[]);
    int Show(int argc, char* argv[]);
    int Resize(int argc, char* argv[]);
};