/*==========================================================*/
//
//	time.h (タイム管理)
// 
// 
// ------------------------------ 2025/05/13 製作者：佐藤琢磨
//
/*==========================================================*/
#ifndef _TIME_H
#define _TIME_H

class Time
{
private:
	//インスタンス化禁止
	Time() = default;
	~Time() = default;

public:
	static float m_deltaTime;
	static float m_deltaTimeSpan;
};

#endif //_TIME_H