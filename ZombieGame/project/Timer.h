#pragma once
class Timer
{
public:
	Timer(float delay);
	~Timer();

	void Reset();
	void Update(float deltaT);
	void SetDelay(float delay);

	float GetTime() const;

	bool IsDone() const;

private:
	float m_Time;	
	float m_Delay;

	bool m_IsDone;

};

