#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>

typedef struct section_header{
    char *name;
    int type;
    int offset;
    int size;
}Section_header;

int size_smaller(char* dir_name, int size){
    DIR* dir;
    struct dirent *dirEntry;
    struct stat inode;
    char *Name = (char*)malloc(sizeof(char)*PATH_MAX);
    dir=opendir(dir_name);
    if (dir == 0) {
		perror ("Error opening directory");
		return -1;
	}
    while((dirEntry=readdir(dir)) != 0){
        if(strcmp(dirEntry->d_name,".") != 0 && strcmp(dirEntry->d_name,"..") != 0){
            snprintf(Name, PATH_MAX, "%s/%s",dir_name,dirEntry->d_name);
            lstat (Name, &inode);
            if(S_ISREG(inode.st_mode)){
                if(inode.st_size < size){
                    printf("%s\n",Name);
                }
            }
        }

    }
    free(Name);
    closedir(dir);
    return 0;
}
int size_smaller_rec(char* dir_name, int size){
    DIR* dir;
    struct dirent *dirEntry;
    struct stat inode;
    char *Name = (char*)malloc(sizeof(char)*PATH_MAX);
    dir=opendir(dir_name);
    if (dir == 0) {
		perror ("Error opening directory");
		exit(1);
	}
    int searched = size_smaller(dir_name,size);
    if(searched == -1){
        closedir(dir);
        return -1;
    }else{
        while((dirEntry=readdir(dir)) != 0){
            snprintf(Name, PATH_MAX, "%s/%s",dir_name,dirEntry->d_name);
            lstat (Name, &inode);
            if (S_ISDIR(inode.st_mode) && strcmp(dirEntry->d_name,".") !=0 && strcmp(dirEntry->d_name,"..") !=0 ){
                size_smaller_rec(Name,size);
            }
        }
       
    }
    return 0;
}
int search_dir(char* dir_name){
    DIR* dir;
    struct dirent *dirEntry;
    struct stat inode;
    char *Name = (char*)malloc(sizeof(char)*PATH_MAX);
    dir=opendir(dir_name);
    if (dir == 0) {
		perror ("Error opening directory");
		return -1;
	}
    while((dirEntry=readdir(dir)) != 0){
        if(strcmp(dirEntry->d_name,".") != 0 && strcmp(dirEntry->d_name,"..") != 0){
            snprintf(Name, PATH_MAX, "%s/%s",dir_name,dirEntry->d_name);
            lstat (Name, &inode);
            printf("%s\n",Name);
        }

    }
    free(Name);
    closedir(dir);
    return 0;
}
int search_tree(char* dir_name){

    DIR* dir;
    struct dirent *dirEntry;
    struct stat inode;
    char *Name = (char*)malloc(sizeof(char)*PATH_MAX);
    dir=opendir(dir_name);
    if (dir == 0) {
		perror ("Error opening directory");
		exit(1);
	}
    int searched = search_dir(dir_name);
    if(searched == -1){
        closedir(dir);
        return -1;
    }else{
        while((dirEntry=readdir(dir)) != 0){
            snprintf(Name, PATH_MAX, "%s/%s",dir_name,dirEntry->d_name);
            lstat (Name, &inode);
            if (S_ISDIR(inode.st_mode) && strcmp(dirEntry->d_name,".") !=0 && strcmp(dirEntry->d_name,"..") !=0 ){
                search_tree(Name);
            }
        }
       
    }
    return 0;
}
int permissions_files(char* dir_name,char* p){
    DIR* dir;
    struct dirent *dirEntry;
    struct stat inode;
    char *Name=(char*)malloc(sizeof(char)*PATH_MAX);
    dir=opendir(dir_name);
    if (dir == 0) {
		perror ("Error opening directory");
		return -1;
	}
    while((dirEntry=readdir(dir)) != 0){
        if(strcmp(dirEntry->d_name,".") != 0 && strcmp(dirEntry->d_name,"..") != 0){
            snprintf(Name, PATH_MAX, "%s/%s",dir_name,dirEntry->d_name);
            lstat (Name, &inode);
            if(S_ISREG(inode.st_mode) || S_ISDIR(inode.st_mode)){
                if(((inode.st_mode & S_IREAD) && p[0]=='r') || (!(inode.st_mode & S_IREAD) && p[0]=='-') ){
                    if(((inode.st_mode & S_IWRITE) && p[1]=='w') || (!(inode.st_mode & S_IWRITE) && p[1]=='-') ){
                        if(((inode.st_mode & S_IEXEC) && p[2]=='x') || (!(inode.st_mode & S_IEXEC) && p[2]=='-') ){
                            if(((inode.st_mode & S_IRGRP) && p[3]=='r') || (!(inode.st_mode & S_IRGRP) && p[3]=='-') ){
                                if(((inode.st_mode & S_IWGRP) && p[4]=='w') || (!(inode.st_mode & S_IWGRP) && p[4]=='-') ){
                                    if(((inode.st_mode & S_IXGRP) && p[5]=='x') || (!(inode.st_mode & S_IXGRP) && p[5]=='-') ){
                                        if(((inode.st_mode & S_IROTH) && p[6]=='r') || (!(inode.st_mode & S_IROTH) && p[6]=='-') ){
                                            if(((inode.st_mode & S_IWOTH) && p[7]=='w') || (!(inode.st_mode & S_IWOTH) && p[7]=='-') ){
                                                if(((inode.st_mode & S_IXOTH) && p[8]=='x') || (!(inode.st_mode & S_IXOTH) && p[8]=='-') ){
                                                    printf("%s\n",Name);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                    }

                }
            }
        }

    }
    free(Name);
    closedir(dir);
    return 0;
}
int permissions_tree(char* dir_name, char *p){
    DIR* dir;
    struct dirent *dirEntry;
    struct stat inode;
    char *Name = (char*)malloc(sizeof(char)*PATH_MAX);
    dir=opendir(dir_name);
    if (dir == 0) {
		perror ("Error opening directory");
		exit(1);
	}
    int searched = permissions_files(dir_name,p);
    if(searched == -1){
        closedir(dir);
        return -1;
    }else{
        while((dirEntry=readdir(dir)) != 0){
            snprintf(Name, PATH_MAX, "%s/%s",dir_name,dirEntry->d_name);
            lstat (Name, &inode);
            if (S_ISDIR(inode.st_mode) && strcmp(dirEntry->d_name,".") !=0 && strcmp(dirEntry->d_name,"..") !=0 ){
                permissions_tree(Name,p);
            }
        }
       
    }
    return 0;
}
int valid_file(char * file_name){
    int fd;
    char *magic = (char*)malloc(sizeof(char) * 5);
    int header_size = 0;
    int version = 0;
    int nr_of_sections = 0;
    fd = open(file_name,O_RDONLY);
    if(fd < 0){
        perror("open");
        return -1;
    }
    lseek(fd,-4,SEEK_END);
    read(fd,magic,4);
    if(strcmp(magic,"iXIO") != 0){
        printf("ERROR\n");
        printf("wrong magic\n");
        close(fd);
        free(magic);
        return -1;
    }
    free(magic);
    lseek(fd,-6,SEEK_END);
    read(fd,&header_size,2);
    lseek(fd,-header_size,SEEK_END);
    read(fd,&version,2);
    if(version < 92 || version > 158){
        printf("ERROR\n");
        printf("wrong version\n");
        close(fd);
        return -1;
    }
    read(fd,&nr_of_sections,1);
    if(nr_of_sections < 3 || nr_of_sections > 20){
        printf("ERROR\n");
        printf("wrong sect_nr\n");
        close(fd);
        return -1;
    }
    Section_header *headers = (Section_header*)malloc(sizeof(Section_header)*nr_of_sections);
    int i;
    for(i=0;i<nr_of_sections;i++){
        headers[i].name = (char*)malloc(sizeof(char)*13);
        read(fd,headers[i].name,13);
        read(fd,&headers[i].type,1);
        if(headers[i].type != 65 && headers[i].type != 87 && headers[i].type != 55 && headers[i].type != 89 && headers[i].type != 43){
            printf("ERROR\n");
            printf("wrong sect_types\n");
            close(fd);
            return -1;
        }
        read(fd,&headers[i].offset,4);
        read(fd,&headers[i].size,4);
    }
    printf("SUCCESS\n");
    printf("version=%d\n",version);
    printf("nr_sections=%d\n",nr_of_sections);
    for(i=0;i<nr_of_sections;i++){
        printf("section%d: %s %d %d\n",i+1,headers[i].name,headers[i].type,headers[i].size);
        
    }
    close(fd);
    return 0;
}
int valid_sections(char* file_name){
    int fd;
    char *magic = (char*)malloc(sizeof(char) * 5);
    int header_size = 0;
    int version = 0;
    int nr_of_sections = 0;
    fd = open(file_name,O_RDONLY);
    if(fd < 0){
        perror("open");
        return -1;
    }
    lseek(fd,-4,SEEK_END);
    read(fd,magic,4);
    if(strcmp(magic,"iXIO") != 0){
        close(fd);
        free(magic);
        return -1;
    }
    free(magic);
    lseek(fd,-6,SEEK_END);
    read(fd,&header_size,2);
    lseek(fd,-header_size,SEEK_END);
    read(fd,&version,2);
    if(version < 92 || version > 158){
        close(fd);
        return -1;
    }
    read(fd,&nr_of_sections,1);
    if(nr_of_sections < 3 || nr_of_sections > 20){
        close(fd);
        return -1;
    }
    int section87=0;
    Section_header *headers = (Section_header*)malloc(sizeof(Section_header)*nr_of_sections);
    int i;
    for(i=0;i<nr_of_sections;i++){
        headers[i].name = (char*)malloc(sizeof(char)*13);
        read(fd,headers[i].name,13);
        read(fd,&headers[i].type,1);
        if(headers[i].type != 65 && headers[i].type != 87 && headers[i].type != 55 && headers[i].type != 89 && headers[i].type != 43){
            close(fd);
            return -1;
        }
        if(headers[i].type == 87){
            section87 ++;
        }
        read(fd,&headers[i].offset,4);
        read(fd,&headers[i].size,4);
    }
    if(section87 < 2){
        return -1;
    }
    return 0;
}
int findall(char* dir_name){
    DIR* dir;
    struct dirent *dirEntry;
    struct stat inode;
    char *Name = (char*)malloc(sizeof(char)* PATH_MAX);
    dir=opendir(dir_name);
    if (dir == 0) {
		perror ("Error opening directory");
		exit(4);
	}
    while((dirEntry=readdir(dir)) != 0){
        snprintf(Name, PATH_MAX, "%s/%s",dir_name,dirEntry->d_name);
        lstat (Name, &inode);
        if (S_ISREG(inode.st_mode)){
           int valid = valid_sections(Name);
           if(valid == 0){
               printf("%s\n",Name);
           }
        }
    }
    free(dirEntry);
    free(Name);
    closedir(dir);
    return 0;
}
int findall_rec(char * dir_name){
    DIR* dir;
    struct dirent *dirEntry;
    struct stat inode;
    char *Name = (char*)malloc(sizeof(char)*PATH_MAX);
    dir=opendir(dir_name);
    if (dir == 0) {
		perror ("Error opening directory");
		exit(4);
	}
    int searched = findall(dir_name);
    if(searched == -1){
        closedir(dir);
        return 0;
    }else{
        while((dirEntry=readdir(dir)) != 0){
            snprintf(Name, PATH_MAX, "%s/%s",dir_name,dirEntry->d_name);
            lstat (Name, &inode);
            if (S_ISDIR(inode.st_mode) && strcmp(dirEntry->d_name,".") !=0 && strcmp(dirEntry->d_name,"..") !=0 ){
                findall_rec(Name);
            }
        }
       
    }
    return 0;
}
int extraction(char* file_name, int section, int line){
    int fd;
    char *magic = (char*)malloc(sizeof(char) * 5);
    int header_size = 0;
    int version = 0;
    int nr_of_sections = 0;
    fd = open(file_name,O_RDONLY);
    if(fd<0){
        perror("open");
        return -1;
    }
    lseek(fd,-4,SEEK_END);
    read(fd,magic,4);
    if(strcmp(magic,"iXIO") != 0){
        printf("ERROR\n");
        printf("wrong magic\n");
        close(fd);
        free(magic);
        return -1;
    }
    free(magic);
    lseek(fd,-6,SEEK_END);
    read(fd,&header_size,2);
    lseek(fd,-header_size,SEEK_END);
    read(fd,&version,2);
    if(version < 92 || version > 158){
        printf("ERROR\n");
        printf("wrong version\n");
        close(fd);
        return -1;
    }
    read(fd,&nr_of_sections,1);
    if(nr_of_sections < 3 || nr_of_sections > 20){
        printf("ERROR\n");
        printf("wrong sect_nr\n");
        close(fd);
        return -1;
    }
    Section_header *headers = (Section_header*)malloc(sizeof(Section_header)*nr_of_sections);
    int i;
    for(i=0;i<nr_of_sections;i++){
        headers[i].name = (char*)malloc(sizeof(char)*13);
        read(fd,headers[i].name,13);
        read(fd,&headers[i].type,1);
        if(headers[i].type != 65 && headers[i].type != 87 && headers[i].type != 55 && headers[i].type != 89 && headers[i].type != 43){
            printf("ERROR\n");
            printf("wrong sect_types\n");
            close(fd);
            return -1;
        }
        read(fd,&headers[i].offset,4);
        read(fd,&headers[i].size,4);
    }
    if( section > nr_of_sections){
        printf("ERROR\n");
        printf("invalid section\n");
        return -1;
    }
    lseek(fd,0,SEEK_SET);
    int start = 0;
    int size = 0;
    bool end_of_line;
    start = headers[section - 1].offset;
    lseek(fd,start,SEEK_SET);
    int end = 0;
    end = headers[section - 1].size;
    int nr_of_lines = 1;
    while(size < end){

        end_of_line = false;
        while(end_of_line == false && size < end){
            char c;
            int size_read = read(fd,&c,sizeof(c));
            if(size_read < 0){
                perror("Can't read");
                return -1;
            }
            if(size_read == 0)
            {
                exit(1);
            }
            size+=size_read;
            if(c == '\r'){
                read(fd,&c,sizeof(c));
                size++;
                if(c == '\n'){
                    end_of_line = true;
                }
            }
        }
        nr_of_lines ++;
    }
    if(line > nr_of_lines){
        printf("ERROR\n");
        printf("invalid line\n");
        return -1;
    }
    int current_line = nr_of_lines - line;
    lseek(fd,start,SEEK_SET);
    size = 0;
    int reverse = 1;
    while(reverse < current_line){

        end_of_line = false;
        while(end_of_line == false){
            char c;
            int size_read = read(fd,&c,sizeof(c));
            if(size_read < 0){
                perror("Can't read");
                exit(1);
            }
            size+=size_read;
            if(c == '\r'){
                read(fd,&c,sizeof(c));
                size++;
                if(c == '\n'){
                    end_of_line = true;
                }
            }
        }
        reverse ++;
    }
    char* print_line = (char*)malloc(sizeof(char)*300000);
    end_of_line = false;
    while(end_of_line == false){
        char c;
        int size_read = read(fd,&c,sizeof(c));
        if(size_read < 0){
            perror("Can't read");
            return -1;
        }
        size+=size_read;
        strcat(print_line,&c);
        if(c == '\r'){
            read(fd,&c,sizeof(c));
            size++;
            if(c =='\n'){
                end_of_line = true;
            }
        }
    }
    char * line_print = (char*)malloc(sizeof(char)*300000);
    int lp=0;
    for(i=strlen(print_line) - 1; i>=0 ;i--){
        line_print[lp] = print_line[i];
        lp++;
    }
    printf("SUCCESS\n");
    printf("%s\n",line_print);
    close(fd);
    free(line_print);
    free(print_line);
    return 0;
}
int main(int argc, char **argv){

    int listIndex = 0;
    int pathIndex = 0;
    int filterIndex = 0;
    int parseIndex = 0;
    int extractIndex = 0;
    int sectionIndex = 0;
    int lineIndex = 0;
    int findallIndex = 0;
    bool isRecursive = false;
    bool hasFiltering = false;
    if(argc == 2){
        if(strcmp(argv[1], "variant") == 0){
            printf("52365\n");
        }
    }
    else if(argc == 3 || argc == 4 || argc == 5)
    {
        for(int i = 1; i<argc;i++){
            if(strcmp(argv[i],"list") == 0){
                listIndex = i;
            }
            else if(strstr(argv[i],"path") != 0){
                pathIndex = i;
            }
            else if(strcmp(argv[i],"recursive") == 0){
                isRecursive = true;
            }
            else if(strstr(argv[i],"size") != 0 || strstr(argv[i],"permissions") != 0){
                hasFiltering = true;
                filterIndex = i;
            }
            
            else if(strcmp(argv[i],"parse") == 0){
                parseIndex = i;
            }
            
            else if(strcmp(argv[i],"findall") == 0){
                findallIndex = i;
            }
            
            else if(strcmp(argv[i],"extract") == 0){
                extractIndex = i;
            }
            
            else if(strstr(argv[i],"section") != 0){
                sectionIndex = i;
            }
            else if(strstr(argv[i],"line") != 0){
                lineIndex = i;
            }  
        }
        if(listIndex != 0){
            if(isRecursive == false && hasFiltering == false){
                char *p = strtok(argv[pathIndex],"=");
                p = strtok(NULL,"=");
                struct stat fileMetadata;
                stat(p,&fileMetadata);
                if (S_ISDIR(fileMetadata.st_mode)) { 
                    printf("SUCCESS\n");
		            search_dir(p);
	            }else {
                    printf("ERROR\n");
	    	        printf("invalid directory path\n");
	            }
            }
            else if(isRecursive && hasFiltering == false){
                char *p = strtok(argv[pathIndex],"=");
                p = strtok(NULL,"=");
                struct stat fileMetadata;
                stat(p,&fileMetadata);
                if (S_ISDIR(fileMetadata.st_mode)) { 
                    printf("SUCCESS\n");
		            search_tree(p);
		        
	            }else {
	    	        printf("ERROR\n");
	    	        printf("invalid directory path\n");
	            }
            }
            if(hasFiltering == true){
                    if(isRecursive == false){
                        char *p = strtok(argv[pathIndex],"=");
                        p = strtok(NULL,"=");
                        struct stat fileMetadata;
                        stat(p,&fileMetadata);
                        if (S_ISDIR(fileMetadata.st_mode)) { 
                            char *aux = strtok(argv[filterIndex],"=");
                            if(strstr(aux,"size") != 0){
                                aux = strtok(NULL,"=");
                                int size = atoi(aux);
                                printf("SUCCESS\n");
                                size_smaller(p,size);
                            }
                            else if(strcmp(aux,"permissions") == 0){
                                aux = strtok(NULL,"=");
                                printf("SUCCESS\n");
                                permissions_files(p,aux);
                            }
	                    }else {
	    	                printf("ERROR\n");
	    	                printf("invalid directory path\n");
	                    }
                    }
                    else if(isRecursive){
                        char *p = strtok(argv[pathIndex],"=");
                        p = strtok(NULL,"=");
                        struct stat fileMetadata;
                        stat(p,&fileMetadata);
                        if (S_ISDIR(fileMetadata.st_mode)) { 
                            char *aux = strtok(argv[filterIndex],"=");
                            if(strstr(aux,"size") != 0){
                                aux = strtok(NULL,"=");
                                int size = atoi(aux);
                                printf("SUCCESS\n");
                                size_smaller_rec(p,size);
                            }
                            else if(strcmp(aux,"permissions") == 0){
                                aux = strtok(NULL,"=");
                                printf("SUCCESS\n");
                                permissions_tree(p,aux);
                            }
	                    }else {
	    	                printf("ERROR\n");
	    	                printf("invalid directory path\n");
	                    }
                    }
                }
            
        }
        else if(parseIndex != 0){
            char *p = strtok(argv[pathIndex],"=");
            p = strtok(NULL,"=");
            struct stat fileMetadata;
            stat(p,&fileMetadata);
            if(S_ISREG(fileMetadata.st_mode)){
                valid_file(p);
            }else{
                printf("%s is not a file\n",p);
            }
        }
        else if(findallIndex != 0){
            char *p = strtok(argv[pathIndex],"=");
            p = strtok(NULL,"=");
            struct stat fileMetadata;
            stat(p,&fileMetadata);
            if(S_ISDIR(fileMetadata.st_mode)){
                printf("SUCCESS\n");
                findall_rec(p);
            }else{
                printf("ERROR\n");
                printf("invalid directory path\n");
            }
        }
        else if(extractIndex != 0){
            char *p = strtok(argv[pathIndex],"=");
            p = strtok(NULL,"=");
            struct stat fileMetadata;
            stat(p,&fileMetadata);
            if(S_ISREG(fileMetadata.st_mode)){
                char *aux1=strtok(argv[sectionIndex],"=");
                aux1=strtok(NULL,"=");
                int section = atoi(aux1);
                char *aux2=strtok(argv[lineIndex],"=");
                aux2=strtok(NULL,"=");
                int line = atoi(aux2);
                extraction(p,section,line);
            }else{
                printf("ERROR\n");
                printf("invalid file\n");
            }
        }
    }
    return 0;
}