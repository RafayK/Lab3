#pragma once

#include "map.h"
#include <geometry_msgs/PoseWithCovarianceStamped.h>

using namespace std;
using namespace geometry_msgs;

#define MAX_RETRY 50

class Milestone {
  public:
  	Milestone(const geometry_msgs::PoseWithCovarianceStamped pose);
    Milestone(Milestone* origin, float velocityLinear,float velocityAngular, int duration, Map map);
    float getVelocityLinear() {return mVelocityLinear;};
    float getVelocityAngular() {return mVelocityAngular;};
    int getDuration() {return mDuration;};
    Milestone* getOrigin() {return mOrigin;};
    Pose getDestination(Map map);
    Milestone* makeRandomNode(Map map);
    bool isValid() {return mIsValid;};

  private:
    Milestone* mOrigin;
    float mVelocityLinear;
    float mVelocityAngular;
    int mDuration;
    bool mIsValid;
    Pose mDestination;
};

