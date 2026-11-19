#include "pch.h"
#include "main.h"
#include "CommandManager.h"

int main(int argc, char* argv[])
{
	CommandManager cm;
	return cm.Start(argc, argv);
}
