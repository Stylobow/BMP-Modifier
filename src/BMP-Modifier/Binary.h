#pragma once

class Binary
{
public:
	bool HideMessage(BYTE* buffer, int size, const char* hiddenMessage);
	bool ExtractMessage(const char* dest);
	const char* GetBinaryMessage(const char* message);
};

