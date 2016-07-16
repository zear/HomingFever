#include "fileio.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "game.h"

char *configDir;

void getConfigDir()
{
	char *homeDir = getenv("HOME");

	if(homeDir != NULL)
	{
		configDir = (char *)malloc(strlen(homeDir) + strlen("/.homingFever") + 1);
		if(configDir != NULL)
		{
			sprintf(configDir, "%s/.homingFever", homeDir);
			mkdir(configDir, 0755); /* Create the directory if it doesn't exist. */
		}
	}
}

void getConfig()
{
	FILE *f;
	char *config;
	char line[15];

	if(!configDir)
	{
		printf("Config directory doesn't exist.\n");
		return;
	}

	config = (char *)malloc(strlen(configDir) + strlen("/game.cfg") + 1);

	if(!config)
	{
		return;
	}

	sprintf(config, "%s/game.cfg", configDir);

	f = fopen(config, "r");

	if(f == NULL)
	{
		printf("Failed to open config file: \"%s\" for reading.\n", config);
		free(config);
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
	free(config);
}

void storeConfig()
{
	FILE *f;
	char *config;

	if(!configDir)
	{
		printf("Config directory doesn't exist.\n");
		return;
	}

	config = (char *)malloc(strlen(configDir) + strlen("/game.cfg") + 1);

	if(!config)
	{
		return;
	}

	sprintf(config, "%s/game.cfg", configDir);

	f = fopen(config, "w");

	if(f == NULL)
	{
		printf("Failed to open config file: \"%s\" for writing.\n", config);
		free(config);
		return;
	}

/*	fprintf(f, "HI %d\nHI2 %d\nHI3 %d\nHI4 %d\nHI5 %d\n", hiscore[0], hiscore[1], hiscore[2], hiscore[3], hiscore[4]);*/

	fclose(f);
	free(config);
}


void getHiscore()
{
	FILE *f;
	char *save;
	char header[] = HISCORE_HEADER;
	uint8_t version;

	if(!configDir)
	{
		printf("Config directory doesn't exist.\n");
		return;
	}

	save = (char *)malloc(strlen(configDir) + strlen("/score.dat") + 1);

	if(!save)
	{
		return;
	}

	sprintf(save, "%s/score.dat", configDir);

	f = fopen(save, "rb");

	if(f == NULL)
	{
		printf("Failed to open score file: \"%s\" for writing.\n", save);
		free(save);
		return;
	}

	fread(&header, sizeof(char), strlen(header), f);

	if (strcmp(header, HISCORE_HEADER))
	{
		printf("File \"%s\" is not a valid score file.\n", save);
		fclose(f);
		free(save);
		return;
	}

	fread(&version, sizeof(uint8_t), 1, f);

	if (version != HISCORE_FORMAT_VERSION)
	{
		fclose(f);
		free(save);
		printf("Incompatible score file in version %d. Required version: %d.\n", version, SAVE_FORMAT_VERSION);
		return;
	}

	fread(&bestTime, sizeof(uint16_t), 1, f);

	fclose(f);
	free(save);
}

void storeHiscore()
{
	FILE *f;
	char *save;
	char header[] = HISCORE_HEADER;
	uint8_t version = HISCORE_FORMAT_VERSION;

	if(!configDir)
	{
		printf("Config directory doesn't exist.\n");
		return;
	}

	save = (char *)malloc(strlen(configDir) + strlen("/score.dat") + 1);

	if(!save)
	{
		return;
	}

	sprintf(save, "%s/score.dat", configDir);

	f = fopen(save, "wb");

	if(f == NULL)
	{
		printf("Failed to open score file: \"%s\" for writing.\n", save);
		free(save);
		return;
	}

	fwrite(&header, sizeof(char), strlen(header), f);
	fwrite(&version, sizeof(uint8_t), 1, f);
	fwrite(&bestTime, sizeof(uint16_t), 1, f);

	fclose(f);
	free(save);
}
