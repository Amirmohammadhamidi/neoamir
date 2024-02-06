#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<time.h>
#include<utime.h>
char neogit_project_location[60];
char lastfolder[90];
char time_path[90];
    int check_exist(char searchfile[],char orgfile[]){
        char search[80];
        strcpy(search,searchfile);
        strcat(search,"\n");
        FILE * file = fopen(orgfile,"r");
        char pathes[80];
        int flag=0;
        while(fgets(pathes,80,file)){
            if(strcmp(pathes,search)==0){
                flag++;
                break;
            }
        }
        fclose(file);
        return flag;
    }
    int find_file(char file_name[]){
    char filecopy[50];
    strcpy(filecopy,file_name);
    strcat(filecopy,"\n");
    int test=0;
    system("touch details.txt");
    system("ls -a > details.txt");
    FILE * file = fopen("details.txt","r");
    char search[50];
    while(fgets(search,50,file)){
        if(strcmp(search,filecopy)==0){
            test++;
            break;
        }
    }
    fclose(file);
    system("rm details.txt");
    return test;
    }
// it has our present project .neogit folder location;
    int check_neogit(){
    int test=0;
    system("touch details.txt");
    system("ls -a > details.txt");
    FILE * file = fopen("details.txt","r");
    char search[70];
    while(fgets(search,70,file)){
        if(strcmp(search,".neogit\n")==0){
            // save our present neogit folder
            system("touch loc.txt");
            system("pwd > loc.txt");
            FILE * loc = fopen("loc.txt","r");
            fgets(neogit_project_location,60,loc);
            int len  = strlen(neogit_project_location);
            neogit_project_location[len-1]='\0';
            fclose(loc);
            system("rm loc.txt");
            //finish;
            test++;
            break;
        }
    }
    if(test==1){
        fclose(file);
        system("rm details.txt");
        return 0;
    }
    fclose(file);
    system("rm details.txt");
    return 1;
    }   
    int check_root(){
    system("touch details.txt");
    system("pwd > details.txt");
    FILE * file = fopen("details.txt","r");
    char search[60];
    fgets(search,60,file);
    if(strcmp(search,"/\n")==0){
        fclose(file);
        system("rm details.txt");
        return 0;
    }else{
        fclose(file);
        system("rm details.txt");
        return 1;
    }
    }
    int testproject(){
    int test=0;
    int flag=0;
    while(1){
        if(check_neogit()==0){
            flag++;
            break;
        }else if(check_root()==0){
            test++;
        }
        if(test>1){
            break;
        }   
        chdir("..");
    }
    return flag;
    }
    int assign_local(char * type , char * name){
    if(testproject()==0){
        printf("you didn't make any new project\n");
        return 0;
    }
    else if(strcmp(type,"user.name")==0){
        chdir(neogit_project_location);
        chdir(".neogit");
        FILE * file = fopen("account_name.txt","w");
        fprintf(file,"%s",name);
        fclose(file);
    }else if(strcmp(type, "user.email")==0){
        chdir(neogit_project_location);
        chdir(".neogit");
        FILE * file = fopen("account_email.txt","w");
        fprintf(file,"%s",name);
        fclose(file);
    }   
    return 1;
    }
    void assign_global(char * type , char * name){
        char search[70];
        if(strcmp(type,"user.name")==0){
            FILE * global_username = fopen("/home/.neogit_app/global_names.txt","w");
            fprintf(global_username , "%s", name);
            fclose(global_username);
        }else if(strcmp(type,"user.email")==0){
            FILE * global_useremail=fopen("/home/.neogit_app/global_emails.txt","w");
            fprintf(global_useremail , "%s", name);
            fclose(global_useremail);
        } 
    }
//it is ok!
    int checkdirectory(char * path){
        struct stat filestat ;
        if(stat(path,&filestat)==0){
            if(S_ISDIR(filestat.st_mode)){
                return 1;
            }
            return 0;
        }
        return -1;
    }
    int copy_mtime_file(char path1[],char path2[]){
    //it copy mtime of path1 to path2:
        struct utimbuf new_times;
        struct stat filestat1;
        struct stat filestat2;
        if (stat(path1, &filestat1) != 0) {
            perror("Error getting file status");
            return 1;
        }
        if (stat(path2, &filestat2) != 0) {
            perror("Error getting file status");
            return 1;
        }
        new_times.actime = filestat2.st_atime;  
        new_times.modtime = filestat1.st_mtime; 
        if (utime(path2, &new_times) != 0) {
            perror("Error setting file modification time");
            return 1;
        }
        return 0;
    }
    int copy_mtime_folder(char path1[],char path2[]){
        //i want to copy path1 to path2 :
        //we assign last modify time of all subfolders of path1 to path2;
        //that means all subfolders of path1 in any depth are same in path2:
        chdir(path1);
        system("ls -a > path1_details.txt");
        FILE * path1_details = fopen("path1_details.txt","r");
        char search[50];
        char files[60][50];
        int count=0;
        while(fgets(search,50,path1_details)){
            char search_copy[90];
            strcpy(search_copy,search);
            int len = strlen(search_copy);
            search_copy[len-1]='\0';
            strcpy(files[count],search_copy);
            count++;
        }
        fclose(path1_details);
        system("rm path1_details.txt");
        for(int i=0;i<count;i++){
            char file_path1[90];
            strcpy(file_path1,path1);
            strcat(file_path1,"/");
            strcat(file_path1,files[i]);
            char file_path2[90];
            strcpy(file_path2,path2);
            strcat(file_path2,"/");
            strcat(file_path2,files[i]);
            if((checkdirectory(file_path1)==0)&&(strcmp(files[i],"path1_details.txt")!=0)){
                copy_mtime_file(file_path1,file_path2);
            }else if(((checkdirectory(file_path1)==1)&&(strcmp(files[i],".")!=0))&&(strcmp(files[i],"..")!=0)){
                chdir(file_path1);
                copy_mtime_folder(file_path1,file_path2);
                chdir("..");
            }   
        }
        return 1;
    }
//it is ok!
    int checktracked_files(char file[]){
        char location[90];
        strcpy(location,neogit_project_location);
        strcat(location,"/.neogit/staged_files/trackted_files.txt");
        FILE * trackted_files = fopen(location,"r");
        char search[80];
        int flag=0;
        while(fgets(search,80,trackted_files)){
            if(strcmp(file,search)==0){
                flag++;
                break;
            }
        }
        fclose(trackted_files);
        return flag;
    }
    void writetrcktedfiles(char file[]){
        char location[90];
        strcpy(location,neogit_project_location);
        strcat(location,"/.neogit/staged_files/trackted_files.txt");
        FILE *trackted_files = fopen(location,"a");
        fprintf(trackted_files,"%s",file);
        fclose(trackted_files);
    }
    int deleted_added_files(){
        if(checkdirectory("/home/.neogit_app/deleted_files.txt")==-1)
        return 0;
        // now we get our present path:
        int p_len = strlen(neogit_project_location);
        FILE * deleted_files = fopen("/home/.neogit_app/deleted_files.txt","r");
        char search[90];
        char files_path[90];
        char command[100];
        while(fgets(search,90,deleted_files)){
            int len = strlen(search);
            search[len-1]='\0';
            strcpy(files_path,search);
            strcpy(files_path,files_path+p_len+1);
            strcpy(command,"rm ");
            strcat(command,files_path);
            printf("%s",command);
            system(command);
        }
        system("rm /home/.neogit_app/deleted_files.txt");
    }   
    int check_deleted_files(char path[]){
        char trackted_files[90];
        strcpy(trackted_files,neogit_project_location);
        strcat(trackted_files,"/.neogit/staged_files/trackted_files.txt");
        FILE * trackt = fopen(trackted_files,"r");
        char search[100];
        char pathcopy[100];
        strcpy(pathcopy,path);
        strcat(pathcopy,"\n");
        int flag=0;
        while(fgets(search,100,trackt)){
            if(strcmp(search,pathcopy)==0){
                flag++;
            }
        }
        fclose(trackt);
        if(flag==0)   return flag;
        FILE * deleted_files = fopen("/home/.neogit_app/deleted_files.txt","a");
        fprintf(deleted_files,"%s",pathcopy);
        fclose(deleted_files);
        return flag;
    }
    int modifyfile(char file[],int mode){
    //file is path of our file;strtok
        char path[80];
        char location[80];
        system("touch path.txt");
        system("pwd > path.txt");
        FILE * pathfile = fopen("path.txt","r");
        fgets(path,80,pathfile);
        fclose(pathfile);
        system("rm path.txt");
        int len = strlen(path);
        path[len-1]='/';
        strcpy(location,path);
        strcat(path,file);
        char main_path[80];
        strcpy(main_path,path);
        struct stat filestat ;
        if(stat(path , &filestat)==0){
            strcat(path,"\n");
            time_t present = filestat.st_mtime;
            char timelineloc[80];
            char temp[80];
            strcpy(timelineloc,neogit_project_location);
            strcat(timelineloc,"/.neogit/commits/timeline.txt");
            FILE * timeline = fopen(timelineloc,"r");
            char search[70];
            int flag=0;
            while(fgets(search,70,timeline)){
                if(strcmp(search,path)==0){
                    fgets(search,70,timeline);
                    if(strcmp(search,ctime(&present))!=0){
                        flag++;
                    }
                    flag++;
                }
            }
            if(((flag==0||flag==2)||(checktracked_files(path)==0))||(mode==1)){
                if(checktracked_files(path)==0){
                    writetrcktedfiles(path);
                }
                char loc[80];
                strcpy(loc,neogit_project_location);
                strcat(loc,"/.neogit/staged_files");
                char COMMAND[180];
                strcpy(COMMAND,"cp ");
                strcat(COMMAND,main_path);
                strcat(COMMAND," ");
                strcat(COMMAND,loc);
                system(COMMAND);
                //now similarize mtime of this two :
                char file_staged_location[90];
                strcpy(file_staged_location,loc);
                strcat(file_staged_location,"/");
                // now we should get the last part of file(word) witch tokinized with '/'
                char file_copy[50];
                strcpy(file_copy,file);
                char piece[20];
                char * var = strtok(file,"/");
                while(1){
                    strcpy(piece,var);
                    var = strtok(NULL,"/");
                    if(var == NULL) break;
                }
                strcat(file_staged_location,piece);
                if(copy_mtime_file(main_path,file_staged_location)){
                    printf("error 1\n");
                }
                //finished;
                strcat(loc,"/staged_files.txt");
                FILE * staged_files = fopen(loc,"a");
                fprintf(staged_files,"%s",path);
                fprintf(staged_files,"%s",ctime(&present));
                fclose(staged_files);
            }
            fclose(timeline);
            chdir(location);
            return 1;
        }else{
            perror("Error");
            return -1;
        }
    }
    //it is ok!
    int modify(int mode){
        int count=0;
        system("touch file.txt");
        system("ls -a > file.txt");
        FILE * files = fopen("file.txt","r");
        char search[50];
        char filenames[50][50];
        while(fgets(search,50,files)){
            if(strcmp(search,"file.txt\n")!=0){
            strcpy(filenames[count],search);
            count++;
            }
        }
        fclose(files);
        system("rm file.txt");
        for(int i=0;i<count;i++){
            int len = strlen(filenames[i]);
            filenames[i][len-1]='\0';
            char path[90];
            system("touch path.txt");
            system("pwd > path.txt");
            FILE * pathfile = fopen("path.txt","r");
            fgets(path,90,pathfile);
            int length = strlen(path);
            path[length-1]='/';
            strcat(path,filenames[i]);
            fclose(pathfile);
            system("rm path.txt");
            //path is compoletely true;
            int type = checkdirectory(path);
            char Astage[80];
            strcpy(Astage,neogit_project_location);
            strcat(Astage,"/.neogit/staged_files/Astagedfiles.txt");
            if((((type==1)&&(strcmp(filenames[i],".neogit")!=0))&&(strcmp(filenames[i],".")!=0))&&(strcmp(filenames[i],"..")!=0)){
                if(check_exist(path,Astage)==0){
                    FILE * add = fopen(Astage,"a");
                    fprintf(add,"%s",path);
                    fprintf(add,"\n");
                    fclose(add);
                }
                chdir(path);
                modify(mode);
                chdir("..");
            }else if (((strcmp(filenames[i],".neogit")!=0))&&(type==0)){
                if(check_exist(path,Astage)==0){
                    FILE * add = fopen(Astage,"a");
                    fprintf(add,"%s",path);
                    fprintf(add,"\n");
                    fclose(add);
                    modifyfile(filenames[i],mode);
                }
            }
        }
    }
    //it is ok!
    int  add_file(char filepath[],int add_mode){
        char permisson[5];
        if(checkdirectory("/home/.neogit_app/permisson.txt")==0){
            printf("you don't have access to add any file\n");
            printf("you should checkout to HEAD commit first!\n");
        }
        char path[90];
        char Astage[80];
        system("touch path.txt");
        system("pwd > path.txt");
        FILE * pathfile = fopen("path.txt","r");
        fgets(path,90,pathfile);
        fclose(pathfile);
        system("rm path.txt");
        int len = strlen(path);
        path[len-1]='\0';
        strcat(path,"/");
        strcat(path,filepath);  
        strcpy(Astage,neogit_project_location);
        strcat(Astage,"/.neogit/staged_files/Astagedfiles.txt");
        if(checkdirectory(path)==1){  
            if(check_exist(path,Astage)==0){
                FILE * add = fopen(Astage,"a");
                fprintf(add,"%s",path);
                fprintf(add,"\n");
                fclose(add);
            }
            chdir(path);
            modify(add_mode);
            chdir("..");
        }else if(checkdirectory(path)==0){
            if(check_exist(path,Astage)==0){
                FILE * add = fopen(Astage,"a");
                fprintf(add,"%s",path);
                fprintf(add,"\n");
                fclose(add);
                modifyfile(filepath,add_mode);
            }
        }else{
            //check trackted files:
            if(check_deleted_files(path)==0){
                printf("there isn't any file or directory with this name");
            }
            return 0;
        }
            return 1;
    }
    //it is ok!
    void search_name(char * path){
        char location[80];
        char pathcopy[80];
        strcpy(pathcopy,path);
        strcat(pathcopy,"\n");
        strcpy(location,neogit_project_location);
        strcat(location,"/.neogit/staged_files/Astagedfiles.txt");
        FILE * Astagedfiles = fopen(location,"r");
        char search[80];
        int flag=0;
        while(fgets(search,80,Astagedfiles)){
            if(strcmp(pathcopy,search)==0){
                flag++;
                printf("%s",path);
                printf(" is in staging area.\n");
                break;
            }
        }
        if(flag==0){
            printf("%s",path);
            printf(" isn't in staging area.\n");
        }
    }
    void search_stagedfiles(int depth){
        int count=0;
        system("touch file.txt");
        system("ls -a > file.txt");
        FILE * files = fopen("file.txt","r");
        char search[50];
        char filenames[50][50];
        while(fgets(search,50,files)){
            if(strcmp(search,"file.txt\n")!=0){
            strcpy(filenames[count],search);
            count++;
            }
        }
        fclose(files);
        system("rm file.txt");
        for(int i=0;i<count;i++){
            int len = strlen(filenames[i]);
            filenames[i][len-1]='\0';
            char path[90];
            system("touch path.txt");
            system("pwd > path.txt");
            FILE * pathfile = fopen("path.txt","r");
            fgets(path,90,pathfile);
            int length = strlen(path);
            path[length-1]='/';
            strcat(path,filenames[i]);
            fclose(pathfile);
            system("rm path.txt");
            //path is compoletely true;
            int type = checkdirectory(path);
            if(((((type==1)&&(strcmp(filenames[i],".neogit")!=0))&&(strcmp(filenames[i],".")!=0))&&(strcmp(filenames[i],"..")!=0))&&(depth>1)){
                search_name(path);
                chdir(path);
                search_stagedfiles(depth-1);
                chdir("..");
            }
            else if(((((type==1)&&(strcmp(filenames[i],".neogit")!=0))&&(strcmp(filenames[i],".")!=0))&&(strcmp(filenames[i],"..")!=0))&&(depth==1)){
                search_name(path);
            }
            else if (((strcmp(filenames[i],".neogit")!=0))&&(type==0)){
                search_name(path);
            }
        }
    }
    void addredo(char path[]){
        char loc[80];
        strcpy(loc,neogit_project_location);
        strcat(loc,"/.neogit/staged_files/reset.txt");
        FILE * reset = fopen(loc,"a");
        fprintf(reset,"%s",path);
        fclose(reset);
    }
    int delstage(char path[] , int mode){
        char loc[100];
        char temploc[100];
        char pathcopy[100];
        strcpy(pathcopy,path);
        strcat(pathcopy,"\n");
        strcpy(loc,neogit_project_location);
        strcat(loc,"/.neogit/staged_files/Astagedfiles.txt");
        FILE * Astagedfiles = fopen(loc,"r");
        strcpy(temploc,neogit_project_location);
        strcat(temploc,"/.neogit/staged_files/temp.txt");
        FILE * temp = fopen(temploc,"w");
        char search[100];
        while(fgets(search,100,Astagedfiles)){
            if(strcmp(search,pathcopy)!=0){
                fprintf(temp,"%s",search);
            }
        }
        fclose(Astagedfiles);
        fclose(temp);
        char remove[200];
        strcpy(remove,"rm ");
        strcat(remove,loc);
        system(remove);
        char rename[200];
        strcpy(rename,"mv ");
        strcat(rename,temploc);
        strcat(rename," ");
        strcat(rename,loc);
        system(rename);
        if(mode==0){
            char stageloc[80];
            char stemploc[80];
            strcpy(stageloc,neogit_project_location);
            strcat(stageloc,"/.neogit/staged_files/staged_files.txt");
            FILE * staged_files = fopen(stageloc,"r");
            strcpy(stemploc,neogit_project_location);
            strcat(stemploc,"/.neogit/staged_files/stempfiles.txt");
            FILE * stemp = fopen(stemploc,"w");
            char search[80];
            while(fgets(search,80,staged_files)){
                if(strcmp(search,pathcopy)==0){
                    addredo(pathcopy);
                    fgets(search,80,staged_files);
                    continue;
                }
                fprintf(stemp,"%s",search);
            }
            fclose(staged_files);
            fclose(stemp);
            char sremove[90];
            strcpy(sremove,"rm ");
            strcat(sremove,stageloc);
            system(sremove);
            char srename[120];
            strcpy(srename,"mv ");
            strcat(srename,stemploc);
            strcat(srename," ");
            strcat(srename,stageloc);
            system(srename);
            //remove file from staged_files folder:
            char COMMAND[100];
            strcpy(COMMAND,"rm ");
            strcat(COMMAND,neogit_project_location);
            strcat(COMMAND,"/.neogit/staged_files/");
            char * ptr = strtok(path,"/");
            char file[80];
            while(1){
                strcpy(file,ptr);
                ptr = strtok(NULL,"/");
                if(ptr==NULL) break;
            }
            strcat(COMMAND,file);
            system(COMMAND);
        }
    }
    void reset(char apath[]){
            //now we get our fullpath of file;
        if(checkdirectory(apath)==1){
            delstage(apath,1);
            chdir(apath);
            int count=0;
            system("touch file.txt");
            system("ls -a > file.txt");
            FILE * files = fopen("file.txt","r");
            char search[80];
            char filenames[80][80];
            while(fgets(search,80,files)){
                if(strcmp(search,"file.txt\n")!=0){
                    strcpy(filenames[count],search);
                    count++;
                    }
                }
            fclose(files);
            system("rm file.txt");
            for(int i=0 ; i<count ; i++){
                int len = strlen(filenames[i]);
                filenames[i][len-1]='\0';
                char path[90];
                system("touch path.txt");
                system("pwd > path.txt");
                FILE * pathfile = fopen("path.txt","r");
                fgets(path,90,pathfile);
                fclose(pathfile);
                system("rm path.txt");
                int length = strlen(path);
                path[length-1]='/';
                strcat(path,filenames[i]);
                if(((strcmp(filenames[i],".neogit")!=0)&&(strcmp(filenames[i],".")!=0))&&(strcmp(filenames[i],"..")!=0))
                reset(path);
            }
        }else if(checkdirectory(apath)==0){
            delstage(apath,0);
        }else{
            printf("there isn't any file or directory with this name");
        }
    }
    int check_stagedfiles(char path[]){
        strcat(path,"\n");
        char slocation[90];
        strcpy(slocation,neogit_project_location);
        strcat(slocation,"/.neogit/staged_files/staged_files.txt");
        if(checkdirectory(slocation)==-1){
            return -1;
        }
        FILE * staged_files = fopen(slocation,"r");
        char search[80];
        int flag=0;
        while(fgets(search,80,staged_files)){
            if(strcmp(search,path)==0){
                fgets(search,80,staged_files);
                strcpy(time_path,search);
                return 1;
            }
            flag++;
        }
        fclose(staged_files);
        if(flag==0){
            return 0;
        }
    }
    void search_timeline(char path[]){
        int flag=0;
        strcat(path,"\n");
        char search[80];
        char aloc[80];
        strcpy(aloc,neogit_project_location);
        strcat(aloc,"/.neogit/commits/timeline.txt");
        FILE * timeline = fopen(aloc,"r");
        while(fgets(search,80,timeline)){
            if(strcmp(search,path)==0){
                flag++;
            }
        }
        fclose(timeline);
        if(flag==0){
            int len = strlen(path);
            path[len-1]='\0';
            printf("%s : ",path);
            if(check_stagedfiles(path)==1)
            printf("+A\n");
            else
            printf("-A\n");
        }
    }
    void check_A(){
        int count=0;
        system("touch file.txt");
        system("ls -a > file.txt");
        FILE * files = fopen("file.txt","r");
        char search[50];
        char filenames[50][50];
        while(fgets(search,50,files)){
            if(strcmp(search,"file.txt\n")!=0){
            strcpy(filenames[count],search);
            count++;
            }
        }
        fclose(files);
        system("rm file.txt");
        for(int i=0;i<count;i++){
            int len = strlen(filenames[i]);
            filenames[i][len-1]='\0';
            char path[90];
            system("touch path.txt");
            system("pwd > path.txt");
            FILE * pathfile = fopen("path.txt","r");
            fgets(path,90,pathfile);
            int length = strlen(path);
            path[length-1]='/';
            strcat(path,filenames[i]);
            fclose(pathfile);
            system("rm path.txt");
            //path is compoletely true;
            int type = checkdirectory(path);
            if((((type==1)&&(strcmp(filenames[i],".neogit")!=0))&&(strcmp(filenames[i],".")!=0))&&(strcmp(filenames[i],"..")!=0)){
                chdir(path);
                check_A();
                chdir("..");
            }
            else if (((strcmp(filenames[i],".neogit")!=0))&&(type==0)){
                search_timeline(path);
            }
        }
    }
    void status(char alocation[]){
        testproject();
        FILE * timeline = fopen(alocation,"r");
        char search[80];
        char pathcopy1[90];
        char pathcopy2[90];
        char path[90];
        char pathcopy[90];
        char abbrevation[90];
        strcpy(abbrevation,neogit_project_location);
        int a_len = strlen(abbrevation);
        abbrevation[a_len-1]='\0';
        while(fgets(search,80,timeline)){
            strcpy(path,search);
            int length = strlen(path);
            path[length-1]='\0';
            strcpy(pathcopy1,path);
            strcpy(pathcopy2,path);
            strcpy(pathcopy,path);
            strcpy(pathcopy,pathcopy+a_len);
            printf("%s : ",pathcopy);
            if(checkdirectory(path)==-1){
                fgets(search,80,timeline);
                if(check_stagedfiles(pathcopy1)==1){
                    printf("+");
                } else if((check_stagedfiles(pathcopy2)==-1)||(check_stagedfiles(pathcopy2)==0)){
                    printf("-");
                }
                printf("D\n");
            }else{
                struct stat filestat;
                if(stat(path,&filestat)==0){
                    time_t present = filestat.st_mtime;
                    fgets(search,80,timeline);
                    if(strcmp(search,ctime(&present))!=0){
                        if(check_stagedfiles(pathcopy1)==1){
                            printf("+");
                        } else if((check_stagedfiles(pathcopy2)==-1)||(check_stagedfiles(pathcopy2)==0)){
                            printf("-");
                        }
                        printf("M\n");
                    }else{
                        int flag=0;
                        if(check_stagedfiles(pathcopy1)==1){
                            flag++;
                        }                 
                        if(flag==1){
                            printf("+A\n");
                        }else{
                            printf("not modified\n");
                        }
                    }
                }
            }
        }
        fclose(timeline);
        check_A();
    }
    void writetimeline(){
        int count=0;
        system("touch file.txt");
        system("ls -a > file.txt");
        FILE * files = fopen("file.txt","r");
        char search[80];
        char filenames[80][80];
        while(fgets(search,80,files)){
            if(strcmp(search,"file.txt\n")!=0){
            strcpy(filenames[count],search);
            count++;
            }
        }
        fclose(files);
        system("rm file.txt");
        for(int i=0;i<count;i++){
            int len = strlen(filenames[i]);
            filenames[i][len-1]='\0';
            char path[90];
            system("touch path.txt");
            system("pwd > path.txt");
            FILE * pathfile = fopen("path.txt","r");
            fgets(path,90,pathfile);
            int length = strlen(path);
            path[length-1]='/';
            strcat(path,filenames[i]);
            fclose(pathfile);
            system("rm path.txt");
            //path is compoletely true;
            int type = checkdirectory(path);
            if((((type==1)&&(strcmp(filenames[i],".neogit")!=0))&&(strcmp(filenames[i],".")!=0))&&(strcmp(filenames[i],"..")!=0)){
                chdir(path);
                writetimeline();
                chdir("..");
            }else if (((strcmp(filenames[i],".neogit")!=0))&&(type==0)){
                char firstlocation[80];
                strcpy(firstlocation,neogit_project_location);
                strcat(firstlocation,"/.neogit/commits/timeline.txt");
                FILE * timeline = fopen(firstlocation,"a");
                struct stat filestat;
                if(stat(path,&filestat)==0){
                    fprintf(timeline,"%s\n",path);
                    time_t present = filestat.st_mtime;
                    fprintf(timeline,"%s",ctime(&present));
                }
                fclose(timeline);
            }
        }
    }
    int makecommits_hash(char file_hash[]){
        FILE * commits_hashs = fopen("/home/.neogit_app/commits_hashs.txt","r");
        char hash[60];
        fgets(hash , 60 , commits_hashs);
        fclose(commits_hashs);
        int length = strlen(hash);
        if(hash[length-1]=='\n')
        hash[length-1]='\0';
        //get all present location:
        system("pwd > present_loc.txt");
        FILE * present_location = fopen("present_loc.txt","r");
        char present_loc[90];
        fgets(present_loc,90,present_location);
        fclose(present_location);
        system("rm present_loc.txt");
        int len = strlen(present_loc);
        present_loc[len-1]='/';
        len--;
        //now we get that;
        //now we find out if there is any directory (commits) or not:if there is we get its last directory:chdir
        system("ls > det.txt");
        FILE * details = fopen("det.txt","r");
        char search_det[30];
        char search_det_copy[30];
        int search_det_flag=0;
        while(fgets(search_det,30,details)){
            if(strcmp(search_det,"det.txt\n")==0)
            continue;
            strcpy(search_det_copy,search_det);
            search_det_flag++;
        }
        fclose(details);
        system("rm det.txt");
        //now we get the last folder too;
        FILE * commits_hashs2 = fopen("/home/.neogit_app/commits_hashs.txt","w");
        int HASH;
        sscanf(hash ,"%i", &HASH);
        HASH++;
        fprintf(commits_hashs,"%i\n",HASH);
        fclose(commits_hashs);
        char hashcopy[60];
        sprintf(hashcopy , "%i" , HASH);
        if(search_det_flag>0){
            int searc_det_copy_len=strlen(search_det_copy);
            search_det_copy[searc_det_copy_len-1]='\0';
            char copy_command[160];
            strcpy(copy_command,"cp -r ");
            strcat(copy_command,search_det_copy);
            strcat(copy_command," ");
            strcat(copy_command,hashcopy);
            system(copy_command);
            //now i should similarize files.st_mtime :
            char origin_path[90];
            strcpy(origin_path,present_loc);
            strcat(origin_path,search_det_copy);
            char destination_path[90];
            strcpy(destination_path,present_loc);
            strcat(destination_path,hashcopy);
            copy_mtime_folder(origin_path,destination_path);
            chdir(destination_path);
        }else{
            char command[70];
            strcpy(command ,"mkdir ");
            strcat(command , hashcopy);
            system(command);
            chdir(hashcopy);
        }
        strcpy(file_hash,hashcopy);
    }
    int makefolders(char relativepath[] , char absolutepath[] , char hash[]){
        //get our present location:
        char ploc[80];
        system("pwd > ploc.txt");
        FILE * loc = fopen("ploc.txt","r");
        fgets(ploc,80,loc);
        fclose(loc);
        system("rm ploc.txt");
        int length = strlen(ploc);
        ploc[length-1]='/';
        //now we get our location;
        char relcopy[80];
        strcpy(relcopy,relativepath);
        char allfiles[90];
        char * ptr = strtok(relativepath,"/");
        strcpy(allfiles,ptr);
        int flag=0;
        while(strcmp(allfiles,relcopy)!=0){
            char test[100];
            strcpy(test,ploc);
            strcat(test,allfiles);
            if(checkdirectory(test)==-1) {
                char make_directory[90];
                strcpy(make_directory,"mkdir ");
                strcat(make_directory,allfiles);
                system(make_directory);
            }
            ptr = strtok(NULL,"/");
            strcpy(lastfolder,allfiles);
            strcat(allfiles,"/");
            strcat(allfiles,ptr);
            flag++;
        }
        char COMMAND[120];
        //find out new path:
        system("pwd > present_hash.txt");
        FILE * present_hash = fopen("present_hash.txt","r");
        char present_location[90];
        fgets(present_location,90,present_hash);
        fclose(present_hash);
        system("rm present_hash.txt");
        int location_len = strlen(present_location);
        present_location[location_len-1]='\0';
        if(flag!=0){
            strcat(present_location,"/");
            strcat(present_location,lastfolder);
        }
        chdir("..");
        strcpy(COMMAND,"cp ");
        strcat(COMMAND,neogit_project_location);
        strcat(COMMAND,"/.neogit/staged_files/");
        char * var = strtok(absolutepath,"/");
        char file[80];
        while(1){
            strcpy(file,var);
            var = strtok(NULL,"/");
            if(var==NULL) break;
        }
        strcat(COMMAND,file);
        strcat(COMMAND," ");
        strcat(COMMAND,present_location);
        system(COMMAND);
        char origin_path[120];
        strcpy(origin_path,neogit_project_location);
        strcat(origin_path,"/.neogit/staged_files/");
        strcat(origin_path,file);
        strcat(present_location,"/");
        strcat(present_location,file);
        // now i will similarize mtime of copy of file and original file:
        copy_mtime_file(origin_path,present_location);
        chdir(hash);
    }
    void find_relative_path(char hash[]){
        char staged_file_path[90];
        strcpy(staged_file_path,neogit_project_location);
        strcat(staged_file_path,"/");
        int len = strlen(staged_file_path);
        strcat(staged_file_path,"/.neogit/staged_files/staged_files.txt");
        FILE * staged_files = fopen(staged_file_path,"r");
        char search[90];
        char pathes[300][90];
        char pathescopy[300][90];
        int counter=0;
        while(fgets(search,90,staged_files)){
            int length = strlen(search);
            search[length-1]='\0';
            if(search[0]=='/'){
                strcpy(pathes[counter],search);
                strcpy(pathes[counter],pathes[counter]+len);
                strcpy(pathescopy[counter],pathes[counter]);
                counter++;
            }
        }
        fclose(staged_files);
        //now go to staged_file to copy files from that:
        for(int i=0;i<counter;i++){
            char path[90];
            strcpy(path,pathes[i]);
            makefolders(pathes[i],path,hash);
        }
    }
    void commits_details(char branch[],char hash[],int number_files,char author_name[],char message[]){
        char present_path[80];
        system("pwd > present_path.txt");
        FILE * p_path = fopen("present_path.txt","r");
        fgets(present_path,80,p_path);
        fclose(p_path);
        system("rm present_path.txt");
        // now we save all commits of our project in commits folder in .neogit:
        char allcommits_locations[90];
        strcpy(allcommits_locations,neogit_project_location);
        strcat(allcommits_locations,"/.neogit/commits/allcommits.txt");
        FILE * allcommits = fopen(allcommits_locations,"a");
        fprintf(allcommits,"%s",present_path);

        fclose(allcommits);
        //hash
        //date
        //number of files
        //author name
        //message
        //branch;
        FILE * details = fopen("commits_details.txt","w");
        fprintf(details,"%s\n",hash);
        time_t present = time(NULL);
        fprintf(details,"%s%d\n%s\n%s\n%s\n",ctime(&present),number_files,author_name,message,branch);
        fclose(details);
        printf("ID : %s\n",hash);
        printf("DATE : %s",ctime(&present));
        printf("MESSAGE : %s\n",message);
    }
    int mounth_convert(char time[]){
        if(strcmp(time,"Jan")==0)  return 1;
        if(strcmp(time,"Feb")==0)  return 2;
        if(strcmp(time,"Mar")==0)  return 3;
        if(strcmp(time,"Apr")==0)  return 4;
        if(strcmp(time,"May")==0)  return 5;
        if(strcmp(time,"June")==0)  return 6;
        if(strcmp(time,"July")==0)  return 7;
        if(strcmp(time,"Aug")==0)  return 8;
        if(strcmp(time,"Sept")==0)  return 9;
        if(strcmp(time,"Oct")==0)  return 10;
        if(strcmp(time,"Nov")==0)  return 11;
        if(strcmp(time,"Dec")==0)  return 12;
        
    }
    int weekday_convert(char time[]){
        if(strcmp(time,"Sun")==0)  return 1;
        if(strcmp(time,"Mon")==0)  return 2;
        if(strcmp(time,"Tues")==0)  return 3;
        if(strcmp(time,"Wed")==0)  return 4;
        if(strcmp(time,"Thurs")==0)  return 5;
        if(strcmp(time,"Fri")==0)  return 6;
        if(strcmp(time,"Sat")==0)  return 7;
    }
    int checktime(int timer , char file_time[], char weekday[] , char mounth[] , char week[] , char second[] , char year[] ){    
        char file_weekday[10];
        char file_mounth[10];
        int file_hour;
        int file_min;
        int file_second;
        int file_year;
        int file_week_count;
        sscanf(file_time,"%s %s %d %d:%d:%d %d",file_weekday,file_mounth,&file_week_count,&file_hour,&file_min,&file_second,&file_year);
        int input_year;
        sscanf(year,"%d",&input_year);
        if(((input_year>file_year)&&(timer==0))||(input_year<file_year)&&(timer==1))  return 0;
        if(((mounth_convert(mounth)>mounth_convert(file_mounth))&&(timer==0))||((mounth_convert(mounth)<mounth_convert(file_mounth))&&(timer==1))) return 0;
        int input_week_count;
        sscanf(week,"%d",&input_week_count);
        if(((input_week_count>file_week_count)&&(timer==0))||((input_week_count>file_week_count)&&(timer==0))) return 0;
        if(((weekday_convert(weekday)>weekday_convert(file_weekday))&&(timer==0))||((weekday_convert(weekday)<weekday_convert(file_weekday))&&(timer==1))) return 0;
        int input_hour;
        int input_min;
        int input_sec;
        sscanf(second,"%d:%d:%d",&input_hour,&input_min,&input_sec);
        input_sec=input_sec+60*input_min+3600*input_hour;
        file_second=file_second+60*file_min+3600*file_hour;
        if(((input_sec>file_second)&&(timer==0))||((input_sec>file_second)&&(timer==0))) return 0;
        return 1;
    }
    void renew_timeline(){
        //when we call this function we are in commit folder where it has timeline.txt:
        //we go one folder back to access timeline.txt and also staged_files.txt:
        FILE * temp_timeline = fopen("temp_timeline.txt","w");
        FILE * timeline = fopen("timeline.txt","r");
        char search[90];
        char path[80];
        while(fgets(search,90,timeline)){
            fprintf(temp_timeline,"%s",search);
            strcpy(path,search);
            int len = strlen(path);
            path[len-1]='\0';
            if(check_stagedfiles(path)==1){
                fprintf(temp_timeline,"%s",time_path);
                fgets(search,90,timeline);
            }else if(check_stagedfiles(path)==0){
                fgets(search,90,timeline);
                fprintf(temp_timeline,"%s",search);
            }
        }
        fclose(temp_timeline);
        fclose(timeline);
        system("rm timeline.txt");
        system("mv temp_timeline.txt timeline.txt");
    }
    int copy_untracktedfiles2(char result_folder_name[],char result_folder_path[],char list[]){
        int len = strlen(neogit_project_location);
        char result[90];
        if(checkdirectory(list)==1){ 
            chdir(list);
            system("ls -a > list_files.txt");
            FILE * listed_files = fopen("list_files.txt","r");
            char s[90];
            int flag=0;
            while(fgets(s,90,listed_files)){
                if(((strcmp(s,".\n")==0)||(strcmp(s,"..\n")==0))||(strcmp(s,"list_files.txt\n")==0))  continue;
                flag++;
                break;
            }
            fclose(listed_files);
            system("rm list_files.txt");
            chdir("..");
            //now comeback to main work:
            if(flag==0) return 0;
            strcpy(list,list+len+1);
            strcpy(result,result_folder_path);
            strcat(result,"/");
            strcat(result,list);
            if(checkdirectory(result)==-1){
                char command[120];
                strcpy(command,"mkdir ");
                strcat(command,result);
                system(command);
            }
        }else{
            char command[200];
            strcpy(command,"cp ");
            strcat(command,list);
            strcat(command," ");
            char orginal_path[90];
            strcpy(orginal_path,list);
            strcpy(list,list+len+1);
            strcpy(result,result_folder_path);
            strcat(result,"/");
            strcat(result,list);
            strcat(command,result);
            system(command);
            copy_mtime_file(orginal_path,result);
        }
        return 0;
    }
    void copy_untracktedfiles(char result_folder_name[],char result_folder_path[]){
        system("pwd > present_location.txt");
        FILE * present_location_file = fopen("present_location.txt","r");
        char present_location[90];
        fgets(present_location,90,present_location_file);
        int length = strlen(present_location);
        present_location[length-1]='\0';
        fclose(present_location_file);
        system("rm present_location.txt");
        //now we get present location;
        system("ls -a > subfiles.txt");
        FILE * files = fopen("subfiles.txt","r");
        char file_names[300][90];
        char search[90];
        int counter=0;
        while(fgets(search,90,files)){
            int len = strlen(search);
            search[len-1]='\0';
            if(strcmp(search,"subfiles.txt")==0) continue;
            strcpy(file_names[counter],search);
            counter++;
        }
        fclose(files);
        system("rm subfiles.txt");
        char list[90];
        for(int i=0 ; i<counter; i++){
            strcpy(list,present_location);
            strcat(list,"/");
            strcat(list,file_names[i]);
            if((strcmp(file_names[i],"..")==0)||(strcmp(file_names[i],".")==0))
            continue;
            char listcopy[90];
            strcpy(listcopy,list);
            copy_untracktedfiles2(result_folder_name,result_folder_path,list);
            if(checkdirectory(listcopy)==1){
                chdir(listcopy);
                copy_untracktedfiles(result_folder_name,result_folder_path);
                chdir("..");
            }
        }
    }
    void remove_tracktedfiles(){
        FILE * trackted_files = fopen(".neogit/staged_files/trackted_files.txt","r");
        char search[90];
        char remove_command[100];
        while(fgets(search,90,trackted_files)){
            int len = strlen(search);
            search[len-1]='\0';
            if(checkdirectory(search)==-1)  continue;
            strcpy(remove_command,"rm ");
            strcat(remove_command,search);
            system(remove_command);
        }
        fclose(trackted_files);
    }
    //search,date_bday,date_bsecond
    //now only thing that remains in checkout command is to check untracted files:
    void handle_untracted_files(char result_folder_name[],char result_folder_path[]){
        remove_tracktedfiles();
        system("rm -r .neogit");
        copy_untracktedfiles(result_folder_name,result_folder_path);
        //now we are in neogit_project_location
    }
    int check_star_add(char input[]){
        int len = strlen(input);
        int flag=0;
        for(int i=0;i<len;i++){
            if(input[i]=='*'){
                flag++;
                break;
            }
        }
        if(flag==1){
            char command[100];
            strcpy(command,"ls -a ");
            strcat(command,input);
            strcat(command," > wildcard.txt");
            system(command);
            char search[30];
            FILE * wildcard = fopen("wildcard.txt","r");
            char files[300][90];
            int counter=0;
            while(fgets(search,30,wildcard)){
                int length = strlen(search);
                search[length-1]='\0';
                strcpy(files[counter],search);
                counter++;
            }
            fclose(wildcard);
            system("rm wildcard.txt");
            for(int i=0;i<counter;i++){
                add_file(files[i],0);
            }
        }
        return flag;
    }
    void swap(char first[] , char last[]){
        char temp[50];
        strcpy(temp,last);
        strcpy(last,first);
        strcpy(first,temp);
    }
    int diffchecker(char line1[],char line2[]){
        char line1copy[90];
        strcpy(line1copy,line1);
        char line2copy[90];
        strcpy(line2copy,line2);
        int difference=0;
        char * ptr1 = strtok(line1," ");
        char res1[90];
        strcpy(res1,ptr1);
        while(1){
            ptr1 = strtok(NULL," ");
            if(ptr1==NULL) break;
            strcat(res1,ptr1);
        }
        char * ptr2 = strtok(line2," ");
        char res2[90];
        strcpy(res2,ptr2);
        while(1){
            ptr2 = strtok(NULL," ");
            if(ptr2==NULL) break;
            strcat(res2,ptr2);
        }
        if(strcmp(res1,res2)!=0)   difference++;
        strcpy(line1,line1copy);
        strcpy(line2,line2copy);
        return difference;
    }
    void diff_folderchecker(char path1[],char path2[],int length){
        chdir(path1);
        system("ls > path1_details.txt");
        FILE * path1_details = fopen("path1_details.txt","r");
        char search[50];
        char files[60][50];
        int count=0;
        while(fgets(search,50,path1_details)){
            char search_copy[90];
            strcpy(search_copy,search);
            int len = strlen(search_copy);
            search_copy[len-1]='\0';
            strcpy(files[count],search_copy);
            count++;
        }
        fclose(path1_details);
        system("rm path1_details.txt");
        for(int i=0;i<count;i++){
            char file_path1[90];
            strcpy(file_path1,path1);
            strcat(file_path1,"/");
            strcat(file_path1,files[i]);
            char file_path2[90];
            strcpy(file_path2,path2);
            strcat(file_path2,"/");
            strcat(file_path2,files[i]);
            if(((checkdirectory(file_path1)==0)&&(strcmp(files[i],"path1_details.txt")!=0))&&(strcmp(files[i],"commits_details.txt")!=0)){
                if(checkdirectory(file_path2)==-1){
                    strcpy(file_path2,file_path2+length);
                    printf("this file is not exist in other commit %s\n",file_path2);
                }
                char command[250];
                strcpy(command,"neogit diff -f ");
                strcat(command,file_path1);
                strcat(command," ");
                strcat(command,file_path2);
                system(command);
            }else if(((checkdirectory(file_path1)==1)&&(strcmp(files[i],".")!=0))&&(strcmp(files[i],"..")!=0)){
                chdir(file_path1);
                diff_folderchecker(file_path1,file_path2,length);
                chdir("..");
            }   
        }
    }
    void diff_folderchecker_reverse(char path1[],char path2[],int length){
        chdir(path1);
        system("ls > path1_details.txt");
        FILE * path1_details = fopen("path1_details.txt","r");
        char search[50];
        char files[60][50];
        int count=0;
        while(fgets(search,50,path1_details)){
            char search_copy[90];
            strcpy(search_copy,search);
            int len = strlen(search_copy);
            search_copy[len-1]='\0';
            strcpy(files[count],search_copy);
            count++;
        }
        fclose(path1_details);
        system("rm path1_details.txt");
        for(int i=0;i<count;i++){
            char file_path1[90];
            strcpy(file_path1,path1);
            strcat(file_path1,"/");
            strcat(file_path1,files[i]);
            char file_path2[90];
            strcpy(file_path2,path2);
            strcat(file_path2,"/");
            strcat(file_path2,files[i]);
            if(((checkdirectory(file_path1)==0)&&(strcmp(files[i],"path1_details.txt")!=0))&&(strcmp(files[i],"commits_details.txt")!=0)){
                if(checkdirectory(file_path2)==-1){
                    strcpy(file_path2,file_path2+length);
                    printf("this file is not exist in other commit %s\n",file_path2);
                }
            }else if(((checkdirectory(file_path1)==1)&&(strcmp(files[i],".")!=0))&&(strcmp(files[i],"..")!=0)){
                chdir(file_path1);
                diff_folderchecker(file_path1,file_path2,length);
                chdir("..");
            }   
        }
    }
int main(int argc , char * argv[]){
    // we pure all txt files that we need for our programm in .neogit_app folder in /home and we made that before;
    //now we have this 2D strig which have all commands:
    char allcommands[50][50];
    //first step is to build neogit init command
    if(strcmp(argv[1],"init")==0){
            //we save all locations of our projects in locations.txt in /home:
        //check that if there is .neogit folder in our present directory or above not?
        system("touch project_location.txt");
        system("pwd > project_location.txt");
        FILE * file = fopen("project_location.txt","r");
        char first_location[70];
        fgets(first_location,70,file);
        fclose(file);
        system("rm project_location.txt");
        // get our present location;
        int test=0;
        while(1){
            if(test>1){
                break;
            }else if(check_neogit()==0){
                printf("you initialized neogit before in this project\n");
                break;
            }else if(check_root()==0){
                test++;
            }
            chdir("..");
        }
        if(test==2){
            //make this project folder:
            // if(checkdirectory("/home/.neogit_app/locations.txt")!=-1){
            //     chdir("/home/.neogit_app");
            //     FILE * neogit_app_locations = fopen("locations.txt","r");
            //     char search[100];
            //     int number_projects=0;
            //     while(fgets(search,100,neogit_app_locations)){
            //         number_projects++;
            //     }
            //     fclose(neogit_app_locations);
            //     char number[20];
            //     sprintf(number,"%d",number_projects);
            //     char mkdir_command[20];
            //     strcpy(mkdir_command,"mkdir ");
            //     strcat(mkdir_command,number);
            //     system(mkdir_command);
            // }
            FILE * location = fopen("locations.txt","a");
            int len=strlen(first_location);
            first_location[len-1]='\0';
            fprintf(location,"%s\n",first_location);
            fclose(location);
            chdir(first_location);
            strcpy(neogit_project_location,first_location);
            system("mkdir .neogit");
            chdir(".neogit");
            // we make staged files & primary directory in this place:
            system("mkdir staged_files");
            chdir("staged_files");
            system("touch trackted_files.txt");
            system("touch Astagedfiles.txt");
            chdir("..");
            system("mkdir commits");
            chdir("commits");
            system("touch allbranches.txt");
            system("touch HEAD.txt");
            chdir("../..");
            ////////
            writetimeline();
            chdir(".neogit");
            //now we are back in .neogit folder;
            //copy all project to our primary directories;
            //and also we make 2 account.txt for save our email and username
            char NAME[30] , EMAIL[30];
            FILE * global_username = fopen("/home/.neogit_app/global_names.txt","r+");
            char search1[30];
            if(fgets(search1,30,global_username)){
                strcpy(NAME,search1);
                fclose(global_username);
            }else{
                fclose(global_username);
            }
            FILE * global_useremail= fopen("/home/.neogit_app/global_emails.txt","r+");
            if(fgets(search1,30,global_useremail)){
                strcpy(EMAIL,search1);
                fclose(global_useremail);
            }else{
                fclose(global_useremail);
            }
            FILE * account = fopen("account_name.txt","w");
            fprintf(account, "%s", NAME);
            fclose(account);
            FILE *account2 = fopen("account_email.txt","w");
            fprintf(account,"%s",EMAIL);
            fclose(account2);
        }
    }
    //user.name and user.email
    else if(strcmp(argv[1],"config")==0){
        if(strcmp(argv[2],"-global")==0){
            assign_global(argv[3],argv[4]);
        }else if(strcmp(argv[2],"-global")!=0){
            assign_local(argv[2],argv[3]);
        }
    }
    //part of add:i am betee than this 
    else if(strcmp(argv[1],"add")==0){
        if(testproject()==0){
            printf("you didn't initialized neogit in your project\n");
            return 0;
        }
        if(strcmp(argv[2],"-f")==0){
            for(int i=3 ; i< argc ;i++){
                add_file(argv[i],0);
            }
        }else if(strcmp(argv[2],"-n")==0){
            int depth;
            if(argv[3]==NULL){
                depth=1;
                search_stagedfiles(depth);
            }else{
                sscanf(argv[3],"%d",&depth);
                search_stagedfiles(depth);
            }
        }else if(strcmp(argv[2],"-redo")==0){
            char floc[80];
            system("pwd > pq.txt");
            FILE * present = fopen("pq.txt","r");
            fgets(floc,80,present);
            system("rm pq.txt");
            int len = strlen(floc);
            char loc[80];
            strcpy(loc,neogit_project_location);
            strcat(loc,"/.neogit/staged_files/reset.txt");
            FILE * reset = fopen(loc,"r");
            char search[80];
            while(fgets(search,80,reset)){
                strcpy(search,search+len);
                int length = strlen(search);
                search[length-1]='\0';
                add_file(search,1);
            }
            fclose(reset);
        }else{
            if(check_star_add(argv[2])==0){
                add_file(argv[2],0);
            }
        }
    }else if(strcmp(argv[1],"reset")==0){
        if(testproject()==0){
            printf("you didn't initialized neogit in your project\n");
            return 0;
        }
        // argv[2] is path of our file:
        // reset function will do this part:
        if(strcmp(argv[2],"-f")==0){
            for(int i = 3 ; i<argc ; i++ ){
                char path[90];
                system("touch path.txt");
                system("pwd > path.txt");
                FILE * pathfile = fopen("path.txt","r");
                fgets(path,90,pathfile);
                fclose(pathfile);
                system("rm path.txt");
                int length = strlen(path);
                path[length-1]='/';
                strcat(path,argv[i]);
                reset(path);
            }
        }else if (strcmp(argv[2],"-undo")==0){




        }else{
            char path[90];
            system("touch path.txt");
            system("pwd > path.txt");
            FILE * pathfile = fopen("path.txt","r");
            fgets(path,90,pathfile);
            fclose(pathfile);
            system("rm path.txt");
            int length = strlen(path);
            path[length-1]='/';
            strcat(path,argv[2]);
            reset(path);
        }
    }else if(strcmp(argv[1],"status")==0){
        testproject();
        char allfiles[80];
        strcpy(allfiles,neogit_project_location);
        strcat(allfiles,"/.neogit/commits/timeline.txt");
        status(allfiles);
    }else if((strcmp(argv[1],"commit")==0)&&(strcmp(argv[2],"-m")==0)){
        if(testproject()==0){
            printf("you didn't initialized neogit in your project\n");
            return 0;
        }
        else if(argv[3]==NULL){
            printf("please enter message.\n");
            return 0;
        }else if(argv[4]!=NULL){
            printf("%s",argv[4]);
            printf("invalid message\n");
            return 0;
        }else {
            int len = strlen(argv[3]);
            if(len>72){
                printf("your message is not valid\n");
                return 0;
            }
        }
        // now we passed message validity;
        int number_files_commited=0;
        //we should update time line ,remove Astaged.txt and staged_files.txt
        // chdir(".neogit/commits");
        //char pathes[300][80];
        //char files_abs_path[300][80];
        char location[90];
        strcpy(location,neogit_project_location);
        strcat(location,"/.neogit/staged_files");
        int length = strlen(location);
        length++;
        //we go to staged_files but we will get back at the end:
        chdir(location);
        //now we get all files that stored in stage area :
        system("ls > files.txt");
        FILE * staged_files = fopen("files.txt","r");
        char search[90];
        int flag=0;
        int counter=0;
        for(int i=0 ; fgets(search,90,staged_files);i++){
            int len = strlen(search);
            search[len-1]='\0';
            if((((strcmp(search,"Astagedfiles.txt")!=0)&&(strcmp(search,"staged_files.txt")!=0))&&(strcmp(search,"trackted_files.txt")!=0))&&(strcmp(search,"files.txt")!=0)){
                number_files_commited++;
                flag++;
            }
        }
        fclose(staged_files);
        system("rm files.txt");
        chdir(neogit_project_location);
        if(flag==0){
            FILE * del_files = fopen("/home/.neogit_app/deleted_files.txt","r");
            char test[90];
            int flag2=0;
            if(fgets(test,90,del_files)){
                flag2++;
            }
            fclose(del_files);
            if(flag2==0){
                printf("there isn't any file in stagine area!\n");
                return 0;
            }   
        }
        char message[90];
        strcpy(message,argv[3]);
        chdir(".neogit");
        char author_name[50];
        FILE * name = fopen("account_name.txt","r");
        fgets(author_name,50,name);
        fclose(name);
        int name_length = strlen(author_name);
        author_name[name_length-1]='\0';
        //
        chdir("commits");
        char fullpath[80];
        system("pwd > fullpath.txt");
        FILE * PWD = fopen("fullpath.txt","r");
        fgets(fullpath,80,PWD);
        fclose(PWD);
        int len_path = strlen(fullpath);
        fullpath[len_path-1]='/';
        system("rm fullpath.txt");
        // it is going to changed:
        //get our present_branch:
        FILE * present_branch = fopen("HEAD.txt","r");
        char HEAD_branch[90];
        if(fgets(HEAD_branch,90,present_branch)){
        } else {
            system("mkdir master");
            FILE * branch_file = fopen("HEAD.txt","w");
            fprintf(branch_file,"master\n");
            fclose(branch_file);
            strcpy(HEAD_branch,"master\n");
            FILE * allbranches = fopen("allbranches.txt","a");
            fprintf(allbranches,"master\n");
            fclose(allbranches);
        }
        fclose(present_branch);
        int HEAD_length = strlen(HEAD_branch);
        HEAD_branch[HEAD_length-1]='\0';
        //at first renew timeline.txt
        //renew timeline.txt
        renew_timeline();
        ///now we get our present branch:
        strcat(fullpath,"/");
        strcat(fullpath,HEAD_branch);
        chdir(fullpath);
        char hash[80];
        makecommits_hash(hash);
        find_relative_path(hash);
        commits_details(HEAD_branch,hash,number_files_commited,author_name,message);
        deleted_added_files();
        //remove all staged_files:
        chdir("../..");
        FILE * present_commit_file = fopen("present_commit.txt","w");
        fprintf(present_commit_file,"%s\n",hash);
        fclose(present_commit_file);
        //now all files where commited there last modified time has been commited:
        chdir("..");
        system("mkdir tempstagingarea");
        system("cp staged_files/trackted_files.txt tempstagingarea");
        system("rm -r staged_files");
        system("mv tempstagingarea staged_files");
        chdir("staged_files");
        system("touch Astagedfiles.txt");
        system("touch staged_files.txt");
    }else if(strcmp(argv[1],"set")==0){
        if(argv[5]==NULL){
            printf("you didn't enter shortcut\n");
            return 0;
        }
        char message[100];
        strcpy(message,argv[3]);
        char shortcut[70];
        strcpy(shortcut,argv[5]);
        FILE * shortcuts_file = fopen("/home/.neogit_app/shortcuts_file.txt","a");
        fprintf(shortcuts_file,"%s\n",shortcut);
        fprintf(shortcuts_file,"%s\n",message);
        fclose(shortcuts_file);
    }else if(strcmp(argv[1],"replace")==0){
        char shortcut[90];
        strcpy(shortcut,argv[5]);
        strcat(shortcut,"\n");
        FILE * shortcuts_file = fopen("/home/.neogit_app/shortcuts_file.txt","r");
        FILE * shortcuts_temp = fopen("/home/.neogit_app/shortcuts_temp.txt","w");
        char search[90];
        while(fgets(search,90,shortcuts_file)){
            fprintf(shortcuts_temp,"%s",search);
            if(strcmp(search,shortcut)==0){
                fgets(search,90,shortcuts_file);
                fprintf(shortcuts_temp,"%s\n",argv[3]);
            }
        }
        fclose(shortcuts_file);
        fclose(shortcuts_temp);
        char command1[50];
        strcpy(command1,"rm /home/.neogit_app/shortcuts_file.txt");
        system(command1);
        char command2[120];
        strcpy(command2,"mv /home/.neogit_app/shortcuts_temp.txt /home/.neogit_app/shortcuts_file.txt");
        system(command2);
    }else if((strcmp(argv[1],"commit")==0)&&(strcmp(argv[2],"-s")==0)){
        FILE * shortcuts_file = fopen("/home/.neogit_app/shortcuts_file.txt","r");
        char search[90];
        char shortcut[90];
        strcpy(shortcut,argv[3]);
        strcat(shortcut,"\n");
        while(fgets(search,90,shortcuts_file)){
            if(strcmp(search,shortcut)==0){
                fgets(search,90,shortcuts_file);
                char message[90];
                message[0]='"';
                strcat(message,search);
                int len = strlen(message);
                message[len-1]='"';
                char command[80];
                strcpy(command,"./neogit ");
                strcat(command,"commit -m ");
                strcat(command,message);
                system(command);
                break;
            }
        }
    }else if(strcmp(argv[1],"remove")==0){
        char shortcut[90];
        strcpy(shortcut,argv[3]);
        strcat(shortcut,"\n");
        FILE * shortcuts_file = fopen("/home/.neogit_app/shortcuts_file.txt","r");
        FILE * shortcuts_temp = fopen("/home/.neogit_app/shortcuts_temp.txt","w");
        char search[90];
        int flag=0;
        while(fgets(search,90,shortcuts_file)){
            if(strcmp(search,shortcut)==0){
                fgets(search,90,shortcuts_file);
                flag++;
                continue;
            }
            fprintf(shortcuts_temp,"%s",search);
        }
        fclose(shortcuts_file);
        fclose(shortcuts_temp);
        char command1[50];
        strcpy(command1,"rm /home/.neogit_app/shortcuts_file.txt");
        system(command1);
        char command2[120];
        strcpy(command2,"mv /home/.neogit_app/shortcuts_temp.txt /home/.neogit_app/shortcuts_file.txt");
        system(command2);
        if(flag==0){
            printf("there isn't any shortcut with this name!\n");
            return 0;
        }
    }else if((strcmp(argv[1],"log")==0)&&(argc==2)){
        testproject();
        char pathes[400][90];
        char allcommits_locations[90];
        strcpy(allcommits_locations,neogit_project_location);
        strcat(allcommits_locations,"/.neogit/commits/allcommits.txt");
        FILE * allcommits = fopen(allcommits_locations,"r");
        int counter=0;
        for(; fgets(pathes[counter],90,allcommits);counter++){
            int len = strlen(pathes[counter]);
            pathes[counter][len-1]='\0';
        }
        fclose(allcommits);
        for(int i=counter-1;i>=0;i--){
            chdir(pathes[i]);
            strcat(pathes[i],"/commits_details.txt");
            FILE * commit = fopen(pathes[i],"r");
            char search[120];
            fgets(search,120,commit);
            printf("ID : %s",search);
            fgets(search,120,commit);
            printf("DATE : %s",search);
            fgets(search,120,commit);
            printf("NUMBER OF COMMITED FILES : %s",search);
            fgets(search,120,commit);
            printf("AUTHOR : %s",search);
            fgets(search,120,commit);
            printf("MESSAGE : %s",search);
            fgets(search,120,commit);
            printf("BRANCH : %s\n",search);
            fclose(commit);
        }
        return 0;
    }else if((strcmp(argv[1],"log")==0)&&(strcmp(argv[2],"-n")==0)){
        char pathes[400][90];
        testproject();
        char allcommits_locations[90];
        strcpy(allcommits_locations,neogit_project_location);
        strcat(allcommits_locations,"/.neogit/commits/allcommits.txt");
        FILE * allcommits = fopen(allcommits_locations,"r");
        int counter=0;
        for(; fgets(pathes[counter],90,allcommits);counter++){
            int len = strlen(pathes[counter]);
            pathes[counter][len-1]='\0';
        }
        fclose(allcommits);
        int n;
        sscanf(argv[3],"%d",&n);
        for(int i=counter-1;i>=counter-n;i--){
            chdir(pathes[i]);
            strcat(pathes[i],"/commits_details.txt");
            FILE * commit = fopen(pathes[i],"r");
            char search[120];
            fgets(search,120,commit);
            printf("ID : %s",search);
            fgets(search,120,commit);
            printf("DATE : %s",search);
            fgets(search,120,commit);
            printf("NUMBER OF COMMITED FILES : %s",search);
            fgets(search,120,commit);
            printf("AUTHOR : %s",search);
            fgets(search,120,commit);
            printf("MESSAGE : %s",search);
            fgets(search,120,commit);
            printf("BRANCH : %s\n",search);
            fclose(commit);
        }
        return 0;
    }else if((strcmp(argv[1],"log")==0)&&(strcmp(argv[2],"-branch")==0)){
        char pathes[400][90];
        testproject();
        char allcommits_locations[90];
        strcpy(allcommits_locations,neogit_project_location);
        strcat(allcommits_locations,"/.neogit/commits/allcommits.txt");
        FILE * allcommits = fopen(allcommits_locations,"r");
        int counter=0;
        for(; fgets(pathes[counter],90,allcommits);counter++){
            int len;
            len = strlen(pathes[counter]);
            pathes[counter][len-1]='\0';
        }
        fclose(allcommits);
        for(int i=counter-1;i>=0;i--){
            chdir(pathes[i]);
            strcat(pathes[i],"/commits_details.txt");
            FILE * commit = fopen(pathes[i],"r");
            char search[100];
            for(int i=0;i<6;i++){
                fgets(search,100,commit);
            }
            int len = strlen(search);
            search[len-1]='\0';
            fclose(commit);
            if(strcmp(search,argv[3])==0){
                FILE * commit2 = fopen(pathes[i],"r"); 
                char details[120];
                fgets(details,120,commit);
                printf("ID : %s",details);
                fgets(details,120,commit);
                printf("DATE : %s",details);
                fgets(details,120,commit);
                printf("NUMBER OF COMMITED FILES : %s",details);
                fgets(details,120,commit);
                printf("AUTHOR : %s",details);
                fgets(details,120,commit);
                printf("MESSAGE : %s\n",details);
                fclose(commit2);
            }
        }
    }else if((strcmp(argv[1],"log")==0)&&(strcmp(argv[2],"-author")==0)){
        char pathes[400][90];
        testproject();
        char allcommits_locations[90];
        strcpy(allcommits_locations,neogit_project_location);
        strcat(allcommits_locations,"/.neogit/commits/allcommits.txt");
        FILE * allcommits = fopen(allcommits_locations,"r");
        int counter=0;
        for(; fgets(pathes[counter],90,allcommits);counter++){
            int len;
            len = strlen(pathes[counter]);
            pathes[counter][len-1]='\0';
        }
        fclose(allcommits);
        for(int i=counter-1;i>=0;i--){
            chdir(pathes[i]);
            strcat(pathes[i],"/commits_details.txt");
            FILE * commit = fopen(pathes[i],"r");
            char search[100];
            for(int i=0;i<4;i++){
                fgets(search,100,commit);
            }
            int len = strlen(search);
            search[len-1]='\0';
            fclose(commit);
            if(strcmp(search,argv[3])==0){
                FILE * commit2 = fopen(pathes[i],"r"); 
                char details[120];
                fgets(details,120,commit);
                printf("ID : %s",details);
                fgets(details,120,commit);
                printf("DATE : %s",details);
                fgets(details,120,commit);
                printf("NUMBER OF COMMITED FILES : %s",details);
                fgets(details,120,commit);
                printf("AUTHOR : %s",details);
                fgets(details,120,commit);
                printf("MESSAGE : %s",details);
                fgets(details,120,commit);
                printf("BRANCH : %s\n",details);
                fclose(commit2);
            }
        }
    }else if((strcmp(argv[1],"log")==0)&&((strcmp(argv[2],"-since")==0)||(strcmp(argv[2],"-before")==0))){
        int timer;
        if(strcmp(argv[2],"-since")==0) timer==0;
        if(strcmp(argv[2],"-before")==0) timer==1;
        //it is classify by ctime output;
        char pathes[400][90];
        testproject();
        char allcommits_locations[90];
        strcpy(allcommits_locations,neogit_project_location);
        strcat(allcommits_locations,"/.neogit/commits/allcommits.txt");
        FILE * allcommits = fopen(allcommits_locations,"r");
        int counter=0;
        for(; fgets(pathes[counter],90,allcommits);counter++){
            int len = strlen(pathes[counter]);
            pathes[counter][len-1]='\0';
        }
        fclose(allcommits);
        int flag=0;
        for(int i=counter-1;i>=0;i--){
            chdir(pathes[i]);
            strcat(pathes[i],"/commits_details.txt");
            FILE * commit = fopen(pathes[i],"r");
            char search[100];
            for(int i=0;i<2;i++){
                fgets(search,100,commit);
            }
            int len = strlen(search);
            search[len-1]='\0';
            fclose(commit);
            if(checktime(timer,search,argv[3],argv[4],argv[5],argv[6],argv[7])){
                FILE * commit2 = fopen(pathes[i],"r"); 
                char details[120];
                fgets(details,120,commit);
                printf("ID : %s",details);
                fgets(details,120,commit);
                printf("DATE : %s",details);
                fgets(details,120,commit);
                printf("NUMBER OF COMMITED FILES : %s",details);
                fgets(details,120,commit);
                printf("AUTHOR : %s",details);
                fgets(details,120,commit);
                printf("MESSAGE : %s\n",details);
                fclose(commit2);
            }
        }
    }else if((strcmp(argv[1],"log")==0)&&(strcmp(argv[2],"-search")==0)){
        char pathes[400][90];
        testproject();
        char allcommits_locations[90];
        strcpy(allcommits_locations,neogit_project_location);
        strcat(allcommits_locations,"/.neogit/commits/allcommits.txt");
        FILE * allcommits = fopen(allcommits_locations,"r");
        int counter=0;
        for(; fgets(pathes[counter],90,allcommits);counter++){
            int len;
            len = strlen(pathes[counter]);
            pathes[counter][len-1]='\0';
        }
        fclose(allcommits);
        for(int i=counter-1;i>=0;i--){
            chdir(pathes[i]);
            strcat(pathes[i],"/commits_details.txt");
            FILE * commit = fopen(pathes[i],"r");
            char search[100];
            for(int i=0;i<5;i++){
                fgets(search,100,commit);
            }
            int len = strlen(search);
            search[len-1]='\0';
            fclose(commit);
            int flag=0;
            for(int i=3;argv[i]!=NULL;i++){
                if(strstr(search,argv[i])!=NULL)
                flag++;
            }
            if(flag>0){
                FILE * commit2 = fopen(pathes[i],"r"); 
                char details[120];
                fgets(details,120,commit);
                printf("ID : %s",details);
                fgets(details,120,commit);
                printf("DATE : %s",details);
                fgets(details,120,commit);
                printf("NUMBER OF COMMITED FILES : %s",details);
                fgets(details,120,commit);
                printf("AUTHOR : %s",details);
                fgets(details,120,commit);
                printf("MESSAGE : %s\n",details);
                fgets(details,120,commit);
                printf("BRANCH : %s\n",details);
                fclose(commit2);
            }
        }
    }else if((strcmp(argv[1],"branch")==0)&&(argc==2)){
        testproject();
        char location[100];
        strcpy(location,neogit_project_location);
        strcat(location,"/.neogit/commits/allbranches.txt");
        FILE * allbranches = fopen(location,"r");
        char search[90];
        while(fgets(search,90,allbranches)){
            printf("%s",search);
        }
    }else if((strcmp(argv[1],"branch")==0)&&(argc==3)){
        char branchcopy[90];
        strcpy(branchcopy,argv[2]);
        strcat(branchcopy,"\n");
        testproject();
        chdir(".neogit/commits");
        FILE * allbranches = fopen("allbranches.txt","r");
        char search[90];
        int flag=0;
        while(fgets(search,90,allbranches)){
            if(strcmp(search,branchcopy)==0){
                printf("this branch already exist!\n");
                flag++;
                break;
            }
        }
        fclose(allbranches);
        if(flag>0) 
        return 0;
        int commit_flag=0;
        FILE * lastcommit= fopen("allcommits.txt","r");
        char searchcopy[90];
        while(fgets(search,90,lastcommit)){
            strcpy(searchcopy,search);
            commit_flag++;
        }
        fclose(lastcommit);
        if(commit_flag==0){
            printf("there isn't any commit yet!\n");
            return 0;
        }
        char makefolder_command[90];
        strcpy(makefolder_command,"mkdir ");
        strcat(makefolder_command,argv[2]);
        system(makefolder_command);
        //now we made our new branch;
        //first part is to handle allbranches.txt:
        FILE * new_allbranches = fopen("allbranches.txt","a");
        fprintf(new_allbranches,"%s",branchcopy);
        fclose(new_allbranches);
        //it will not change HEAD;
        //third part is to make firstcommit in this new branch by last commit: HOW?
        //all pahtes of commits are saved in allcommits.txt:
        char new_branch_absolutepath[90];
        strcpy(new_branch_absolutepath,neogit_project_location);
        strcat(new_branch_absolutepath,"/.neogit/commits/");
        strcat(new_branch_absolutepath,argv[2]);
        int len = strlen(searchcopy);
        searchcopy[len-1]='\0';
        chdir(searchcopy);
        chdir("..");
        system("pwd > loc.txt");
        FILE * location = fopen("loc.txt","r");
        char loc[90];
        fgets(loc,90,location);
        fclose(location);
        system("rm loc.txt");
        int loc_len = strlen(loc);
        loc[loc_len-1]='/';
        char loc_copy[90];
        strcpy(loc_copy,loc);
        int lloc_len = strlen(loc_copy);
        loc_copy[lloc_len-1]='\0';
        //now we get our present branch;
        system("ls > commitsdet.txt");
        FILE * commits_det = fopen("commitsdet.txt","r");
        char search_commits[90];
        char cp_command[150];
        while(fgets(search_commits,90,commits_det)){
            char commit_location[90];
            strcpy(commit_location,loc);
            strcat(commit_location,"/");
            int len = strlen(search_commits);
            search_commits[len-1]='\0';
            strcat(commit_location,search_commits);
            if(strcmp(search_commits,"commitsdet.txt")==0)  continue;
            strcpy(cp_command,"cp -r ");
            strcat(cp_command,search_commits);
            strcat(cp_command," ");
            strcat(cp_command,new_branch_absolutepath);
            system(cp_command);
            char destination_path[90];
            strcpy(destination_path,new_branch_absolutepath);
            strcat(destination_path,"/");
            strcat(destination_path,search_commits);
            copy_mtime_folder(commit_location,destination_path);
            chdir(loc_copy);
            if(strcmp(search_commits,searchcopy)==0){
                break;
            }
        }
        fclose(commits_det);
        system("rm commitsdet.txt");
        //details will not change even ID(hash);
    }else if((strcmp(argv[1],"checkout")==0)&&(strcmp(argv[2],"-b")==0)){   
        if(testproject()==0){
            printf("you didn't initialized neogit in your project.\n");
            return 0;
        }
        chdir("..");
        system("pwd > loc_neogit_project.txt");
        FILE * loc = fopen("loc_neogit_project.txt","r");
        char in_loc[90];
        fgets(in_loc,90,loc);
        int in_loc_len = strlen(in_loc);
        in_loc[in_loc_len-1]='\0';
        fclose(loc);
        system("rm loc_neogit_project.txt");
        chdir(neogit_project_location);
        system("neogit status");
        system("neogit status > status_temp.txt");
        char search[90];
        FILE * status = fopen("status_temp.txt","r");
        int flag=0;
        while(fgets(search,90,status)){
            if((strstr(search,": +M")!=NULL)||(strstr(search,": -M")!=NULL)){
                flag++;
                printf("there is modified file in your project!\n");
                break;
            }
        }
        fclose(status);
        system("rm status_temp.txt");
        if(flag==1) return 0;
        char branch_commit_location[90];
        strcpy(branch_commit_location,neogit_project_location);
        strcat(branch_commit_location,"/.neogit/commits/");
        strcat(branch_commit_location,argv[3]);
        if(checkdirectory(branch_commit_location)==-1){
            printf("there isn't branch with this name\n");
            return 0;
        }
        chdir(branch_commit_location);
        system("ls > commit_checkout.txt");
        FILE * files = fopen("commit_checkout.txt","r");
        char search_commits[90];
        char search_commits_copy[90];
        while(fgets(search_commits,90,files)){
            if(strcmp(search_commits,"commit_checkout.txt\n")==0)  continue;
            strcpy(search_commits_copy,search_commits);
        }
        int len = strlen(search_commits_copy);
        search_commits_copy[len-1]='\0';
        fclose(files);
        system("rm commit_checkout.txt");
        strcat(branch_commit_location,"/");
        strcat(branch_commit_location,search_commits_copy);
        char command1[200];
        strcpy(command1,"cp -r ");
        strcat(command1,branch_commit_location);
        strcat(command1," ");
        strcat(command1,in_loc);
        system(command1);
        //now lets similrize mtime of this to folders :
        //original path : branch_commit_location ;
        char destination_path[90];
        strcpy(destination_path,in_loc);
        strcat(destination_path,"/");
        strcat(destination_path,search_commits_copy);
        copy_mtime_folder(branch_commit_location,destination_path);
        //we update present_commit.txt in .neogit and then copy .neogit to it:
        chdir("../..");
        FILE * present_commit_file = fopen("present_commit.txt","w");
        fprintf(present_commit_file,"%s\n",search_commits_copy);
        fclose(present_commit_file);
        //now it gets update; 
        char command2[200];
        strcpy(command2,"cp -r ");
        strcat(command2,neogit_project_location);
        strcat(command2,"/.neogit");
        strcat(command2," ");
        strcat(command2,in_loc);
        strcat(command2,"/");
        strcat(command2,search_commits_copy);
        system(command2);
        //similarize this two folders mtime:
        char origin_path[90];
        strcpy(origin_path,neogit_project_location);
        strcat(origin_path,"/.neogit");
        char res_path[120];
        strcpy(res_path,in_loc);
        strcat(res_path,"/");
        strcat(res_path,search_commits_copy);
        strcat(res_path,"/.neogit");
        copy_mtime_folder(origin_path,res_path);
        //its finished;
        chdir(neogit_project_location);
        char result_folder_loc[200];
        strcpy(result_folder_loc,in_loc);
        strcat(result_folder_loc,"/");
        strcat(result_folder_loc,search_commits_copy);
        handle_untracted_files(search_commits_copy,result_folder_loc);
        chdir("..");
        char rm[90];
        strcpy(rm,"rm -r ");
        strcat(rm,neogit_project_location);
        system(rm);
        //now we should rename the hash;
        char command3[200];
        char temp[90];
        strcpy(temp,in_loc);
        strcat(temp,"/");
        strcat(temp,search_commits_copy);
        strcpy(command3,"mv ");
        strcat(command3,temp);
        strcat(command3," ");
        strcat(command3,neogit_project_location);
        system(command3);
        chdir(neogit_project_location);
        //we should change HEAD :
        FILE * HEAD_file  = fopen(".neogit/commits/HEAD.txt","w");
        fprintf(HEAD_file,"%s\n",argv[3]);
        fclose(HEAD_file);
        //now it has changed!
        system("rm commits_details.txt");
        return 0;
    }else if((strcmp(argv[1],"checkout")==0)&&((strcmp(argv[2],"-c")==0)||(strcmp(argv[2],"-d")))){
        if(testproject()==0){
            printf("you didn't initialized neogit in your project.\n");
            return 0;
        }
        chdir("..");
        system("pwd > loc_neogit_project.txt");
        FILE * loc = fopen("loc_neogit_project.txt","r");
        char in_loc[90];
        fgets(in_loc,90,loc);
        int in_loc_len = strlen(in_loc);
        in_loc[in_loc_len-1]='\0';
        fclose(loc);
        system("rm loc_neogit_project.txt");
        chdir(neogit_project_location);
        system("neogit status > status_temp.txt");
        char search[90];
        FILE * status = fopen("status_temp.txt","r");
        int flag=0;
        while(fgets(search,90,status)){
            if((strstr(search,": +M")!=NULL)||(strstr(search,": -M")!=NULL)){
                flag++;
                printf(" modified file in your project!\n");
                break;
            }
        }
        fclose(status);
        system("rm status_temp.txt");
        chdir(".neogit/commits/");
        FILE * HEADfile = fopen("HEAD.txt","r");
        char HEAD[90];
        fgets(HEAD,90,HEADfile);
        fclose(HEADfile);
        int length = strlen(HEAD);
        HEAD[length-1]='\0';
        chdir(HEAD);
        //now we are in branch where HEAD is pointing to:
        system("ls > search_target_commit.txt");
        FILE * targetfile = fopen("search_target_commit.txt","r");
        char searchcopy[90];
        char target[90];
        strcpy(target,argv[3]);
        strcat(target,"\n");
        while(fgets(search,90,targetfile)){
            strcpy(searchcopy,search);
        }
        fclose(targetfile);
        system("rm search_target_commit.txt");
        int permisson_flag=0;
        if(strcmp(target,searchcopy)==0){
            permisson_flag++;
        }
        chdir("..");
        // i write present_commit.txt witch has name of our commit that we are on it;
        FILE * present_commit_file = fopen("present_commit.txt","w");
        fprintf(present_commit_file,"%s\n",argv[3]);
        fclose(present_commit_file);
        char command1[180];
        strcpy(command1,"cp -r ");
        strcat(command1,argv[3]);
        strcat(command1," ");
        strcat(command1,in_loc);
        system(command1);
        //mtime handle():
        system("pwd > P_W_D.txt");
        FILE * pwdfile = fopen("P_W_D.txt","r");
        char pwd_search[90];
        fgets(pwd_search,90,pwdfile);
        int pwd_len = strlen(pwd_search);
        pwd_search[pwd_len-1]='/';
        fclose(pwdfile);
        system("rm P_W_D.txt");
        strcat(pwd_search,argv[3]);
        char destination[90];
        strcpy(destination,in_loc);
        strcat(destination,"/");
        strcat(destination,argv[3]);
        copy_mtime_folder(pwd_search,destination);
        //now we copy .neogit :
        char location[90];
        strcpy(location,neogit_project_location);
        strcat(location,"/.neogit");
        char command2[200];
        strcpy(command2,"cp -r ");
        strcat(command2,location);
        strcat(command2," ");
        strcat(command2,in_loc);
        strcat(command2,"/");
        strcat(command2,argv[3]);
        system(command2);
        //handle mtimefolder:
        copy_mtime_folder(location,destination);
        //handle untrackted files:
        handle_untracted_files(argv[3],destination);
        //now delete our first folder:
        char command3[100];
        strcpy(command3,"rm -r ");
        strcat(command3,neogit_project_location);
        system(command3);
        //now just rename new folder:
        char command4[200];
        strcpy(command4,"mv ");
        strcat(command4,argv[3]);
        strcat(command4," ");
        strcat(command4,neogit_project_location);
        system(command4);
        //now change permissons :
        if((permisson_flag==0)&&(strcmp(argv[3],"-c")==0)){
            FILE * permisson_file = fopen("/home/.neogit_app/permisson.txt","w");
            fprintf(permisson_file,"NO\n");
            fclose(permisson_file);
            char per_command[100];
            strcpy(per_command,"chmod -R 555 ");
            strcat(per_command,argv[3]);
            system(per_command);
        }else{
            if(checkdirectory("/home/.neogit_app/permisson.txt")==0){
                char permisson_command[90];
                strcpy(permisson_command,"rm ");
                strcat(permisson_command,"/home/.neogit_app/permisson.txt");
                system(permisson_command);
            }
            char per_command[100];
            strcpy(per_command,"chmod -R 777 ");
            strcat(per_command,argv[3]);
            system(per_command);
        }
        return 0;
    }else if((strcmp(argv[1],"checkout")==0)&&(strcmp(argv[2],"HEAD")==0)){
        if(testproject()==0){
            printf("you didn't initialized neogit in your project.\n");
            return 0;
        }
        FILE * HEADfile = fopen(".neogit/commits/HEAD.txt","r");
        char HEAD[30];
        fgets(HEAD,30,HEADfile);
        fclose(HEADfile);
        int len = strlen(HEAD);
        HEAD[len-1]='\0';
        chdir(HEAD);
        system("ls > search_commit.txt");
        FILE * commits = fopen("search_commit.txt","r");
        char search[300][90];
        char target_commit[90];
        int counter=0;
        while(fgets(search[counter],90,commits)){
            counter++;
        }
        fclose(commits);
        system("rm search_commit.txt");
        int return_number=0;
        if(argv[3]!=NULL){
            sscanf(argv[3],"-%d",&return_number);
        }
        strcpy(target_commit,search[counter-1-return_number]);
        int length = strlen(target_commit);
        target_commit[length-1]='\0';
        char result[100];
        strcpy(result,"neogit checkout -c ");
        strcat(result,target_commit);
        system(result);
        return 0;
    }else if((strcmp(argv[1],"revert")==0)&&(strcmp(argv[2],"-n")==0)){
        int hash;
        sscanf(argv[4],"%d",&hash);
        hash--;
        char command[200];
        char H[10];
        sprintf(H,"%i",hash);
        strcpy(command,"neogit checkout -d ");
        strcat(command,H);
        system(command);
    }else if((strcmp(argv[1],"revert"==0))&&(strcmp(argv[2],"[e]")!=0)){
        int hash;
        sscanf(argv[4],"%d",&hash);
        hash--;
        char search[10];
        sprintf(search,"%i",hash);
        testproject();
        char s[90];
        strcpy(s,neogit_project_location);
        strcat(s,"/.neogit/commits/allcommits.txt");
        FILE * file = fopen(s,"r");
        char search_copy[90];
        char path[90];
        char last[90];
        while(fgets(search_copy,90,file)){
            if(strstr(search_copy,seach)!=NULL){
                strcpy(path,search_copy);
                int len = strlen(path);
                path[len-1]='\0';
            }else{
            strcpy(last,search_copy);
            int length = strlen(last);
            last[length-1]='\0';
            }
        }
        fclose(file);

    }else if((strcmp(argv[1],"tag")==0)&&(strcmp(argv[2],"-a")==0)){
        testproject();
        chdir(".neogit/commits");
        // we create tag.txt which has all tags with commits ID in it:
        char tags_location[90];
        strcpy(tags_location,neogit_project_location);
        strcat(tags_location,"/.neogit/commits/tags.txt");
        //at first check if we had this tag before or not?
        if(checkdirectory(tags_location)==0){
            FILE * tag_file = fopen("tags.txt","r");
            char tag_copy[40];
            strcpy(tag_copy,"tag ");
            strcat(tag_copy,argv[3]);
            strcat(tag_copy,"\n");
            char search_tag[40];
            int tag_flag=0;
            int flag=0;
            while(fgets(search_tag,40,tag_file)){
                if(strcmp(search_tag,tag_copy)==0){
                    flag++;
                    break;
                }
            }
            fclose(tag_file);
            if(flag==1){
                printf("there is a tag with this name!\n");
                printf("enter another tag!\n");
                return 0;
            }
        }
        //at first we should checkout that if there is c flag or not:||m flag :
        int f_flag=0;
        int c_flag_num=0;
        int m_flag_num=0;
        char c_flag[50];
        char m_flag[100];
        if((argv[argc-1][0]=='[')&&(argv[argc-1][1]=='f')){
            f_flag++;
            if((argv[argc-3][0]=='[')&&(argv[argc-3][2]=='c')){
                c_flag_num++;
                int len = strlen(argv[argc-2]);
                argv[argc-2][len-1]='\0';
                strcpy(c_flag,argv[argc-2]);
                if((argv[argc-5][0]=='[')&&(argv[argc-5][2]=='m')){
                    m_flag_num++;
                    int length = strlen(argv[argc-4]);
                    argv[argc-4][length-1]='\0';
                    strcpy(m_flag,argv[argc-4]);
                }
            }else if((argv[argc-3][0]=='[')&&(argv[argc-3][2]=='m')){
                m_flag_num++;
                int len = strlen(argv[argc-2]);
                argv[argc-2][len-1]='\0';
                strcpy(m_flag,argv[argc-2]);
            }
        }else{
            if((argv[argc-2][0]=='[')&&(argv[argc-2][2]=='c')){
                c_flag_num++;
                int len = strlen(argv[argc-1]);
                argv[argc-1][len-1]='\0';
                strcpy(c_flag,argv[argc-1]);
                if((argv[argc-4][0]=='[')&&(argv[argc-4][2]=='m')){
                    m_flag_num++;
                    int length = strlen(argv[argc-3]);
                    argv[argc-3][length-1]='\0';
                    strcpy(m_flag,argv[argc-3]);
                }
            }else if((argv[argc-2][0]=='[')&&(argv[argc-2][2]=='m')){
                m_flag_num++;
                int len = strlen(argv[argc-1]);
                argv[argc-1][len-1]='\0';
                strcpy(m_flag,argv[argc-1]);
            }
        }
        FILE * tag_file2 = fopen(tags_location,"a");
        fprintf(tag_file2,"tag %s\n",argv[3]);
        char commit_ID[50];
        if(c_flag_num==0){
            FILE * present_commit_file = fopen("present_commit.txt","r");
            char commit_temp[50];
            fgets(commit_temp,50,present_commit_file);
            fclose(present_commit_file);
            int len = strlen(commit_temp);
            commit_temp[len-1]='\0';
            strcpy(commit_ID,commit_temp);
        }else{
            strcpy(commit_ID,c_flag);
        }
        fprintf(tag_file2,"commit %s\n",c_flag);
        chdir("..");
        char name[50];
        char email[100];
        FILE * name_file = fopen("account_name.txt","r");
        fgets(name,50,name_file);
        int name_len = strlen(name);
        name[name_len-1]='\0';
        fclose(name_file);
        FILE * email_file = fopen("account_email.txt","r");
        fgets(email,50,email_file);
        int email_len = strlen(email);
        email[email_len-1]='\0';
        fclose(email_file);
        printf("Author : %s <%s>\n",name,email);
        time_t present = time(NULL);
        fprintf(tag_file2,"Date : %s",ctime(&present));
        if(m_flag_num==1){
            fprintf(tag_file2,"Message : %s\n",m_flag);
        }else{
            fprintf(tag_file2,"Message : empty\n");
        }
        fprintf(tag_file2,"\n");
        fclose(tag_file2);
    }else if((strcmp(argv[1],"tag")==0)&&(argc==2)){
        testproject();
        char tags[300][50];
        char search[60];
        char tag_file_path[90];
        strcpy(tag_file_path,neogit_project_location);
        strcat(tag_file_path,"/.neogit/commits/tags.txt");
        if(checkdirectory(tag_file_path)==-1){
        printf("you didn't assign any tag for this project!\n");
        return 0;
        }
        int count=0;
        FILE * tag_file = fopen(tag_file_path,"r");
        for(int i=0;fgets(search,60,tag_file);i++){
            char * ptr = strtok(search," ");
            if(strcmp(ptr,"tag")==0){
                ptr = strtok(NULL," ");
                strcpy(tags[i],ptr);
            }
            count++;
        }
        fclose(tag_file);
        for(int i=0 ; i<count;i++){
            for(int j=0;j<count-i;j++){
                if(strcmp(tags[j],tags[j+1])>0){
                    swap(tags[j],tags[j+1]);
                }
            }
        }
        for(int i=0;i<count;i++){
            printf("tag %s\n",tags[i]);
        }
    }else if((strcmp(argv[1],"tag")==0)&&(strcmp(argv[2],"show")==0)){
        testproject();
        char tag_file_path[90];
        strcpy(tag_file_path,neogit_project_location);
        strcat(tag_file_path,"/.neogit/commits/tags.txt");
        if(checkdirectory(tag_file_path)==-1){
        printf("you didn't assign any tag for this project!\n");
        return 0;
        }
        char search[60];
        char test[60];
        strcpy(test,"tag ");
        strcat(test,argv[3]);
        strcat(test,"\n");
        int tag_flag=0;
        FILE * tag_file = fopen(tag_file_path,"r");
        while(fgets(search,60,tag_file)){
            if(strcmp(search,test)==0){
                printf("%s",search);
                for(int i=0;i<4;i++){
                    fgets(search,60,tag_file);
                    printf("%s",search);
                }
                tag_flag++;
                break;
            }
        }
        if(tag_flag==0){
            printf("there isn't any tag with this title\n");
        }
    }else if(strcmp(argv[1],"tree")==0){
        testproject();
        char location[120];
        char all_branchs[10][20];
        int branchs_commits[10][30];
        strcpy(location,neogit_project_location);
        strcat(location,"/.neogit/commits");
        chdir(location);
        system("ls > branchs_finder.txt");
        FILE * branchs = fopen("branchs_finder.txt","r");
        char search[90];
        char second_branch[90];
        char searchcopy[90];
        int second_branch_exist=0;
        while(fgets(search,90,branchs)){
            strcpy(searchcopy,search);
            int len = strlen(searchcopy);
            searchcopy[len-1]='\0';
            char loc[120];
            strcpy(loc,location);
            strcat(loc,"/");
            strcat(loc,searchcopy);
            if(checkdirectory(loc)==1){
                strcpy(second_branch,searchcopy);
                second_branch_exist++;
                break;
            }
        }
        fclose(branchs);
        system("rm branchs_finder.txt");
        //handle master:
        chdir("master");
        system("ls > all_commits.txt");
        FILE * master_commits = fopen("all_commits.txt","r");
        char search2[30];
        char master_commits_ID[30][10];
        int counter=0;
        while(fgets(search2,30,master_commits)){
            int len = strlen(search2);
            search2[len-1]='\0';
            if(strcmp(search2,"all_commits.txt")==0)  continue;
            strcpy(master_commits_ID[counter],search2);
            counter++;
        }
        fclose(master_commits);
        system("rm all_commits.txt");
        chdir("..");
        //now whole this process for second branch:
        // i call this branch main:(locally)
        if(second_branch_exist){
            chdir(second_branch);
            system("ls > all_commits.txt");
            FILE * main_commits = fopen("all_commits.txt","r");
            char search3[30];
            char main_commits_ID[30][10];
            int counter2=0;
            while(fgets(search3,30,master_commits)){
                char search_copy[30];
                strcpy(search_copy,search3);
                if(strcmp(search3,"all_commits.txt\n")==0)  continue;
                int slen = strlen(search_copy);
                search_copy[slen-1]='\0';
                strcpy(main_commits_ID[counter2],search_copy);
                counter2++;
            }
            fclose(main_commits);
            system("rm all_commits.txt");
            chdir("..");
            //now lets draw it :
        // first point is the last common point betwen to branches:
            int first_point=0;
            int merge_point=0;
            int flag=0;
            for(int i=0;i<counter; i++){
                if(strcmp(master_commits_ID[i],main_commits_ID[i])!=0){
                    if(flag==0){
                        first_point=i-1;
                        flag++;
                    }
                }else if((flag==1)&&(strcmp(master_commits_ID[i],main_commits_ID[i])==0)){
                    merge_point=i;
                    break;
                }
            }
        //master till first_point : 
            for(int i=0;i<=first_point;i++){
                printf("%s\n",master_commits_ID[i]);
                if(i==first_point){
                    printf(" |\\");
                    printf("\n");
                    printf(" | \\");
                    printf("\n");
                    printf(" |  \\");
                    printf("\n");
                    printf(" |   \\");
                    printf("\n");
                }else{
                    printf(" |");
                    printf("\n");
                    printf(" |");
                    printf("\n");
                }
            }
            for(int i=first_point+1;i<4;i++){
                printf("%s  %s\n",master_commits_ID[i],main_commits_ID[i]);
                if(i==merge_point-1){
                    printf(" |   /\n");
                    printf(" |  /\n");
                    printf(" | /\n");
                    printf( "|/\n");
                }else{
                    printf(" |     |\n");
                    printf(" |     |\n");
                }
            }
            // for(int i=merge_point;i<counter;i++){
            //     printf("%s\n",master_commits_ID[i]);
            //     if(i!=counter-1){
            //         printf(" |\n");
            //         printf(" |\n");
            //     }
            // }
        }else{
            printf("%s\n",master_commits_ID[0]);
            for(int i=1 ; i<counter;i++){
                printf(" |\n");
                printf("%s\n",master_commits_ID[i]);
            }
        }   
    }else if(strcmp(argv[1],"grep")==0){
        char file_name[20];
        strcpy(file_name,argv[3]);
        char word[50];
        strcpy(word,argv[5]);
        int n_flag=0;
        int commit_flag=0;
        char commit_ID[10];
        if(argc==9){
            n_flag++;
            commit_flag++;
            strcpy(commit_ID,argv[8]);
            int len = strlen(commit_ID);
            commit_ID[len-1]='\0';
        }else if(argc==8){
            commit_flag++;
            strcpy(commit_ID,argv[7]);
            int len = strlen(commit_ID);
            commit_ID[len-1]='\0';
        }else if(argc==7){
            n_flag++;
        }
        char file_location[90];
        if(commit_flag==1){
            testproject();
            chdir(neogit_project_location);
            chdir("commits");
            FILE * allcommits = fopen("allcommits.txt","r");
            char search[90];
            char searchcopy[90];
            while(fgets(search,90,allcommits)){
                if(strstr(search,commit_ID)){
                    strcpy(searchcopy,search);
                    int length = strlen(searchcopy);
                    searchcopy[length-1]='\0';
                    strcpy(file_location,searchcopy);   
                }
            }
            fclose(allcommits);
        }
        if(commit_flag==0){
            strcpy(file_location,file_name);
        }
        FILE * file = fopen(file_location,"r");
        char temp[120];
        int counter=1;
        while(fgets(temp,120,file)){
            if(strstr(temp,word)){
                char * ptr = strstr(temp,word);
                int ptr_len = strlen(ptr);
                int temp_len = strlen(temp);
                int word_len = strlen(word);
                if(n_flag==1)
                printf("%d   ",counter);
                for(int i=0;i<temp_len-ptr_len;i++){
                    printf("%c",temp[i]);
                }
                printf ("\033[0;31m%s\033[0m",word);
                strcpy(ptr,ptr+word_len);
                printf("%s",ptr);
            }
            counter++;
        }
    }else if((strcmp(argv[1],"diff")==0)&&(strcmp(argv[2],"-f")==0)){
        // char file1[30];
        // strcpy(file1,argv[3]);
        // char file2[30];
        // strcpy(file2,argv[4]);
        int line_flag=0;
        int first_file_begin=0;
        int first_file_end=0;
        int second_file_begin=0;
        int second_file_end=0;
        if(argc>5){
            line_flag++;
            sscanf(argv[6],"%d-%d",&first_file_begin,&first_file_end);
            sscanf(argv[8],"%d-%d",&second_file_begin,&second_file_end);
        }
        char present_location[90];
        FILE * file1 = fopen(argv[3],"r");
        char line1[90];
        char file1_lines[300][90];
        int count=0;
        int begin1=0;
        //number of lines is count;
        if(line_flag==0){
            for(int i=0;fgets(line1,90,file1);i++){
                if(strcmp(line1,"\n")==0)  continue;
                if(begin1==0)
                begin1=i+1;
                strcpy(file1_lines[count],line1);
                count++;
            }
        }else{
            int line_number=0;
            for(int i=0;fgets(line1,90,file1);i++){
                if(strcmp(line1,"\n")==0)  continue;
                if((line_number>=first_file_begin-1)&&(line_number<first_file_end)){     
                    if(begin1==0)
                    begin1=i+1;       
                    strcpy(file1_lines[count],line1);
                    count++;  
                }
                line_number++;
            }
        }
        fclose(file1);
        FILE * file2 = fopen(argv[4],"r");
        char line2[90];
        char file2_lines[300][90];
        int count2=0;
        int begin2=0;
        if(line_flag==0){
            for(int i=0;fgets(line2,90,file2);i++){
                if(strcmp(line2,"\n")==0)  continue;
                if(begin2==0)
                begin2 = i+1;
                strcpy(file2_lines[count2],line2);
                count2++;
            }
        }else{
            int line2_number=0;
            for(int i=0;fgets(line2,90,file2);i++){
                if(strcmp(line2,"\n")==0)  continue;
                if((line2_number>=second_file_begin-1)&&(line2_number<=second_file_end)){ 
                    if(begin2==0)
                    begin2=i+1;           
                    strcpy(file2_lines[count2],line2);
                    count2++;  
                }
                line2_number++;
            }
        }
        fclose(file2);
        int flag=0;
        for(int i=0;i<count;i++){
            if(diffchecker(file1_lines[i],file2_lines[i])){
                printf("<file1 : %s> <%d> \n",argv[3],begin1+i);
                printf("%s",file1_lines[i]);
                if(i==count-1){
                    int l = strlen(file1_lines[count-1]);
                    if(file1_lines[count-1][l-1]!='\n') printf("\n");
                }
                printf("<file2 : %s> <%d> \n",argv[4],begin2+i);
                printf("%s",file2_lines[i]);
                if(i==count-1){
                    int ll= strlen(file2_lines[count-1]);
                    if(file2_lines[count-1][ll-1]!='\n') printf("\n");
                }
            }

        }
            printf("diff command has finished for this two files\n");
    }else if((strcmp(argv[1],"diff")==0)&&(strcmp(argv[2],"-c")==0)){
        testproject();
        char commit1[90];
        char commit2[90];
        char mainpath[100];
        strcpy(mainpath,neogit_project_location);
        strcat(mainpath,"/");
        strcat(mainpath,".neogit/commits/");
        int length = strlen(mainpath);
        FILE * allcommits = fopen(".neogit/commits/allcommits.txt","r");
        char search[90];
        while(fgets(search,90,allcommits)){
            if(strstr(search,argv[3])!=NULL){
                strcpy(commit1,search);
                int len = strlen(commit1);
                commit1[len-1]='\0';
            }else if(strstr(search,argv[4])!=NULL){
                strcpy(commit2,search);
                int len = strlen(commit2);
                commit2[len-1]='\0';
            }
        }
        diff_folderchecker(commit1,commit2,length);
        diff_folderchecker_reverse(commit2,commit1,length);
    }else if((strcmp(argv[1],"merge")==0)&&(strcmp(argv[2],"-b")==0)){
        
    }else if((strcmp(argv[1],"config")==0)&&(argv[2][0]=='a')){
        char search; 
        search = argv[2][0];
        int counter=0;
        while(search!='.'){
            counter++;
            search = argv[2][counter];
        }
        strcpy(argv[2],argv[2]+counter+1);
        // now we pure the command:
        strcpy(argv[3],argv[3]+1);
        int len = strlen(argv[3]);
        argv[len-1]='\0';
        //verify argv[3]:
        int test=0;
        for(int i=50 ;1; i++){
            if(strcmp(argv[3],allcommands[i])==0){
                test++;
                break;
            }
        }
        if(test==0){
            printf("invalid command\n");
        }else{
            // now we open txt file for alias commands:
        FILE * alias = fopen("/home/.neogit_app/alias_commands.txt","a");
        fprintf(alias,"%s",argv[2]);
        fprintf(alias,"%s",argv[3]);
        // in our alias file every time that we use alias command it goes here and append name of new alias and then
        //our common command;
        fclose(alias);
        }
    }else{
        char newcommand[30];
        strcpy(newcommand,argv[1]);
        for(int i=2 ;argv[i]!=NULL; i++){
            strcat(newcommand,argv[i]);
        }
        FILE * alias = fopen("/home/.neogit_app/alias_commands.txt","r");
        char commands[30];
        while(fgets(commands,30,alias)){
            if(strcmp(newcommand,commands)==0){
                char order[40];
                strcpy(order,"./neogit ");
                fgets(commands,30,alias);
                strcat(order,commands);
                system(order);
            }
        }
    }
        

}