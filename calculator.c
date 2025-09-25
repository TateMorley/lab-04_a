#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage: calculator <req_read_fd> <reply_write_fd>\n");
    exit(1);
  }

  int req_read_fd = atoi(argv[1]);    // FD to read requests from User
  int reply_write_fd = atoi(argv[2]); // FD to write replies to User

  printf("This is the Calculator process (id = %d).\n", getpid());

  char buffer[100];
  int num1, num2;
  char op;
  int result;

  while (1)
  {
    printf("Calculator Waiting to receive from FD %d\n", req_read_fd);

    // Read request from User
    int bytes_read = read(req_read_fd, buffer, sizeof(buffer));
    if (bytes_read <= 0)
    {
      break; // Exit when pipe is closed
    }
    buffer[bytes_read] = '\0'; // Null-terminate the buffer

    // Parse the request
    int parsed = sscanf(buffer, "%d %c %d", &num1, &op, &num2);
    if (parsed != 3)
    {
      result = -1; // Error: invalid format
    }
    else
    {
      // Perform calculation
      switch (op)
      {
      case '+':
        result = num1 + num2;
        break;
      case '-':
        result = num1 - num2;
        break;
      case '*':
        result = num1 * num2;
        break;
      case '/':
        if (num2 != 0)
          result = num1 / num2;
        else
          result = -1; // Error: division by zero
        break;
      default:
        result = -1; // Error: invalid operation
      }
    }

    printf("    Calculator received the following: %s from FD %d\n", buffer, req_read_fd);

    printf("Calculator sent the following to the User: %d on FD %d\n", result, reply_write_fd);

    // Send result back to User
    write(reply_write_fd, &result, sizeof(int));
  }

  close(req_read_fd);
  close(reply_write_fd);

  return 0;
}