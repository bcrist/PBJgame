#ifndef FLOW_CONTROL_H_
#define FLOW_CONTROL_H_

#include "pbj/_pbj.h"

namespace pbj
{
namespace net
{
	class FlowControl
	{
	public:
		FlowControl();
		void reset();
		void update(F32, F32);
		F32 getSendRate();

	private:
		static F32 RTT_Threshold;

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
}
}
#endif