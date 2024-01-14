#include "../../include/subsystems/TheEye.h"

/* Note on this reference: HORRIBLE WAY TO DO THIS! NO SIZE SAFETY! ICKY ICKY!! */
void *april_reference;


void *server_loop(void *parm)
{
    int sockfd; struct sockaddr_in cliaddr;

    /* Bind the socket to the port for further questioning */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){printf("The Eye cannot open socket... AHHHHHHHHHHH!!!!!!!\n");}
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(THE_EYE_PORT);
    cliaddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
    while(1)
    {
        recvfrom(sockfd,april_reference,sizeof(struct aprildata),0,NULL,0);
        //printf("RECEIVED\n");
    }
}

void launch_server(struct aprildata *current_tag)
{
    pthread_t thread; int thread_ret;
    april_reference = (void *)current_tag;
    thread_ret = pthread_create(&thread,NULL,server_loop,NULL);
}

void print_data(struct aprildata current_tag)
{
    printf("\nAPRIL DATA FRAME: ");
    printf("\nID: %i",current_tag.id);
    printf("\nAREA: %i",current_tag.area);
    printf("\nX: %f",current_tag.center_x);
    printf("\nY: %f",current_tag.center_y);
}