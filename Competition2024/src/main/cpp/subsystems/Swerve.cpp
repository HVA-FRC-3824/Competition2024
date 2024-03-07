#include "../../include/subsystems/Swerve.hpp"
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include <math.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <string>

void Swerve::clear_swerve_memory()
{
    for(int i = 0; i < 4; i++)
    {
        this->math_dest.wheel_angle[i] = 0;
        this->math_dest.wheel_speeds[i] = 0;
        this->raw_usable[i] = 0;
        this->last_units[i] = 0; 
    }
}

Swerve::Swerve(float length, float width)
{
    this->chassis_info.length = length;
    this->chassis_info.width = width;

    clear_swerve_memory();

    for(int i = 0; i < 4; i++)
    {
        /* Set current limit */
        this->DRIVE_MOTORS[i]->SetSmartCurrentLimit(SWERVE_MAX_AMPERAGE);
        this->ANGLE_MOTORS[i]->SetSmartCurrentLimit(SWERVE_MAX_AMPERAGE);

        /* Turn on brake coast mode, snappier */
        this->DRIVE_MOTORS[i]->SetIdleMode(CANSparkMax::IdleMode::kBrake);
        this->ANGLE_MOTORS[i]->SetIdleMode(CANSparkMax::IdleMode::kBrake);

        /* PIDs */
        this->PID_CONTROLLERS[i] = new SparkMaxPIDController(this->ANGLE_MOTORS[i]->GetPIDController());
        this->PID_CONTROLLERS[i]->SetP(SWERVE_P);
        this->PID_CONTROLLERS[i]->SetI(SWERVE_I);
        this->PID_CONTROLLERS[i]->SetD(SWERVE_D);

        /* Get real relative encoders */
        this->ANGLE_ENCODERS[i] = new SparkRelativeEncoder(this->ANGLE_MOTORS[i]->GetEncoder());
        this->ABS_ENCODERS[i].ConfigAbsoluteSensorRange(ctre::phoenix::sensors::AbsoluteSensorRange::Signed_PlusMinus180);

        /* Burn flash everytime */
        this->DRIVE_MOTORS[i]->BurnFlash();
        this->ANGLE_MOTORS[i]->BurnFlash();
    }
};

void Swerve::calculate_wheel_information(wheel_info *dest, struct size_constants cons, float fwd, float str, float rotate)
{
    float forward = fwd;
	float strafe = str;
    float gyro = navx.GetYaw();

    frc::SmartDashboard::PutNumber("Gyro Input: ", gyro);

	/* If field centric take in account the gyro */
    float temp = (fwd * cosf(gyro * ANTIMAGIC_NUMBER)) + (str * sinf(gyro * ANTIMAGIC_NUMBER));
    strafe = -(fwd * sinf(gyro * ANTIMAGIC_NUMBER)) + (str * cosf(gyro * ANTIMAGIC_NUMBER));
	forward = temp;

	float R = sqrtf((cons.length * cons.length)+(cons.width * cons.width));

	float A, B, C, D;
	A = strafe - rotate * (cons.length/R);
	B = strafe + rotate * (cons.length/R);
	C = forward - rotate * (cons.width/R);
	D = forward + rotate * (cons.width/R);

	/* Fill out our dest struct */
	
	float a_sqrd, b_sqrd, c_sqrd, d_sqrd;
	a_sqrd = A*A;
	b_sqrd = B*B;
	c_sqrd = C*C;
	d_sqrd = D*D;

	//float wheel_speeds[4];

	dest->wheel_speeds[0] = sqrtf((b_sqrd) + (c_sqrd));
	dest->wheel_speeds[1] = sqrtf((b_sqrd) + (d_sqrd));
	dest->wheel_speeds[2] = sqrtf((a_sqrd) + (d_sqrd));
	dest->wheel_speeds[3] = sqrtf((a_sqrd) + (c_sqrd));

	float max = dest->wheel_speeds[0];

	/* Do normalisation on the wheel speeds, so that they are 0.0 <-> +1.0 */
	int i;
	for(i = 1; i < 3; i++)
	{
		if(dest->wheel_speeds[i] > max) { max = dest->wheel_speeds[i]; }
	}

	if(max > 1)
	{
		for(i = 0; i < 4; i++) { dest->wheel_speeds[i] = dest->wheel_speeds[i] / max; }
	}

	dest->wheel_angle[0] = atan2f(B,C) * MAGIC_NUMBER;
	dest->wheel_angle[1] = atan2f(B,D) * MAGIC_NUMBER;
	dest->wheel_angle[2] = atan2f(A,D) * MAGIC_NUMBER;
	dest->wheel_angle[3] = atan2f(A,C) * MAGIC_NUMBER;


    /* Instead of flippig 180 just reverse our speeds, this might be a problem and needs testing */
    for(i = 0; i < 4; i++)
    {
        if(dest->wheel_angle[i] == 180){dest->wheel_speeds[i] = dest->wheel_speeds[i] * -1; continue;}
        if(dest->wheel_angle[i] < -90)
        {
            dest->wheel_angle[i] += 180;
            dest->wheel_speeds[i] = dest->wheel_speeds[i] * -1;
        }
        if(dest->wheel_angle[i] > 90)
        {
            dest->wheel_angle[i] -= 180;
            dest->wheel_speeds[i] = dest->wheel_speeds[i] * -1;
        }
    } 
	return;
}

void Swerve::snap_wheels_to_abs()
{
    for(int i = 0; i < 4; i++)
    {
        /* Snap to abs then set to zero */
        //this->PID_CONTROLLERS[i]->SetReference(-(this->ABS_ENCODERS[i].GetAbsolutePosition()/360)*SWERVE_WHEEL_COUNTS_PER_REVOLUTION,CANSparkMax::ControlType::kPosition);
        this->ANGLE_ENCODERS[i]->SetPosition((this->ABS_ENCODERS[i].GetAbsolutePosition()/360)*SWERVE_WHEEL_COUNTS_PER_REVOLUTION);
    }
}

void Swerve::deadzone_correction(float *x, float *y, float *x2)
{
    /* Pass in the values and they are corrected, PRAYING TO GOD that it doesn't seg fault or something :D .*/
    bool y_deadzone = false;
    bool x_deadzone = false;
    bool y_move_abs = false;

    /* Ignore our deadzone and fix the moving forward issue */
    if(*y < DEADZONE_THRES && *y > -DEADZONE_THRES)
    {
        *y = 0;
        y_deadzone = true;
    }   
    if(*x < DEADZONE_THRES && *x > -DEADZONE_THRES)
    {
        *x = 0;
        x_deadzone = true;
        if(!y_deadzone)
        {
            /* Sets so that the true forward value is used incase of no strafing. Horrible hack but it works. */
            y_move_abs = true;
        }
    } 
    if(*x2 < DEADZONE_THRES && *x2 > -DEADZONE_THRES)
    {
        *x2 = 0;
    }

    int i;

    /* Allows for forward movement even when X is in deadzone */
    if(y_move_abs)
    {
        for(i = 0; i < 4; i++)
        {
            this->math_dest.wheel_speeds[i] = abs(*y);
        }
    }

    /* IF DEADZONE IS REACHED WE USED LAST KNOWN VALUE, WE SHOULD NEVER RESET ANGLE (according to dr hart) */

    /* Allows for sideways movement even when Y is in deadzone */
    if(y_deadzone)
    {
        for(i = 0; i < 4; i++)
        {
            this->math_dest.wheel_speeds[i] = abs(*x);
        }
    }

    /* Re Use old units instead of spinning again */
    if(y_deadzone && x_deadzone){
        this->use_old = true;
    }

    y_deadzone = false;
    x_deadzone = false;
    y_move_abs = false;
}

void Swerve::print_swerve_math(wheel_info math)
{
    std::cout << "\n";
    for(int i = 0; i < 4; i++)
    {
        std::cout << math.wheel_speeds[i] << " SPEED " << i << "\n";
        std::cout << math.wheel_angle[i] << " ANGLE " << i << "\n";
    }
}

/* When field centric mode is disabled 'gyro' is ignored */

void Swerve::toggle_xwheels()
{
    this->x_wheels = !this->x_wheels;
    frc::SmartDashboard::PutBoolean("XWHEELS? ", this->x_wheels);
}

void Swerve::drive(float y, float x, float x2, float gyro)
{
    // lock wheels in x position
    if(this->x_wheels)
    {
        for(int i = 0; i < 4; i++)
        {
            this->DRIVE_MOTORS[i]->Set(0);
        }
        this->PID_CONTROLLERS[0]->SetReference(-7.875,CANSparkMax::ControlType::kPosition);
        this->PID_CONTROLLERS[1]->SetReference(7.875,CANSparkMax::ControlType::kPosition);
        this->PID_CONTROLLERS[2]->SetReference(2.625,CANSparkMax::ControlType::kPosition);
        this->PID_CONTROLLERS[3]->SetReference(-2.625,CANSparkMax::ControlType::kPosition);

        return;
    }

    /* Correct/Sanitize our inputs */
    deadzone_correction(&x, &y, &x2);

    /* Generate our math and store in dest struct to be converted / used */
    calculate_wheel_information(&this->math_dest,this->chassis_info,y,x,x2);

    /* Find the percent to max angle (180 or -180) and then multiple by the counts required to get to that required angle.       */
    /* Equivalent to x / SWERVE_WHEEL_COUNTS_PER_REVOLUTION = y / 360 where y is angle and x is raw sensor units for the encoder */
    for(int i = 0; i < 4; i++)
    { this->raw_usable[i] = ((this->math_dest.wheel_angle[i] / 360) * SWERVE_WHEEL_COUNTS_PER_REVOLUTION); }

    print_swerve_math(this->math_dest);

    /* Only run our motors once everything is calculated */
    for(int i = 0; i < 4; i++)
    {
        frc::SmartDashboard::PutNumber(fmt::v10::format("CANCODER_POS_{}: ", i), this->ABS_ENCODERS[i].GetAbsolutePosition());
        //frc::SmartDashboard::PutNumber(fmt::v9::format("SPARKMAX_POS_{}: ", i), this->ANGLE_ENCODERS[i]->GetPosition());
        this->DRIVE_MOTORS[i]->Set(this->math_dest.wheel_speeds[i]);

        if(use_old)
        {
            this->PID_CONTROLLERS[i]->SetReference(this->last_units[i],CANSparkMax::ControlType::kPosition);
            //std::cout << i << "Actual: " << this->ANGLE_ENCODERS[i]->GetPosition() << " Old_Desired: " << this->last_units[i] <<"\n";
        } else 
        {
            this->PID_CONTROLLERS[i]->SetReference(this->raw_usable[i],CANSparkMax::ControlType::kPosition);
            //std::cout << i << "Actual: " << this->ANGLE_ENCODERS[i]->GetPosition() << " Desired: " << this->raw_usable[i] <<"\n";
            this->last_units[i] = this->raw_usable[i];
        }

        /* Clear "sticky" values that are stuck in memory, if the robot is receiving input this doesn't matter anyways. */
        /* Only affects the robot when stopped!! */
        this->math_dest.wheel_speeds[i] = 0;
    }
    this->use_old = false;
}

bool Swerve::toggle_field_centricity()
{
    this->field_centered = !this->field_centered;
    return this->field_centered;
}