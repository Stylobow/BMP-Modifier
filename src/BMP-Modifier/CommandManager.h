#pragma once

#define PREFIX '-'

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