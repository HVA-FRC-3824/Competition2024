#include "../../include/subsystems/TheEye.h"
#include <unistd.h>

struct TheEye *eye_reference;
struct aprildata current_tag;

/* Note: none of this works on WPILib's scheduler, nor their custom periodic functions 
   custom timing will be implemented, however it WILL NOT follow the standard 20ms per cycle timing
   however this does not rate limit packet receiving by 20ms however it will be used as standard for timeout
   DO NOT use ANYTHING IN HERE as standard periodic timing... */

/* server_loop ONLY RECEIVES DATA! This thread should BLOCK UNTIL DATA IS RECEIVED
   This function ONLY sets packet and time, IT DOES NOT DO STATUS! IF A TAG'S STATUS IS IGNORE PACKET IS DROPPED!
*/
void *server_loop(void *parm)
{
    int sockfd; struct sockaddr_in cliaddr;
    printf("server started\n");
    /* Bind the socket to the port for further questioning */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){printf("The Eye cannot open socket... AHHHHHHHHHHH!!!!!!!\n");}
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(THE_EYE_PORT);
    cliaddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
    /* WARNING: this MAY crash watchdog (I HATE WPILIB!!! WATCHDOGS FOR LOSERS ) */
    while(1)
    {
        recvfrom(sockfd,&current_tag,sizeof(struct aprildata),0,NULL,0);
        /* If not filtered, save time and make active */
        if(eye_reference->TheEyes_Subjects[current_tag.id-1].tag_status)
        {
            eye_reference->TheEyes_Subjects[current_tag.id-1].tag_status = ACTIVE;
            eye_reference->TheEyes_Subjects[current_tag.id-1].time_since = clock();
        }
    }
}

/* REMINDER: check camera for ID bound checking!!! BIG DEAL!! */
void *processor(void *parm)
{
    printf("processor started\n");
    int i;
    clock_t current_time = clock(); /* Avoid recalling clock() every loop, delay between if statements neglible */
    while(1)
    {
        usleep(20 * 1000); /* stupid watchdog restrictions */
        for(i = 0; i < MAX_TAGS; i++)
        {
            /* Check for ignore status, 0x00 */
            if(eye_reference->TheEyes_Subjects[i].tag_status)
            {
                /* ME PERSONALLY i don't CARE about the fact that ARM PROCESSORS CAN'T DIVIDE EFFICIENTLY!! SUCK IT!! */
                if(((current_time - eye_reference->TheEyes_Subjects[i].time_since)/CLOCKS_PER_SEC) > THE_EYE_TIMEOUT)
                {
                    eye_reference->TheEyes_Subjects[i].tag_status = INACTIVE;
                }
            }
        }
    }
}

void launch_server(struct TheEye Eye_Struct)
{
    pthread_t thread; int thread_ret;
    eye_reference = &Eye_Struct;
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

void open_TheEye(struct TheEye Eye, uint8_t *block_list, uint8_t blocked_tags)
{
    /* Basic initialization */
    /* Set all tags inactive then selectively disable ones we don't need / use */
    for(int i = 0; i < MAX_TAGS; i++) { Eye.TheEyes_Subjects[i].tag_status = INACTIVE;}
    for(int i = 0; i < blocked_tags; i++)
    {
        Eye.TheEyes_Subjects[block_list[i]-1].tag_status = IGNORE;
    }

    launch_server(Eye);
    pthread_t thread; int thread_ret;
    thread_ret = pthread_create(&thread,NULL,processor,NULL);
}