#include "ATMEGA2560_ros_driver/ATMEGA2560_ros_driver.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "ATMEGA2560_ros_driver/i2c.h"
#include "ATMEGA2560_ros_driver/interp.h"

#include <time.h>
#include <stdlib.h>

MCU_Interface::MCU_Interface ():loop_rate(10){
    thruster_forces_sub = nh.subscribe("/thrust/thruster_forces", 10, &MCU_Interface::thruster_forces_cb, this);
    thruster_arm_sub = nh.subscribe("/thrust/arm", 10, &MCU_Interface::thruster_arm_cb, this);
    arming_service = nh.advertiseService("arming_service", &MCU_Interface::arming_service_cb, this);
    thrusters_write_service = nh.advertiseService("thrusters_write_service", &MCU_Interface::thrusters_write_service_cb, this);

    if (!nh.getParam("/propulsion/thrusters/num", num_thrusters)) {
        ROS_WARN("Could not get parameter '/propulsion/thrusters/num', using default.");
        num_thrusters = 8;
    }

    std::vector<double> default_vec(num_thrusters, 0.0);

    if (!nh.getParam("/propulsion/thrusters/characteristics/thrust",  lookup_thrust)) {
        ROS_WARN("Could not get parameter '/propulsion/thrusters/characteristics/thrust', using default.");
        lookup_thrust = default_vec;
    }

    if (!nh.getParam("/propulsion/thrusters/characteristics/pulse_width", lookup_pulse_width)) {
        ROS_WARN("Could not get parameter '/propulsion/thrusters/characteristics/pulse_width', using default.");
        lookup_pulse_width = default_vec;    
    }

    
    i2c_init(0x8);

}


/** Callbacks */

void MCU_Interface::thruster_forces_cb(const vortex_msgs::ThrusterForces &msg) {

    if (!is_healthy(msg)) return;

    std::vector<double> thrust = msg.thrust;
    std::vector<int> microseconds;

    for(int i = 0; i < num_thrusters; i++) {
        microseconds.push_back(thrust_to_microseconds(thrust[i]));
    }

    transfer_to_mcu(microseconds);
}

void MCU_Interface::thruster_arm_cb(const std_msgs::String &msg) {
    if (msg.data == "arm me daddy") {
        ROS_WARN("ARMING THRUSTERS, WATCH YOUR FINGERS, TOES, AND ANY OTHER EXPOSED LIMBS");
        transfer_to_mcu(69);
    }

}

bool MCU_Interface::arming_service_cb(vortex_msgs::ThrusterArming::Request &req, vortex_msgs::ThrusterArming::Response &res){
    transfer_to_mcu(69);
    res.result = "Thrusters armed";
    return true;
}

bool MCU_Interface::thrusters_write_service_cb(vortex_msgs::ThrustersWrite::Request &req, vortex_msgs::ThrustersWrite::Response &res){   
   

    //if (!is_healthy(req)) return;

    std::vector<double> thrust = req.thruster_values;
    std::vector<int> microseconds;

    for(int i = 0; i < num_thrusters; i++) {
        microseconds.push_back(thrust_to_microseconds(thrust[i]));
    }

    transfer_to_mcu(microseconds);

    return true;

}


/** Utility*/

void MCU_Interface::output_to_zero() {
    double neutral_pulse_width = thrust_to_microseconds(0);
    std::vector<int> microseconds;

    for(int i = 0; i < num_thrusters; i++) {
        microseconds.push_back(neutral_pulse_width);
    }

    transfer_to_mcu(microseconds);

}

bool MCU_Interface::is_healthy(const vortex_msgs::ThrusterForces &msg) {
    std::vector<double> thrust = msg.thrust;

    if( thrust.size() != num_thrusters) {
        ROS_WARN("Size mismatch between thruster force message and defined number of thrusters, ignoring...");
        return false;
    }

    for(const float tau : thrust) {
        if(isnan(tau) || isinf(tau) || abs(tau) > thrust_range_limit) {
            ROS_WARN("Force element out of range, ignoring...");
            return false;
        }
    }

    return true;
}


double MCU_Interface::thrust_to_microseconds(const double thrust) {
    return interpolate(thrust, lookup_thrust, lookup_pulse_width);
}

void MCU_Interface::i2c_init(int MCU_addr) {
    int bus;
    //const char *data = "01110";

    /* First open i2c bus */
    if ((bus = i2c_open("/dev/i2c-8")) == -1) {

        perror("Open i2c bus error");
    }

    /* Fill i2c device struct */
    MCU_Interface::device.bus = bus;
    MCU_Interface::device.addr = MCU_addr;
    MCU_Interface::device.tenbit = 0;
    MCU_Interface::device.delay = 10;
    MCU_Interface::device.flags = 0;
    MCU_Interface::device.page_bytes = 10;
    MCU_Interface::device.iaddr_bytes = 0; /* Set this to zero, and using i2c_ioctl_xxxx API will ignore chip internal address */
}

void MCU_Interface::transfer_to_mcu(const std::vector<int> pwm) {
// To compile and run all files:
// cc -c ./i2c_test.c && cc ./i2c.o ./i2c_test.o -o main && ./main

    char num_str[num_thrusters+2];
    for (int i = 0; i < num_thrusters+2; i++) { // +2 to account for the start and stop byte 
        if (i == 0) {
            num_str[i] = 30;
            continue;
        }

        else if (i == (num_thrusters + 1)) {
            num_str[i] = 40;
            continue;
        }

        else {
            uint8_t tmp_int8 = 0.128 * pwm[i+1] - 1;
            num_str[i] = tmp_int8;
        }


    }

    ROS_INFO("Array being transferred to MCU: [%s]\n", num_str);

    if (i2c_ioctl_write(&device, 0x0, num_str, strlen(num_str)) != strlen(num_str))
    {
        /* Error process */
    }
}

void MCU_Interface::transfer_to_mcu(u_int8_t a_byte) {
    // To compile and run all files:
    // cc -c ./i2c_test.c && cc ./i2c.o ./i2c_test.o -o main && ./main
    //const I2CDevice dev_1 = MCU_Interface::device;
    
    char num_str[num_thrusters];
    for (int i = 0; i < num_thrusters; i++)
    {
        num_str[i] = a_byte;
    }

    if (i2c_ioctl_write(&device, 0x0, num_str, strlen(num_str)) != strlen(num_str))
    {
        /* Error process */
    }
}

void MCU_Interface::recv_from_mcu() {
    // To compile and run all files:
    // cc -c ./i2c_test.c && cc ./i2c.o ./i2c_test.o -o main && ./main

    char recv_array[8];

    if (i2c_ioctl_read(&device, 0x0, recv_array, strlen(recv_array)) != strlen(recv_array))
    {
        /* Error process */
    }
    ROS_WARN("Getting callback from slave. Recv byte array: %d %d %d %d %d %d %d %d \n", recv_array[0], recv_array[1], recv_array[2], recv_array[3], recv_array[4], recv_array[5], recv_array[6], recv_array[7]);
}

void MCU_Interface::execute(){
    while (ros::ok()){
        ros::spinOnce();
        loop_rate.sleep();
    }
}

int main(int argc, char **argv){
    ros::init(argc,argv,"mcu_interface");
    MCU_Interface mcu_i;
    mcu_i.execute();
}
