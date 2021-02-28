#pragma once

#include "ros/ros.h"

#include <vortex_msgs/Pwm.h>
#include <vortex_msgs/ThrusterForces.h>
#include <vortex_msgs/ThrusterArming.h>
#include <vortex_msgs/ThrustersWrite.h>
#include <std_msgs/String.h>

#include <string>
#include <cmath>
#include <vector>

#include "ATMEGA2560_ros_driver/i2c.h"
#include "ATMEGA2560_ros_driver/interp.h"

#define MESSAGE_START 30
#define MESSAGE_END 40

class MCU_Interface{
    public:
        MCU_Interface();

        /** Calculate PWM from thruster forces and write to MCU*/

        void execute();
    private:
        I2CDevice device;

        ros::NodeHandle nh;
        ros::Subscriber thruster_forces_sub;
        ros::Subscriber thruster_arm_sub;
        ros::Rate loop_rate;

        int num_thrusters;
        std::vector<double> lookup_thrust;
        std::vector<double> lookup_pulse_width;

        const int thrust_range_limit = 100;

        /** Callbacks */
        void thruster_forces_cb(const vortex_msgs::ThrusterForces &msg);
        void thruster_arm_cb(const std_msgs::String &msg);

        /** Utility */
        bool is_healthy(const vortex_msgs::ThrusterForces &msg);

        double thrust_to_microseconds(const double thrust);
        void output_to_zero();

        void i2c_init(int MCU_addr);
        void transfer_to_mcu(const std::vector<int> pwm);
        void transfer_to_mcu(u_int8_t a_byte);
        void recv_from_mcu();


        /** Arming service */
        ros::ServiceServer arming_service;
        bool arming_service_cb(vortex_msgs::ThrusterArming::Request &req, vortex_msgs::ThrusterArming::Response &res);

        /** Thruster write service */
        ros::ServiceServer thrusters_write_service;
        bool thrusters_write_service_cb(vortex_msgs::ThrustersWrite::Request &req, vortex_msgs::ThrustersWrite::Response &res);

};