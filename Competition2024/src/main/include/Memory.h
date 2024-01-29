#ifndef CUSTOM_MEMORY_H
#define CUSTOM_MEMORY_H

/* File to define structs to share memory throughout whole program. DO NOT TOUCH! */

/* Swerve and turret heading, used for alignment */
struct angle_shares
{
    float swerve_heading; /* Calculated through yaw on NavX, potentially through encoders on drive however that isn't accurate */
    float turret_heading; /* Turret motor encoders */
};

typedef angle_shares angle_mem_share;


#endif