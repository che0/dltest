#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>

void print_help()
{
	printf(
		"dltest [-d] <library_to_check> ...\n"
		"dltest -h\n"
		"\n"
		"dltest will try to dlopen() all libs it gets on command line and\n"
		"will print errors when something fails. When run with -d, it also\n"
		"deletes libraries which fail to open.\n"
	);
}

int main(int argc, char ** argv)
{
	int delete_failed = 0;
	
	int opt;
	while ((opt = getopt(argc, argv, "hd")) != -1)
	{
		switch (opt)
		{
			case 'h':
				print_help();
				exit(EXIT_SUCCESS);
			break;
			
			case 'd':
				delete_failed = 1;
			break;
			
			default:
				fprintf(stderr, "Usage: %s [-d] library.so ...\n", argv[0]);
				exit(EXIT_FAILURE);
			break;
		}
	}
	
	int i;
	int final_ret = EXIT_SUCCESS;
	for (i = optind; i < argc; i++)
	{
		void * dl_handle;
		if (argv[i][0] == '/')
		{ // absolute pathname
			dl_handle = dlopen(argv[i], RTLD_NOW);
		}
		else
		{ // relative pathname, add "./" prefix
			char * prefixed_path;
			asprintf(&prefixed_path, "./%s", argv[i]);
			dl_handle = dlopen(prefixed_path, RTLD_NOW);
			free(prefixed_path);
		}
		
		if (dl_handle != NULL)
		{ // dlopen succeeded
			dlclose(dl_handle);
			continue;
		}
		
		// dlopen has failed
		fprintf(stderr, "%s\n", dlerror());
		if (delete_failed)
		{
			unlink(argv[i]);
		}
		final_ret = EXIT_FAILURE;
	}
	
	return final_ret;
}
