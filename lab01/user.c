#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct user{
    char* username;
    char* password;
    int status;
    int bloked;
    struct user *next;
    int signin;
};
typedef struct user USER;
USER* Readfile(char fileName[])
   { FILE *fp;
    char line[128];
    USER *current,*head;
    head=current=NULL;
    fp = fopen(fileName, "r");

    while(fgets(line, sizeof(line), fp)){
        USER *node = malloc(sizeof(USER));
        char *string1=strtok(line," ");
        node->username = strdup(string1);//note : strdup is not standard function
        string1 = strtok(NULL, " ");
        node->password=strdup(string1);
        string1 = strtok(NULL, " ");
        node->status=atoi(string1);
        node->bloked=0;
        node->signin=0;
        node->next =NULL;

        if(head == NULL){
            current = head = node;
        } else {
            current = current->next = node;
        }
    }
    fclose(fp);
    //test print
    for(current = head; current ; current=current->next){
        printf("%s %s %d\n", current->username,current->password,current->status);
    }
    //need free for each node
    return head;
}
int testuser(USER *head,char username[])
{
    USER *p;
    for(p=head;p;p=p->next)
    {   
        if(strcmp(p->username,username)==0)
        {
         return 0;
        }
    }
    return 1;
}
int checkblocked(USER *head,char username[])
{
    USER *p=head;
    USER *t = malloc(sizeof(USER));
    int test=testuser(head,username);
            if(test==1){
                printf("Cannot find account\n");
                return 0;
            }
    while(p!=NULL&&(strcmp(p->username,username)!=0)){
        p=p->next;
    }
    if(p->status==0) {
        printf("Account is blocked\n");
        return 0;
    }
    return 1;
}
void Register(USER *head,char username[],char password[] ,char fileName[])
{
    USER *p;
    USER *t = malloc(sizeof(USER));
    int i=0;
    for(p=head;p->next!=NULL;p=p->next)
    { i++;}
    t->username=strdup(username);
    t->password=strdup(password);
    t->status=2;
    t->bloked=0;
    t->signin=1;
    p->next=t;
    t->next=NULL;
    printf("Successful registration.Activation required.\n");
    FILE * fp;
    fp = fopen (fileName,"a");
    fprintf(fp,"%s %s %d\n", t->username,t->password,t->status);
    fclose (fp);
}
int Activate(USER *head,char username[],char password[] ,int mssv)
{
    USER *p=head;
    USER *t = malloc(sizeof(USER));
    int i=0;
    while(p!=NULL&&(strcmp(p->username,username)!=0)){
        p=p->next;
    }
    if(mssv!=20184222){
        p->bloked++;
        if(p->bloked>=4){
            p->status=0;
            printf("Activation code is incorrect.\nAccount is blocked.\n");
            return 0;
        }
        else{
            printf("Activation code is incorrect.\n");
            return 0;
        }
    }
    else{
        printf("Account is activated.\n");
        p->status=1;
    }
    return 0;
}
int Login(USER *head,char username[],char password[])
{
    USER *p=head,*current;
    USER *t = malloc(sizeof(USER));
    while(p!=NULL&&(strcmp(p->username,username)!=0)){
        p=p->next;
    }
    if(strcmp(p->password,password)!=0){
        p->bloked++;
        if(p->bloked>=3){
            p->status=0;
            printf("Password is incorrect.\nAccount is blocked.\n");
            FILE * fp;
            fp = fopen ("user.txt","w");
            for(current=head;current!=NULL;current=current->next){
            fprintf(fp,"%s %s %d\n", current->username,current->password,current->status);}
            fclose (fp);
            return 0;
        }
        else{
            printf("Password is incorrect.\n");
        }
    }
    else{
        p->signin=1;
        printf("Hello %s.\n",p->username);
    }
    return 0;
}
int Search(USER *head,char username[])
{
    USER *p=head;
    USER *t = malloc(sizeof(USER));
    while(p!=NULL&&(strcmp(p->username,username)!=0)){
        p=p->next;
    }
    
    if(p->status==0)
    {
        printf("Account is blocked\n");
    }
    else if(p->signin==0){
        printf("User is not logged in\n");
    }
    else if(p->status==1){
        printf("Account is active\n");
    }
    return 0;
}
int ChangePassword(USER *head,char username[],char password[],char newpassword[])
{
    USER *p=head,*current;
    USER *t = malloc(sizeof(USER));
    while(p!=NULL&&(strcmp(p->username,username)!=0)){
        p=p->next;
    }
    if(p->signin==0){
        printf("User is not logged in\n");
        return 0;
    }
    if(strcmp(p->password,password)!=0){
            printf("Current Password is incorrect.\nPlease try again.\n");
            return 0;
        }
        else{
            p->password=strdup(newpassword);
            printf("Password is changed.\n");
            FILE * fp;
            fp = fopen ("user.txt","w");
            for(current=head;current!=NULL;current=current->next){
            fprintf(fp,"%s %s %d\n", current->username,current->password,current->status);}
            fclose (fp);
        }
        return 0;
    }
int Logout(USER *head,char username[])
{
    USER *p=head;
    USER *t = malloc(sizeof(USER));
    while(p!=NULL&&(strcmp(p->username,username)!=0)){
        p=p->next;
    }
    if(p->signin==0){
        printf("User is not logged in\n");
        return 0;
    }
        else{
            printf("Goodbye %s\n",username);
        }
    return 0;
}
int main(void)
{
    int n,test, chon = 0 , x, kq,mssv;
    USER *head;
    head=Readfile("user.txt");
    char username[20],password[20],code[20],newpassword[20];
    do
    {
        printf("\t\t\t ============MENU=========\n");
        printf("\t\t\t |1.Register              |\n");
        printf("\t\t\t |2.Activate              |\n");
        printf("\t\t\t |3.Signin                |\n");
        printf("\t\t\t |4.Search                |\n");
        printf("\t\t\t |5.Change password       |\n");
        printf("\t\t\t |6.Sign out              |\n");
        printf("\t\t\t |7.Quit                  |\n");
        printf("\t\t\t =========================\n");
        printf("You Choice: ");
        scanf("%d", &chon);
        switch(chon)
        {
            case 1:
            printf("Nhap Name: ");
            fgets(username, sizeof(username), stdin);  // read string
            gets(username); 
            test=testuser(head,username);
            if(test==0){
                printf("Account existed");
            }
            else{
                printf("Nhap Password: ");
                gets(password);
                Register(head,username,password,"user.txt");              
            } 
            break;
            case 2:
            printf("Nhap Name: ");
            fgets(username, sizeof(username), stdin);  // read string
            gets(username); 
            test=testuser(head,username);
            if(test==1){
                printf("Account existed");
            }
            else{
                printf("Nhap Password: ");
                fgets(password, sizeof(password), stdin);  // read string
                printf("Nhap Activation Code: ");
                fgets(code, sizeof(code), stdin);
                mssv=atoi(code);
                Activate(head,username,password,mssv);
            } 
                break;
            case 3:
             printf("Nhap Name: ");
            fgets(username, sizeof(username), stdin);  // read string
            gets(username);
            test=checkblocked(head,username);
            if(test!=0){
            printf("Nhap Password: ");
            gets(password);
            Login(head,username,password);}
                break;
            case 4:
            printf("Nhap Name: ");
            fgets(username, sizeof(username), stdin);  // read string
            gets(username);
            test=testuser(head,username);
            if(test==1){
            printf("Cannot find account");
            }
            else{
           Search(head,username);}
                break;
            case 5:
            printf("Nhap Name: ");
            fgets(username, sizeof(username), stdin);  // read string
            gets(username);
            test=testuser(head,username);
            if(test==1){
            printf("Cannot find account");
            return 0;
            }
            else{
            printf("Nhap Password: ");
            gets(password);
            printf("Nhap NewPassword: ");
            gets(newpassword);
           ChangePassword(head,username,password,newpassword);}
                break;
            case 6:
            printf("Nhap Name: ");
            fgets(username, sizeof(username), stdin);  // read string
            gets(username); 
            test=testuser(head,username);
            if(test==1){
                printf("Cannot find account");
            }
            else{
                Logout(head,username);
            } 
                break;
            case 7:
                 printf("Ban da thoat chuong trinh");
                 break;
            default:
                break;
        }
    }while(chon!=7);
    return 0;
}