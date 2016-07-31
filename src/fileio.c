#include "fileio.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if defined(HOME_DIR)
#include <sys/stat.h>
#endif
#include "game.h"

char configDir[FILE_MAX_PATH];

int getConfigDir()
{
#if defined(HOME_DIR)
	char *homeDir = getenv("HOME");

	if (!homeDir)
		return -1;

	if (snprintf(configDir, FILE_MAX_PATH, "%s/.homingFever", homeDir) >= FILE_MAX_PATH)
		return -1;

	mkdir(configDir, 0755); /* Create the directory if it doesn't exist. */
#else
	strcpy(configDir, ".");
#endif
	return 0;
}

void getConfig()
{
	FILE *f;
	char config[FILE_MAX_PATH];
	char line[15];

	if (snprintf(config, FILE_MAX_PATH, "%s/game.cfg", configDir) >= FILE_MAX_PATH)
	{
		printf("Failed to retrieve config file path.\n");
		return;
	}

	f = fopen(config, "r");

	if(f == NULL)
	{
		printf("Failed to open config file: \"%s\" for reading.\n", config);
		return;
	}

	while(fgets(line, sizeof(line), f))
	{
		char *arg = strchr(line, ' ');

		if(!arg)
		{
			continue;
		}
		*arg = '\0';
		arg++;

/*		if(!strcmp(line, "HI"))*/
/*		{*/
/*			sscanf(arg, "%d", &hiscore[0]);*/
/*		}*/
/*		else if(!strcmp(line, "HI2"))*/
/*		{*/
/*			sscanf(arg, "%d", &hiscore[1]);*/
/*		}*/
/*		else if(!strcmp(line, "HI3"))*/
/*		{*/
/*			sscanf(arg, "%d", &hiscore[2]);*/
/*		}*/
/*		else if(!strcmp(line, "HI4"))*/
/*		{*/
/*			sscanf(arg, "%d", &hiscore[3]);*/
/*		}*/
/*		else if(!strcmp(line, "HI5"))*/
/*		{*/
/*			sscanf(arg, "%d", &hiscore[4]);*/
/*		}*/
	}

	fclose(f);
}

void storeConfig()
{
	FILE *f;
	char config[FILE_MAX_PATH];

	if (snprintf(config, FILE_MAX_PATH, "%s/game.cfg", configDir) >= FILE_MAX_PATH)
	{
		printf("Failed to retrieve config file path.\n");
		return;
	}

	f = fopen(config, "w");

	if(f == NULL)
	{
		printf("Failed to open config file: \"%s\" for writing.\n", config);
		return;
	}

/*	fprintf(f, "HI %d\nHI2 %d\nHI3 %d\nHI4 %d\nHI5 %d\n", hiscore[0], hiscore[1], hiscore[2], hiscore[3], hiscore[4]);*/

	fclose(f);
}


void getHiscore()
{
	FILE *f;
	char save[FILE_MAX_PATH];
	char header[] = HISCORE_HEADER;
	uint8_t version;

	if (snprintf(save, FILE_MAX_PATH, "%s/score.dat", configDir) >= FILE_MAX_PATH)
	{
		printf("Failed to retrieve save file path.\n");
		return;
	}

	f = fopen(save, "rb");

	if(f == NULL)
	{
		printf("Failed to open score file: \"%s\" for writing.\n", save);
		return;
	}

	fread(&header, sizeof(char), strlen(header), f);

	if (strcmp(header, HISCORE_HEADER))
	{
		printf("File \"%s\" is not a valid score file.\n", save);
		fclose(f);
		return;
	}

	fread(&version, sizeof(uint8_t), 1, f);

	if (version != HISCORE_FORMAT_VERSION)
	{
		fclose(f);
		printf("Incompatible score file in version %d. Required version: %d.\n", version, SAVE_FORMAT_VERSION);
		return;
	}

	fread(&bestTime, sizeof(uint16_t), 1, f);

	fclose(f);
}

void storeHiscore()
{
	FILE *f;
	char save[FILE_MAX_PATH];
	char header[] = HISCORE_HEADER;
	uint8_t version = HISCORE_FORMAT_VERSION;

	if (snprintf(save, FILE_MAX_PATH, "%s/score.dat", configDir) >= FILE_MAX_PATH)
	{
		printf("Failed to retrieve save file path.\n");
		return;
	}

	f = fopen(save, "wb");

	if(f == NULL)
	{
		printf("Failed to open score file: \"%s\" for writing.\n", save);
		return;
	}

	fwrite(&header, sizeof(char), strlen(header), f);
	fwrite(&version, sizeof(uint8_t), 1, f);
	fwrite(&bestTime, sizeof(uint16_t), 1, f);

	fclose(f);
}
