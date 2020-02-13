#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#define MAX_PATH 255

void print_usage(char *name)
{
    printf("Usage: %s [-h] [-L -d -g -i -p -s -t -u] [dirname]\n", name);
    printf("\t-h: Print the help message.\n");
    printf("\t-L: Possible to follow symbolinks. Default will be not follow links.\n");
    printf("\t-t: Print the file type information.\n");
    printf("\t-p: Print the file permission bits as rwxrwx.\n");
    printf("\t-i: Print the number of links to file in inode table.\n");
    printf("\t-u: Print the UID associated with the file.\n");
    printf("\t-g: Print the GID associated with the file.\n");
    printf("\t-s: Print the size of file in bytes, kb, mb and gb.\n");
    printf("\t-l: This option will be same with multi parameters : -tpiugs.\n");
}

int main(int argc, char *argv[])
{
    int opt;
    int max_queue = 1024, queue_begin = 0, queue_end = 0;
    char queue[max_queue][MAX_PATH];
    char currentpath[MAX_PATH];
    char pathname[MAX_PATH];
    char *dir_temp = 0;

    DIR *dir;
    struct dirent *ent;
    struct stat sb;

    int p_h, p_L, p_d, p_g, p_i, p_p, p_s, p_t, p_u, p_l;
    p_h = p_L = p_d = p_g = p_i = p_p = p_s = p_t = p_u = p_l = 0;

    memset(queue, 0, max_queue * MAX_PATH);
    
    while ((opt = getopt(argc, argv, ":if:hLdgipstul")) != -1)
    {   
	switch (opt)
	{
	    case 'h': p_h = 1; break;
	    case 'L': p_L = 1; break;
	    case 'd': p_d = 1; break;
	    case 'g': p_g = 1; break;
	    case 'i': p_i = 1; break;
	    case 'p': p_p = 1; break;
	    case 's': p_s = 1; break;
	    case 't': p_t = 1; break;
	    case 'u': p_u = 1; break;
	    case 'l': p_l = 1; break;
	}    
    }
    
    if (p_h == 1) {
	print_usage(argv[0]);
        return 0;
    }

    if (argc == 1 || argv[argc - 1][0] == '-')
	getcwd(currentpath, sizeof(currentpath));
    else
        strcpy(currentpath, argv[argc - 1]);

    dir_temp = currentpath; 
    printf("%s\n", dir_temp);

    do {
	dir = opendir(dir_temp);
        if (!dir) {
	    perror("invalid filepath");
	}
	else {
	while ((ent = readdir(dir)) != NULL) {
	    memset(pathname, 0, MAX_PATH);
	    strcat(pathname, dir_temp);
	    strcat(pathname, "/");
	    strcat(pathname, ent->d_name);	
	    
	    stat(pathname, &sb);

	    if (strcmp(".", ent->d_name) && strcmp("..", ent->d_name)) {	    
	        if (ent->d_type == DT_DIR) {
		    strcat(queue[queue_end], pathname);
		    queue_end++;
		    if (queue_end >= max_queue)
		       break;
	        }
		else {
		    if (p_l == 1 || p_t == 1)
			printf("%04x: ", (sb.st_mode & S_IFMT));
		    if (p_l == 1 || p_p == 1) {
			printf((sb.st_mode & S_IRUSR) ? "r" : "-");
			printf((sb.st_mode & S_IWUSR) ? "w" : "-");
			printf((sb.st_mode & S_IXUSR) ? "x" : "-");
			printf((sb.st_mode & S_IRGRP) ? "r" : "-");
			printf((sb.st_mode & S_IWGRP) ? "w" : "-");
			printf((sb.st_mode & S_IXGRP) ? "x" : "-");
			printf((sb.st_mode & S_IROTH) ? "r" : "-");
			printf((sb.st_mode & S_IWOTH) ? "w" : "-");
			printf((sb.st_mode & S_IXOTH) ? "x" : "-");
			printf(": ");
		    }
		    if (p_d == 1) {
			struct tm *local = localtime(&sb.st_mtime);
			printf("%02d-%02d-%02d %02d/%02d/%04d: ", 
				local->tm_hour, 
				local->tm_min,
				local->tm_sec,
				local->tm_mday,
				local->tm_mon + 1,
				local->tm_year + 1900);
		    }
   		    if (p_l == 1 || p_i == 1) {
			printf("%ld: ", (long)sb.st_ino);
		    }
		    if (p_l == 1 || p_u == 1) {
			printf("%ld: ", (long)sb.st_uid);
		    }
		    if (p_l == 1 || p_g == 1) {
			printf("%ld: ", (long)sb.st_gid);
		    }
		    if (p_l == 1 || p_s == 1) {
			if (sb.st_size < 1024)
	   		    printf("%lldBytes: ", (long long)sb.st_size);
			else if (sb.st_size < 1024 * 1024)
	   		    printf("%.2fKB: ", (float)sb.st_size / 1024);
			else if (sb.st_size < 1024 * 1024 * 1024)
	   		    printf("%.2fMB: ", (float)sb.st_size / 1024 / 1024);
			else
	   		    printf("%.2fGB: ", (float)sb.st_size / 1024 / 1024 / 1024);
		    }

		    printf("%s\n", pathname);
		}
	    }
	}
	}
	dir_temp = queue[queue_begin];
	queue_begin++;
    } while (queue_end > 0 && queue_end < max_queue && queue_end >= queue_begin);
    
    return 1;
}

