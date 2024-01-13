#include "../../include/subsystems/TheEye.hpp"

/* Note on this reference: HORRIBLE WAY TO DO THIS! NO SIZE SAFETY! ICKY ICKY!! */
void *april_reference;

void *server_loop(void *parm)
{
    int sockfd; struct sockaddr_in cliaddr;

    /* Bind the socket to the port for further questioning */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){std::cout << "The Eye cannot open socket... AHHHHHHHHHHH!!!!!!!\n";}
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(THE_EYE_PORT);
    cliaddr.sin_addr.s_addr = INADDR_ANY;


    while(1)
    {
        recvfrom(sockfd,april_reference,sizeof(struct aprildata),MSG_WAITALL,NULL,0);
    }
}

void TheEye::launch_server()
{
    april_reference = (void *)&current_tag;
    thread_ret = pthread_create(thread,NULL,server_loop,NULL);
}

void TheEye::print_data()
{
    std::cout << "APRIL DATA FRAME: ";
    std::cout << "\nID: " << this->current_tag.id;
    std::cout << "\nAREA: " << this->current_tag.area;
    std::cout << "\nX: " << this->current_tag.center_x;
    std::cout << "\nY: " << this->current_tag.center_y;
}

TheEye::TheEye()
{
    print_data();
}