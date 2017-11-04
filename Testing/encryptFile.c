#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct filez {
	uint8_t *buffer;
	uint64_t size;
};
typedef struct filez Filez;

//Function prototypes
Filez readfile(uint8_t *);
void writefile(uint8_t *, uint8_t *, uint64_t);
void encryptDecrypt(uint8_t *, uint8_t *, uint64_t);

int main(int argc, uint8_t *argv[])
{
	if (argc != 3) {
		printf("Usage %s <filename>\n", argv[0]);
		return 1;
	}

	// read file into buffer
	Filez file = readfile(argv[1]);
	
	// encrypt file
	uint8_t encrypted[file.size];
	encryptDecrypt(file.buffer, encrypted, file.size);

	// write out to disk
	writefile(argv[2], encrypted, file.size);

	free(file.buffer);
	return 0;
}

void encryptDecrypt(uint8_t *input, uint8_t *output, uint64_t size) {
	uint8_t key[] = {'K', 'C', 'Q'}; //Can be any chars, and any size array
	
	for(int i = 0; i < size; i++) {
		output[i] = input[i] ^ key[i % (sizeof(key)/sizeof(uint8_t))];
	}
}

void writefile(uint8_t *name, uint8_t *content, uint64_t size) {
	FILE *fp = fopen(name, "wb");
	fwrite(content, 1, size, fp);
}

Filez readfile(uint8_t *name) {
	Filez file;
	// uint8_t *buffer;
	// get file pointer
	FILE *fp = fopen(name, "rb");

	// get file size
	fseek(fp, 0, SEEK_END);
	file.size = ftell(fp);
	printf("File length: %ld\n", file.size);
	rewind(fp);

	// initalize buffer with file size
	file.buffer = (uint8_t *)malloc((file.size+1)*sizeof(uint8_t));

	// read all characters into buffer
	fread(file.buffer, 1, file.size, fp);

	// close file
	fclose(fp);

	return file;
}
