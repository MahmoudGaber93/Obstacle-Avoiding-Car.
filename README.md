# Obstacle-Avoiding-Car.
Obstacle avoiding car using HC_SR04 Ultrasonic sensor and DC motors based on AVR micro-controller with applying static design on all the project modules

Requirments
Use the Ultrasonic sensor with a 4WD car to detect objects 
If there were no object detected the car moves forward with 80% of its max speed.
If there is an object detected 50 cm distant from the car then the car should slow down to 30% of its maximum speed.
If there is an object detected 30 cm distant from the car then the car will stop then turn right and continue moving under the same distance and speed criteria.
If there is an object detected less than 30 cm distant from the car then the car will stop then moves backward until the distance is 30 cm then stop and turn right and continue moving under the same distance and speed criteria.
