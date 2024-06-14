#include "headers.h"

int iman(char *command)
{
    struct hostent *he;
    he = gethostbyname("man.he.net");
    if (he == NULL)
    {
        perror("gethostbyname");
        exit(EXIT_FAILURE);
    }
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80); // HTTP port
    server_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(server_addr.sin_zero), '\0', 8);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    char request[1024];
    snprintf(request, sizeof(request), "GET /?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n", command);
    if (send(sockfd, request, strlen(request), 0) == -1)
    {
        perror("send");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    char buffer[1024];
    int bytes_read;
    char *response_buffer = NULL;
    size_t buffer_size = 0;
    size_t total_bytes_received = 0;

    while ((bytes_read = recv(sockfd, buffer, sizeof(buffer), 0)) > 0)
    {
        response_buffer = realloc(response_buffer, total_bytes_received + bytes_read);

        if (response_buffer == NULL)
        {
            perror("realloc");
            exit(EXIT_FAILURE);
        }

        memcpy(response_buffer + total_bytes_received, buffer, bytes_read);

        total_bytes_received += bytes_read;
    }
    // printf("%s",response_buffer);
    // return 1;
    char *tag = "<h2";
    char *check = strstr(response_buffer, tag);
    // printf("%s",search);
    if(check!=NULL)
    {
        printf("Invalid Command! \n");
        return 1;
    }
    

    char replaced[1024];
    strcpy(replaced, "<STRONG>");
    char *findtag = "<STRONG>";
    char *endtag = "</STRONG>";
    char *search = strstr(response_buffer, findtag);
    if (findtag)
    {
        search += strlen(findtag);
        char *end = strstr(search, endtag);
        if (end)
        {
            size_t length = end - search;
            char title[length + 1];
            if (title)
            {
                strncpy(title, search, length);
                title[length] = '\0';

                strcat(replaced, title);
            }
        }
    }
    strcat(replaced, "</STRONG>");

    char *pos = strstr(response_buffer, replaced);
    while ((pos = strstr(pos, replaced)) != NULL)
    {
        size_t find_len = strlen(replaced);
        size_t replace_len = strlen("");
        size_t new_len = strlen(response_buffer) - find_len + replace_len;
        char temp[new_len + 1];
        memset(temp, 0, sizeof(temp));

        strncpy(temp, response_buffer, pos - response_buffer);

        strcat(temp, "");

        strcat(temp, pos + find_len);

        strcpy(response_buffer, temp);
        pos += replace_len;
    }

    char *start_tag = "<PRE>";
    char *end_tag = "</PRE>";
    char *start = strstr(response_buffer, start_tag);
    if (start)
    {
        start += strlen(start_tag);
        char *end = strstr(start, end_tag);
        if (end)
        {
            size_t length = end - start;
            char title[length+1];
            if (title)
            {
                strncpy(title, start, length);
                title[length] = '\0';

                printf("%s", title);

            }
        }
    }
    printf("\n");
    free(response_buffer);
    close(sockfd);
}