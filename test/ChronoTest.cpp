#include <chrono>
#include <iostream>

using namespace std;

typedef chrono::high_resolution_clock Clock;
typedef chrono::microseconds MicrosecondDuration;
typedef chrono::time_point<Clock, MicrosecondDuration> TimePoint;

int main()
{
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::cout << now.time_since_epoch().count() << std::endl;
	//return Timestamp{current_point};
	return 0;


}
