#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>

int main(int argc, char ** argv)
{
	int i;
	int final_ret = EXIT_SUCCESS;
	
	for (i = 1; i < argc; i++)
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
		final_ret = EXIT_FAILURE;
	}
	
	return final_ret;
}
