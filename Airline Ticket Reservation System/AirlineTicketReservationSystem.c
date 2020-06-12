/*this application is a flight ticket booking application, which creates two types of account namely:
ADMINISTRATIVE and PUBLIC. With PUBLIC account you can book ticket, see the available flight schedules , edit your details
and change password. And with ADMINSTRATIVE account you can schedule flights and remove the took off flights.*/
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include<dos.h>
#include<time.h>
#include<unistd.h>
#include<errno.h>
#include<sys/stat.h>
#include<ctype.h>

//Function to get password in secure mode
//Entered characters are displayed as '*'
void getpassword(char passwd[])
{
    int p=0;
    char character;
    do
    {
        character = getch();
        if(character=='\r')
        break;
        else if(character=='\b')
        {
            printf("\b");//Moves the cursor back
            p--;//Overwrites on the previous letter
        }
        else
        {
        printf("*");
        *(passwd+p)=character;
        p++;
        }
    }while(1);
    *(passwd+p) = '\0';

}
struct Card//Atm Card details
{
    char cardnumber[20];
	char name[50];
	char expiry_date[10];
	char cvv[5];//phone number is already given
}card;

//address
struct address
{
    char door_no[10];
    char street[20];
    char area[20];
    char district[20];
    char zipcode[10];
    char state[20];
    char city[20];
}addr;

struct ticket_price
{

    int ticket;
    int price;
    char passengername[30];
    char from_dest[20];
    char to_dest[20];
    char ret_dest[20];
    char date[10],ret_date[10];

}book[10];

struct flightdetails//details of flight
{
    char fromdest[5];
    char todest[5];
    char flightname[30];
    char departtime[10];
    char arrivaltime[10];
    char date[10];
}flight;
//details of the user
struct details
{
    char username[30];
    char name[20];
    char password[20];
    char nationality[10];
    char marital_status;
    char gender;
    char phone_number[15];
    char email_id[30];
    char dob[10];
    char account[20];
}user;

//tempname is used for storing the
//directory for any files
char tempname[] = "flights";////

//Creates a directory
//in which schedule file is kept
int createDir()
{
    int check = mkdir(tempname);
    if(check <0)
    {
        if(errno == EEXIST)
        {
            return 1;
        }
    }
    return 0;
}

//returns the number of lines in a file
int lineCounter(FILE *file)
{
    int lineCount=0,checker=0;
    char character;
    while((fscanf(file,"%c",&character))!=EOF)
    {
        checker++;

        if(character=='\n')
        {
         lineCount++;
        }
    }
    if(checker==0)
    return 0;
    else
    return lineCount;
}

//arranges the schedule file in ascending factors
void reorderStruct(struct flightdetails flightarr[],int num)
{
    struct flightdetails temp;
    for(int i=0;i<num-1;i++)//insertion sorting
    {
        for(int j=i;j<num;j++)
        {
            if(strcmp(flightarr[i].date,flightarr[j].date)>0)
            {
                temp = flightarr[i];
                flightarr[i] = flightarr[j];
                flightarr[j] = temp;
            }
            else if(strcmp(flightarr[i].date,flightarr[j].date)==0)//if date is same
            {
                if(strcmp(flightarr[i].departtime,flightarr[j].departtime)>0)
                {
                    temp = flightarr[i];
                    flightarr[i] = flightarr[j];
                    flightarr[j] = temp;
                }
                else if(strcmp(flightarr[i].departtime,flightarr[j].departtime)==0)//if depart time is same
                {
                    if(strcmp(flightarr[i].arrivaltime,flightarr[j].arrivaltime)>0)
                    {
                        temp = flightarr[i];
                        flightarr[i] = flightarr[j];
                        flightarr[j] = temp;
                    }
                }
            }
        }
    }
}

//creates a struct of flightdetails to sort it
int getFlightStructs()
{
    FILE *fp;
    fp = fopen("flights\\schedule.txt","r");////
    int num = lineCounter(fp);
    if(num==0) return 0;//if the file is empty

    struct flightdetails newflight[num];

    fseek(fp,0,SEEK_SET);
    for(int ind=0;ind<num;ind++)
    {
        fscanf(fp,"%s %s %s %s %s %s\n",newflight[ind].date,newflight[ind].fromdest,newflight[ind].todest,newflight[ind].flightname,newflight[ind].departtime,newflight[ind].arrivaltime);
    }
    fclose(fp);

    reorderStruct(newflight,num);
    //rewriting the file

    fp = fopen("flights\\schedule.txt","w");////
    for(int i=0;i<num;i++)
    {
        fprintf(fp,"%s  %s  %s  %s  %s  %s\n",newflight[i].date,newflight[i].fromdest,newflight[i].todest,newflight[i].flightname,newflight[i].departtime,newflight[i].arrivaltime);
    }
    fclose(fp);
    return 1;
}

//removes the first flight schedule
void popo()
{
    FILE *fp=fopen("flights\\schedule.txt","r");////
    int num = lineCounter(fp);
    printf("%d\n",num);
    if(num==0)
    {
        printf("\nNothing to delete");
        return;
    }
    struct flightdetails newflight[num];
    fclose(fp);
    fp = fopen("flights\\schedule.txt","r");////
    for(int ind=0;ind<num;ind++)
    {
        fscanf(fp,"%s %s %s %s %s %s\n",newflight[ind].date,newflight[ind].fromdest,newflight[ind].todest,newflight[ind].flightname,newflight[ind].departtime,newflight[ind].arrivaltime);
    }
    fclose(fp);
    printf("\n");
    fp = fopen("flights\\schedule.txt","w");////
    for(int i=1;i<num;i++)
    {
        fprintf(fp,"%s  %s  %s  %s  %s  %s\n",newflight[i].date,newflight[i].fromdest,newflight[i].todest,newflight[i].flightname,newflight[i].departtime,newflight[i].arrivaltime);
        printf("%s  %s  %s  %s  %s  %s\n",newflight[i].date,newflight[i].fromdest,newflight[i].todest,newflight[i].flightname,newflight[i].departtime,newflight[i].arrivaltime);
    }
    fclose(fp);
}

//Method to get the data about the user
//Sign in
//Returns one on completion
int createStruct()//Updater is used to find whether we are updating the details or creating
{
    char na[30];
    getchar();
    reenter:
    system("cls");


        printf("--------------------        SIGN IN         ------------------------");

        //getting the username
        printf("\nEnter the username:");
        gets(user.username);

        //checking whether it already exists
        char name[30];
        strcpy(name,user.username);
           strcat(name,".txt");
           strcpy(na,tempname);
           strcat(na,"\\");
           strcat(na,name);
           FILE* fp;
           fp = fopen(na,"r");
        if(fp!=NULL)//username already exists
        {
            printf("\nusername already exists\npress any button to continue");
            getch();
            return 0;
        }
        fclose(fp);

    char confirmation[10];

        printf("\nEnter the password:");
        getpassword(user.password);
        printf("\nConfirm the password:");
        getpassword(confirmation);
        if(strcmp(user.password,confirmation)!=0) //If confirmation and password is not same it is repeated
        {
            printf("\nPasswords did not match");
            printf("\nEnter any button");
            getch();
            goto reenter;
        }



        system("cls");
        printf("\n----------------      PERSONAL DETAILS        -------------------------\n");

         printf("\nEnter the Full Name:");
        gets(user.name);

         printf("\n'M' for Male and 'F' for female and 'O' for others\ngender:");
        user.gender = getchar();
        getchar();

         printf("\nEnter the date of birth in dd/mm/yyyy format:");
         gets(user.dob);

        printf("\nFor marital status:\n'M' for married\n'U' for Unmarried\nInput:");
        user.marital_status = getchar();
        getchar();
        printf("\nEnter Nationlity:");
        gets(user.nationality);

        printf("\nEnter the Phone number:");
        gets(user.phone_number);

        printf("\nEnter the email id:");
        gets(user.email_id);
        printf("\nEnter the type of account in caps(ADMINISTRATIVE or PUBLIC)\n(ADMINISTRATIVE accounts have access to both):");
        gets(user.account);



        system("cls");
        printf("\n------------        ADDRESS         -------------------\n");
        //getting the address
        printf("Enter the Flat no\\Door no:");
        gets(addr.door_no);
        printf("Enter the Street:");
        gets(addr.street);
        printf("Enter the Area:");
        gets(addr.area);
        printf("Enter the City:");
        gets(addr.city);
        printf("Enter the District:");
        gets(addr.district);
        printf("Enter the State:");
        gets(addr.state);
        printf("Enter the Zipcode:");
        gets(addr.zipcode);
        printf("\n--------------------------------------------------------\n");
        return 1;
}

void  flight_name(char temp[30])
{
    printf("   Flight : %s",temp);
}

//loads the details of user in the struct
int getStruct(FILE* file)
{
    fscanf(file,"%s\n",user.name);//newline specifies the next item
    fscanf(file,"%s\n",user.password);
    fscanf(file,"%s\n",user.account);
    fscanf(file,"%s\n",user.nationality);
    fscanf(file,"%c\n%c\n",&user.marital_status,&user.gender);
    fscanf(file,"%s\n%s\n",user.phone_number,user.email_id);
    //date of birth
    fscanf(file,"%s\n",user.dob);
    //address
    fscanf(file,"%s\n%s\n%s\n",addr.door_no,addr.street,addr.area);
    fscanf(file,"%s\n%s\n%s\n%s",addr.city,addr.district,addr.state,addr.zipcode);
    return 1;
}

//Edit the details of user
int editDetails(void)
{
    int choice;
    /*char name[30];
    strcpy(name,user.username);
    char filename[40]="c:\\flights\\";
    strcat(filename,user.username);
    strcat(filename,".txt");
    FILE *fp;
    fp = fopen(filename,"r");
    getStruct(fp);
    printStruct();
    getch();
    fclose(fp);*/

    reenterChoice:
    printf("\nWhich Details do you want to change:");
    printf("\n1.Personal details     2.Address details    3.Back(If you don't want to change)\n");
    scanf("%d",&choice);
    if(choice==1)
    {
        system("cls");
        printf("\n----------------      PERSONAL DETAILS        -------------------------\n");

         getchar();
         printf("\nEnter the Full Name:");
        gets(user.name);

         printf("\n'M' for Male and 'F' for female and 'O' for others\ngender:");
        user.gender = getchar();
        getchar();
         printf("\nEnter the date of birth in dd/mm/yyyy format:");
         gets(user.dob);

        printf("\nFor marital status:\n'M' for married\n'U' for Unmarried\nInput:");
        user.marital_status = getchar();
        getchar();
        printf("\nEnter Nationlity:");
        gets(user.nationality);

        printf("\nEnter the Phone number:");
        gets(user.phone_number);

        printf("\nEnter the email id:");
        gets(user.email_id);

        return 1;
    }
    else if(choice==2)
    {
         system("cls");
         char trash[3];
        printf("\n------------        ADDRESS         -------------------\n");
        //getting the address
        gets(trash);
        printf("Enter the Flat no\\Door no:");
        scanf("%[^\n]%*c",addr.door_no);
        printf("Enter the Street:");
        gets(addr.street);
        printf("Enter the Area:");
        gets(addr.area);
        printf("Enter the City:");
        gets(addr.city);
        printf("Enter the District:");
        gets(addr.district);
        printf("Enter the State:");
        gets(addr.state);
        printf("Enter the Zipcode:");
        gets(addr.zipcode);
        return 1;
    }
    else if(choice==3)
    {
        return 1;
    }
    else
    {
        printf("\n\nInvalid choice");
        getch();
        goto reenterChoice;
    }
}

//Log in
//If the account  already exists
int  logIn(char acc[])
{
    char name[30],na[40];
    char user_name[30];
    int code,code1;
    reenter:
    system("cls");
    printf("\n--------------      LOG IN      -----------------------\n");
    getchar();
    printf("\nEnter the Username:");
    //int character = getchar();
    scanf("%[^\n]%*c",&name);
    strcpy(user_name,name);
    strcat(name,".txt");//appending ".txt" to the user name
    strcpy(na,tempname);
    strcat(na,"\\");
    strcat(na,name);
    FILE* fp = fopen(na,"r");

    if(fp == NULL && errno==2)//checks for the user name is correct
    {
        printf("\nThe username you entered doesn't exist.\n");
        fclose(fp);
        printf("Press any key to continue.");
        getch();
        return 1;
    }
    strcpy(user.username,user_name);//as it isn't updated in the structure
    //getting the name and password
    /*fscanf(fp,"%s\n",user.name);
    fscanf(fp,"%s\n",user.password);
    fscanf(fp,"%s\n",user.account);*/
    //using getstruct to get the values
    getStruct(fp);
    fclose(fp);
    char passwd[20];
    if((strcmp(user.account,acc)!=0)&&(strcmp(user.account,"ADMINISTRATIVE")!=0))
    {
        printf("\nYou are not permitted in this type of account\n");
        fclose(fp);
        printf("Press any key to continue.\n");
        getch();
        return 1;
    }
    printf("\nEnter the password:");
    getpassword(passwd);
    if(strcmp(user.password,passwd)!=0)
    {
        printf("\nInvalid password\n");
        printf("Please try again");
        getch();
        goto reenter;//if the entered password is incorrect
    }
    srand(time(0));
    code=rand();
    printf("\nEnter the captcha code:\n%d\n",code);
    scanf("%d",&code1);
    if(code!=code1)
    {
        printf("\nInvalid captcha code");
        sleep(1);
        goto reenter;
    }
    else
    {
        printf("\nYou have successfully logged in.");
        printf("\nPlease wait");
        sleep(1);
        printf(".");
        sleep(1);
        printf(".");
        sleep(1);
        printf(".\n");
        sleep(1);
        return 0;
    }
}


//Printing the profile of the user for reference
void printStruct(void)
{
    system("cls");
    printf("\nName : %s",user.name);
    printf("\nUsername : %s",user.username);
    printf("\nPassword : %s",user.password);
    printf("\nAccount : %s",user.account);
    printf("\nNationlity : %s",user.nationality);
    if(user.marital_status=='M')
        printf("\nMarital status : Married");
    else
        printf("\nMarital status : Unmarried");
    if(user.gender=='M')
        printf("\nGender : Male");
    else if(user.gender=='F')
        printf("\nGender : Female");
    else
        printf("\nGender : Others");

    printf("\nPhone number : %s",user.phone_number);
    printf("\nEmail id: %s",user.email_id);
    printf("\nDate of birth : %s",user.dob);

    //Address
    printf("\nFlat number\\Door number: %s",addr.door_no);
    printf("\nStreet: %s\nArea : %s\nDistrict : %s\nCity : %s\nState : %s",addr.street,addr.area,addr.district,addr.city,addr.state);
    printf("\nZip code : %s",addr.zipcode);
    printf("\nPress any key to continue:");
    getch();
}

int trip_choice,dest_choice,i,num_tickets,ticket_no,tot_amt;
char ch,frm_des[3],to_des[3],re_des[20],d_a_t_e[10],ret_d_a_t_e[10],depart[10],dept[10],arrv[10],flight_choice[30];
int price,price2;
void onewaytrip()
{
    printf("\nPrice per ticket is Rs.3000\n");
    ticket_no=1;
    printf("Enter the passenger's name:\n");
    for(i=0;i<num_tickets;i++)
    {
    scanf("%s",book[i].passengername);
    book[i].ticket=ticket_no;
    ticket_no++;
    book[i].price=price;
    strcpy(book[i].from_dest,frm_des);
    strcpy(book[i].to_dest,to_des);
    strcpy(book[i].date,d_a_t_e);
    }
    tot_amt=price*num_tickets;
}
void roundtrip()
{
    printf("\nPrice per ticket is Rs.7500\n");
    ticket_no=1;
    printf("\nEnter return date(dd/mm/yy)\n");
    scanf("%s",ret_d_a_t_e);
    printf("Enter the passenger's name:\n");
    for(i=0;i<num_tickets;i++)
    {
     scanf("%s",book[i].passengername);
     book[i].ticket=ticket_no;
     ticket_no++;
     book[i].price=price2;
     strcpy(book[i].from_dest,frm_des);
     strcpy(book[i].to_dest,to_des);
     strcpy(book[i].ret_dest,re_des);
     strcpy(book[i].date,d_a_t_e);
     strcpy(book[i].ret_date,ret_d_a_t_e);
    }
    tot_amt=price2*num_tickets;

}

//Creating the file for the user
//User name is used as the file name
void createFile(FILE* file)
{
    fprintf(file,"%s\n",user.name);//newline specifies the next item
    fprintf(file,"%s\n",user.password);
    fprintf(file,"%s\n",user.account);
    fprintf(file,"%s\n",user.nationality);
    fprintf(file,"%c\n%c\n",user.marital_status,user.gender);
    fprintf(file,"%s\n%s\n",user.phone_number,user.email_id);
    //date of birth
    fprintf(file,"%s\n",user.dob);
    //address
    fprintf(file,"%s\n%s\n%s\n",addr.door_no,addr.street,addr.area);
    fprintf(file,"%s\n%s\n%s\n%s\n",addr.city,addr.district,addr.state,addr.zipcode);
    printf("\nProfile is created");

    return;
}



//Details Of ATM card
int getCardDetails(char* card_type)
{
     system("cls");
     int code,code1;
     printf("Enter your %s card number:",card_type);
     scanf("%s",card.cardnumber);
     printf("Enter the card holder's name:");
     scanf("%s",card.name);
     printf("Enter the expiry date of the card(m/yy):");
     scanf("%s",card.expiry_date);
     printf("Enter the CVV number of the card:");
     getpassword(card.cvv);
     srand(time(0));
     code=rand();
     printf("\nEnter the Captcha Code:%d\n",code);
     scanf("%d",&code1);
     if(code==code1)
     {
        return 1;
     }
     else
     {
        return 0;
     }
}

int changePassword()
{
    char password[20],confirmation[20],na[30];
    char name[30];
    printf("\nEnter the old password:");
    getpassword(password);
    if(strcmp(password,user.password)!=0)
    {
        printf("\nInvalid password");
        getch();
        return 0;
    }
    reenterPassword:
    getchar();
    printf("\nEnter the new password:");
    getpassword(password);
    printf("\nReenter the new password:");
    getpassword(confirmation);
    if(strcmp(password,confirmation)==0)
    {
        strcpy(name,user.username);
        strcat(name,".txt");
        strcpy(user.password,password);
        strcpy(na,tempname);
        strcat(na,"\\");
        strcat(na,name);
        FILE* fp = fopen(na,"w");
        createFile(fp);
        printf("\nPlease wait for a while");
        sleep(1);
        printf(".");
        sleep(1);
        printf(".");
        sleep(1);
        printf(".");
        sleep(1);
        printStruct();//For reference
        fclose(fp);
        return 1;
    }
    else
    {
        printf("\nThe two password are not same");
        getch();
        goto reenterPassword;
    }

}


void createschedule(FILE *tra)
{
    fprintf(tra,"%s  ",flight.date);//newline specifies the next item
    fprintf(tra,"%s  ",flight.fromdest);
    fprintf(tra,"%s  ",flight.todest);
    fprintf(tra,"%s  ",flight.flightname);
    fprintf(tra,"%s  ",flight.departtime);
    fprintf(tra,"%s\n",flight.arrivaltime);
    return;
}




void main()
{
   int choice;
   char name[30],na[20];
   start://If the user has to reenter the details
   system("cls");
   printf("                                               WELCOME TO THE DOMESTIC AIRLINES\n\n");
   printf("1.New User                                     2.Existing User                                 3.Exit\n");
   printf("Enter the operation to be executed:");
   scanf("%d",&choice);


   if(choice==1)//creating a new profile
   {
       printf("%d",createDir());
       if(createStruct())
       {
           printf("Please wait");
           sleep(1);
           printf(".");
           sleep(1);
           printf(".");
           sleep(1);
           printf(".\n");
           sleep(1);
           system("cls");
           printf("\nProfile Created\n");
       }
       else//if the structure is not created
       {
           printf("\nError in creating the file");
           sleep(1);
           goto start;
       }
       printStruct();
       char tempname[30] = "flights";////

       strcpy(name,user.username);
       strcat(name,".txt");
       strcpy(na,tempname);
       strcat(na,"\\");
       strcat(na,name);
       FILE* fp;
       fp = fopen(na,"a+");//File is created in the name of user name
       if(fp == NULL)
       {
           printf("\nError in creating the account\nTo try again now press 'y' else press 'n' to try again later");
           int choice = getchar();
           if(choice == 'y') goto start;
           exit(0);
       }
       createFile(fp);
       fclose(fp);
       getch();
       goto start;
   }

  else if(choice==2)//if already exists
  {
    int acc,operation;
    printf("\n--------------------------------------------\n");
    printf("1.PUBLIC ACCOUNT\n2.ADMINISTRATIVE ACCOUNT\n\nEnter the type(number) of account:");
    scanf("%d",&acc);
    if(acc!=1&&acc!=2)
    {
       printf("\nInvalid choice.\nPress any key to continue.");
       getch();
       goto start;
    }
    else if(acc==2)//for administrative type accounts
    {
       if(logIn("ADMINISTRATIVE"))//Logging in
       goto start;
       op:
       system("cls");
       printf("                          OPERATION\n");
       printf("1.Flight Schedule Upload            2.remove flight schedule          3.Log OUT\nEnter the operation from above to be performed:");
       scanf("%d",&operation);
       if(operation==3)//exit
       {
           sleep(1);
           goto start;
       }
       else if(operation==2)//remove flight schedule
       {
           system("cls");
           char trash[2];
           if(getFlightStructs()==0)
           {
               printf("\nThe schedules are empty");
               printf("\nPress any button to continue:");
               getch();
               goto op;
           }
           printf("\n\n");
           char popChoice;
           do
           {
               printf("-----------AVAILABLE FLIGHT SCHEDULES----------\n");
               popo();
               gets(trash);
               printf("\nIf you want to remove another flight schedule enter 'y' if not enter 'n':\n");
               scanf("%c",&popChoice);
           }while(popChoice=='y');
           goto op;
       }

       //upload schedule
       char date_of_travel[10],cha;
       printf("%d",createDir());
       FILE *trv=fopen("flights\\schedule.txt","a+");////

       printf("\n------------------------------------------\n");
       printf("\n\n        AVAILABLE AIRLINES      \n");
       printf("1.    SpiceJet\n");
       printf("2.    IndiGo\n");
       printf("3.    AirAsia\n");
       printf("4.    GoAir\n");
       printf("5.    AirIndia\n\n");
       printf("----------------------------------------------------------\n\n");
       printf("\t\t|\tINDIAN AIRLINE CODE\t|\n");
       printf("\t\t---------------------------------\n");
       printf("1.  Mumbai Chattrapathi Shivaji International Airport [ Code BOM ]\n");
       printf("2.  Bangalore Bengaluru International Airport [ Code BLR ]\n");
       printf("3.  New Delhi Indira Gandhi International Airport [ Code DEL ]\n");
       printf("4.  Hyderabad Rajiv Gandhi International Airport [ Code HYD ]\n");
       printf("5.  Chennai Meenambarkkam International Airport [ Code MAA ]\n");
       printf("6.  Kolkata Netaji Subhash Chandra Bose International Airport [ Code CCU ]\n");
       printf("7.  Ahmedabad Sardar Vallabhbhai Patel International Airport [ Code AMD ]\n");
       printf("8.  Dabolim Goa International Airport [ Code GOI ]\n");
       printf("9.  Pune Pune Airport [ Code PNQ ]\n");
       printf("10.  Guwahati Lokpriya Gopinath Bordoloi International Airport [ Code GAU ]\n");
       printf("11.  Lucknow Chaudhary Charan Singh Airport [ Code LKO ]\n");
       printf("12.  Jaipur Jaipur International Airport [ Code JAI ]\n");
       printf("13.  Bhubaneswar Biju Patnaik International Airport [ Code BBI ]\n");
       printf("14.  Patna Jaya Prakash Narayan Airport [ Code PAT ]\n\n");
       printf("Press any key to proceed");
       getch();
       system("cls");
       printf("\n------------------------------------------\n");
       printf("India Airport Codes (IATA / ICAO)\n");
       printf("Mumbai-BOM\nNew Delhi-DEL\nChennai-MAA\nKolkata-CCU\nBengaluru-BLR\nHyderabad-HYD");
       do
       {
         printf("\n------------------------------------------\n");
         printf("Enter the date of schedule for uploading:");
         scanf("%s",date_of_travel);
         printf("\nEnter the starting point(code):");
         scanf("%s",flight.fromdest);
         printf("\nEnter the destination point(code):");
         scanf("%s",flight.todest);
         printf("\nEnter the name of the flight:");
         scanf("%s",flight.flightname);
         printf("\nEnter the departure time:");
         scanf("%s",flight.departtime);
         printf("\nEnter the arrival time:");
         scanf("%s",flight.arrivaltime);
         strcpy(flight.date,date_of_travel);
         createschedule(trv);
         getchar();
         printf("\nDo you want to continue(y/n):");
         scanf("%c",&cha);
       }while(cha=='y');
       fclose(trv);
       sleep(1);
       goto op;
    }


    else if(acc==1)//if public type account
    {
       char character[20];
       if(logIn("PUBLIC"))//Logging in
        {goto start;}

        char filename[40];
        strcpy(filename,tempname);
        strcat(filename,"\\");
        strcat(filename,user.username);
        strcat(filename,".txt");//creating the filepath in case of edit

       pub:
       system("cls");
       printf("1.TICKET BOOKING\n2.SCHEDULE CHECKING\n3.CHANGE PASSWORD\n4.EDIT DETAILS\n5.LOG OUT\n\nEnter the operation from above to be performed:");
       scanf("%d",&operation);
       if(operation==5)
       {
           sleep(1);
           goto start;
       }
       else if(operation==2)
       {
         printf("\n------------------------------------------\n");
         printf("\n\n        AVAILABLE AIRLINES      \n");
         printf("1.    SpiceJet\n");
         printf("2.    IndiGo\n");
         printf("3.    AirAsia\n");
         printf("4.    GoAir\n");
         printf("5.    AirIndia\n\n");
         printf("----------------------------------------------------------\n\n");
         printf("\t\t|\tINDIAN AIRLINE CODE\t|\n");
         printf("\t\t---------------------------------\n");
         printf("1.  Mumbai Chattrapathi Shivaji International Airport [ Code BOM ]\n");
         printf("2.  Bangalore Bengaluru International Airport [ Code BLR ]\n");
         printf("3.  New Delhi Indira Gandhi International Airport [ Code DEL ]\n");
         printf("4.  Hyderabad Rajiv Gandhi International Airport [ Code HYD ]\n");
         printf("5.  Chennai Meenambarkkam International Airport [ Code MAA ]\n");
         printf("6.  Kolkata Netaji Subhash Chandra Bose International Airport [ Code CCU ]\n");
         printf("7.  Ahmedabad Sardar Vallabhbhai Patel International Airport [ Code AMD ]\n");
         printf("8.  Dabolim Goa International Airport [ Code GOI ]\n");
         printf("9.  Pune Pune Airport [ Code PNQ ]\n");
         printf("10.  Guwahati Lokpriya Gopinath Bordoloi International Airport [ Code GAU ]\n");
         printf("11.  Lucknow Chaudhary Charan Singh Airport [ Code LKO ]\n");
         printf("12.  Jaipur Jaipur International Airport [ Code JAI ]\n");
         printf("13.  Bhubaneswar Biju Patnaik International Airport [ Code BBI ]\n");
         printf("14.  Patna Jaya Prakash Narayan Airport [ Code PAT ]\n\n");
         printf("%d",createDir());
         FILE *trv=fopen("flights\\schedule.txt","r+");////
         printf("\n\nFlight Details:\n\n");
         int scheduleChecker=0;//to check is there are any schedules
         printf("\nDATE   FROM   TO   FLIGHT  DEPART    ARRIVAL\n");
         while(fscanf(trv,"%[^\n]%*c",character)!=EOF)
         {
             scheduleChecker++;
             printf("%s\n",character);
         }
         if(scheduleChecker==0)
         {
             printf("No flight schedules is available");
         }
         fclose(trv);
         printf("\n\nPress any key to continue:");
         getch();
         goto pub;
       }
       else if(operation==3)
       {
           int check = changePassword();
           if(!check)
           {
               printf("\n\nError in the process");
               printf("\nEnter any button to continue");
           }
           else
           {
               FILE *fp=fopen(filename,"w");
               createFile(fp);
               fclose(fp);
           }
           goto pub;
       }
       else if(operation==4)
       {
           if(editDetails()==1)
           {
            printf("\nEdit complete");
            FILE *fp=fopen(filename,"w");
               createFile(fp);
               fclose(fp);
           }
           else
           {
            printf("\nError in editing");
           }
           printStruct();
       getch();
       goto pub;
       }

       //selection of flight from the schedules
       price=3500;
       price2=7000;
       trip:
       system("cls");
       printf("\n\n                                                  AVAILABLE TRIPS\n\n");
       printf(" ----------------------------------------------------------------------------------------------------------------------\n");
       printf(" |                            1. ONE WAY TRIP                             2. ROUND TRIP                               |\n");
       printf(" ----------------------------------------------------------------------------------------------------------------------\n\n");
       printf("Enter Your Trip Choice:");
       scanf("%d",&trip_choice);
       if(trip_choice!=1&&trip_choice!=2)
       {
         printf("\nWrong Travel Route Choice. \nPress any key to continue");
         getch();
         goto trip;
       }
       printf("\n\n        AVAILABLE AIRLINES      \n");
       printf("1.    SpiceJet\n");
       printf("2.    IndiGo\n");
       printf("3.    AirAsia\n");
       printf("4.    GoAir\n");
       printf("5.    AirIndia\n\n");
       printf("----------------------------------------------------------\n\n");
       printf("\n\n");
       printf("\t\t---------------------------------\n");
       printf("\t\t|\tINDIAN AIRLINE CODE\t|\n");
       printf("\t\t---------------------------------\n");
       printf("1.  Mumbai Chattrapathi Shivaji International Airport [ Code BOM ]\n");
       printf("2.  Bangalore Bengaluru International Airport [ Code BLR ]\n");
       printf("3.  New Delhi Indira Gandhi International Airport [ Code DEL ]\n");
       printf("4.  Hyderabad Rajiv Gandhi International Airport [ Code HYD ]\n");
       printf("5.  Chennai Meenambarkkam International Airport [ Code MAA ]\n");
       printf("6.  Kolkata Netaji Subhash Chandra Bose International Airport [ Code CCU ]\n");
       printf("7.  Ahmedabad Sardar Vallabhbhai Patel International Airport [ Code AMD ]\n");
       printf("8.  Dabolim Goa International Airport [ Code GOI ]\n");
       printf("9.  Pune Pune Airport [ Code PNQ ]\n");
       printf("10.  Guwahati Lokpriya Gopinath Bordoloi International Airport [ Code GAU ]\n");
       printf("11.  Lucknow Chaudhary Charan Singh Airport [ Code LKO ]\n");
       printf("12.  Jaipur Jaipur International Airport [ Code JAI ]\n");
       printf("13.  Bhubaneswar Biju Patnaik International Airport [ Code BBI ]\n");
       printf("14.  Patna Jaya Prakash Narayan Airport [ Code PAT ]\n\n");
       printf("\nFrom which place?(please enter the airport code) : ");
       scanf("%s",frm_des);

        for(int i=0;i<3;i++)
        {
            if(islower(frm_des[i]))
            frm_des[i]=frm_des[i]-32;
        }
        printf("\nTo which place?(please enter the airport code) : ");
        scanf("%s",to_des);
    for(int i=0;i<3;i++)
        {
            if(islower(to_des[i]))
            to_des[i]=to_des[i]-32;
        }
    printf("\n\n");
    if(strcmp(frm_des,to_des)==0)
    {
        printf("Starting point and Destination point cannot be same.\nPress any key to continue.");
        getch();
        goto trip;
    }
    printf("\nEnter date of travel(dd/mm/yy) : ");
    scanf("%s",d_a_t_e);

    printf("\n\nHow many tickets do you want to book?(<10 tickets) : ");
    scanf("%d",&num_tickets);
    printf("\n\n");
    char seat[30];
    int flag=0;
    FILE *trv=fopen("flights\\schedule.txt","r+");////
    while(fscanf(trv,"%s",seat)!=EOF)
    {
        if(strcmp(seat,d_a_t_e)==0)
        {
            fscanf(trv,"%s",seat);
            if(strcmpi(seat,frm_des)==0)
            {
                fscanf(trv,"%s",seat);
                if(strcmpi(seat,to_des)==0)
                {
                    flag=1;
                    fscanf(trv,"%s",seat);
                    strcpy(flight_choice,seat);
                    fscanf(trv,"%s",seat);
                    strcpy(dept,seat);
                    fscanf(trv,"%s",seat);
                    strcpy(arrv,seat);
                    break;
                }
            }
        }
    }
    fclose(trv);
    if(flag==0)
    {
        printf("\nThere are no available flights according to your service.\nPlease look at the schedule.");
        sleep(2);
        goto pub;

    }
    if(trip_choice==1)
    {
      onewaytrip();
    }
    else if(trip_choice==2)
    {
      roundtrip();
    }
    char cha;
    getchar();
    printf("\nDo you want to proceed for payment(y/n)");
    scanf("%c",&cha);
    if(cha=='y'||cha=='Y')
    {
       char card_type[20];
        double amount;
       int numberOfAdmits;
       system("cls");


       //Transaction begins
       printf("--------------------------Payment transaction---------------------------------\n");
       int cardChoice;
       int option;
       reenterCardMode://If the choice is wrong
       printf("Do you want to\n1.Proceed with transaction\n2.Cancel your payment:");
       scanf("%d",&option);
       if(option==1)
       {
          printf("Modes of Payment:\n");
          printf("1.Credit card                             2.Debit card\nEnter your choice:");
          scanf("%d",&cardChoice);
          //Defining card_type based on the choice
          if(cardChoice==1)
          {
            strcpy(card_type,"credit");
          }
          else if(cardChoice==2)
          {
            strcpy(card_type,"debit");
          }
          else
          {
            printf("Choice out of range.\nPlease enter a valid choice\n");
            sleep(1);
            goto reenterCardMode;
          }
          reenterCardDetails:
          if(getCardDetails(card_type))
          {
            /*Getting the number of seats
            and for flight it has to be less than or equal to 5*/
            reenterAdmits:
            printf("Your payment process is progressing\n");
            sleep(2);
            sleep(2);
            printf("\nPress any key to continue");
            getch();
            printf("\nYour transaction is completed\n");
            printf("Your E-ticket is processing....");
            sleep(2);
            system("cls");

            //generating a e-ticket model
            time_t original;
            time(&original);
            printf("\t\t      _\n");
            printf("\t\t    -=\\\'\\\n");
            printf("\t        |\\ ____\\_\\__\n");
            printf("\t      -=\\C\'\"\"\"\"\"\" \" \')\t\t\t\t\t\t\t\t\tPNR:%d\n",(rand()%100000)+999999);
            printf("\t         \'~~~~~/ /~~\'\n");
            printf("\t           -==/ /\n");
            printf("\t\t     \'-\'\n");
            printf("\t\t\t\t<><><><><><><><><><><><><><><><><><><><><><><><><><><><><>\n");
            printf("\t\t\t\t|\t\t\tFlight ticket\t\t\t |\n");
            printf("\t\t\t\t<><><><><><><><><><><><><><><><><><><><><><><><><><><><><>\n");
            printf("  \t\t\t------------------------------------------------------------------------\n");
            printf("  \t\t\t           Date of Booking             ||    %s",asctime(gmtime(&original)));
            printf("  \t\t\t------------------------------------------------------------------------\n");
            printf("  \t\t\t              Status                   ||       CONFIRMED\n");
            printf("  \t\t\t------------------------------------------------------------------------\n");
            printf("  \t\t\t         Payment Status                ||       Approved\n");
            printf("  \t\t\t------------------------------------------------------------------------\n");
            printf("\n\n\n");
            printf("  --------------------------------------------------------------------------------------------------------------------\n");
            printf("  |                                                  Passengers                                                      |\n");
            printf("  --------------------------------------------------------------------------------------------------------------------\n");
            for(int i=0;i<num_tickets;i++)
            {
             printf("\t%d.%s\t",i+1,book[i].passengername);//for printing passengers names
            }
            printf("\n  --------------------------------------------------------------------------------------------------------------------\n\n\n");
            printf("  --------------------------------------------------------------------------------------------------------------------\n");
            printf("  |      Date     |      From     |      Depart      |     Flight Number     |      Check-in|To      |    Arrives    |\n");
            printf("  --------------------------------------------------------------------------------------------------------------------\n");
            printf("  |   %s    |      %s      |      %s       |        %d         |         %s           |      %s    |\n",d_a_t_e,frm_des,dept,rand(),to_des,arrv);//printing the information received from the user
            printf("  --------------------------------------------------------------------------------------------------------------------\n\n\n");
            printf("\n  Price : %d\n\n",tot_amt);
            flight_name(flight_choice);
            printf("\nTRAVEL AND BAGGAGE INFORMATION:\n\n");
            printf("%s->%s\n",frm_des,to_des);
            printf("Fare type : Regular Fare\n");
            printf("Please do reach airport 2 hours before flight departure\n");
            printf("Boarding gates close 25 minutes prior to scheduled departure time\n");
            printf("Check in counter close 45 minutes prior to the scheduled departure time\n");
            printf("Check in baggage allowance : 15kg\n");
            printf("Hand Baggage Allowance : 7kg\n\n");
            printf("WEBSITE : www.flightreservation.com");
            printf("\n\nPress any key to continue:");
            getch();
            goto start;



          }
          else
          {
            printf("\nError in getting the card details.\nYour Transaction has been failed.\n");
            sleep(1);
            printf("Please enter it again");
            sleep(1);
            printf(".");
            sleep(1);
            printf(".");
            sleep(1);
            printf(".");
            sleep(1);
            goto reenterCardDetails;
          }
       }
       else if(option==2)
       {
          goto pub;
       }
       else
       {
          printf("Invalid Choice\nPress any key to continue");
          getch();
          goto reenterCardMode;
       }
    }
    else
    {
     goto pub;
    }
   }
  }
  else
  {
    printf("\n\n\n\n\n\n\n\n                                                   THANK YOU\n\n\n\n\n\n\n\n");
    exit(0);
  }

}
