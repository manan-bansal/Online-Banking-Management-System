#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include<fcntl.h>
#include <pthread.h>

typedef struct normal_user{
	int userID;
	char name[50];
	char password[20];
	int account_no;
	float balance;
	char status[20];
}normal_user;

typedef struct joint_user{
	int userID;
	char name1[50];
	char name2[50];
	char password[20];
	int account_no;
	float balance;
	char status[20];
}joint_user;

typedef struct admin_user{
	int userID;
	char username[50];
	char password[20];
}admin_user;


admin_user getadmin_user(int ID){
	int i=ID-1000;
	admin_user user;
	int fd=open("adminUserdata",O_RDONLY,0744);
	int fcntl_value;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(admin_user);    	     
	lock.l_len=sizeof(admin_user);	             
	lock.l_pid=getpid();
	
	fcntl_value=fcntl(fd,F_SETLKW,&lock);	//Initiating the Lock

	lseek(fd,(i)*sizeof(admin_user),SEEK_SET);  
	read(fd,&user,sizeof(admin_user));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock); //Unlocking

	close(fd);
	return user;
}


normal_user getnormal_user(int ID){
	int i=ID-1000;
	normal_user user;
	int fd=open("normalUserdata",O_RDONLY,0744);
	
	int fcntl_value;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(normal_user);    	     
	lock.l_len=sizeof(normal_user);	             
	lock.l_pid=getpid();
	
	fcntl_value=fcntl(fd,F_SETLKW,&lock);	//Initiating the Lock

	lseek(fd,(i)*sizeof(normal_user),SEEK_SET);  
	read(fd,&user,sizeof(normal_user));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock); //Unlocking

	close(fd);
	return user;
}

joint_user getjoint_user(int ID){
	int i=ID-1000;
	joint_user user;
	int fd=open("jointUserdata",O_RDONLY,0744);
	
	int fcntl_value;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(joint_user);    	     
	lock.l_len=sizeof(joint_user);	             
	lock.l_pid=getpid();
	
	fcntl_value=fcntl(fd,F_SETLKW,&lock);	//Initiating the Lock

	lseek(fd,(i)*sizeof(joint_user),SEEK_SET);  
	read(fd,&user,sizeof(joint_user));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock); //Unlocking

	close(fd);
	return user;
}


bool checkadmin_user(admin_user user){
	int i=user.userID-1000;
	int fd=open("adminUserdata",O_RDONLY,0744);
	bool result;
	admin_user temp;
	
	int fcntl_value;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(admin_user);    	     
	lock.l_len=sizeof(admin_user);	             
	lock.l_pid=getpid();
	
	fcntl_value=fcntl(fd,F_SETLKW,&lock);	 //Initiating the Lock
	

	lseek(fd,(i)*sizeof(admin_user),SEEK_SET);  
	read(fd,&temp,sizeof(admin_user));
	if(!strcmp(temp.password,user.password))	
	{
		result=true;
	}
	else
	{	
		result=false;
	}
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock); //Unlocking

	close(fd);
	return result;
}


bool checknormal_user(normal_user user){
	int i=user.userID-1000;
	int fd=open("normalUserdata",O_RDONLY,0744);
	bool result;
	normal_user temp;
	
	int fcntl_value;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(normal_user);    	     
	lock.l_len=sizeof(normal_user);	             
	lock.l_pid=getpid();
	
	fcntl_value=fcntl(fd,F_SETLKW,&lock);	//Initiating the Lock

	lseek(fd,(i)*sizeof(normal_user),SEEK_SET);  
	read(fd,&temp,sizeof(normal_user));
	// printf("A\n");
	if(!strcmp(temp.password,user.password) && !strcmp(temp.status,"ACTIVE")){
		result=true;
	}
	else {
		result=false;
	}

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock); //Unlocking

	close(fd);
	return result;
}

bool checkjoint_user(joint_user user){
	int i=user.userID-1000;
	int fd=open("jointUserdata",O_RDONLY,0744);
	bool result;
	joint_user temp;
	
	int fcntl_value;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(joint_user);    	    
	lock.l_len=sizeof(joint_user);	             
	lock.l_pid=getpid();
	
	fcntl_value=fcntl(fd,F_SETLKW,&lock);	//Initiating the Lock

	lseek(fd,(i)*sizeof(joint_user),SEEK_SET);  
	read(fd,&temp,sizeof(joint_user));
	if(!strcmp(temp.password,user.password) && !strcmp(temp.status,"ACTIVE"))	
	{
		result=true;
	}
	else
	{						
		result=false;
	}
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock); //Unlocking

	close(fd);
	return result;
}


bool taskDeposit(int account_type,int ID,float amt){
	int i=ID-1000;
	if(account_type==1){
		int fd=open("normalUserdata",O_RDWR,0744);
		bool result;
		int fcntl_value;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(normal_user);    
		lock.l_len=sizeof(normal_user);	             
		lock.l_pid=getpid();
	
		fcntl_value=fcntl(fd,F_SETLKW,&lock);	//Initiating the Lock
		 

		normal_user user;
		lseek(fd,(i)*sizeof(normal_user),SEEK_SET);  
		read(fd,&user,sizeof(normal_user));
		
		if(!strcmp(user.status,"ACTIVE")){
			user.balance+=amt;
			lseek(fd,sizeof(normal_user)*(-1),SEEK_CUR);
			write(fd,&user,sizeof(normal_user));
			result=true;
		}
		else	
		{
			result=false;
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock); //Unlocking

		close(fd);
		return result;		
	}
	else if(account_type==2){
		int fd=open("jointUserdata",O_RDWR,0744);
		bool result;
		int fcntl_value;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(joint_user);    
		lock.l_len=sizeof(joint_user);	             
		lock.l_pid=getpid();
	
		fcntl_value=fcntl(fd,F_SETLKW,&lock);	//Initiating the Lock
		

		joint_user user;
		lseek(fd,(i)*sizeof(joint_user),SEEK_SET);  
		read(fd,&user,sizeof(joint_user));
		
		if(!strcmp(user.status,"ACTIVE")){
			user.balance+=amt;
			lseek(fd,sizeof(joint_user)*(-1),SEEK_CUR);
			write(fd,&user,sizeof(joint_user));
			result=true;
		}
		else	
		{
			result=false;
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);//Unlocking

		close(fd);
		return result;	
	}
	return false;
}

bool taskWithdraw(int account_type,int ID,float amt){
	int i=ID-1000;
	if(account_type==1){
		int fd=open("normalUserdata",O_RDWR,0744);
		bool result;
		int fcntl_value;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(normal_user);    
		lock.l_len=sizeof(normal_user);	             
		lock.l_pid=getpid();
	
		fcntl_value=fcntl(fd,F_SETLKW,&lock);	//Initiating the Lock
		

		normal_user user;
		lseek(fd,(i)*sizeof(normal_user),SEEK_SET);  
		read(fd,&user,sizeof(normal_user));
		
		if(!strcmp(user.status,"ACTIVE") && user.balance>=amt){
			user.balance-=amt;
			lseek(fd,sizeof(normal_user)*(-1),SEEK_CUR);
			write(fd,&user,sizeof(normal_user));
			result=true;
		}
		else	
		{
			result=false;
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock); //Unlocking

		close(fd);
		return result;	
	}
	else if(account_type==2){
		int fd=open("jointUserdata",O_RDWR,0744);
		bool result;
		int fcntl_value;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(joint_user);    
		lock.l_len=sizeof(joint_user);	             
		lock.l_pid=getpid();
	
		fcntl_value=fcntl(fd,F_SETLKW,&lock);	//Initiating the Lock
		

		joint_user user;
		lseek(fd,(i)*sizeof(joint_user),SEEK_SET);  
		read(fd,&user,sizeof(joint_user));
		
		if(!strcmp(user.status,"ACTIVE") && user.balance>=amt){
			user.balance-=amt;
			lseek(fd,sizeof(joint_user)*(-1),SEEK_CUR);
			write(fd,&user,sizeof(joint_user));
			result=true;
		}
		else	
		{
			result=false;
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock); //Unlocking

		close(fd);
		return result;
	}
	return false;
}

float taskBalance(int account_type,int ID){
	int i=ID-1000;
	float result;
	if(account_type==1){
		int i=ID-1000;
		int fd=open("normalUserdata",O_RDONLY,0744);
		normal_user temp;
	
		int fcntl_value;
		struct flock lock;
		lock.l_type = F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(normal_user);    	     
		lock.l_len=sizeof(normal_user);	             
		lock.l_pid=getpid();
	
		fcntl_value=fcntl(fd,F_SETLKW,&lock);	//Initiating the Lock
		

		lseek(fd,(i)*sizeof(normal_user),SEEK_SET);  
		read(fd,&temp,sizeof(normal_user));
		if(!strcmp(temp.status,"ACTIVE"))	
		{
			result=temp.balance;
		}
		else
		{	
			result=0;
		}

		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);//Unlocking

		close(fd);
		return result;
	}
	else if(account_type==2){
		int i=ID-1000;
		int fd=open("jointUserdata",O_RDONLY,0744);
		joint_user temp;
	
		int fcntl_value;
		struct flock lock;
		lock.l_type = F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(joint_user);    	     
		lock.l_len=sizeof(joint_user);	             
		lock.l_pid=getpid();
	
		fcntl_value=fcntl(fd,F_SETLKW,&lock);	//Initiating the Lock
		

		lseek(fd,(i)*sizeof(joint_user),SEEK_SET);  
		read(fd,&temp,sizeof(joint_user));
		if(!strcmp(temp.status,"ACTIVE"))	
		{
			result=temp.balance;
		}
		else
		{					
			result=0;
		}

		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);//Unlocking

		close(fd);
		return result;
	}
	return 0;
}

bool passwordChange(int account_type,int ID,char newPwd[10]){
	int i=ID-1000;
	if(account_type==1){
		int fd=open("normalUserdata",O_RDWR,0744);
		bool result;
		int fcntl_value;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(normal_user);    
		lock.l_len=sizeof(normal_user);	             
		lock.l_pid=getpid();
	
		fcntl_value=fcntl(fd,F_SETLKW,&lock);	//Initiating the Lock
		

		normal_user user;
		lseek(fd,(i)*sizeof(normal_user),SEEK_SET);  
		read(fd,&user,sizeof(normal_user));
		
		if(!strcmp(user.status,"ACTIVE")){
			strcpy(user.password,newPwd);
			lseek(fd,sizeof(normal_user)*(-1),SEEK_CUR);
			write(fd,&user,sizeof(normal_user));
			result=true;
		}
		else	
		{
			result=false;
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);//Unlocking
		close(fd);
		return result;
	}
	else if(account_type==2){
		int fd=open("jointUserdata",O_RDWR,0744);
		bool result;
		int fcntl_value;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(joint_user);    
		lock.l_len=sizeof(joint_user);	             
		lock.l_pid=getpid();
	
		fcntl_value=fcntl(fd,F_SETLKW,&lock);	//Initiating the Lock
		

		joint_user user;
		lseek(fd,(i)*sizeof(joint_user),SEEK_SET);  
		read(fd,&user,sizeof(joint_user));
		
		if(!strcmp(user.status,"ACTIVE")){
			strcpy(user.password,newPwd);
			lseek(fd,sizeof(joint_user)*(-1),SEEK_CUR);
			write(fd,&user,sizeof(joint_user));
			result=true;
		}
		else	
		{
			result=false;
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);//Unlocking

		close(fd);
		return result;
	}
	return false;
}

bool addnormal_user(normal_user record){
	int fd=open("normalUserdata",O_RDWR,0744);
	bool result;
	
	int fcntl_value;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_END;
	lock.l_start=(-1)*sizeof(normal_user);    
	lock.l_len=sizeof(normal_user);	             
	lock.l_pid=getpid();
	
	fcntl_value=fcntl(fd,F_SETLKW,&lock);	//Initiating the Lock
	

	normal_user endUser;
	lseek(fd,(-1)*sizeof(normal_user),SEEK_END);  
	read(fd,&endUser,sizeof(normal_user));
		
	record.userID=endUser.userID+1;
	record.account_no=endUser.account_no+1;
	strcpy(record.status,"ACTIVE");
	
	int j=write(fd,&record,sizeof(normal_user));
	if(j!=0) 
	{
		result=true;
	}
	else
	{	
		result=false;
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);//Unlocking
	
	close(fd);
	return result;	
}

bool addjoint_user(joint_user record){
	int fd=open("jointUserdata",O_RDWR,0744);
	bool result;
	
	int fcntl_value;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_END;
	lock.l_start=(-1)*sizeof(joint_user);    
	lock.l_len=sizeof(joint_user);	             
	lock.l_pid=getpid();
	
	fcntl_value=fcntl(fd,F_SETLKW,&lock);	//Initiating the Lock
	

	joint_user endUser;
	lseek(fd,(-1)*sizeof(joint_user),SEEK_END);  
	read(fd,&endUser,sizeof(joint_user));
		
	record.userID=endUser.userID+1;
	record.account_no=endUser.account_no+1;
	strcpy(record.status,"ACTIVE");
	
	int j=write(fd,&record,sizeof(joint_user));
	if(j!=0) {
		result=true;
	}
	else 
	{	
		result=false;
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);//Unlocking
	
	close(fd);
	return result;	
}

bool deletenormal_user(int ID){
	int i=ID-1000;
	int fd=open("normalUserdata",O_RDWR,0744);
	bool result;
	
	int fcntl_value;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(normal_user);    
	lock.l_len=sizeof(normal_user);	             
	lock.l_pid=getpid();
	
	fcntl_value=fcntl(fd,F_SETLKW,&lock);	//Initiating the Lock
	

	normal_user user;
	lseek(fd,(i)*sizeof(normal_user),SEEK_SET);  
	read(fd,&user,sizeof(normal_user));
	
	if(!strcmp(user.status,"ACTIVE")){	
		strcpy(user.status,"CLOSED");
		user.balance=0;
		
		lseek(fd,(-1)*sizeof(normal_user),SEEK_CUR); 
		int j=write(fd,&user,sizeof(normal_user));
		if(j!=0)
		{
			result=true;
		}
		else{
			result=false;
		}
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);//Unlocking
	
	close(fd);
	return result;	
}

bool deletejoint_user(int ID){
	int i=ID-1000;
	int fd=open("jointUserdata",O_RDWR,0744);
	bool result;
	
	int fcntl_value;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(joint_user);    
	lock.l_len=sizeof(joint_user);	             
	lock.l_pid=getpid();
	
	fcntl_value=fcntl(fd,F_SETLKW,&lock);	//Initiating the Lock
	

	joint_user user;
	lseek(fd,(i)*sizeof(joint_user),SEEK_SET);  
	read(fd,&user,sizeof(joint_user));
	
	if(!strcmp(user.status,"ACTIVE")){	
		strcpy(user.status,"CLOSED");
		user.balance=0;
		
		lseek(fd,(-1)*sizeof(joint_user),SEEK_CUR); 
		int j=write(fd,&user,sizeof(joint_user));
		if(j!=0){
			result=true;
		}
		else{
			result=false;
		}
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);//Unlocking
	
	close(fd);
	return result;	
}

bool modifynormal_user(normal_user muser){
	int i=muser.userID-1000;
	int fd=open("normalUserdata",O_RDWR,0744);
	bool result=false;
	
	int fcntl_value;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(normal_user);    
	lock.l_len=sizeof(normal_user);	             
	lock.l_pid=getpid();
	
	fcntl_value=fcntl(fd,F_SETLKW,&lock);	//Initiating the Lock
	

	normal_user user;
	lseek(fd,(i)*sizeof(normal_user),SEEK_SET);  
	read(fd,&user,sizeof(normal_user));
	
	if(!strcmp(user.status,"ACTIVE") && (muser.account_no==user.account_no)){	
		strcpy(muser.status,"ACTIVE");
		lseek(fd,(-1)*sizeof(normal_user),SEEK_CUR); 
		int j=write(fd,&muser,sizeof(normal_user));
		if(j!=0)
		{	result=true;
		}
		else{
			result=false;
		}
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);//Unlocking
	
	close(fd);
	return result;	
}

bool modifyjoint_user(joint_user muser){
	int i=muser.userID-1000;
	int fd=open("jointUserdata",O_RDWR,0744);
	bool result=false;
	
	int fcntl_value;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(joint_user);    
	lock.l_len=sizeof(joint_user);	             
	lock.l_pid=getpid();
	
	fcntl_value=fcntl(fd,F_SETLKW,&lock);	//Initiating the Lock
	

	joint_user user;
	lseek(fd,(i)*sizeof(joint_user),SEEK_SET);  
	read(fd,&user,sizeof(joint_user));
	
	if(!strcmp(user.status,"ACTIVE")  && (muser.account_no==user.account_no)){	
		strcpy(muser.status,"ACTIVE");
		lseek(fd,(-1)*sizeof(joint_user),SEEK_CUR); 
		int j=write(fd,&muser,sizeof(joint_user));
		if(j!=0){	
			result=true;
		}
		else
		{	
			result=false;
		}
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);//Unlocking
	
	close(fd);
	return result;	
}

void serverMain(int nsd){
	int msgLength,select,type,option,account_type,userID;
	bool result;
	while(1){
		read(nsd,&option,sizeof(option));
		printf("Option : %d\n",option);

		if(option==1){
			normal_user nuser;
			account_type=1;
			msgLength=read(nsd,&nuser,sizeof(normal_user));
			printf("Username : %d\n",nuser.userID);
			printf("Password : %s\n",nuser.password);
			userID=nuser.userID;
			result=checknormal_user(nuser);
			write(nsd,&result,sizeof(result));
		}
		else if(option==2){
			joint_user juser;
			account_type=2;
			msgLength=read(nsd,&juser,sizeof(joint_user));
			userID=juser.userID;
			printf("Username : %d\n",juser.userID);
			printf("Password : %s\n",juser.password);
			result=checkjoint_user(juser);
			write(nsd,&result,sizeof(result));
		}
		else if(option==3){
			admin_user auser;
			account_type=3;
			msgLength=read(nsd,&auser,sizeof(admin_user));
			userID=auser.userID;
			printf("Username : %d\n",auser.userID);
			printf("Password : %s\n",auser.password);
			result=checkadmin_user(auser);
			write(nsd,&result,sizeof(result));
		}
		else{
			result=false;
			write(nsd,&result,sizeof(result));
		}
		if(result)	break;		
	}

	while(1){
		read(nsd,&select,sizeof(int));
		if(option==1 || option==2){
			if(select==1){
				float amt;
				read(nsd,&amt,sizeof(float));
				result=taskDeposit(account_type,userID,amt);
				write(nsd,&result,sizeof(result));
			}
			else if(select==2){
				float amt;
				read(nsd,&amt,sizeof(float));
				result=taskWithdraw(account_type,userID,amt);
				write(nsd,&result,sizeof(result));
			}
			else if(select==3){
				float amt;
				amt=taskBalance(account_type,userID);
				write(nsd,&amt,sizeof(float));
			}
			else if(select==4){
				char pwd[10];
				read(nsd,pwd,sizeof(pwd));
				result=passwordChange(account_type,userID,pwd);
				write(nsd,&result,sizeof(result));
			}
			else if(select==5){
				if(option==1){
					normal_user user1=getnormal_user(userID);
					write(nsd,&user1,sizeof(normal_user));
				}
				else if(option==2){
					joint_user user2=getjoint_user(userID);
					write(nsd,&user2,sizeof(joint_user));
				}
			}
			else if(select==6)	break;
		}
		else if(option==3){
			read(nsd,&type,sizeof(int));
			if(select==1){
				if(type==1){
					normal_user newUser1;
					read(nsd,&newUser1,sizeof(normal_user));
					result=addnormal_user(newUser1);
					write(nsd,&result,sizeof(result));
				}
				else if(type==2){
					joint_user newUser2;
					read(nsd,&newUser2,sizeof(joint_user));
					result=addjoint_user(newUser2);
					write(nsd,&result,sizeof(result));
				}
			}
			else if(select==2){
				if(type==1){
					int delUserID1;
					read(nsd,&delUserID1,sizeof(int));
					result=deletenormal_user(delUserID1);
					write(nsd,&result,sizeof(result));
				}
				else if(type==2){
					int delUserID2;
					read(nsd,&delUserID2,sizeof(int));
					result=deletejoint_user(delUserID2);
					write(nsd,&result,sizeof(result));
				}
			}
			else if(select==3){
				if(type==1){
					normal_user muser1;
					read(nsd,&muser1,sizeof(normal_user));
					result=modifynormal_user(muser1);
					write(nsd,&result,sizeof(result));
				}
				else if(type==2){
					joint_user muser2;
					read(nsd,&muser2,sizeof(joint_user));
					result=modifyjoint_user(muser2);
					write(nsd,&result,sizeof(result));
				}
			}
			else if(select==4){
				if(type==1){
					normal_user searchUser1;
					int userID1;
					read(nsd,&userID1,sizeof(int));
					searchUser1=getnormal_user(userID1);
					write(nsd,&searchUser1,sizeof(normal_user));
				}
				else if(type==2){
					joint_user searchUser2;
					int userID2;
					read(nsd,&userID2,sizeof(int));
					searchUser2=getjoint_user(userID2);
					write(nsd,&searchUser2,sizeof(joint_user));
				}
			}
			else if(select==5)	break;
		}
	}
	close(nsd);
	write(1,"Ended..\n",sizeof("Ended..\n"));
	return;
}

void *connection_handler(void *nsd) {
	int nsfd = *(int*)nsd;
	serverMain(nsfd);
}


int main(){
	struct sockaddr_in server,client;
	int sd,nsd,clientLen;
	pthread_t threads;
	bool result;
	sd=socket(AF_INET,SOCK_STREAM,0);

	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(5555);

	bind(sd,(struct sockaddr *)&server,sizeof(server));
	listen(sd,5);	
	
	write(1,"Waiting..\n",sizeof("Waiting..\n"));
	while(1){
		clientLen=sizeof(client);
		nsd=accept(sd,(struct sockaddr *)&client,&clientLen);

		write(1,"Connection Established\n",sizeof("Connection Established\n"));
		if(pthread_create(&threads,NULL,connection_handler,(void*) &nsd)<0){
			perror("Could not create thread");
			return 1;
		}		
	}
	pthread_exit(NULL);
	close(sd);
	return 0;
}