#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>


#define check_error(expr,userMsg) \
	do {\
		if (!(expr)) {\
			perror(userMsg);\
            exit(EXIT_FAILURE);\
		}\
	} while(0)

#define MAX_PATH 1048

void fileInfo(char*);

char execName[128];

int p_h = 0; 
int p_L = 0; 
int p_d = 0; 
int p_g = 0; 
int p_i = 0; 
int p_p = 0; 
int p_s = 0; 
int p_t = 0; 
int p_u = 0; 

typedef struct node node;

struct node {
    node* next;
    char path[MAX_PATH];
};

node*
node_new(char* p)
{
    node* n = malloc(sizeof(node));
    n->next = NULL;
    strncpy(n->path, p, MAX_PATH);
    fileInfo(p);
    return n;
}

typedef struct {
    node* first;
    node* last;
} queue;

queue
queue_init(node* n)
{
    queue q;
    q.first = n;
    q.last = n;
    return q;
}

void
queue_dequeue(queue* q)
{
    node* tmp = q->first;
    q->first = q->first->next; 
    free(tmp);
}

void
queue_enqueue(queue* q, node* n)
{
    q->last->next = n;
    q->last = n;
}

char* getUserName(uid_t ID){
	
	struct passwd* pwd = getpwuid(ID);
	check_error(pwd != NULL, "getpwuid error");
	
	char* name = malloc(strlen(pwd->pw_name) + 1);
	strcpy(name, pwd->pw_name);
	
	return name;
}

char* getGroupName(gid_t ID){
	struct group* grp = getgrgid(ID);
	check_error(grp != NULL, "getgrgid error");
	
	char* name = malloc(strlen(grp->gr_name) + 1);
	strcpy(name, grp->gr_name);
	
	return name;
}

void fileInfo(char* path) {
	struct stat fInfo;

    char err_msg[256];
    memset(err_msg, 0, 256);
    strcat(err_msg, execName);
    strcat(err_msg, ": Error: ");
    strcat(err_msg, path);

    if(p_L == 1){
        if (stat(path, &fInfo) == -1){
            perror(err_msg);
            return;
        }
    }
    else{
        if (lstat(path, &fInfo) == -1){
            perror(err_msg);
            return;
        }
    }
	
    if(p_t == 1){
        switch(fInfo.st_mode & S_IFMT) {
            case S_IFREG:
                printf("-");
                break;
            case S_IFDIR:
                printf("d");
                break;
            case S_IFCHR:
                printf("c");
                break;
            case S_IFBLK:
                printf("b");
                break;
            case S_IFIFO:
                printf("p");
                break;
            case S_IFSOCK:
                printf("s");
                break;
            case S_IFLNK:
                printf("l");
                break;
        }
    }

    if(p_p == 1){
        char fileMode[10];
        strcpy(fileMode, "rwxrwxrwx");

        if (!(fInfo.st_mode & S_IRUSR))
            fileMode[0] = '-';
        if (!(fInfo.st_mode & S_IWUSR))
            fileMode[1] = '-';
        if (!(fInfo.st_mode & S_IXUSR))
            fileMode[2] = '-';
        if (!(fInfo.st_mode & S_IRGRP))
            fileMode[3] = '-';
        if (!(fInfo.st_mode & S_IWGRP))
            fileMode[4] = '-';
        if (!(fInfo.st_mode & S_IXGRP))
            fileMode[5] = '-';
        if (!(fInfo.st_mode & S_IROTH))
            fileMode[6] = '-';
        if (!(fInfo.st_mode & S_IWOTH))
            fileMode[7] = '-';
        if (!(fInfo.st_mode & S_IXOTH))
            fileMode[8] = '-';
        
        printf("%s ", fileMode);
    }

    if(p_i == 1){
        printf("%10.ld ", (long)fInfo.st_ino);
    }
	
    if(p_u == 1){
        char* userName = getUserName(fInfo.st_uid);
        printf("%s ", userName);
        free(userName);
    }
	
    if(p_g == 1){
        char* groupName = getGroupName(fInfo.st_gid);
        printf("%s ", groupName);
        free(groupName);
    }
	
    if(p_s == 1){
        intmax_t sz = fInfo.st_size;
        if(sz >  1073741824){
            printf("%4.jdG ", sz / 1073741824);
        }
        else if(sz > 1048576){
            printf("%4.jdM ", sz / 1048576);
        }
        else if(sz > 1024){
            printf("%4.jdK ", sz / 1024);
        }
        else printf("%5.jd ", sz);
    }
	

    if(p_d == 1){
        struct tm *lTime = localtime(&fInfo.st_mtime);
        char readableTime[16];
        strftime(readableTime, 16, "%b %d %R", lTime);
        printf("%s ", readableTime);
    }

    if(path[0] == '.'){
        if(path[1] == '/'){
            path[0] = ' ';
            path[1] = ' ';
            printf("%s\n", path);
        }
        else printf("  .\n");
    } 
    else printf("%s\n", path);
}



void 
print_usage()
{
    printf("Usage: bt [-h] [-L -d -g -i -p -s -t -u | -l] [dirname]\n");
    printf("\t-h: Print a help message and exit.\n");
    printf("\t-L: Follow symbolic links, if any.\n");
    printf("\t-d: Show the time of last modification.\n");
    printf("\t-g: Print the GID associated with the file.\n");
    printf("\t-i: Print the number of links to file in inode table.\n");
    printf("\t-p: Print permission bits as rwxrwxrwx.\n");
    printf("\t-s: Print the size of file in readable format.\n");
    printf("\t-t: Print information on file type.\n");
    printf("\t-u: Print the UID associated with the file.\n");
    printf("\t-l: Same as: -tpiudgs.\n");
}

int main(int argc, char *argv[])
{
    strcpy(execName, argv[0]);
    int opt;
    char currentpath[MAX_PATH];
    char pathname[MAX_PATH];
    struct dirent *ent;
    
    while ((opt = getopt(argc, argv, ":if:hLdgipstul")) != -1) {   
        switch (opt) {
            case 'h': p_h = 1; break;
            case 'L': p_L = 1; break;
            case 'd': p_d = 1; break;
            case 'g': p_g = 1; break;
            case 'i': p_i = 1; break;
            case 'p': p_p = 1; break;
            case 's': p_s = 1; break;
            case 't': p_t = 1; break;
            case 'u': p_u = 1; break;
            case 'l': p_d = 1; p_t = 1; p_p = 1; p_i = 1; p_u = 1; p_g = 1; p_s = 1; break;
        }    
    }
    
    if (p_h == 1) {
        print_usage();
        exit(EXIT_SUCCESS);
    }

    if ((argc == 1) || (argv[argc - 1][0] == '-'))
        strcpy(currentpath, ".");
    else
        strcpy(currentpath, argv[argc - 1]);

    node* currNode = node_new(currentpath);
    queue q = queue_init(currNode);

    while(q.first != NULL){
        char tmp_path[MAX_PATH];
        strcpy(tmp_path, (q.first)->path);
        struct stat fInfo;

        char err_msg[256];
        memset(err_msg, 0, 256);
        strcat(err_msg, execName);
        strcat(err_msg, ": Error: ");
        strcat(err_msg, tmp_path);

        check_error(lstat(tmp_path, &fInfo) != -1, err_msg);
        if((fInfo.st_mode & S_IFMT) == S_IFDIR){
            DIR* dir;
            dir = opendir(tmp_path);
            while ((ent = readdir(dir)) != NULL) {
                if(strcmp(ent->d_name, ".") == 0) continue;
                if(strcmp(ent->d_name, "..") == 0) continue;
                memset(pathname, 0, MAX_PATH);
                strcat(pathname, tmp_path);
                strcat(pathname, "/");
                strcat(pathname, ent->d_name);	
                node* tmp = node_new(pathname);
                queue_enqueue(&q, tmp);
            }
        }
        queue_dequeue(&q);
    }

    exit(EXIT_SUCCESS);
}

