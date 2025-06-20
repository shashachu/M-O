#ifndef MOCONFIG_H
#define MOCONFIG_H

#define START_DEGREE_VALUE  90
#define DEFAULT_SPEED 300
#define DEFAULT_EASING EASE_LINEAR

// Smoothing settings - more aggressive for steering/turning servos
#define DEFAULT_FILTER_SIZE 8
#define DEFAULT_DEADBAND 3
#define DEFAULT_SMOOTHING_FACTOR 0.15f

// Less aggressive smoothing for head tilt (needs more responsiveness)
#define TILT_FILTER_SIZE 5
#define TILT_DEADBAND 2
#define TILT_SMOOTHING_FACTOR 0.2f

ServoConfig steeringConfig = { 
  0, /* channel */
  0, /* pcaPin */
  7, /* inputPin */
  15, /* minAngle */
  165, /* maxAngle */
  DEFAULT_SPEED,
  DEFAULT_EASING,
  DEFAULT_FILTER_SIZE,
  DEFAULT_DEADBAND,
  DEFAULT_SMOOTHING_FACTOR
};

ServoConfig headTiltConfig = { 
  2, /* channel */
  1, /* pcaPin */
  9, /* inputPin */
  60, /* minAngle */
  115, /* maxAngle */
  DEFAULT_SPEED,
  DEFAULT_EASING,
  TILT_FILTER_SIZE,
  TILT_DEADBAND,
  TILT_SMOOTHING_FACTOR
};

ServoConfig headTurnConfig = {
  3, /* channel */
  2, /* pcaPin */
  10, /* inputPin */
  15, /* minAngle */
  165, /* maxAngle */
  DEFAULT_SPEED,
  DEFAULT_EASING,
  DEFAULT_FILTER_SIZE,
  DEFAULT_DEADBAND,
  DEFAULT_SMOOTHING_FACTOR
};

#endif // MOCONFIG_H