/*Program to create three or more records in a file*/
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

typedef struct normal_user{
	int userID;     // ID assigned to the user
	char name[50];  // Name of the user
	char password[20];  // Password set by the user
	int account_no;   // User's account number
	float balance;   // Account's current balance
	char status[20];  // Tells whether the account is ACTIVE or not
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

int getnormalUserID();
int getjointUserID();
int getadminUserID();

// The following three functions get the user ID for the user 

int getnormalUserID(){
	int fd=open("normalUserdata",O_RDONLY,0744);
	normal_user user_data;
	lseek(fd,-sizeof(normal_user),SEEK_END);
	read(fd,&user_data,sizeof(normal_user));
	close(fd);
	return user_data.userID+1;
}

int getjointUserID(){
	int fd=open("jointUserdata",O_RDONLY,0744);
	jointAcc_user user_data;
	lseek(fd,-sizeof(jointAcc_user),SEEK_END);
	read(fd,&user_data,sizeof(jointAcc_user));
	close(fd);
	return user_data.userID+1;
}

int getadminUserID(){
	int fd=open("adminUserdata",O_RDONLY,0744);
	admin_user user_data;
	lseek(fd,-sizeof(admin_user),SEEK_END);
	read(fd,&user_data,sizeof(admin_user));
	close(fd);
	return user_data.userID+1;
}

int main(){

	int fd=open("normalUserdata",O_RDWR | O_CREAT,0744);
	int choice = 0;
	normal_user user;
	printf("Enter the name of the normal user: ");
	scanf(" %[^\n]",user.name);
	printf("Enter the password (not more than 20 characters): ");
	scanf(" %[^\n]",user.password);
	user.userID=1000;
	printf("Your userID is : %d\n",user.userID);
	user.balance=1000;
	user.account_no=(user.userID-1000)+100000;
	printf("Your account number : %d\n",user.account_no);
	strcpy(user.status,"ACTIVE");
	write(fd,&user,sizeof(normal_user));
	printf("Enter 1 to add more users else enter 0: ");
	scanf("%d",&choice);
	while(choice)
	{
		printf("Enter the name of the normal user: ");
		scanf(" %[^\n]",user.name);
		printf("Enter the password (not more than 20 characters): ");
		scanf(" %[^\n]",user.password);
		user.userID=getnormalUserID();
		printf("Your userID is : %d\n",user.userID);
		user.balance=1000;
		user.account_no=(user.userID-1000)+100000;
		printf("Your account number : %d\n",user.account_no);
		strcpy(user.status,"ACTIVE");
		write(fd,&user,sizeof(normal_user));
		printf("Enter 1 to add more users else enter 0: ");
		scanf("%d",&choice);
	}
	close(fd);

	fd=open("jointUserdata",O_RDWR | O_CREAT,0744);
	choice = 0;
	jointAcc_user userJ;
	printf("Enter the main name of the joint user: ");
	scanf(" %[^\n]",userJ.name1);
	printf("Enter the second name of the joint user: ");
	scanf(" %[^\n]",userJ.name2);
	printf("Enter the password (not more than 20 characters): ");
	scanf(" %[^\n]",userJ.password);
	userJ.userID=1000;
	printf("Your userID is : %d\n",userJ.userID);
	userJ.balance=1000;
	userJ.account_no=(userJ.userID-1000)+100000;
	printf("Your account number : %d\n",user.account_no);
	strcpy(userJ.status,"ACTIVE");
	write(fd,&userJ,sizeof(jointAcc_user));
	printf("Enter 1 to add more users else enter 0: ");
	scanf("%d",&choice);
	while(choice)
	{
		printf("Enter the main name of the joint user: ");
		scanf(" %[^\n]",userJ.name1);
		printf("Enter the second name of the joint user: ");
		scanf(" %[^\n]",userJ.name2);
		printf("Enter the password (not more than 20 characters): ");
		scanf(" %[^\n]",userJ.password);
		userJ.userID=getjointUserID();
		printf("Your userID is : %d\n",userJ.userID);
		userJ.balance=1000;
		userJ.account_no=(userJ.userID-1000)+100000;
		printf("Your account number : %d\n",userJ.account_no);
		strcpy(userJ.status,"ACTIVE");
		write(fd,&userJ,sizeof(jointAcc_user));
		printf("Enter 1 to add more users else enter 0: ");
		scanf("%d",&choice);
	}
	close(fd);

	fd=open("adminUserdata",O_RDWR | O_CREAT,0744);
	choice = 0;
	admin_user userA;
	printf("Enter the name of the admin: ");
	scanf(" %s",userA.username);
	printf("Enter the password (not more than 20 characters): ");
	scanf(" %s",userA.password);
	userA.userID=1000;
	printf("Your userID is : %d\n",userA.userID);
	write(fd,&userA,sizeof(userA));
	printf("Enter 1 to add more users else enter 0: ");
	scanf("%d",&choice);
	while(choice)
	{
		printf("Enter the name of the admin: ");
		scanf(" %[^\n]",userA.username);
		printf("Enter the password (not more than 20 characters): ");
		scanf(" %[^\n]",userA.password);
		userA.userID=getadminUserID();
		printf("Your userID is : %d\n",userA.userID);
		write(fd,&userA,sizeof(admin_user));
		printf("Enter 1 to add more users else enter 0: ");
		scanf("%d",&choice);
	}
	close(fd);
	return 0;
}