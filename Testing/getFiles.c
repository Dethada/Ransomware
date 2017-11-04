#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ftw.h>
#include <sys/stat.h>
#include <string.h>

int list(const char *name, const struct stat *status, int type);

char** files;

char **add_string(char **array, const char *string) {

    int lastIndex = 0;
    while (array[lastIndex] != '\0')
        lastIndex++;

    /*reallocate so the array of strings can hold one more string; note that lastIndex is zero-based; 
    hence, the size of the current array is lastIndex+1, and consequently the size of the new array 
    needs to be lastIndex+2 */
    char** newArray = (char**)realloc(array, (lastIndex+2) * sizeof (char*));
    if (!newArray) return array;

    /*allocate memory for new string*/
    char* newString = strdup(string);
    if (!newString) return newArray;

    newArray[lastIndex++] = newString;
    newArray[lastIndex] = '\0';

    return newArray;
}

void printArray (char** array) {
    char* str; int i=0;
    while ((str = array[i]) != '\0') {
        printf("%d:%s\n", i, str);
        i++;
    }
}

int list(const char *name, const struct stat *status, int type) {
	if(type == FTW_NS)
		return 0;

	if(type == FTW_F) {
		files = add_string(files, name);
		// printf("%s\n", name);
	}

	return 0;
}

// FTW_F    The object is a  file
// FTW_D    ,,    ,,   ,, ,, directory
// FTW_DNR  ,,    ,,   ,, ,, directory that could not be read
// FTW_SL   ,,    ,,   ,, ,, symbolic link
// FTW_NS   The object is NOT a symbolic link and is one for 
//          which stat() could not be executed

int main(int argc, char *argv[]) {
	files = (char**) malloc(1 * sizeof(char*));
	files[0] = '\0';

	if(argc == 1)
		ftw(".", list, 1);
	else
		ftw(argv[1], list, 1);

    printArray(files);
    return 0;
}