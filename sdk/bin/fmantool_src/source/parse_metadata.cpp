#include "common.h"

void MetadataParser::ParseBlob(char* blob)
{
	char* pos = blob;
	char* endPos;
	for (;;)
	{
		endPos = strchr(pos, '\r');
		if (endPos && endPos[1] == '\n')
			*endPos++ = 0;
		else
			endPos = strchr(pos, '\n');

		if (endPos) *endPos = 0;
		while (isspace(*pos)) pos ++;

		do
		{
			if (!*pos)
				break;

			if (*pos == ';' || *pos == '#' || *pos == ':') // comment or syntax error
				break;

			char* valuePos = strchr(pos, ':');
			if (!valuePos)
				break;

			*valuePos = 0;
			for (char* prevPos = valuePos - 1; isspace(*prevPos); prevPos --)
				*prevPos = 0;

			for (valuePos++; isspace(*valuePos); valuePos++)
				*valuePos = 0;

			for (char* prevPos = valuePos + strlen(valuePos) - 1; prevPos > valuePos && isspace(*prevPos); prevPos--)
				*prevPos = 0;

			mw.AddEntry(pos, valuePos);
		} while(0);

		if (!endPos)
			break;

		pos = endPos + 1;
	}

	mw.Preprocess();
}
