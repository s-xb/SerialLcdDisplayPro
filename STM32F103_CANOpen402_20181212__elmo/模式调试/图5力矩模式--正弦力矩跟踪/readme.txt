力矩模式：正弦力矩跟踪
正弦幅值4000*500 cnt
正弦周期：4096*2ms
指令发送周期：2ms

	position_RangeLimitMin(0,NodeId[3],-5000000);
	position_RangeLimitMax(0,NodeId[3],5000000);
	position_SoftwareLimitMin(0,NodeId[3],-4000000);
	position_SoftwareLimitMax(0,NodeId[3],4000000);
	speed_MotorMax(0,NodeId[3],500000000);
	veolcity_profileMAX(0,NodeId[3],400000000);
	veolcity_profile(0,NodeId[3],40000000);
	acceleration_profile(0,NodeId[3],990000000);
	deceleration_profile(0,NodeId[3],990000000);
	acceleration_Max(0,NodeId[3],1000000000);
	deceleration_Max(0,NodeId[3],1000000000);
	
	toeque_Max(0,NodeId[3],1250);
	current_Max(0,NodeId[3],1250);
	current_MotorRated(0,NodeId[3],11000);
	torque_MotorRated(0,NodeId[3],9960);