#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool int
#define true 1
#define false 0

bool LoadFile(const char* file_name, char** buffer)
{
	FILE* file = fopen(file_name, "rb");
	if (!file)
	{
		fprintf(stderr, "Could not open file: %s\n", file_name);
		return false;
	}

	fseek(file, 0L, SEEK_END);
	size_t file_size = ftell(file);
	rewind(file);

	*buffer = (char*)malloc(file_size + 1);
	if (!*buffer) 
	{
		fprintf(stderr, "Memory allocation failed!\n");
		fclose(file);
		return false;
	}
	size_t bytes_read = fread(*buffer, sizeof(char), file_size, file);
	if (bytes_read != file_size) 
	{
		fprintf(stderr, "Failed to read the entire file: %s\n", file_name);
		free(*buffer);
		fclose(file);
		return false;
	}
	(*buffer)[bytes_read] = '\0';

	fclose(file);
	return true;
}

bool GetSourceFileList(int argc, char **argv, char** source_files)
{
	if (argc > 2) 
    {
        fprintf(stderr, "Usage: %s <header_list>\n", argv[0]);
        return false;
    }
    if (argc == 2)
    {
        if (!LoadFile(argv[1], source_files)) 
		{
			return false;
		}
    }
    else
    {
        const char* default_file = "../test/test.h";
        *source_files = malloc(strlen(default_file) + 1);
        if (!*source_files) {
            fprintf(stderr, "Memory allocation failed!\n");
            return false;
        }
        strcpy(*source_files, default_file);
    }
	return true;
}

int main(int argc, char **argv)
{
	char* source_files = NULL;
	if (!GetSourceFileList(argc, argv, &source_files))
	{
		return -1;
	}

	printf("Source files: %s\n", source_files);

	char* header_file = NULL;
	LoadFile(source_files, &header_file);

	printf("Header file content:\n%s\n", header_file);

	free(source_files);
	free(header_file);

	return 0;
}