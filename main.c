#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct taikhoan {
    char username[49];
    char password[15];
    int status;     //0,1,2
    struct taikhoan* next;
}*start, *curent;

void add(char* username,char* password, int status){
    struct taikhoan* tmp = (struct taikhoan*) malloc(sizeof(struct taikhoan));
    strcpy(tmp->username, username);
    strcpy(tmp->password, password);
    tmp->status = status;
    tmp->next=NULL;
    if( start == NULL){
        start = tmp;
        curent = tmp;
    }else{
        curent->next = tmp;
        curent = tmp;
    }
}

void print(){
    struct taikhoan* tmp = start;
    while(tmp != NULL){
        printf("%s %s %d\n",tmp->username, tmp->password, tmp->status);
        tmp = tmp->next;
    }
}

 struct taikhoan* findName(char* name){
    struct taikhoan* tmp = start;
    
    while (tmp != NULL)
    {
        if(!strcmp(name, tmp->username))
            break;
        tmp = tmp->next;
    }
    
    return tmp;
}

void UpdateNameFile(char* fileName, char* name, int status){
    FILE* f = fopen(fileName, "r+");
    char user[49];
    char pass[15];
    int sta, ktra;
    if( f == NULL){
        printf("Error!!!");
    }else{
        while( !feof(f) ){
            ktra = fscanf(f, "%s", &user);
            if(ktra == -1)
                break;
            fscanf(f, "%s", &pass);
            if( !strcmp(user,name) ){
                fgetc(f);
                fseek(f, 0, SEEK_CUR);
                fprintf(f,"%d", status);
                break;
            }
            fscanf(f, "%d", &sta);
            add(user, pass, sta);
        }
    }
    fclose(f);
}

void freeAll(){
    struct taikhoan* tmp;
    while(start != NULL){
        tmp = start;
        start = start->next;
        tmp->next = NULL;
        free(tmp);
    }
}

void inputFile(char* fileName){
    FILE* f = fopen(fileName, "r");
    char user[49];
    char pass[15];
    int sta, ktra;
    if( f == NULL){
        printf("Error!!!");
    }else{
        while( !feof(f) ){
            ktra = fscanf(f, "%s", &user);
            if(ktra == -1 || !strcmp(user,"."))
                break;
            fscanf(f, "%s", &pass);
            fscanf(f, "%d", &sta);
            add(user, pass, sta);
        }
    }
    fclose(f);
}

void storeFile(char* fileName, char* name, char* pass, int sta){
    FILE* f = fopen(fileName, "r+");
    fseek(f, -1, SEEK_END);
    fprintf(f,"\n%s %s %d.", name, pass, sta);
    fclose(f);
}

int main(){
    char fileName[] = "NguoiDung.txt";
    char codeAcitve[] = "20194557";

    int chucNang, ktraI;
    ktraI = 0;
    char usName[49], pass[15], pass1[15], code[15], ktra[49];
    struct taikhoan* tmp;

    inputFile(fileName);

    do{
        printf("--------------Menu-----------------");
        printf("\n1.Register\n2.Activate\n3.Sign in\n4.Search\n5.Change password\n6.Sign out\n");
        printf("-----------------------------------\n");

        printf("Chuc nang: ");
        scanf("%d",&chucNang);
        switch (chucNang)
        {
        case 1:
            fflush(stdin);
            printf("USERNAME: ");
            scanf("%s",&usName);
            if( findName(usName))
                printf("Account existed \n");
            else{
                printf("PASSWORD: ");
                scanf("%s",&pass);
                add(usName,pass,2);
                storeFile(fileName, usName, pass, 2);
                printf("Successful registration. Activation required.\n");
            }
            break;
        
        case 2: 
            fflush(stdin);
            printf("USERNAME: ");
            scanf("%s",&usName);
            tmp = findName(usName);
            if( tmp == NULL ){
                printf("Tai khoan chua duoc dang ky!!!\n");
            }
            else{
                printf("PASSWORD: ");
                scanf("%s",&pass);
                if( !strcmp(tmp->password, pass) )
                {
                    for(int i = 0; i <= 4; i++)
                    {
                        printf("CODE: ");
                        scanf("%s",&code);
                        if(strcmp(code, codeAcitve)==0){
                            tmp->status = 1;
                            printf("Account is activated\n");
                            break;
                        }else if( i == 4 ){
                            printf("Activation code is incorrect.Account is blocked\n");
                            tmp->status = 0;
                        }else{
                            printf("Account is not activated\n",i+1);
                        }
                    }
                }
                else{
                    printf("Password is incorrect\n");
                }
            }
            break;

        case 3:
            fflush(stdin);
            printf("USERNAME: ");
            scanf("%s",&usName);
            tmp = findName(usName);
            if( tmp != NULL){
                if( tmp->status == 1 ){
                    for(int i = 0; i <= 3; i++){
                        fflush(stdin);
                        printf("PASSWORD: ");
                        scanf("%s",&pass);
                        if(!strcmp(pass, tmp->password)){
                            ktraI = 1;
                            strcpy(ktra, usName);
                            printf("Hello %s\n",usName);
                            break;
                        }else if(i== 3){
                            printf("Password is incorrect. Account is blocked\n");
                            tmp->status = 0;
                            UpdateNameFile(fileName, usName, 0);
                        }else{
                            printf("Password is incorrect\n", i+1);
                        }
                    }  
                }else{
                    printf("Account is ");
                    if(tmp->status == 0)
                        printf("Blocked\n");
                    else
                        printf("idle\n");
                }
            }else
                printf("Cannot find account\n");
            break;
        case 4:
            fflush(stdin);
            if(ktraI == 1){
                printf("USERNAME: ");
                scanf("%s",&usName);
                tmp = findName(usName);
                if(tmp == NULL){
                    printf("Cannot find account\n");
                }else{
                    printf("%s is ", "Account");
                    if(tmp->status == 1){
                        printf("active\n");
                    }else if(tmp->status == 0){
                        printf("blocked\n");
                    }
                }
            }else
                printf("Account is not sign in");
            break;

        case 5:
            fflush(stdin);
            printf("USERNAME: ");
            scanf("%s",&usName);
            tmp = findName(usName);
            if (tmp != NULL && !strcmp(ktra,usName)){
                printf("Password: ");
                scanf("%s", &pass);
                printf("NewPassword: ");
                scanf("%s", &pass1);
                if(!strcmp(tmp->password, pass)){
                    strcpy(tmp->password, pass1);
                    printf("Password is changed\n");
                }else{
                    printf("Current password is incorrect. Please try again\n");
                }
            }else if(strcmp(ktra,usName)){
                printf("Account is not sign in\n");
            }else{
                printf("Cannot find account\n");
            }
            break;
        
        case 6:
            fflush(stdin);
            printf("USERNAME: ");
            scanf("%s",&usName);
            tmp = findName(usName);
            if(tmp != NULL){
                if(!strcmp(usName, ktra)){
                    printf("Goodbye %s\n", tmp->username);
                }else{
                    printf("Account is not sign in \n");
                }
            }else
                printf("Cannot find account\n");
            break;
        default:
            break;
        }
    }while (chucNang>=1 && chucNang<=6);

    freeAll();
}