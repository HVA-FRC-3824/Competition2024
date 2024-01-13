#ifndef THE_EYE_H
#define THE_EYE_H
#include "../Constants.h"
#include <frc2/command/SubsystemBase.h>
#include <pthread.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <iostream>

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

class TheEye : frc2::SubsystemBase
{
    /* FUCK AHHHHHHHHHHHHHHHHH GODDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD HAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA */
    public:
        TheEye();
        struct aprildata current_tag;
        void Periodic() override;
    private:
        void print_data();     /* Debug function to print data               */
        void fill_in_data();   /* Fills in data as it comes into current_tag */
        void launch_server();  /* Launches threads to receive the data       */
        pthread_t *thread; int thread_ret;
};


#endif