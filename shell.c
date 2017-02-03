# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <sys/utsname.h>
#include <sys/wait.h>
#include <stdlib.h>
# include <fcntl.h> // for open
struct utsname unameData;

int parseInput(char input[]);
int cd();
int showUSR();
void  INThandler(int sig)
{
     char  c;

     signal(sig, SIG_IGN);
     printf("\nDo you really want to quit? [y/n] ");
     c = getchar();
     if (c == 'y' || c == 'Y')
          exit(0); // remove this if you dont want ctrl+c to quit and only want the quit command to quit
     else
          signal(SIGINT, INThandler);
     getchar(); // Get new line character
}
int main()
{
  signal(SIGINT, INThandler);
  showUSR();
  return 0;
}
int extractSemi(char input[])
{
  // printf("The whole string is %s\n",input); This code didn't work, trying alternate method
  // char *keep_track = strtok(input,";");
  // parseInput(keep_track);
  // while(keep_track!=NULL)
  // {
  //   keep_track= strtok(NULL,";");
  //   printf("The rest is keep_track is %s\n",keep_track);
  //   if(keep_track!=NULL)
  //   {
  //   parseInput(keep_track);
  // }
  // }
  int i = 0;
  char **tokarr;
  tokarr = malloc(sizeof(char*)*1024);
  char *word = strtok(input,";");
  while(word!=NULL)
  {
    tokarr[i] = word;
    ++i;
    word = strtok(NULL,";");
  }
  int k=0,tkn=0;

  for(k=0;k<i;k++)
  {
    //printf("This time k is %d and (i-1) is %d\n",k,(i-1));
  parseInput(tokarr[k]); // add showUSR(); after this if required
}
showUSR();
  return 0;
}
int showUSR()
{
  uname(&unameData);
  char dir[100],user[100];
  getcwd(dir,100);
  strcpy(dir,strrchr(dir,'/'));
  strcpy(user,getlogin());
  printf("%s@%s:%s$ ",user,unameData.sysname,dir+1);
  char input[200];
  gets(input);
  extractSemi(input);
  return 0;
}
int echo(char* parse)
{
  while(parse!=NULL)
  {
    parse = strtok(NULL, " `");
    if(parse!=NULL)
    printf("%s ",parse);
  }
  //showUSR();
  return 0;
}
int cd(char *parse)
{
  chdir(parse);
//  showUSR();
  return 0;
}
int pwd()
{
  char loccur[100];
  getcwd(loccur,100);
  printf("%s\n",loccur);
  //showUSR();
  return 0;
}
int otherFunc(char input[])
{
  pid_t pid, wpid;
  int status;
  char *command = strtok(input," ");
  char *command1 = command;
  char *comtemp;
  char *args[100];
  args[0]=command1;
  int keep = 1;
  int flag_keep = 0;
  int flagnew1=0;
  char tempcopy[100];
  while(command!=NULL)
  {
    command = strtok(NULL," ");
    if (command!=NULL)
    {
    if(strcmp(command,"&")==0)
    {
      flag_keep = 1;
      args[keep]=NULL;
      ++keep;
    }
    else if(strcmp(command,">")==0)
    {
      flagnew1=1;
      args[keep]=NULL;
      ++keep;
      command = strtok(NULL," ");
    //  printf("Command rn is %s\n",command);
      strcpy(tempcopy,command);
    }
    else if(strcmp(command,"<")==0)
    {
      flagnew1=2;
      args[keep]=NULL;
      ++keep;
      command = strtok(NULL," ");
    //  printf("Command rn is %s\n",command);
      strcpy(tempcopy,command);
    }
    else if(strcmp(command,">>")==0)
    {
      flagnew1=3;
      args[keep]=NULL;
      ++keep;
      command = strtok(NULL," ");
    //  printf("Command rn is %s\n",command);
      strcpy(tempcopy,command);
    }
    else{
    args[keep] = command;
    ++keep;
  }
}
else
{
  args[keep] = command;
  ++keep;
}
  }
//printf("Command rn is %s\n",tempcopy);
  pid = fork();

  if(pid==0)
  {
    int current_out=0;
    if(flagnew1==1)
    {
      int filed;
      //printf("Command rn is %s\n",tempcopy);
      command = strtok(NULL," ");
      filed = creat(tempcopy,0644);
      dup2(filed, STDOUT_FILENO);
      current_out = dup(1);
      close(filed);
    }
    if(flagnew1==2)
    {
      int filed;
      //printf("Command rn is %s\n",tempcopy);
      command = strtok(NULL," ");
      filed = open(tempcopy, O_RDONLY, 0);
      dup2(filed, STDIN_FILENO);
      current_out = dup(0);
      close(filed);
    }
    if(flagnew1==3)
    {
      int filed;
      //printf("Command rn is %s\n",tempcopy);
      command = strtok(NULL," ");
      filed = open(tempcopy, O_RDWR | O_APPEND, 0777);
      dup2(filed, STDOUT_FILENO);
      current_out = dup(1);
      close(filed);
    }
    if(execvp(args[0],args)==-1){
      printf("Error 1.\n");
    }
    exit(EXIT_FAILURE);
  }
  else if(pid<0)
  {
    printf("Error in forking.\n");
  }
  else{
    if(flag_keep!=1)
    {
    //do {
     waitpid(pid, &status, WUNTRACED);
  //  }while (!WIFEXITED(status) && !WIFSIGNALED(status));
     if(pid<0)
     printf("Error in bg.\n");
  }
}
  return 0;
}
int parseInput(char input[])
{
  char tempInput[100];
  strcpy(tempInput,input);
  char *parse = strtok(input," ");
  if(strcmp(parse,"cd")==0)
  {
    parse = strtok(NULL, " ");
    cd(parse);
  }
  if(strcmp(parse,"quit")==0)
  exit(0);
  if(strcmp(parse,"pwd")==0)
  {
    pwd();
  }
  if(strcmp(parse,"echo")==0)
  {
    echo(parse);
  }
  else
  {
    otherFunc(tempInput);
  }

  return 0;
}
