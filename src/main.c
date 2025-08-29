#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

#define bool int
#define true 1
#define false 0

#define uint64_t unsigned long long
#define int64_t long long
#define uint32_t unsigned int

#define DEFAULT_HEADER_LIST "test/test_header_list.txt"

typedef struct
{
	char* name;
} Function;

typedef struct Array_Function
{
    Function* arr;
    int len;
    int cap;
} Array_Function;

typedef struct Array_Char
{
    char* arr;
    int len;
    int cap;
} Array_Char;

uint64_t hash(char* s, uint32_t len)
{
    uint64_t h = 14695981039346656037ULL;
    for (uint32_t i = 0; i < len; i++)
    {
        h = (h ^ s[i]) * 1099511628211;
    }
    return h;
}

uint64_t hash_c_string(char* s)
{
    uint64_t h = 14695981039346656037ULL;
    while (*s)
    {
        h = (h ^ *s) * 1099511628211;
        s += 1;
    }
    return h;
}

bool LoadFile(const char* file_name, char** buffer, size_t* file_size)
{
	FILE* file = fopen(file_name, "rb");
	if (!file)
	{
		fprintf(stderr, "Could not open file: %s\n", file_name);
		return false;
	}

	fseek(file, 0L, SEEK_END);
	*file_size = ftell(file);
	rewind(file);

	*buffer = (char*)malloc(*file_size + 1);
	if (!*buffer) 
	{
		fprintf(stderr, "Memory allocation failed!\n");
		fclose(file);
		return false;
	}
	size_t bytes_read = fread(*buffer, sizeof(char), *file_size, file);
	if (bytes_read != *file_size) 
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

bool SaveFile(const char* file_name, const char* buffer, size_t file_size)
{
	FILE* file = fopen(file_name, "wb");
	if (!file)
	{
		fprintf(stderr, "Could not open file for writing: %s\n", file_name);
		return false;
	}

	size_t bytes_written = fwrite(buffer, sizeof(char), file_size, file);
	if (bytes_written != file_size)
	{
		fprintf(stderr, "Failed to write the entire buffer to file: %s\n", file_name);
		fclose(file);
		return false;
	}

	fclose(file);
	return true;
}

bool GetHeaderFileList(int argc, char **argv, char** header_file_list, size_t* file_size)
{
	if (argc > 2) 
    {
        fprintf(stderr, "Usage: %s <header_list>\n", argv[0]);
        return false;
    }
    if (argc == 2)
    {
        if (!LoadFile(argv[1], header_file_list, file_size)) 
		{
			return false;
		}
    }
    else
    {
        if (!LoadFile(DEFAULT_HEADER_LIST, header_file_list, file_size)) 
		{
			return false;
		}
    }
	return true;
}

bool ParseLines(char* source, char* lines[], int* line_count)
{
	char *scanner_start, *scanner_current = source;
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
			lines[(*line_count)++] = scanner_start;
		}

		while (*scanner_current == '\n' || *scanner_current == ' ' || *scanner_current == '\t' || *scanner_current == '\r')
		{
			*scanner_current = '\0';
			scanner_current++;
		}
	}
	return true;
}

bool ParseFunctions(char* header_file, Array_Function* functions)
{
	// DUMMY IMPLEMENTATION; TODO
	Array_Append(functions, (Function){ .name = "test" });
	return true;
}

int main(int argc, char **argv)
{
	char* header_file_list = NULL;
	size_t header_file_list_size;
	if (!GetHeaderFileList(argc, argv, &header_file_list, &header_file_list_size))
	{
		return -1;
	}

	printf("\nHeaders to parse:\n%s\n\n", header_file_list);

	char* headers[1024];
	int header_count = 0;

	if (!ParseLines(header_file_list, headers, &header_count))
	{
		free(header_file_list);
		return -1;
	}

	Array_Function functions = {0};
	Array_Initialize(&functions, 0);

	for (int i = 0; i < header_count; i++)
	{
		char* header_file = NULL;
		size_t file_size;
		LoadFile(headers[i], &header_file, &file_size);
		printf("`%s`\n%s\n\n", headers[i], header_file);

		if (!ParseFunctions(header_file, &functions))
		{
			fprintf(stderr, "Failed to parse functions from: %s\n", headers[i]);
		}

		free(header_file);
	}

	char* header_guard = "#ifndef HEADERSCRIPT_H\n#define HEADERSCRIPT_H\n\n";
	char* cpp_compatibility = "#ifdef __cplusplus\nextern \"C\" {\n#endif\n\n";

	Array_Char out = {0};
	Array_Initialize(&out, 16384);
	memset(out.arr, 0, out.cap);

	#define Append_fmt(fmt, ...) \
	do { \
		int needed = snprintf(NULL, 0, fmt, ##__VA_ARGS__); \
		if (needed < 0) \
		{ \
			fprintf(stderr, "snprintf encoding error!\n"); \
			break; \
		} \
		while (out.len + needed + 1 > out.cap) \
		{ \
			int new_cap = out.cap * 2; \
			if (new_cap < out.len + needed + 1) { \
				new_cap = out.len + needed + 1; \
			} \
			void* temp_ptr = realloc(out.arr, new_cap); \
			if (temp_ptr == NULL) \
			{ \
				fprintf(stderr, "Failed to reallocate array memory!\n"); \
				break;\
			} \
			out.arr = temp_ptr; \
			out.cap = new_cap; \
		} \
		int written = snprintf(out.arr + out.len, out.cap - out.len, fmt, ##__VA_ARGS__); \
		if (written > 0) out.len += written; \
	} while (0)

	Append_fmt("%s", header_guard);
	Append_fmt("%s", cpp_compatibility);

	for (int i = 0; i < header_count; i++)
	{
		Append_fmt("#include \"%s\"\n", headers[i]);
	}

	char* VM_Prototypes = "\nint HEADERSCRIPT_VM(char* input);\n";

	Append_fmt("%s", VM_Prototypes);

	char* header_footer = "\n#ifdef __cplusplus\n}\n#endif\n\n#endif // HEADERSCRIPT_H\n";
	Append_fmt("%s", header_footer);

	char* implementation_guard = "#ifdef HEADERSCRIPT_IMPLEMENTATION\n";
	Append_fmt("%s", implementation_guard);

	char* VM_Implementation = 
		"\nunsigned long long hash_c_string(char* s)\n{\n  unsigned long long h = 14695981039346656037ULL;\n  while (*s)\n  {\n    h = (h ^ *s) * 1099511628211; s += 1;\n  } return h;\n}\n\n"
		"\nint HEADERSCRIPT_VM(char* input)\n"
		"{\n"
		"  char *lines[1024];\n"
		"  int line_count = 0;\n"
		"  char *scanner_start, *scanner_current = input;\n"
		"  while (true)\n"
		"  {\n"
		"    scanner_start = scanner_current;\n"
		"    if (*scanner_current == '\\0') break;\n"
		"    while (*scanner_current != '\\0' && *scanner_current != '\\n' && *scanner_current != ' ' && *scanner_current != '\\t') scanner_current++;\n"
		"    if (scanner_current > scanner_start) lines[line_count++] = scanner_start;\n"
		"    while (*scanner_current == '\\n' || *scanner_current == ' ' || *scanner_current == '\\t' || *scanner_current == '\\r')\n"
		"    {\n"
		"      *scanner_current = '\\0';\n"
		"      scanner_current++;\n"
		"    }\n"
		"  }\n"
		"  for (int i = 0; i < line_count; i++)\n"
		"  {\n"
		"    switch(hash_c_string(lines[i]))\n"
		"    {\n";
		
	Append_fmt("%s", VM_Implementation);

	for (int i = 0; i < functions.len; i++)
	{
		Append_fmt("      case %llu: %s(); break;\n", hash_c_string(functions.arr[i].name), functions.arr[i].name);
	}
	
	char* implementation_footer = 
		"    }\n" // switch end
		"  }\n"   // for end
		"}\n"     // VM end
		"\n#endif // HEADERSCRIPT_IMPLEMENTATION\n";
	Append_fmt("%s", implementation_footer);

	#undef Append_fmt

	printf("Generated output:\n\n%s\n", out.arr);

	if (!SaveFile("test/headerscript.h", out.arr, out.len))
	{
		fprintf(stderr, "Failed to save output file!\n");
	}

	free(header_file_list);

	return 0;
}