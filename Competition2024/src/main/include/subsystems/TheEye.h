#ifndef THE_EYE_H
#define THE_EYE_H
#include "../Constants.h"
#include <pthread.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h>
#include <time.h>

/* No need for the hexa just looks coola */
#define ACTIVE   0x01
#define INACTIVE 0xff
#define TRACKING 0x0f 
#define IGNORE   0x00

struct aprildata
{
		uint32_t   camera_id;   /* ID of Camera                      */
		float      dist;        /* Distance from tag                 */
		uint32_t   area;        /* Area in pixels of tag             */
		uint32_t   id;          /* ID of the tag                     */
		float      angle;       /* Angle from the camera (WIP)       */
		double     center_x;    /* Center X tag in image             */
		double     center_y;    /* Center Y tag in image             */
};

struct TheEye_Frame
{
	struct aprildata tag;
	uint8_t tag_status;
	clock_t time_since; /* Time since last detection */
};

struct TheEye
{
	struct TheEye_Frame TheEyes_Subjects[MAX_TAGS];
};

void *server_loop(void *parm);
void *processor(void *parm); /* Note: This handles logic for last detected tag */
void print_data(struct aprildata current_tag); /* Debug function to print data                 */
void fill_in_data();                           /* Handles incoming data to TheEye and sorts it */
void launch_server(struct aprildata *tag);     /* Launches threads to receive the data         */
void open_TheEye(struct TheEye Eye, uint8_t *block_list, uint8_t blocked_tags); /* PLEASE DON'T BLOCK ME!!!! */

#endif