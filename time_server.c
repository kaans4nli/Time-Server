#include <stdio.h>
#include <string.h> // for strlen
#include <sys/socket.h>
#include <arpa/inet.h> // for inet_addr
#include <unistd.h>    // for write
#include <time.h>      // for date command

#define PORT_NUMBER 60000

int main(int argc, char *argv[])
{
    int socket_desc, new_socket, c, valread;
    struct sockaddr_in server, client;
    char *message;
    char buffer[1024] = {0};

    time_t rawtime;
    struct tm *timeinfo;
    // Used to store the time
    // returned by localetime() function
    char time_info[80];
    char date_info[80];
    char time_date_info[80];
    char time_zone_info[80];
    char day_of_week_info[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_info, 80,
             "Time is %I:%M:%S\n",
             timeinfo);
    strftime(date_info, 80,
             "Date is %d.%m.%Y\n",
             timeinfo);
    strftime(time_date_info, 80,
             "%I:%M:%S, %d.%m.%Y\n",
             timeinfo);
    strftime(time_zone_info, 80,
             "%Z\n",
             timeinfo);
    strftime(day_of_week_info, 80,
             "Day is %A\n",
             timeinfo);
    // strftime() function stores the
    // current time as Hours : Minutes
    // %I %M and %p-> format specifier
    // of Hours minutes and am/pm respectively*/
    // prints the formatted time
    // puts(time_info);

    int aa, b, cc, d, e, f, g;
    char str2[] = "GET_TIME";
    char str3[] = "GET_DATE";
    char str4[] = "GET_TIME_DATE";
    char str5[] = "GET_TIME_ZONE";
    char str6[] = "GET_DAY_OF_WEEK";
    char str7[] = "CLOSE_SERVER";

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        puts("Could not create socket");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT_NUMBER);

    // Bind
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        puts("Bind failed");
        return 1;
    }
    puts("Socket is binded");

    // Listen
    listen(socket_desc, 3);

    // Accept and incoming connection
    puts("Waiting for incoming connections...");

    aa = 0;
    c = sizeof(struct sockaddr_in);
    while (new_socket =
               accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c))
    {
        puts("Connection accepted");

        // write message to client
        message = "Hello client, I received your connection.\n\n";
        write(new_socket, message, strlen(message));

        while (aa == 0)
        {
            // read to client
            valread = read(new_socket, buffer, 1024);

            // compare to writing string in client
            b = strncmp(buffer, str2, 8);
            cc = strncmp(buffer, str3, 8);
            d = strncmp(buffer, str4, 13);
            e = strncmp(buffer, str5, 13);
            f = strncmp(buffer, str6, 15);
            g = strncmp(buffer, str7, 12);

            if (d == 0)
            {
                send(new_socket, time_date_info, strlen(time_date_info), 0);
            }
            else if (cc == 0)
            {
                send(new_socket, date_info, strlen(date_info), 0);
            }
            else if (e == 0)
            {
                send(new_socket, time_zone_info, strlen(time_zone_info), 0);
            }
            else if (b == 0)
            {
                send(new_socket, time_info, strlen(time_info), 0);
            }
            else if (f == 0)
            {
                send(new_socket, day_of_week_info, strlen(day_of_week_info), 0);
            }
            else if (g == 0)
            {
                send(new_socket, "GOOD BYE\n", strlen("GOOD BYE\n"), 0);
                aa = 1;
                return 0;
            }
            else
            {
                send(new_socket, "INCORRECT REQUEST\n", strlen("INCORRECT REQUEST\n"), 0);
            }
        }
    }

    return 0;
}

// https://www.geeksforgeeks.org/socket-programming-cc/
// https://stackoverflow.com/questions/8222495/how-to-compare-strings-in-an-if-statement
// https://www.programiz.com/c-programming/library-function/string.h/strcmp
// https://www.ibm.com/docs/en/zos/2.4.0?topic=functions-strftime-convert-formatted-time
// https://www.geeksforgeeks.org/time-h-header-file-in-c-with-examples/