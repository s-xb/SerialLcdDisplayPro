位置模式：正弦轨迹位置跟踪
正弦幅4000*5 cnt
正弦周期：4096*2ms
指令发送周期：2ms


	position_RangeLimitMin(0,NodeId[3],-500000000);
	position_RangeLimitMax(0,NodeId[3],500000000);
	position_SoftwareLimitMin(0,NodeId[3],-50000000);
	position_SoftwareLimitMax(0,NodeId[3],50000000);
	speed_MotorMax(0,NodeId[3],10000000);
	veolcity_profileMAX(0,NodeId[3],9000000);
	veolcity_profile(0,NodeId[3],500000);
	acceleration_profile(0,NodeId[3],500000);
	deceleration_profile(0,NodeId[3],500000);
	acceleration_Max(0,NodeId[3],900000);
	deceleration_Max(0,NodeId[3],900000);