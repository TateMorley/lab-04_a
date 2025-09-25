#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
   if (argc != 3)
   {
      fprintf(stderr, "Usage: user <req_write_fd> <reply_read_fd>\n");
      exit(1);
   }
   
   // Formats args into ints
   int req_write_fd = atoi(argv[1]);
   int reply_read_fd = atoi(argv[2]); 
   
   printf("This is the User process (id = %d).\n", getpid());
   printf("    User: Please, enter:  value1  operation  value2\n");
   
   int num1, num2;
   char op;
   char buffer[100];
   int result;
   
   while (1)
   {
      // Read input from user
      if (scanf("%d %c %d", &num1, &op, &num2) != 3)
      {
         // Stops if scanning goes wrong or stops
         break;
      }
      
      // Format the request
      sprintf(buffer, "%d %c %d", num1, op, num2);
      
      printf("    User Sending the following: %s on FD %d\n", buffer, req_write_fd);
      
      // Send request to Calculator
      write(req_write_fd, buffer, strlen(buffer) + 1);
      
      // Read result from Calculator
      read(reply_read_fd, &result, sizeof(int));
      
      printf("User Received the following result: %d from FD %d\n\n", result, reply_read_fd);
   }
   
   close(req_write_fd);
   close(reply_read_fd);
   
   return 0;
}