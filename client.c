#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdbool.h>

typedef struct normal_user{
	int userID;
	char name[50];
	char password[20];
	int account_no;
	float balance;
	char status[20];
}normal_user;

typedef struct jointAcc_user{
	int userID;
	char name1[50];
	char name2[50];
	char password[20];
	int account_no;
	float balance;
	char status[20];
}jointAcc_user;

typedef struct admin_user{
	int userID;
	char username[50];
	char password[20];
}admin_user;

void attemptNormalUserLogin(int sd);
void attemptJointUserLogin(int sd);
void attemptAdminLogin(int sd);
void showMenu(int sd);
void deposit(int sd);
void withdraw(int sd);
void balanceEnquiry(int sd);
void changePassword(int sd);
void viewDetails(int sd);
void addAccount(int sd);
void deleteAccount(int sd);
void modifyAccount(int sd);
void searchAccount(int sd);
int option,currUserID;

void chooseOption(int sd){
	while(1){
		write(1,"Welcome! Choose an option from below : ",sizeof("Welcome! Choose an option from below : "));
		printf("\n");
		write(1,"1 : Normal-User Login\n",sizeof("1 : Normal-User Login\n"));
		write(1,"2 : Joint Account-User Login\n",sizeof("2 : Joint Account-User Login\n"));
		write(1,"3 : Admin Login\n",sizeof("3 : Admin Login\n"));

		// printf("1 : Normal-User Login\n");
		// printf("2 : Joint Account-User Login\n");
		// printf("3 : Admin Login\n");
		
		printf("Option : ");
		scanf("%d",&option);
		// printf("B\n");
		if(option == 1){
			// printf("A\n");
			attemptNormalUserLogin(sd); 
			break;
		}
		else if(option == 2){
			attemptJointUserLogin(sd);
			break;
		}
		else if(option == 3){
			attemptAdminLogin(sd);
			break;
		}
		else {
			printf("Invalid option. Try again\n");
		}
	}
	return;
}

void showMenu(int sd){
	int select;
	if(option==1 || option==2){
		write(1,"1 : Deposit Money\n",sizeof("1 : Deposit Money\n"));
		write(1,"2 : Withdraw Money\n",sizeof("2 : Withdraw Money\n"));
		write(1,"3 : Balance Enquiry\n",sizeof("3 : Balance Enquiry\n"));
		write(1,"4 : Change Password\n",sizeof("4 : Change Password\n"));
		write(1,"5 : View Details\n",sizeof("5 : View Details\n"));
		write(1,"6 : Exit\n",sizeof("6 : Exit\n"));
	
		write(1,"Choose an option : ",sizeof("Choose an option : "));
		scanf("%d",&select);
		
		
		if(select == 1)
		{
			deposit(sd);
		}
		else if(select == 2)
		{
			withdraw(sd);
		}
		else if(select == 3)
		{
			balanceEnquiry(sd);
		}
		else if(select == 4)
		{
			changePassword(sd);
		}
		else if(select == 5)
		{
			viewDetails(sd);
		}
		else if(select == 6)
		{
			write(sd,&select,sizeof(int));
			printf("Bye\n");
			exit(0);
		}
		else
		{
			printf("Invalid option. Try again\n");
			showMenu(sd);
		}
	}
	else if(option==3){
		write(1,"1 : Add Account\n",sizeof("1 : Add Account\n"));
		write(1,"2 : Delete Account\n",sizeof("2 : Delete Account\n"));
		write(1,"3 : Modify Account\n",sizeof("3 : Modify Account\n"));
		write(1,"4 : Search Account\n",sizeof("4 : Search Account\n"));
		write(1,"5 : Exit\n",sizeof("6 : Exit\n"));

		write(1,"Choose an option : ",sizeof("Choose an option : "));
		scanf("%d",&select);
		
		if(select == 1)
		{
			addAccount(sd);
		}
		else if(select == 2)
		{
			deleteAccount(sd);
		}
		else if(select == 3)
		{
			modifyAccount(sd);
		}
		else if(select == 4)
		{
			searchAccount(sd);
		}
		else if(select == 5)
		{
			write(sd,&select,sizeof(int));
			printf("Bye!\n");
			exit(0);
		}
		else
		{
			printf("Invalid option. Try Again\n");
			showMenu(sd);
		}

	}
}

void attemptNormalUserLogin(int sd){
	bool result;
	normal_user currUser;
	write(1,"User ID : ",sizeof("User ID : "));
	scanf("%d",&currUser.userID);
	currUserID=currUser.userID;
	write(1,"Password : ",sizeof("Password : "));
	scanf("%s",currUser.password);

	//Sending to the server
	write(sd,&option,sizeof(int));
	write(sd,&currUser,sizeof(normal_user));

	read(sd,&result,sizeof(result)); 
	// printf("A\n");
	if(!result)
	{
		printf("Invalid Login. Try Again\n");
		// printf("B\n");
		chooseOption(sd);
	}
	else{
		printf("Successful Login.\n");
	}
	return;
}

void attemptJointUserLogin(int sd){
	bool result;
	jointAcc_user currUser;
	write(1,"User ID : ",sizeof("User ID : "));
	scanf("%d",&currUser.userID);
	currUserID=currUser.userID;
	write(1,"Password : ",sizeof("Password : "));
	scanf("%s",currUser.password);

	//Sending to the server
	write(sd,&option,sizeof(int));
	write(sd,&currUser,sizeof(jointAcc_user));

	read(sd,&result,sizeof(result)); 

	if(!result)
	{
		printf("Invalid Login. Try Again\n");
		chooseOption(sd);
	}
	else
	{
		printf("Successful Login.\n");
		
	}
	return;
}

void attemptAdminLogin(int sd){
	bool result;
	admin_user currUser;
	write(1,"User ID : ",sizeof("User ID : "));
	scanf("%d",&currUser.userID);
	currUserID=currUser.userID;
	write(1,"Password : ",sizeof("Password : "));
	scanf("%s",currUser.password);

	//Sending to the server
	write(sd,&option,sizeof(int));
	write(sd,&currUser,sizeof(admin_user));

	read(sd,&result,sizeof(result)); //from the server

	if(!result)
	{
		printf("Invalid Login. Try Again\n");
		chooseOption(sd);
	}
	else
	{
		printf("Successful Login.\n");
		
	}
	return;
}

void deposit(int sd){
	float amt;
	int select=1;
	bool result;

	write(1,"Amount to Deposit : Rs.",sizeof("Amount to Deposit : Rs."));
	scanf("%f",&amt);

	while(amt<=0){
		printf("Enter a valid amount.\n");
		write(1,"Amount to Deposit : Rs.",sizeof("Amount to Deposit : Rs."));
		scanf("%f",&amt);
	}

	//Sending to the server
	write(sd,&select,sizeof(int));
	write(sd,&amt,sizeof(float));

	read(sd,&result,sizeof(result)); 

	if(!result)
	{
		printf("Error Occurred\n");
	}
	else
	{
		printf("Succesfully deposited.\n");
		
	}
	showMenu(sd);
	return;
}

void withdraw(int sd){
	float amt;
	int select=2;
	bool result;

	write(1,"Amount to Withdraw : Rs.",sizeof("Amount to Withdraw : Rs."));
	scanf("%f",&amt);

	while(amt<=0){
		printf("Enter a valid amount.\n");
		write(1,"Amount to Withdraw : Rs.",sizeof("Amount to Withdraw : Rs."));
		scanf("%f",&amt);
	}

	//Sending to the server
	write(sd,&select,sizeof(int));
	write(sd,&amt,sizeof(float));

	read(sd,&result,sizeof(result)); 

	if(!result)
	{
		printf("Error Occurred\n");
		
	}
	else
	{
		printf("Succesfully withdrawn.\n");
	}
	showMenu(sd);
	return;
}

void balanceEnquiry(int sd){
	float amt;
	int select=3;
	int len;

	//Sending to the server
	write(sd,&select,sizeof(int));
	
	len=read(sd,&amt,sizeof(float));	

	write(1,"Available Balance :: Rs.",sizeof("Available Balance :: Rs."));
	printf("%0.2f\n\n",amt);

	showMenu(sd);
	return;
}

void changePassword(int sd){
	int select=4;
	char nPassword[20];
	bool result;

	write(1,"Enter the new password(max upto 20 characters) : ",sizeof("Enter the new password(max upto 20 characters) : "));
	scanf("%s",nPassword);

	//Sending to the server
	write(sd,&select,sizeof(int));
	write(sd,nPassword,sizeof(nPassword));

	read(sd,&result,sizeof(result)); //from the server

	if(!result)
	{
		printf("Error Occurred\n");
		
	}
	else
	{
		printf("Successful\n");
		
	}
	showMenu(sd);
	return;
}

void viewDetails(int sd){
	int select=5;

	//Sending to the server
	write(sd,&select,sizeof(int));

	if(option==1){
		normal_user nuser;
		read(sd,&nuser,sizeof(normal_user));
		
		printf("User ID : %d\n",nuser.userID);
		printf("Name : %s\n",nuser.name);
		printf("Account Number : %d\n",nuser.account_no);
		printf("Available Balance : Rs.%0.2f\n",nuser.balance);
		printf("Status : %s\n\n",nuser.status);
	}
	else if(option==2){
		jointAcc_user juser;
		read(sd,&juser,sizeof(jointAcc_user));
		
		printf("User ID : %d\n",juser.userID);
		printf("Main Account Holder's Name : %s\n",juser.name1);
		printf("Other Account Holder's Name : %s\n",juser.name2);
		printf("Account Number : %d\n",juser.account_no);
		printf("Available Balance : Rs.%0.2f\n",juser.balance);
		printf("Status : %s\n\n",juser.status);
	}
	showMenu(sd);
	return;
}

void addAccount(int sd){
	int select=1;
	int type;
	bool result;

	write(sd,&select,sizeof(int));

	write(1,"Enter the type account(1: Normal Account 2: Joint Account) : ",sizeof("Enter the type account(1: Normal Account 2: Joint Account) : "));
	scanf("%d",&type);

	write(sd,&type,sizeof(int));

	if(type==1){
		normal_user nuser;
		write(1,"Name of the account holder : ",sizeof("Name of the account holder : "));
		scanf(" %[^\n]",nuser.name);
		write(1,"Password (Not more than 20 characters) : ",sizeof("Password (Not more than 20 characters) : "));
		scanf("%s",nuser.password);
		write(1,"Initial Deposit : Rs.",sizeof("Initial Deposit : Rs."));
		scanf("%f",&nuser.balance);
		write(sd,&nuser,sizeof(normal_user));
	}

	if(type==2){
		jointAcc_user juser;
		write(1,"Name of the primary account holder : ",sizeof("Name of the primary account holder : "));
		scanf(" %[^\n]",juser.name1);
		write(1,"Name of the other account holder : ",sizeof("Name of the other account holder : "));
		scanf(" %[^\n]",juser.name2);
		write(1,"Password (Not more than 20 characters) : ",sizeof("Password (Not more than 20 characters) : "));
		scanf("%s",juser.password);
		write(1,"Initial Deposit : Rs.",sizeof("Initial Deposit : Rs."));
		scanf("%f",&juser.balance);
		write(sd,&juser,sizeof(jointAcc_user));
	}
	
	read(sd,&result,sizeof(result)); //from the server

	if(!result)
	{
		printf("Error Occurred. Try Again\n");
		
	}
	else
	{
		printf("Successful!\n");
		
	}
	showMenu(sd);
	return;
}

void deleteAccount(int sd){
	int select=2;
	int type,userID;
	bool result;

	write(sd,&select,sizeof(int));

	write(1,"Enter the type account(1: Normal Account 2: Joint Account) : ",sizeof("Enter the type account(1: Normal Account 2: Joint Account) : "));
	scanf("%d",&type);
	
	write(sd,&type,sizeof(int));

	write(1,"User ID : ",sizeof("User ID : "));
	scanf("%d",&userID);

	//Sending to the server
	write(sd,&userID,sizeof(int));
	
	read(sd,&result,sizeof(result)); \

	if(!result)
	{
		printf("Error Occurred. Try Again\n");
	}
	else
	{
		printf("Successful!\n");
	}
	showMenu(sd);
	return;
}

void modifyAccount(int sd){
	int select=3;
	int type;
	bool result;

	write(sd,&select,sizeof(int));

	write(1,"Enter the type account(1: Normal Account 2: Joint Account) : ",sizeof("Enter the type account(1: Normal Account 2: Joint Account) : "));
	scanf("%d",&type);

	write(sd,&type,sizeof(int));

	if(type==1){
		normal_user nuser;
		write(1,"User ID : ",sizeof("User ID : "));
		scanf("%d",&nuser.userID);
		write(1,"Account Number : ",sizeof("Account Number : "));
		scanf("%d",&nuser.account_no);
		write(1,"New Name of the account holder : ",sizeof("New Name of the account holder : "));
		scanf(" %[^\n]",nuser.name);
		write(1,"New Password (Not more than 20 characters) : ",sizeof("New Password (Not more than 20 characters) : "));
		scanf("%s",nuser.password);
		write(1,"New Balance : ",sizeof("New Balance : "));
		scanf("%f",&nuser.balance);
		write(sd,&nuser,sizeof(normal_user));
	}

	if(type==2){
		jointAcc_user juser;
		write(1,"User ID : ",sizeof("User ID : "));
		scanf("%d",&juser.userID);
		write(1,"Account Number : ",sizeof("Account Number : "));
		scanf("%d",&juser.account_no);
		write(1,"New Name of the primary account holder : ",sizeof("New Name of the primary account holder : "));
		scanf(" %[^\n]",juser.name1);
		write(1,"New Name of the other account holder : ",sizeof("New Name of the other account holder : "));
		scanf(" %[^\n]",juser.name2);
		write(1,"New Password (Not more than 20 characters) : ",sizeof("New Password (Not more than 20 characters) : "));
		scanf("%s",juser.password);
		write(1,"New Balance : ",sizeof("New Balance : "));
		scanf("%f",&juser.balance);
		write(sd,&juser,sizeof(jointAcc_user));
	}
	
	read(sd,&result,sizeof(result)); //from the server

	if(!result)
	{
		printf("Error Occurred. Try Again\n");
		
	}
	else
	{
		printf("Successful!\n");
		
	}
	showMenu(sd);
	return;
}

void searchAccount(int sd){
	int select=4;
	int type,len;
	bool result;

	write(sd,&select,sizeof(int));

	write(1,"Enter the type account(1: Normal Account 2: Joint Account) : ",sizeof("Enter the type account(1: Normal Account 2: Joint Account) : "));
	scanf("%d",&type);

	write(sd,&type,sizeof(int));

	if(type==1){
		normal_user nuser;
		int userID;
		write(1,"User ID : ",sizeof("User ID : "));
		scanf("%d",&userID);
		write(sd,&userID,sizeof(int));
		
		len=read(sd,&nuser,sizeof(normal_user));
		if(len==0)
		{
			write(1,"Invalid User ID!\n\n",sizeof("Invalid User ID!\n\n"));
		}
		else
		{
			
			printf("Name : %s\n",nuser.name);
			printf("Account Number : %d\n",nuser.account_no);
			printf("Available Balance : Rs.%0.2f\n",nuser.balance);
			printf("Status : %s\n\n",nuser.status);
		}
	}

	if(type==2){
		jointAcc_user juser;
		int userID1;
		write(1,"User ID : ",sizeof("User ID : "));
		scanf("%d",&userID1);
		write(sd,&userID1,sizeof(int));
		
		len=read(sd,&juser,sizeof(jointAcc_user));
		if(len==0)
		{
			write(1,"Invalid User ID!!\n\n",sizeof("Invalid User ID!!\n\n"));
		}
		else{
			printf("Main Account Holder's Name : %s\n",juser.name1);
			printf("Other Account Holder's Name : %s\n",juser.name2);
			printf("Account Number : %d\n",juser.account_no);
			printf("Available Balance : Rs.%0.2f\n",juser.balance);
			printf("Status : %s\n\n",juser.status);
		}
	}
	showMenu(sd);
	return;
}

int main(){
	struct sockaddr_in server;
	int sd,msgLength;
	char buff[50];
	char result;


	sd=socket(AF_INET,SOCK_STREAM,0);
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1"); 
	server.sin_port=htons(5555);
	connect(sd,(struct sockaddr *)&server,sizeof(server));

	chooseOption(sd);
	showMenu(sd);	

	close(sd);

	return 0;
}