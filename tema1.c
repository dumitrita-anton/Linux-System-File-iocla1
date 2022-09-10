#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300
#define MAX_NAME 50

struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

Dir* alocDir (char * name) {

	Dir* aux = (Dir *)calloc(1, sizeof(Dir));

	if(!aux) {
		return NULL;
	}

	aux->name = (char *)malloc(sizeof(name));
	strcpy(aux->name, name);

	//aux->parent = (Dir *)calloc(1, sizeof(Dir));
	//aux->next = (Dir *)calloc(1, sizeof(Dir));
	aux->head_children_dirs = NULL;
	aux->head_children_files = NULL;
	
	return aux;
}

File* alocFile(char * name) {
	File *file = (File *)calloc(1, sizeof(File));
	file->name = (char * )malloc(sizeof(name));
	strcpy(file->name, name);
	//file->parent = (Dir *)calloc(1, sizeof(Dir));
	//file->next = (File *)calloc(1, sizeof(File));
	return file;
}

void DistrugeListFile(File** file) {
	
	while((*file) != NULL) {
		File * aux = (*file);
		free(aux->name);
		(*file) = aux->next;
		aux->parent = NULL;
		aux->next = NULL;
		free(aux);
	}
	
}

void DistrugeListDir(Dir** dir) {
	while((*dir) != NULL) {
		Dir * aux =(*dir);
		free(aux->name);
		if(aux->head_children_files != NULL) {
			DistrugeListFile(&aux->head_children_files);
			aux->head_children_files = NULL;
		}
		if(aux->head_children_dirs != NULL) {
			DistrugeListDir(&aux->head_children_dirs);
			aux->head_children_dirs = NULL;
		}
		(*dir)->parent = NULL;
		(*dir) = aux->next;
		free(aux);
	}
}

int searchFile(Dir* parent, char* name) {
	
	File* aux = parent->head_children_files;
	for(File* p = aux; p != NULL; p = p->next) {
		if(strcmp(p->name, name) == 0)
			return 0;
	}

	return 1;
}

int searchDir(Dir* parent, char* name) {
	
	Dir* aux = parent->head_children_dirs;
	for(Dir* p = aux; p != NULL; p = p->next) {
		if(strcmp(p->name, name) == 0)
			return 0;
	}

	return 1;
}


void touch (Dir* parent, char* name) {
	
	if(searchFile(parent, name) == 0) {
		printf("File already exists\n");
		return;
	}

	if(parent->head_children_files == NULL) {
		parent->head_children_files = alocFile(name);
		parent->head_children_files->parent = parent;
	    parent->head_children_files->next = NULL;
	}
	else
	{
		File* search = parent->head_children_files;
		for(; search->next != NULL; search = search->next);
		search->next = alocFile(name);
		search->parent = parent;
	    search->next->next = NULL;
	}
	
}

void mkdir (Dir* parent, char* name) {

	if(searchDir(parent, name) == 0) {
		printf("Directory already exists\n");
		return;
	}

	if(parent->head_children_dirs == NULL) {
		parent->head_children_dirs = alocDir(name);
		parent->head_children_dirs->parent = parent;
	    parent->head_children_dirs->next = NULL;
	}
	else
	{
		Dir* search = parent->head_children_dirs;
		for(; search->next != NULL; search = search->next);
		search->next = alocDir(name);
		search->parent = parent;
	    search->next->next = NULL;
	}
	
}

void ls (Dir* parent) {
	
	for(Dir* p = parent->head_children_dirs; p != NULL; p = p->next) {
	printf("%s\n", p->name);

	}

	for(File* p = parent->head_children_files; p != NULL; p = p->next) {
	printf("%s\n", p->name);
	}

}


void rm (Dir* parent, char* name) {

	File* Hfile = parent->head_children_files;

	//no file case 
	if(Hfile == NULL) {
		printf("Could not find the file\n");
		return;
	}

	//the file we are searching is parent->head_children_files
	if(strcmp(Hfile->name, name) == 0 && Hfile != NULL) {
		free(Hfile->name);
		parent->head_children_files = Hfile->next;
		Hfile->parent = NULL;
		Hfile->next = NULL;
		free(Hfile);
		return;
	}


	//file we are looking for is not head_children_files
	File *ant;
	File *fileCrt = parent->head_children_files;

	//iterating threw list of files until we find the file 
	while ((fileCrt != NULL) && (strcmp(fileCrt->name, name) != 0))
	{
		ant = fileCrt;
		fileCrt = fileCrt->next;
	}

	//file not found
	if(fileCrt == NULL) {
		printf("Could not find the file\n");
		return;
	}

	//file found
	
	ant->next = fileCrt->next;
	free(fileCrt->name);
	free(fileCrt);
	
}

void rmdir (Dir* parent, char* name) {

	Dir * DirCrt = parent->head_children_dirs;

	if(DirCrt == NULL) {
		printf("Could not find the dir\n");
		return;
	}

	//dir we are searching is parent->head_children_dirs
	if(strcmp(DirCrt->name, name) == 0 && DirCrt != NULL) {
		
		if(DirCrt->head_children_files != NULL) {
			for(File * aux = DirCrt->head_children_files; aux != NULL; aux = aux->next)
				rm(aux->parent, aux->name);
		}
		parent->head_children_dirs = DirCrt->next;

		free(DirCrt->name);
		free(DirCrt);
		return;
	}

	Dir * ant;
	DirCrt = parent->head_children_dirs;

	while ((DirCrt != NULL) && (strcmp(DirCrt->name, name) != 0))
	{
		ant = DirCrt;
		DirCrt = DirCrt->next;
	}

	if(DirCrt == NULL) {
		printf("Could not find the dir\n");
		return;
	}

	if(DirCrt->head_children_files != NULL) {
			for(File * aux = DirCrt->head_children_files; aux != NULL; aux = aux->next)
				rm(aux->parent, aux->name);
		}

	ant->next = DirCrt->next;
	free(DirCrt->name);
	free(DirCrt);
		 
}

void cd(Dir** target, char *name) {
	
	//.. case
	if(strcmp(name, "..") == 0) {
		if((*target)->parent == NULL)
			return;
		else
			{
				(*target) = (*target)->parent;
				return;
			}
	}

	Dir * p = (*target)->head_children_dirs;

	//no list of dirs in current directory
	if(p == NULL) {
		printf("No directories found!\n");
		return;
	}

	while ((p != NULL) && (strcmp(p->name, name) != 0)) {
		p = p->next;
	}

	if(p == NULL) {
		printf("No directories found!\n");
		return;
	}
	else {
		(*target) = p;
	}
	
}


char *pwd (Dir* target) {
	char * path = (char *)malloc(sizeof(path)); 
	int contor = 0;
	strcpy(path, "/home");

	char **pathContent;

	for(Dir * p = target; p != NULL; p = p->parent) {
		contor++;
	}

	contor--;

	pathContent = malloc(contor * sizeof(char *));
	for(int i = 0; i < contor; i++)
		pathContent[i] = malloc(MAX_NAME * sizeof(char));

	for(int i = 0 ;target->parent != NULL; target = target->parent) {
		strcpy(pathContent[i], target->name);
		i++;
	}

	for(int i = contor - 1; i >= 0; i--)
	{
		//printf("%s ", pathContent[i]);
		strcat(path, "/");
		strcat(path, pathContent[i]);
	}

	for(int i = 0; i < contor; i++)
		free(pathContent[i]);	
	free(pathContent);


	return path;

}


void printSpace(int level) {

	for(int i = level - 1; i > 0; i--)
		printf("    ");

}


void tree (Dir* target, int level) {

	if(target == NULL)
		return;

	if(target->head_children_dirs != NULL) {
		
		Dir * p = target->head_children_dirs;
		for(int i = level; i > 0; i--)
			printf("    ");
		printf("%s\n", p->name);
		
		level++;
		tree(p, level);	
	
	}

	if(target->head_children_files != NULL) {

		for(int i = level - 1; i > 0; i--)
			printf("    ");
		printf("%s\n", target->head_children_files->name);
	}
	
	if(target->next != NULL) {
	level--;
	for(int i = level - 1; i > 0; i--)
			printf("    ");
	//printf("%d\n", level);
	
	printf("%s\n", target->next->name);
			 tree(target->next, ++level);
			
	}
}

void stop (Dir* target) {
	if(target->head_children_files != NULL) {
		//DistrugeListFile(&(home->head_children_files));
		DistrugeListFile(&(target->head_children_files));
	}

	if(target->head_children_dirs != NULL) {
		//DistrugeListFile(&(home->head_children_files));
		DistrugeListDir(&(target->head_children_dirs));
	}

	
	free(target->next);
	free(target->parent);
	free(target->name);
	free(target);
}

// void mvFile(Dir* parent, char *oldname, char *newname) {
	
// }

// void mv(Dir* parent, char *oldname, char *newname) {
// 	if((searchDir(parent, oldname) == 1) || (searchFile(parent, oldname)) == 1) {
// 		printf("File/Director not found\n");
// 		return;
// 	}

// 	if((searchDir(parent, newname) == 0) || (searchFile(parent, newname)) == 0) {
// 		printf("File/Director already exists\n");
// 		return;
// 	}

// 	if(searchFile(parent, oldname) == 0 && searchFile(parent, newname) == 1) {
// 		rm(parent, oldname);
// 		touch(parent, newname);
// 		return;
// 	}

	
// }

int main () {

	char *line = (char *)calloc(MAX_INPUT_LINE_SIZE, sizeof(char));
	fgets(line, MAX_INPUT_LINE_SIZE, stdin);
	
	Dir *home = alocDir("home");
	home->parent = NULL;
	home->next = NULL;

	Dir* crt_dir = home;
	char * p;
	
	do
	{
		p = strtok(line, " ");

		if(strcmp(p, "stop\n") == 0){
			stop(home);
			break;
		}

		if(strcmp(p, "touch") == 0) {
			p = strtok(NULL, "\n");
			touch(crt_dir, p);
		}

		if(strcmp(p, "mkdir") == 0) {
			p = strtok(NULL, "\n");
			mkdir(crt_dir, p);
		}

		if(strcmp(p, "rm") == 0) {
			p = strtok(NULL, "\n");
			rm(crt_dir, p);
		}

		if(strcmp(p, "ls\n") == 0) {
			ls(crt_dir);
		}

		if(strcmp(p, "rmdir") == 0) {
			p = strtok(NULL, "\n");
			rmdir(crt_dir, p);
		}

		if(strcmp(p, "cd") == 0) {
		 	p = strtok(NULL, "\n");
		 	cd(&crt_dir, p);
		 }

		if(strcmp(p, "tree\n") == 0) {
			tree(crt_dir, 0);
		}

		if(strcmp(p, "pwd\n") == 0) {
			char * path = pwd(crt_dir);
			printf("%s", path);
			free(path);
		}
		
		fgets(line, MAX_INPUT_LINE_SIZE, stdin);
	} while (1);

	//printf("%s\n", home->head_children_files->name);

	free(line);
	//free(crt_dir);
	
	return 0;
}