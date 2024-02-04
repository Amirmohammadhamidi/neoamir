#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<time.h>
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
        if(checkdirectory("/home/.neogit/deleted_files.txt")==-1)  return 0;
        FILE * deleted_files = fopen("/home/.neogit_app/deleted_files.txt","r");
        char search[90];
        char files_path[90];
        char command[100];
        int length = strlen(neogit_project_location);
        while(fgets(search,90,deleted_files)){
            int len = strlen(search);
            search[len-1]='\0';
            strcpy(files_path,search);
            strcpy(files_path,files_path+length+1);
            strcpy(command,"rm ");
            strcat(command,files_path);
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
    //file is path of our file;
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
        strcat(aloc,"/.neogit/commits/");
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
        present_loc[len-1]='\0';
        len--;
        //now we get that;
        //now we find out if there is any directory (commits) or not:if there is we get its last directory:
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
        int searc_det_copy_len=strlen(search_det_copy);
        search_det_copy[searc_det_copy_len-1]='\0';
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
            char copy_command[160];
            strcpy(copy_command,"cp -r ");
            strcat(copy_command,search_det_copy);
            strcat(search_det_copy,".orig");
            strcat(copy_command," ");
            strcat(copy_command,search_det_copy);
            system(copy_command);
            char rename_command[160];
            strcpy(rename_command,"mv ");
            strcat(rename_command,search_det_copy);
            strcat(rename_command," ");
            strcat(rename_command,hashcopy);
            system(rename_command);
        }else{
            char command[70];
            strcpy(command ,"mkdir ");
            strcat(command , hashcopy);
            system(command);
        }
        chdir(hashcopy);
        strcpy(file_hash,hashcopy);
    }
    void makefolders(char relativepath[] , char absolutepath[] , char hash[]){
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
        char COMMAND[100];
        char dloc[80];
        strcpy(dloc,hash);
        strcat(dloc,"/");
        if(flag!=0)
        strcat(dloc,lastfolder);
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
        strcat(COMMAND,dloc);
        system(COMMAND);
        chdir(hash);
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
    int checktime(char commit_time[],char date[],char second[]){    
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
            strcpy(list,list+len+1);
            strcpy(result,result_folder_path);
            strcat(result,"/");
            strcat(result,list);
            strcat(command,result);
            system(command);
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
            add_file(argv[2],0);
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
        char pathes[300][80];
        char files_abs_path[300][80];
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
                char p[120];
                strcpy(p,location);
                strcat(p,"/");
                strcat(p,search);
                strcpy(files_abs_path[counter],p);
                strcpy(p,p+length);
                strcpy(pathes[counter],p);
                counter++;
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
        ///now we get our present branch:
        strcat(fullpath,"/");
        strcat(fullpath,HEAD_branch);
        chdir(fullpath);
        char hash[80];
        makecommits_hash(hash);
        //now we are inside our commit with hash name;
        for(int i=0 ;i<counter;i++){
            makefolders(pathes[i],files_abs_path[i],hash);
        }
        commits_details(HEAD_branch,hash,number_files_commited,author_name,message);
        deleted_added_files();
        //remove all staged_files:
        chdir("../..");
        //renew timeline.txt
        renew_timeline();
        //now all files where commited there last modified time has been commited:
        chdir("..");
        system("mkdir tempstagingarea");
        system("cp staged_files/trackted_files.txt tempstagingarea");
        //we should copy this version of staged_files to .neogit_app folder to its 
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
            if(flag==0){
                if((checktime(search,argv[3],argv[4])==1)){
                    flag++;
                }else{
                    continue;
                }
            }
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
        //now we get our present branch;
        system("ls > commitsdet.txt");
        FILE * commits_det = fopen("commitsdet.txt","r");
        char search_commits[90];
        char cp_command[150];
        while(fgets(search_commits,90,commits_det)){
            int len = strlen(search_commits);
            search_commits[len-1]='\0';
            if(strcmp(search_commits,"commitsdet.txt")==0)  continue;
            strcpy(cp_command,"cp -r ");
            strcat(cp_command,search_commits);
            strcat(cp_command," ");
            strcat(cp_command,new_branch_absolutepath);
            system(cp_command);
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
            if(strcmp(search_commits,"commit_checkout.txt\n")!=0){
                strcpy(search_commits_copy,search_commits);
            }
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
        char command2[200];
        strcpy(command2,"cp -r ");
        strcat(command2,neogit_project_location);
        strcat(command2,"/.neogit");
        strcat(command2," ");
        strcat(command2,in_loc);
        strcat(command2,"/");
        strcat(command2,search_commits_copy);
        system(command2);
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
    }else if((strcmp(argv[1],"checkout")==0)&&(strcmp(argv[2],"-c")==0)){
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
                printf("there is modified file in your project!\n");
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
        char command1[180];
        strcpy(command1,"cp -r ");
        strcat(command1,argv[3]);
        strcat(command1," ");
        strcat(command1,in_loc);
        system(command1);
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
        if(permisson_flag==0){
            char per_command[100];
            strcpy(per_command,"chmod -R 555 ");
            strcat(per_command,argv[3]);
            system(per_command);
        }else{
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
    }else if((strcmp(argv[1],"revert")==0)&&(strcmp(argv[2],"[e]")==0)){
    }else if(strcmp(argv[1],"tag")&&(argc>=6)){
        testproject();
        chdir(".neogit/commits");
        // we create tag.txt which has all tags with commits ID in it:
        FILE * tag_file = fopen("tags.txt","w");
    }






















    else if((strcmp(argv[1],"config")==0)&&(argv[2][0]=='a')){
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
    return 0;
}