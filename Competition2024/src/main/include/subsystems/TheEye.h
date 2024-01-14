#ifndef THE_EYE_H
#define THE_EYE_H
#include "../Constants.h"
#include <pthread.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h>

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

void *server_loop(void *parm);
void print_data(struct aprildata current_tag); /* Debug function to print data               */
void fill_in_data();                           /* Fills in data as it comes into current_tag */
void launch_server(struct aprildata *tag);     /* Launches threads to receive the data       */

#endif