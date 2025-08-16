#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	char* source_files = NULL;

    if (argc > 2) 
    {
        fprintf(stderr, "Usage: %s <header_list>\n", argv[0]);
        return -1;
    }
    if (argc == 2)
    {
        FILE* file = fopen(argv[1], "rb");
        if (!file) 
        {
            fprintf(stderr, "Could not open file: %s\n", argv[1]);
            return -1;
        }
    
        fseek(file, 0L, SEEK_END);
        size_t file_size = ftell(file);
        rewind(file);
    
        source_files = (char*)malloc(file_size + 1);
        if (!source_files) 
        {
            fprintf(stderr, "Memory allocation failed!\n");
            fclose(file);
            return -1;
        }
        size_t bytes_read = fread(source_files, sizeof(char), file_size, file);
        if (bytes_read != file_size) 
        {
            fprintf(stderr, "Failed to read the entire file: %s\n", argv[1]);
            free(source_files);
            fclose(file);
            return -1;
        }
        source_files[bytes_read] = '\0';
    
        fclose(file);
    }
    else
    {
        // source_files = "(if else for func null foobar 123.456 \"Hello, World!\")";
        source_files = "test.h";
    }

	return 0;
}