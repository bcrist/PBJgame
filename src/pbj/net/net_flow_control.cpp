#ifndef NET_FLOW_CONTROL_H_
#include "pbj/net/net_flow_control.h"
#endif

#include <iostream>

using namespace pbj;
using namespace pbj::net;

F32 FlowControl::RTT_Threshold = 255.0f;

FlowControl::FlowControl()
{
  PBJ_LOG((int)Verbosity::VInfo) << "Flow control initialized" << PBJ_LOG_END;
  reset();
}

void FlowControl::reset()
{
  _mode = FlowControlMode::Bad;
  _penaltyTime = 4.0f;
  _goodConditionsTime = 0.0f;
  _penaltyReductionAccumulator = 0.0f;
}

void FlowControl::update(F32 dt, F32 rtt)
{
  switch(_mode)
  {
  case FlowControlMode::Good:
    goodUpdate(dt, rtt);
    break;
  case FlowControlMode::Bad:
    badUpdate(dt, rtt);
    break;
  default:break;
  }
}

void FlowControl::goodUpdate(F32 dt, F32 rtt)
{
  if(rtt > RTT_Threshold)
  {
    PBJ_LOG((int)Verbosity::VNotice) << "*** dropping to bad mode ***" << PBJ_LOG_END;
    _mode = FlowControlMode::Bad;
    if(_goodConditionsTime < 10.0f && _penaltyTime < 60.0f)
    {
      _penaltyTime *= 2.0f;
      if(_penaltyTime > 60.0f)
        _penaltyTime = 60.0f;
      PBJ_LOG((int)Verbosity::VNotice) << "penalty time increased to " << _penaltyTime << PBJ_LOG_END;
    }
      _goodConditionsTime = 0.0f;
      _penaltyReductionAccumulator = 0.0f;
      return;
  }

  _goodConditionsTime += dt;
  _penaltyReductionAccumulator += dt;

  if(_penaltyReductionAccumulator > 10.0f && _penaltyTime > 1.0f)
  {
    _penaltyTime /= 2.0f;
    if(_penaltyTime < 1.0f)
      _penaltyTime = 1.0f;
    PBJ_LOG((int)Verbosity::VNotice) << "penalty time reduced to " << _penaltyTime << PBJ_LOG_END;
    _penaltyReductionAccumulator = 0.0f;
  }
}

void FlowControl::badUpdate(F32 dt, F32 rtt)
{
  if(rtt <= RTT_Threshold)
    _goodConditionsTime += dt;
  else
    _goodConditionsTime = 0.0f;

  if(_goodConditionsTime > _penaltyTime)
  {
    PBJ_LOG((int)Verbosity::VNotice) << "*** upgrading to good mode ***" << PBJ_LOG_END;
    _goodConditionsTime = 0.0f;
    _penaltyReductionAccumulator = 0.0f;
    _mode = FlowControlMode::Good;
    return;
  }
}

F32 FlowControl::getSendRate()
{
  return (_mode==FlowControlMode::Good)?30.0f:10.0f;
}