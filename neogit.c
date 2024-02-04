#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
char neogit_project_location[30];
// it has our present project .neogit folder location;
int check_neogit(){
    int test=0;
    system("touch details.txt");
    system("ls -a > details.txt");
    FILE * file = fopen("details.txt","r");
    char search[30];
    while(fgets(search,30,file)){
        if(strcmp(search,".neogit\n")==0){
            // save our present neogit folder
            system("touch loc.txt");
            system("pwd > loc.txt");
            FILE * loc = fopen("loc.txt","r");
            fgets(neogit_project_location,30,loc);
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
    char search[30];
    fgets(search,30,file);
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
    if(strcmp(type,"user.name")==0){
        chdir(neogit_project_location);
        chdir(".neogit");
        FILE * file = fopen("account_name.txt","w");
        fprintf(file,"%s",name);
        fclose(file);
    }else if(strcmp(type, "user.email")==0){
        chdir(neogit_project_location);
        chdir(".neogit");
        FILE * file = fopen("account_name.txt","w");
        fprintf(file,"%s",name);
        fclose(file);
    }   
    return 1;
}
void assign_global(char * type , char * name){
        char search[30];
        FILE * file = fopen("/home/locations.txt","r");
    if(strcmp(type,"user.name")==0){
            FILE * global_username = fopen("/home/.neogit_app/global_names.txt","w");
        while(fgets(search,30,file)){
            chdir(search);
            chdir(".neogit");
            FILE * account = fopen("account_name.txt","w");
            fprintf(account ,"%s",name);
            fclose(account);
        }
        fprintf(global_username , "%s", name);
        fclose(global_username);
    }else if(strcmp(type, "user.email")==0){
            FILE * global_useremail=fopen("/home/.neogit_app/global_emails.txt","w");
        while(fgets(search,30,file)){
            chdir(search);
            chdir(".neogit");
            FILE * account = fopen("account_email.txt","w");
            fprintf(account ,"%s",name);
            fclose(account);
        }
        fprintf(global_useremail , "%s", name);
        fclose(global_useremail);
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
        //check that if there is .neogit folder in our present library or not?
            FILE * global_username = fopen("/home/.neogit_app/global_names.txt","r");
            FILE * global_useremail=fopen("/home/.neogit_app/global_emails.txt","r");
        system("touch project_location.txt");
        system("pwd > project_location.txt");
        FILE * file = fopen("project_location.txt","r");
        char first_location[50];
        fgets(first_location,50,file);
        fclose(file);
        system("rm project_location.txt");
        int test=0;
        while(1){
            if(check_neogit()==0){
                printf("you initialized neogit before in this project\n");
                break;
            }else if(check_root()==0){
                test++;
            }
            if(test>1){
                break;
            }
            chdir("..");
        }
        if(test==2){
            int len=strlen(first_location);
            first_location[len-1]='\0';
            chdir(first_location);
            fprintf(location,"%s",first_location);
            strcpy(neogit_project_location,first_location);
            system("mkdir .neogit");
            chdir(".neogit");
            FILE * account = fopen("account_name.txt","w");
            char NAME[30] , EMAIL[30];
            fgets(NAME , 30 , global_username);
            fgets(EMAIL , 30 , global_useremail);
            fprintf(account, "%s", NAME);
            fprintf(account,"%s",EMAIL);
            fclose(location);
        }
        fclose(global_username);
        fclose(global_useremail);
    }
    //user.name and user.email
    else if((strcmp(argv[1],"config")==0)&&((argv[2][0]!='a')&&(argv[3][0]!='a'))){
        if(strcmp(argv[2],"-global")==0){
            assign_global(argv[3],argv[4]);
        }else if(strcmp(argv[2],"-global")!=0){
            assign_local(argv[2],argv[3]);
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