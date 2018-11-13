#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>

int dirsearch(int argc, char *argv[]){
  int keeptrack = 0; //keeps track of first child process for formatting reasons
  int temppidstore; //temp pid store for child
  int parrentpid;  // pid store for parent
  char currentdir[PATH_MAX];
  char *current;// current directory
  char *dest; // destination directory
  char *temp;
  char *outputname;//output directory
  DIR *d;
  DIR *tempdir;
  struct dirent *dir;
  printf("Initial PID: %d\n",getpid());
  parrentpid=getpid();
  printf("%s","PIDS of all child processes:");
  fflush(stdout);
  if(getcwd(currentdir,sizeof(currentdir))){
    outputname=strdup(currentdir);
    temp=strdup(strtok(currentdir,"/"));
    while(temp != 0)
      {
	current = strdup(temp);//tokenize using / to find current directory
	temp=(strtok(NULL,"/"));//temp to keep track of ending
      }//current holds value of starting directory
  }
  if(argv[3] != NULL)
    {
      if(strcmp(argv[3],"-d")==0)
	{
	  if(strcmp(argv[4],current)==0)//current directory is the starting one
	    {
	      dest=strdup(current);
	    }
	  else
	    {
	      dest = strdup(strtok(argv[4],"/"));
	      dest = strtok(NULL,"");
	    }//now d is directory it was told to go to
	  if(argv[5] != NULL)
	    {
	      if(strcmp(argv[5],"-o")==0)
		{
		  if(strcmp(argv[6],current)!=0)//current directory is not same for output
		    {
		      strcat(outputname,"/");
		      strcat(outputname,argv[6]);  // argv[6] holds output destination so output is current/argv[6]
		    }
	      
		}
	    }
	}
      else if(strcmp(argv[3],"-o")==0)
	{
	  if(strcmp(argv[4],current)!=0)//current directory is not same for output
		    {
		     strcat(outputname,"/");
		     strcat(outputname,argv[4]); // argv[4] holds output destination so output is current/argv[4]
		    }
	}
    }
  else
    {
      dest=strdup(current);
    }
  FILE *outputfile;
  d=opendir(dest);
  chdir(dest); //chdir is needed at this point so that if the new directory is lower the program can get to it
  int filenamelen = 0;
  char *csvcheck;
  char *filenamecpy;
  if(d)
    {
      while((dir=readdir(d)) !=NULL)// to traverse through current directory
	{
	  
	  if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!= 0)//makes sure not to do commands on the . and .. that pop up
	    {
	      if(strlen(dir->d_name)>4)//anything with a .csv must be of length 5 min for a character .csv
		{
		  filenamelen = strlen(dir->d_name);
		  filenamecpy=strdup(dir->d_name);
		  csvcheck = &filenamecpy[filenamelen-4];//pushes the pointer to the last 4 if it is a csv this will hold .csv
		  if(strcmp(csvcheck,".csv")==0)
		    {
		      //space here for fork to call merge procedure
		      //after the fork to call the procedure 
		      //needs to break from theis
		      keeptrack++;
		 	temppidstore=fork();
		    	if(temppidstore== 0)
				{
					strcat(outputname,"/");
					filenamecpy=strndup(filenamecpy,(filenamelen-4));				
					strcat(outputname,filenamecpy);
					strcat(outputname,"-sorted-");
					strcat(outputname,argv[2]);
					strcat(outputname,".csv");
					outputfile = fopen(outputname,"w+");//creates a file in the output folder with same name but-sorted-(columnname).csv added
					return 0;

				}
				if(keeptrack == 1)
					{
					 printf(" %d",temppidstore);	
					}
				else
					{
					 printf(",%d",temppidstore);	
					}
				fflush(stdout);// for formating the child pids
				
		    }
		}
	      if(opendir(dir->d_name)!=NULL)//means dir->d_name is the name of a directory
		  {
			keeptrack++;
		    temppidstore=fork();
		    if(temppidstore==0)
		      {
			
			d = opendir(dir->d_name);//opens to one directory lower 
			chdir(dir->d_name);//changes current acting directory to the new one so the program can go lower after
		      }
		     else
			{
				if(keeptrack == 1)
					{
					 printf(" %d",temppidstore);	
					}
				else
					{
					 printf(",%d",temppidstore);	
					}
					fflush(stdout);
			}
				    
		  }
	    }
	  
	}
      closedir(d);
    }
return 0;
}
