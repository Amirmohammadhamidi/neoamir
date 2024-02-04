#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<time.h>
char neogit_project_location[60];
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
int difchecker(FILE * past , FILE * present){
    //if they are same it shows 1 unless it shows 0;
    char past_lines[100];
    char present_lines[100];
    int flag=0;
    while(fgets(past_lines,100,past)){
        flag++;
        fgets(present_lines,100,present);
        if(strcmp(past_lines,present_lines)!=0){
            return 0;
            break;
        }
    }
    if(flag==0){
        if(fgets(present_lines,100,present)){
            return 0;
        }
    }
    return 1;
}
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
            strcpy(temp, neogit_project_location);
            strcat(temp,"/.neogit/commits/temp.txt");
            FILE * timeline = fopen(timelineloc,"r");
            FILE * tempfile = fopen(temp,"w");
            char search[70];
            int flag=0;
            while(fgets(search,70,timeline)){
                fprintf(tempfile,"%s",search);
                if(strcmp(search,path)==0){
                    fgets(search,70,timeline);
                    if(strcmp(search,ctime(&present))!=0){
                        flag++;
                    }
                    fprintf(tempfile,"%s",search);
                    flag++;
                }
            }
            if(flag==0){
                    fprintf(tempfile,"%s",path);
                    fprintf(tempfile,"%s",ctime(&present));
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
            fclose(tempfile);
            chdir(neogit_project_location);
            chdir(".neogit/commits");
            system("rm timeline.txt");
            system("mv temp.txt timeline.txt");
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
        }else if(checkdirectory(path)==0){
            if(check_exist(path,Astage)==0){
                FILE * add = fopen(Astage,"a");
                fprintf(add,"%s",path);
                fprintf(add,"\n");
                fclose(add);
                modifyfile(filepath,add_mode);
            }
        }else{
            printf("there isn't any file or directory with this name");
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
        if(flag==0){
            int len = strlen(path);
            path[len-1]='\0';
            printf("%s : ",path);
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
        FILE * timeline = fopen(alocation,"r");
        char search[80];
        char pathcopy1[90];
        char pathcopy2[90];
        char path[90];
        while(fgets(search,80,timeline)){
            strcpy(path,search);
            int length = strlen(path);
            path[length-1]='\0';
            printf("%s : ",path);
            strcpy(pathcopy1,path);
            strcpy(pathcopy2,path);
            if(check_stagedfiles(pathcopy1)==1)
            printf("+");
            else if((check_stagedfiles(pathcopy2)==-1)||(check_stagedfiles(pathcopy2)==0))
            printf("-");
            if(checkdirectory(path)==-1){
                fgets(search,80,timeline);
                printf("D\n");
            }else{
                struct stat filestat;
                if(stat(path,&filestat)==0){
                    time_t present = filestat.st_mtime;
                    fgets(search,80,timeline);
                    if(strcmp(search,ctime(&present))!=0){
                        printf("M\n");
                    }else{
                        printf("not modified\n");
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
    void makecommits_hash(char file_hash[]){
        FILE * commits_hashs = fopen("/home/.neogit_app/commits_hashs.txt","r");
        char hash[60];
        fgets(hash , 60 , commits_hashs);
        fclose(commits_hashs);
        FILE * commits_hashs2 = fopen("/home/.neogit_app/commits_hashs.txt","w");
        int HASH;
        sscanf(hash ,"%i", &HASH);
        HASH++;
        fprintf(commits_hashs,"%i\n",HASH);
        fclose(commits_hashs);
        char hashcopy[60];
        sprintf(hashcopy , "%i" , HASH);
        char command[70];
        strcpy(command ,"mkdir ");
        strcat(command , hashcopy);
        system(command);
        chdir(hashcopy);
        strcpy(file_hash,hashcopy);
    }
    int makefolders(char relativepath[] , char absolutepath[]){
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
        char lastfolder[90];
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
        }
        char COMMAND[100];
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
        strcat(COMMAND,lastfolder);
        system(COMMAND);
    }
    void commits_details(char hash[],int number_files,char author_name[],char message[]){
        //hash
        //date
        //number of files
        //author name
        //message
        FILE * details = fopen("commits_details.txt","w");
        fprintf(details,"%s\n",hash);
        time_t present = time(NULL);
        fprintf(details,"%s\n%d\n%s\n%s\n",ctime(&present),number_files,author_name,message);
        fclose(details);
        printf(" ID : %s\n",hash);
        printf("DATE : %s\n",ctime(&present));
        printf("MESSAGE : %s\n",message);
    }
int main(int argc , char * argv[]){
    // we pure all txt files that we need for our programm in .neogit_app folder in /home and we made that before;
    //now we have this 2D strig which have all commands:
    char allcommands[50][50];

    //we save all locations of our projects in locations.txt in /home:
    FILE * location = fopen("/home/.neogit_app/locations.txt","a");
    //first step is to build neogit init command
    if(strcmp(argv[1],"init")==0){
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
            int len=strlen(first_location);
            first_location[len-1]='\n';
            fprintf(location,"%s",first_location);
            first_location[len-1]='\0';
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
            chdir("..");
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
        fclose(location);
    }
    //user.name and user.email
    else if(strcmp(argv[1],"config")==0){
        if(strcmp(argv[2],"-global")==0){
            assign_global(argv[3],argv[4]);
        }else if(strcmp(argv[2],"-global")!=0){
            assign_local(argv[2],argv[3]);
        }
    }
    //part of add:
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
        char presentloc[90];
        system("pwd > prloc.txt");
        FILE * file = fopen("prloc.txt","r");
        fgets(presentloc,90,file);
        fclose(file);
        system("rm prloc.txt");
        int length = strlen(presentloc);
        //we should update time line ,remove Astaged.txt and staged_files.txt
        // chdir(".neogit/commits");
        char pathes[300][80];
        char files_abs_path[300][80];
        char location[90];
        strcpy(location,neogit_project_location);
        strcat(location,"/.neogit/staged_files/Astagedfiles.txt");
        FILE * Astagedfiles = fopen(location,"r");
        char search[90];
        int flag=0;
        int counter=0;
        for(int i=0 ; fgets(search,90,Astagedfiles);i++){
            int len = strlen(search);
            search[len-1]='\0';
            if(checkdirectory(search)==0){
                number_files_commited++;
                strcpy(files_abs_path[counter],search);
                strcpy(search,search+length);
                strcpy(pathes[counter],search);
                counter++;
                flag++;
            }
        }
        if(flag==0){
            printf("there isn't any file in stagine area!\n");
            return 0;
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
        strcat(fullpath,"/master");
        if(checkdirectory(fullpath)==-1){
            system("mkdir master");
        }
        chdir("master");
        char hash[80];
        makecommits_hash(hash);
        //now we are inside our commit with hash name;
        for(int i=0 ;i<counter;i++){
            makefolders(pathes[i],files_abs_path[i]);
        }
        commits_details(hash,number_files_commited,author_name,message);
    }else if(strcmp(argv[1],"set")==0){
        if(argv[4]!=NULL){
            printf("you didn't enter shortcut\n");
            return 0;
        }
        char message[100];
        strcpy(message,argv[3]);
        char shortcut[70];
        strcpy(shortcut,argv[4]);
        FILE * sortcuts_file = fopen("/home/.neogit_app/shortcuts_file.txt","a");
        fprintf(sortcuts_file,"%s\n",shortcut);
        fprintf(shortcuts_file,"%s\n",message);
        fclose(shortcuts_file);
    }else if(strcmp(argv[1],"replace")==0){
        char shortcut[90];
        strcpy(shortcut,argv[4]);
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
        



        
    }else if(strcmp(argv[1],"remove")==0){
        char shortcut[90];
        strcpy(shortcut,argv[3]);
        strcat(shortcut,"\n");
        FILE * shortcuts_file = fopen("/home/.neogit_app/shortcuts_file.txt","r");
        FILE * shortcuts_temp = fopen("/home/.neogit_app/shortcuts_temp.txt","w");
        char search[90];
        while(fgets(search,90,shortcuts_file)){
            if(strcmp(search,shortcut)==0){
                fgets(search,90,shortcuts_file);
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