/*
File: saioc.c
Version: 1.1.0
Copyright (c) Arijit Kumar Das <arijitkdgit.official@gmail.com>
This program is licensed under the MIT License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMBUFFER    1024


void clearBuffer (char* buffer)
{
	unsigned int buflen = strlen(buffer);
	for (unsigned int i=0; i<buflen; i++)
		buffer[i] = 0;
}


char* getExtensionlessFilename (char* filename)
{
	unsigned int len = strlen(filename), i;
	char* noextfname = (char*)malloc(sizeof(char)*len);
	clearBuffer(noextfname);
	for (i=len-1; i>0 && filename[i] != '.'; i--);
	unsigned int last_dot_index = (i==0)?len:i;
	for (i=0; i<last_dot_index; i++)
		noextfname[i] = filename[i];
	return noextfname;
}


char* getActualFilename (char* filepath)
{
	char* noextfname = getExtensionlessFilename(filepath);
	unsigned int len = strlen(noextfname), i;
	char* classname = (char*)malloc(sizeof(char)*len);
	clearBuffer(classname);
	for (i=len-1; i>0 && !(noextfname[i] == '/' || noextfname[i] == '\\'); i--);
	unsigned int last_slash_index = (i==0)?-1:i, j=0;
	for (i=last_slash_index+1; i<len; i++)
		classname[j++] = noextfname[i];
	free(noextfname);
	return classname;
}


char* getFileDir (char* filepath)
{
	char* noextfname = getExtensionlessFilename(filepath);
	unsigned int len = strlen(noextfname), i;
	char* dirpath = (char*)malloc(sizeof(char)*len);
	clearBuffer(dirpath);
	for (i=len-1; i>0 && !(noextfname[i] == '/' || noextfname[i] == '\\'); i--);
	unsigned int last_slash_index = (i==0)?0:i;
	for (i=0; i<last_slash_index; i++)
		dirpath[i] = noextfname[i];
	if (last_slash_index == 0)
		dirpath[0] = '.';
	free(noextfname);
	return dirpath;
}


int main (int argc, char* argv[])
{
	if (argc==1)
	{
	    printf ("saioc: No options specified. Abort.\n");
	    return 1;
	}
	if (argc > 3)
	{
		printf ("saioc: Invalid number of arguments. Abort.\n");
		return 1;
	}
	if (strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "help") == 0)
	{
		printf ("SAIOC (version: 1.1.0)\n");
		printf ("A simple all-in-one compiler interface for Termux environment in Android targetting educational purposes and quick prototyping.\n");
		printf ("Copyright (c) Arijit Kumar Das <arijitkdgit.official@gmail.com> (MIT License).\n\n");
		printf ("* Usage: saioc <option> <srcfile>\n");
		printf ("* Supports compilation of only one source file per call.\n");
		printf ("* Supported options:\n");
		printf ("\t-java\t:  Compile and run Java code\n");
		printf ("\t-c\t:  Compile and run C code\n");
		printf ("\t-cpp\t:  Compile and run C++ code\n");
		printf ("\t-rust\t:  Compile and run Rust code\n");
		printf ("\t-kotlin\t:  Compile and run Kotlin code\n");
		printf ("\t-help\t:  Show this help menu and exit\n");
	}
	else if (strcmp(argv[1], "-java") == 0 || strcmp(argv[1], "java") == 0)
	{
		if (argc < 3)
		{
			printf ("saioc: No source file specified. Abort.\n");
			return 1;
		}
		printf ("Compiling %s...\n", argv[2]);
		char command[COMBUFFER];
		int retcode;
		clearBuffer(command);
		snprintf(command, COMBUFFER, "ecj %s", argv[2]);
		retcode = system(command);
		if (retcode == 0)
		{
			printf ("Compiled successfully!\n");
		}
		else
		{
			printf ("\nCompilation failed. See above for errors.\n");
			return 1;
		}
		clearBuffer(command);
		char* javaclass = getActualFilename(argv[2]);
		char* srcdir = getFileDir(argv[2]);
		printf ("\nGenerating dex file...\n");
		snprintf(command, COMBUFFER, "cd %s && dx --dex --output=%s.dex %s.class", srcdir, javaclass, javaclass);
		retcode = system(command);
		if (retcode == 0)
		{
			printf ("Dex file generated successfully!\n");
		}
		else
		{
			printf ("\nFailed to generate dex file. See above for errors.\n");
			return 1;
		}
		clearBuffer(command);
		snprintf(command, COMBUFFER, "cd %s && dalvikvm -cp %s.dex %s", srcdir, javaclass, javaclass);
		printf ("\nRunning program...\n\n");
		retcode = system(command);
		if (retcode == 0)
		{
			printf ("\n[Program exited with return code 0]\n");
		}
		else
		{
			printf ("\n[Program exited with return code %d]\n", retcode);
		}
	}
	else if (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "c") == 0)
	{
		if (argc < 3)
		{
			printf ("saioc: No source file specified. Abort.\n");
			return 1;
		}
		printf ("Compiling %s...\n", argv[2]);
		char command[COMBUFFER];
		int retcode;
		clearBuffer(command);
		char* binname = getActualFilename(argv[2]);
		char* noextfname = getExtensionlessFilename(argv[2]);
		snprintf(command, COMBUFFER, "clang %s -o ~/%s && cp ~/%s %s ", argv[2], binname, binname, noextfname);
		retcode = system(command);
		if (retcode == 0)
		{
			printf ("Compiled successfully!\n");
		}
		else
		{
			printf ("\nCompilation failed. See above for errors.\n");
			return 1;
		}
		clearBuffer(command);
		snprintf(command, COMBUFFER, "~/%s", binname);
		printf ("\nRunning program...\n\n");
		retcode = system(command);
		if (retcode == 0)
		{
			printf ("\n[Program exited with return code 0]\n");
		}
		else
		{
			printf ("\n[Program exited with return code %d]\n", retcode);
		}
		clearBuffer(command);
		snprintf(command, COMBUFFER, "rm ~/%s 1> /dev/null 2> /dev/null", binname);
		system(command);
	}
	else if (strcmp(argv[1], "-cpp") == 0 || strcmp(argv[1], "cpp") == 0)
	{
		if (argc < 3)
		{
			printf ("saioc: No source file specified. Abort.\n");
			return 1;
		}
		printf ("Compiling %s...\n", argv[2]);
		char command[COMBUFFER];
		int retcode;
		clearBuffer(command);
		char* binname = getActualFilename(argv[2]);
		char* noextfname = getExtensionlessFilename(argv[2]);
		snprintf(command, COMBUFFER, "clang++ %s -o ~/%s && cp ~/%s %s ", argv[2], binname, binname, noextfname);
		retcode = system(command);
		if (retcode == 0)
		{
			printf ("Compiled successfully!\n");
		}
		else
		{
			printf ("\nCompilation failed. See above for errors.\n");
			return 1;
		}
		clearBuffer(command);
		snprintf(command, COMBUFFER, "~/%s", binname);
		printf ("\nRunning program...\n\n");
		retcode = system(command);
		if (retcode == 0)
		{
			printf ("\n[Program exited with return code 0]\n");
		}
		else
		{
			printf ("\n[Program exited with return code %d]\n", retcode);
		}
		clearBuffer(command);
		snprintf(command, COMBUFFER, "rm ~/%s 1> /dev/null 2> /dev/null", binname);
		system(command);
	}
	else if (strcmp(argv[1], "-rust") == 0 || strcmp(argv[1], "rust") == 0)
	{
		if (argc < 3)
		{
			printf ("saioc: No source file specified. Abort.\n");
			return 1;
		}
		printf ("Compiling %s...\n", argv[2]);
		char command[COMBUFFER];
		int retcode;
		clearBuffer(command);
		char* binname = getActualFilename(argv[2]);
		char* noextfname = getExtensionlessFilename(argv[2]);
		snprintf(command, COMBUFFER, "rustc %s -o ~/%s && cp ~/%s %s ", argv[2], binname, binname, noextfname);
		retcode = system(command);
		if (retcode == 0)
		{
			printf ("Compiled successfully!\n");
		}
		else
		{
			printf ("\nCompilation failed. See above for errors.\n");
			return 1;
		}
		clearBuffer(command);
		snprintf(command, COMBUFFER, "~/%s", binname);
		printf ("\nRunning program...\n\n");
		retcode = system(command);
		if (retcode == 0)
		{
			printf ("\n[Program exited with return code 0]\n");
		}
		else
		{
			printf ("\n[Program exited with return code %d]\n", retcode);
		}
		clearBuffer(command);
		snprintf(command, COMBUFFER, "rm ~/%s 1> /dev/null 2> /dev/null", binname);
		system(command);
	}
	else if (strcmp(argv[1], "-kotlin") == 0 || strcmp(argv[1], "kotlin") == 0)
	{
		if (argc < 3)
		{
			printf ("saioc: No source file specified. Abort.\n");
			return 1;
		}
		printf ("Compiling %s...\n", argv[2]);
		char command[COMBUFFER];
		int retcode;
		clearBuffer(command);
		snprintf(command, COMBUFFER, "kotlinc %s 2>/dev/null", argv[2]);
		retcode = system(command);
		if (retcode == 0)
		{
			printf ("Compiled successfully!\n");
		}
		else
		{
			printf ("\nCompilation failed. See above for errors.\n");
			return 1;
		}
		clearBuffer(command);
		char* classfile = strcat(getActualFilename(argv[2]), "Kt");
		char* srcdir = getFileDir(argv[2]);
		printf ("\nGenerating dex file...\n");
		snprintf(command, COMBUFFER, "cd %s && dx --dex --output=%s.dex %s.class", srcdir, classfile, classfile);
		retcode = system(command);
		if (retcode == 0)
		{
			printf ("Dex file generated successfully!\n");
		}
		else
		{
			printf ("\nFailed to generate dex file. See above for errors.\n");
			return 1;
		}
		clearBuffer(command);
		snprintf(command, COMBUFFER, "cd %s && dalvikvm -cp %s.dex %s", srcdir, classfile, classfile);
		printf ("\nRunning program...\n\n");
		retcode = system(command);
		if (retcode == 0)
		{
			printf ("\n[Program exited with return code 0]\n");
		}
		else
		{
			printf ("\n[Program exited with return code %d]\n", retcode);
		}
	}
	else
	{
		printf ("saioc: Unknown option \"%s\". Use -help to get the list of available options.\n", argv[1]);
	}
	return 0;
}