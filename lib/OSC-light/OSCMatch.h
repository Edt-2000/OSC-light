#pragma once

//
// Matching algorithm for OSC messages.
//
// OSC-light only supports a subset of the OSC specification concerning address pattern matching
// This is to reduce the very memory and cycle intensive string matching
//
// ON ESP-8266 the ?-wildcard pattern is not supported.
//
//	Supported pattens:
//
//	Pattern						Matches addresses
//
//	/Unit1						/Unit1
//
//	/Unit2/Preset				/Unit2/Preset
//
//	/Unit3/Action/Something		/Unit3/Action/Something
//
//	/Unit?/Preset				/Unit1/Preset
//								/Unit2/Preset
//								/Unit3/Preset
//
// /Unit1						/Unit1/Preset
//								/Unit1/Preset/a
//
//	/*							/Unit1
//								/Unit2
//								/Unit3
//								/Other
//
//	/*/Preset					/Unit1/Preset
//								/Unit2/Preset
//								/Unit3/Preset
//
//	/*/Preset/a					/Unit1/Preset/a
//								/Unit2/Preset/a
//								/Unit3/Preset/a
//
//	/*/Preset/*					/Unit1/Preset/a
//								/Unit1/Preset/b
//								/Unit2/Preset/a
//								/Unit2/Preset/b
//								/Unit3/Preset/a
//								/Unit3/Preset/b
//
//	Algorithm
//
//	1. Full match
//	2. Contains double slash, asterisk or question mark?
//		1. Validate most left section, strip part and restart
//		2. Go until pattern is empty.
//
//

#include <string.h>

#define OSCdevider "/"
#define OSCwildcardFullMatch "*"
#define OSCwildcardSingleMatch "?"

namespace OSC
{
class Match
{
private:
	inline bool isWildcardMatch(const char *address, const char *pattern)
	{
		const int patternLength = strlen(pattern) + 1;

		char replacedAddressBuffer[patternLength];

		strcpy(replacedAddressBuffer, address);

		int i = 0;

		// crawling throught the pattern might not be the fastest way of doing this
		for (i = 0; i < patternLength; ++i)
		{
			if (strncmp(pattern + i, OSCwildcardSingleMatch, 1) == 0)
			{
				strncpy(replacedAddressBuffer + i, OSCwildcardSingleMatch, 1);
			}
		}

		return strncmp(replacedAddressBuffer, pattern, patternLength) == 0;
	}

public:
	bool isMatch(const char *address, const char *pattern, const int addressOffset = 0, const int patternOffset = 0)
	{
		if (strcmp(address, pattern) == 0)
		{
			return true;
		}
		else
		{
			const int addressLength = strlen(address) + 1;

			char patternBuffer[addressLength];
			char addressBuffer[addressLength];

			strcpy(patternBuffer, pattern + patternOffset);
			strcpy(addressBuffer, address + addressOffset);

			if (addressBuffer != NULL)
			{
				auto patternPart = strtok(patternBuffer, OSCdevider);
				auto addressPart = strtok(addressBuffer, OSCdevider);

				if (patternPart != NULL)
				{
					bool continueMatching = false;

					// address pattern compare
					// full match
					if (strcmp(addressPart, patternPart) == 0)
					{
						continueMatching = true;
					}
					// asterisk
					else if (strcmp(patternPart, OSCwildcardFullMatch) == 0)
					{
						continueMatching = true;
					}
					// wildcard character
					else if (strcspn(patternPart, OSCwildcardSingleMatch) < strlen(patternPart))
					{
						continueMatching = isWildcardMatch(addressPart, patternPart);
					}

					if (continueMatching)
					{
						bool patternCanContinue = (strlen(pattern) > patternOffset + strlen(patternPart) + 1);
						bool addressCanContinue = (strlen(address) > addressOffset + strlen(addressPart) + 1);

						if (addressCanContinue && patternCanContinue)
						{
							return isMatch(address, pattern, addressOffset + strlen(addressPart) + 1, patternOffset + strlen(patternPart) + 1);
						}
						else if (addressCanContinue && !patternCanContinue)
						{
							return true;
						}
						else
						{
							return (!addressCanContinue && !patternCanContinue);
						}
					}
				}
			}
		}

		return false;
	}
};
}; // namespace OSC