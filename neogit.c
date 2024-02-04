#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
char neogit_project_location[60];
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
int assign_local(char * type , char * name){
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
    if(flag==0){
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
        FILE * file = fopen("/home/.neogit_app/locations.txt","r+");
    if(strcmp(type,"user.name")==0){
            FILE * global_username = fopen("/home/.neogit_app/global_names.txt","w");
        while(fgets(search,70,file)){
            chdir(search);
            chdir(".neogit");
            FILE * account = fopen("account_name.txt","w");
            fprintf(account ,"%s",name);
            fclose(account);
        }
        fprintf(global_username , "%s", name);
        fclose(global_username);
    }else if(strcmp(type,"user.email")==0){
            FILE * global_useremail=fopen("/home/.neogit_app/global_emails.txt","w");
        while(fgets(search,70,file)){
            chdir(search);
            chdir(".neogit");
            FILE * account = fopen("account_email.txt","w");
            fprintf(account ,"%s",name);
            fclose(account);
        }
        fprintf(global_useremail , "%s", name);
        fclose(global_useremail);
    } 
    fclose(file);
}
void add_file(char file[]){
    if(find_file(file)){
            //we copy our file which it can be folder or any other file;
            //now we made our command:
            char command[100];
            strcpy(command,"cp -ur ");
            char neogit_loc[60];
            strcpy(neogit_loc , neogit_project_location);
            int len = strlen(neogit_loc); 
            neogit_loc[len-1]='\0';
            strcat(command,file);
            strcat(command," ");
            strcat(command,neogit_loc);
            strcat(command ,"/.neogit/staged_files");
            system(command);

        }else{
            printf("there isn't any file with the name of %s\n", file);
        }
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
            system("mkdir commits");
            //commits:
            chdir("commits");
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
            chdir("..");
            //now we are back in .neogit folder;
            //copy all project to our primary directories;
            //and also we make 2 account.txt for save our email and username
            char NAME[30] , EMAIL[30];
            FILE * global_username = fopen("/home/.neogit_app/global_names.txt","r+");
            char search1[30];
            if(fgets(search1,30,global_username)){
                fgets(NAME , 30 , global_username);
                fclose(global_username);
            }else{
                fclose(global_username);
            }
            FILE * global_useremail= fopen("/home/.neogit_app/global_emails.txt","r+");
            if(fgets(search1,30,global_useremail)){
                fgets(EMAIL , 30 , global_useremail);
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
        if(flag==0){
            printf("you didn't initialized neogit in your project\n");
            return 0;
        }
        if(strcmp(argv[2],"-f")==0){
            for(int i=3 ; i< argc ;i++){
            //argv[i] don't have any \n at end of them
            add_file(argv[i]);
            }
        }else if(strcmp(argv[2],"-n")==0){
            
        }else{
            add_file(argv[2]);
        }
        
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