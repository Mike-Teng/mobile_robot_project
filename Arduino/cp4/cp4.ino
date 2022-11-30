#include <Arduino.h>

#include <ros.h>
#include <std_msgs/ByteMultiArray.h>
#include <std_msgs/Byte.h>
#include <std_msgs/Int64.h>

#include "robot.h"

ros::NodeHandle nh;
Robot robot;

void action_cb(const std_msgs::Int64& msg);
void publish();

std_msgs::ByteMultiArray state;

#define STATE_ARR_LEN 11
int8_t state_arr[STATE_ARR_LEN];
ros::Publisher state_pub("state", &state);

ros::Subscriber<std_msgs::Int64> action_sub("action", &action_cb);

unsigned long previousMillis = 0;

double leftsp, rightsp;

void setup()
{
  leftsp = 0;
  rightsp = 0;
  state.data = state_arr;
  state.data_length = STATE_ARR_LEN;
  robot.init();
  robot.pid.set_setpoint(20, 20);
  nh.initNode();
  nh.advertise(state_pub);
  nh.subscribe(action_sub);
}

void loop()
{
  nh.spinOnce();
  robot.pid.set_setpoint(-rightsp, -leftsp);
  robot.run();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 50l)
  {
    previousMillis = currentMillis;

    publish();
  }
  delay(1);
}

void action_cb(const std_msgs::Int64& msg)
{
  if (msg.data == 1)
  {
    leftsp = 6;
    rightsp = 6;
    return;
  }
  else if (msg.data == 4)
  {
    leftsp = -4;
    rightsp = -4;
    return;
  }
  else if (msg.data == 3)
  {
    leftsp = -3;
    rightsp = 3;
    return;
  }
  else if (msg.data == 2)
  {
    leftsp = 3;
    rightsp = -3;
    return;
  }
  else if (msg.data == 0)
  {
    leftsp = 0;
    rightsp = 0;
    return;
  }
}

void publish()
{
  state.data[0] = robot.pr.on;
  state.data[1] = robot.ms.left.data;
  state.data[2] = robot.ms.mid.data;
  state.data[3] = robot.ms.right.data;
  state.data[4] = map(robot.pr.volt, 0, 1023, -128, 127);
  state.data[5] = IrReceiver.decodedIRData.protocol & 0xFF;
  // memcpy(state.data[6], &IrReceiver.decodedIRData.decodedRawData, 4);
  state.data[6] = (IrReceiver.decodedIRData.decodedRawData >> 24) & 0xFF;
  state.data[7] = (IrReceiver.decodedIRData.decodedRawData >> 16) & 0xFF;
  state.data[8] = (IrReceiver.decodedIRData.decodedRawData >> 8) & 0xFF;
  state.data[9] = (IrReceiver.decodedIRData.decodedRawData >> 0) & 0xFF;
  // state.data[6] = (IrReceiver.decodedIRData.address >> 8) & 0xFF;
  // state.data[7] = IrReceiver.decodedIRData.address        & 0xFF;
  // state.data[8] = (IrReceiver.decodedIRData.command >> 8) & 0xFF;
  // state.data[9] = IrReceiver.decodedIRData.command        & 0xFF;
  state.data[10] += 1;

  // state.data[6] = -1;
  state_pub.publish(&state);
}
