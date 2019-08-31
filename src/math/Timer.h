//
// Created by liyh on 2019/8/31.
//

#ifndef TRYGL_TIMER_H
#define TRYGL_TIMER_H

struct TimeSpan {
public:
	explicit TimeSpan(long int nano) : m_nona(nano) {

	}

	bool operator>(const TimeSpan &vRight) const {
		return m_nona > vRight.m_nona;
	}

	bool operator>=(const TimeSpan &vRight) const {
		return m_nona >= vRight.m_nona;
	}

	TimeSpan operator-(const TimeSpan &vRight) const {
		return TimeSpan(m_nona - vRight.m_nona);
	}

	long int ToNano() const {
		return m_nona;
	}

	long int ToMicro() const {
		static const auto THOUSAND = 1000;
		return m_nona / THOUSAND;
	}

	long int ToMilli() const {
		static const auto MILLION = 1000000;
		return m_nona / MILLION;
	}

private:
	long int m_nona;
};

struct Timer {
public:
	static Timer Now() {
		Timer timer;
		int iResult = clock_gettime(CLOCK_MONOTONIC, &timer.m_t);
		assert(iResult == 0);
		return timer;
	}

	struct TimeSpan operator-(const Timer vRight) {
		static const auto BILLION = 1000000000;
		return TimeSpan((m_t.tv_sec - vRight.m_t.tv_sec) * BILLION + m_t.tv_nsec - vRight.m_t.tv_nsec);
	}

private:
	timespec m_t;
};


#endif //TRYGL_TIMER_H
