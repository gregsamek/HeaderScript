#include <stdio.h>
#include <stdlib.h>

#define bool int
#define true 1
#define false 0

bool GetSourceFileList(int argc, char **argv, char* source_files)
{
	if (argc > 2) 
    {
        fprintf(stderr, "Usage: %s <header_list>\n", argv[0]);
        return false;
    }
    if (argc == 2)
    {
        FILE* file = fopen(argv[1], "rb");
        if (!file) 
        {
            fprintf(stderr, "Could not open file: %s\n", argv[1]);
            return false;
        }
    
        fseek(file, 0L, SEEK_END);
        size_t file_size = ftell(file);
        rewind(file);
    
        source_files = (char*)malloc(file_size + 1);
        if (!source_files) 
        {
            fprintf(stderr, "Memory allocation failed!\n");
            fclose(file);
            return false;
        }
        size_t bytes_read = fread(source_files, sizeof(char), file_size, file);
        if (bytes_read != file_size) 
        {
            fprintf(stderr, "Failed to read the entire file: %s\n", argv[1]);
            free(source_files);
            fclose(file);
            return false;
        }
        source_files[bytes_read] = '\0';
    
        fclose(file);
    }
    else
    {
        source_files = "test.h";
    }
	return true;
}

int main(int argc, char **argv)
{
	char* source_files = NULL;
	if (!GetSourceFileList(argc, argv, source_files))
	{
		return 1;
	}

	printf("Source files: %s\n", source_files);

	return 0;
}