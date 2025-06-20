#ifndef MOCONFIG_H
#define MOCONFIG_H

#define START_DEGREE_VALUE  90
#define DEFAULT_SPEED 300
#define DEFAULT_EASING EASE_LINEAR
#define DEFAULT_FILTER_SIZE 5
#define DEFAULT_DEADBAND 2
#define DEFAULT_SMOOTHING_FACTOR 0.2f

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
  DEFAULT_FILTER_SIZE,
  DEFAULT_DEADBAND,
  DEFAULT_SMOOTHING_FACTOR
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