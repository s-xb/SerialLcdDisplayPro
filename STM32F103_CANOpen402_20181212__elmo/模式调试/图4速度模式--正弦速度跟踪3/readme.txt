速度模式：正弦速度跟踪
正弦幅值4000*500 cnt
正弦周期：4096*2ms
指令发送周期：2ms

	position_RangeLimitMin(0,NodeId[3],-5000000);
	position_RangeLimitMax(0,NodeId[3],5000000);
	position_SoftwareLimitMin(0,NodeId[3],-4000000);
	position_SoftwareLimitMax(0,NodeId[3],4000000);
	speed_MotorMax(0,NodeId[3],50000000);
	veolcity_profileMAX(0,NodeId[3],40000000);
	veolcity_profile(0,NodeId[3],40000000);
	acceleration_profile(0,NodeId[3],40000000);
	deceleration_profile(0,NodeId[3],40000000);
	acceleration_Max(0,NodeId[3],50000000);
	deceleration_Max(0,NodeId[3],50000000);