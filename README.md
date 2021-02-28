# ATMEGA2560_Thruster_Interface
This repo contains two folders that are used for intercommunication between the Xavier and the MCU in the Gladlaks AUV.
It is not part of the [Vortex-AUV](https://github.com/vortexntnu/Vortex-AUV) repo, because it is *extremely* hardware dependent.

/ATMEGA2560_ROS_driver:  
* Subscribes to:
  * /thrust/thruster_forces
  * /thrust/arm
  
Note! To arm the thrusters, execute the command
```
rosservice call arming_service
```

Alternatively, you can execute

```
rostopic pub /thrust/arm std_msgs/String "arm me daddy"
```
and click CTRL+C once confirmation for arming has been given in the ROS console on the Xavier

/firmware:  
* Not a ROS package, so it does not subscribe to or publish anything. It does however communicate with the 
ATMEGA2560_ROS_driver node through I2C.
