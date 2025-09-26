//---------------------------------------------------------
// Assignment : Lab-04_a IPC_Pipes
// Date : 9/25/25
//
// Author : Lab-04_a-Pipes_Team02
//
// File Name : go.c
//---------------------------------------------------------
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

// Wrapper fork() method
pid_t Fork(void)
{
   pid_t n = fork();
   if (n < 0)
   {
      perror("Fork failed");
      exit(-1);
   }
   return n;
}

#define READ_END 0
#define WRITE_END 1
#define STDIN_FD 0
#define STDOUT_FD 1

int main(void)
{
   int reqPipe[2], replyPipe[2];
   pid_t user_pid, calc_pid;

   // Make the pipes
   if (pipe(reqPipe) == -1)
   {
      fprintf(stderr, "Request pipe failed");
      return 1;
   }

   if (pipe(replyPipe) == -1)
   {
      fprintf(stderr, "Relpy pipe failed");
      return 1;
   }

   // Initial prints
   printf("\nGo started and created these pipes\n");
   printf("    User-to-Calculator Request pipe: read=%d  write=%d\n", reqPipe[READ_END], reqPipe[WRITE_END]);
   printf("    Calculator-to-User Reply pipe: read=%d  write=%d\n", replyPipe[READ_END], replyPipe[WRITE_END]);

   // Forking user side
   user_pid = Fork();
   if (user_pid == 0)
   {
      // user child processes
      close(reqPipe[READ_END]);
      close(replyPipe[WRITE_END]);

      // Prep command line args
      char req_write[10], reply_read[10];
      sprintf(req_write, "%d", reqPipe[WRITE_END]);
      sprintf(reply_read, "%d", replyPipe[READ_END]);

      execlp("./user", "user", req_write, reply_read, NULL);
      perror("execlp user failed");
      exit(1);
   }

   // Forking calculator side
   calc_pid = Fork();
   if (calc_pid == 0)
   {
      // calc child processes
      close(reqPipe[WRITE_END]);
      close(replyPipe[READ_END]);

      // Prep command line args
      char req_read[10], reply_write[10];
      sprintf(req_read, "%d", reqPipe[READ_END]);
      sprintf(reply_write, "%d", replyPipe[WRITE_END]);

      execlp("./calculator", "calculator", req_read, reply_write, NULL);
      perror("execlp calculator failed");
      exit(1);
   }

   close(reqPipe[READ_END]);
   close(reqPipe[WRITE_END]);
   close(replyPipe[READ_END]);
   close(replyPipe[WRITE_END]);

   // Wait for user to finish
   printf("\nGo is now waiting for User to terminate\n");
   waitpid(user_pid, NULL, 0);

   // Wait for calculator to finish
   printf("Go is now waiting for Calculator to terminate\n");
   waitpid(calc_pid, NULL, 0);

   printf("\nThe Go process has terminated\n");

   return 0;
}
