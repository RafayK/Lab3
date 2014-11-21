#include "rrt.h"
#include "turtlebot_example.h"
#include <tf/transform_datatypes.h>
#include <tf/tf.h>

#include <random>
#include <cmath>

static default_random_engine gen;

Milestone:: Milestone(const geometry_msgs::PoseWithCovarianceStamped startingPoint)
{
	mDestination = startingPoint.pose.pose;
}

Milestone::Milestone(Milestone* origin, float velocityLinear,float velocityAngular, int duration, Map map) 
	: mOrigin(origin),
      mIsValid(false), 
      mVelocityLinear(velocityLinear),
      mVelocityAngular(velocityAngular), 
      mDuration(duration) 
{
  getDestination(map);
}

Pose Milestone::getDestination(Map map) {
  if (mIsValid == false) {
    Pose destination = mOrigin->getDestination(map);
    tf::Quaternion quart;
    double r, p, yaw;

    tf::quaternionMsgToTF(destination.orientation, quart);
    tf::Matrix3x3(quart).getRPY(r, p, yaw);

    for (int i = 0; i < mDuration * UPDATES_PER_SEC; i++) {
      destination.position.x +=
        1 / float(UPDATES_PER_SEC) * mVelocityLinear * cos(yaw);
      destination.position.y +=
        1 / float(UPDATES_PER_SEC) * mVelocityLinear * sin(yaw);
      yaw += 1 / float(UPDATES_PER_SEC) * mVelocityAngular;

      tf::quaternionTFToMsg(
          tf::createQuaternionFromRPY(r, p, yaw),
          destination.orientation);
      if (map.hasObstacle(destination)) {
        return mDestination;
      }
    }

    mIsValid = true;
    mDestination = destination;
  }

  return mDestination;
}

Milestone* Milestone::makeRandomNode(Map map) {
  uniform_real_distribution<> velocityLinearGenerator(0, 0.3);
  uniform_real_distribution<> velocityAngularGenerator(-0.3, 0.3);
  uniform_int_distribution<> durationGenerator(1, 3);
  for (int i = 0; i < MAX_RETRY; i++) {
    float velocityLinear = velocityLinearGenerator(gen);
    float velocityAngular = velocityAngularGenerator(gen);
    int duration = durationGenerator(gen);
    Milestone* newNode = new Milestone(this, velocityLinear, velocityAngular,
        duration, map);
    if (newNode->isValid())
      return newNode;
    else
      delete newNode;
  }

  return NULL;
}