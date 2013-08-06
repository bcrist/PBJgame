////////////////////////////////////////////////////////////////////////////////
/// \file	Z:\Documents\PBJgame\include\pbj\net\net_flow_control.h
///
/// \brief	Declares the net flow control class.
////////////////////////////////////////////////////////////////////////////////
#ifndef NET_FLOW_CONTROL_H_
#define NET_FLOW_CONTROL_H_

#include "pbj/_pbj.h"

namespace pbj
{
namespace net
{
  //////////////////////////////////////////////////////////////////////////////
  /// \class	FlowControl
  ///
  /// \brief	Flow control.
  ///
  /// \author	Peter Bartosch
  /// \date	2013-08-05
  //////////////////////////////////////////////////////////////////////////////
  class FlowControl
  {
  public:
    FlowControl();
    void reset();
    void update(F32, F32);
    F32 getSendRate();

  private:
    static F32 RTT_Threshold;

    ////////////////////////////////////////////////////////////////////////////
    /// \enum	FlowControlMode
    ///
    /// \brief	Values that represent FlowControlMode.
    ////////////////////////////////////////////////////////////////////////////
    enum FlowControlMode
    {
      Good = 0x01,
      Bad = 0x02
    };

    void goodUpdate(F32, F32);
    void badUpdate(F32, F32);
    
    FlowControlMode _mode;
    F32 _penaltyTime;
    F32 _goodConditionsTime;
    F32 _penaltyReductionAccumulator;
  };
} //namespace net
} //namespace pbj
#endif