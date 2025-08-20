#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool int
#define true 1
#define false 0

#define DEFAULT_HEADER_LIST "test/test.txt"

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

bool GetHeaderFileList(int argc, char **argv, char** header_file_list)
{
	if (argc > 2) 
    {
        fprintf(stderr, "Usage: %s <header_list>\n", argv[0]);
        return false;
    }
    if (argc == 2)
    {
        if (!LoadFile(argv[1], header_file_list)) 
		{
			return false;
		}
    }
    else
    {
        if (!LoadFile(DEFAULT_HEADER_LIST, header_file_list)) 
		{
			return false;
		}
    }
	return true;
}

bool ParseHeaderFileList(char* header_file_list, char* headers[], int* header_count)
{
	char *scanner_start, *scanner_current = header_file_list;
	while (true)
	{
		scanner_start = scanner_current;
		if (*scanner_current == '\0')
		{
			break;
		}

		while (*scanner_current != '\0' && *scanner_current != '\n' && *scanner_current != ' ' && *scanner_current != '\t')
		{
			scanner_current++;
		}

		if (scanner_current > scanner_start)
		{
			headers[(*header_count)++] = scanner_start;
		}

		while (*scanner_current == '\n' || *scanner_current == ' ' || *scanner_current == '\t' || *scanner_current == '\r')
		{
			*scanner_current = '\0';
			scanner_current++;
		}
	}
	return true;
}

int main(int argc, char **argv)
{
	char* header_file_list = NULL;
	if (!GetHeaderFileList(argc, argv, &header_file_list))
	{
		return -1;
	}

	printf("\nHeaders to parse:\n%s\n\n", header_file_list);

	char* headers[1024];
	int header_count = 0;

	if (!ParseHeaderFileList(header_file_list, headers, &header_count))
	{
		free(header_file_list);
		return -1;
	}

	for (int i = 0; i < header_count; i++)
	{
		char* header_file = NULL;
		LoadFile(headers[i], &header_file);

		printf("`%s`\n%s\n\n", headers[i], header_file);
		free(header_file);
	}

	free(header_file_list);

	return 0;
}