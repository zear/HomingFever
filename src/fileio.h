#ifndef _FILEIO_H_
#define _FILEIO_H_


#define SAVE_FORMAT_VERSION	1
#define HISCORE_HEADER		"FEVER"
#define HISCORE_FORMAT_VERSION	1

extern char *configDir;

void getConfigDir();
void getConfig();
void storeConfig();
void getHiscore();
void storeHiscore();

#endif /* _FILEIO_H_ */
