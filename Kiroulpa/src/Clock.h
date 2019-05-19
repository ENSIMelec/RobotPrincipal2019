#ifndef CLOCK_H_INCLUDED
#define CLOCK_H_INCLUDED

#include <ctime>
#include <cstdint>
#include <sys/time.h>

class timer
{
	uint64_t _start_time;
	uint64_t _paused_time;

	bool paused = false;

	uint64_t getMicros(timeval t)
	{
		return ((long)(t.tv_usec)) + (((long)(t.tv_sec))*1000000ll);
	}

	public:
	timer() { restart(); }
	void restart()
	{
		struct timeval t;
		gettimeofday(&t, NULL);
		_start_time = getMicros(t) ;

		paused = false;
	}

	void pause()
	{
		struct timeval t;
		gettimeofday(&t, NULL);
		_paused_time = getMicros(t);

		paused = true;
	}

	void resume()
	{
		paused = false;

		struct timeval t;
		gettimeofday(&t, NULL);
		uint64_t pause = getMicros(t) - _paused_time;

		_start_time += pause;
	}

	uint64_t elapsed()
	{
		return elapsed_us();
	}

	uint64_t elapsed_us()
	{
		if (paused) {
			return _paused_time - _start_time;
		} else {
			struct timeval t;
			gettimeofday(&t, NULL);
			return getMicros(t) - _start_time;
		}
	}
	uint64_t elapsed_ms()
	{
		return elapsed_us()/1000ll;
	}

	uint64_t elapsed_s()
	{
		return elapsed_us()/1000000ll;
	}
};


#endif //CLOCK_H_INCLUDED