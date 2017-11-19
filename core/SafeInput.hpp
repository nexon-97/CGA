#pragma once
#include <sstream>

namespace cga
{

template <typename T>
T SafeInput(const char* msg)
{
	bool isOk = false;
	T result;

	while (!isOk)
	{
		std::cout << msg;

		if (std::cin >> result)
		{
			isOk = true;
		}
		else
		{
			std::cout << "Format error! Try again." << std::endl;
			std::cin.clear();
			std::cin.ignore(32767,'\n');
		}
	}
	
	return result;
}

template <typename T>
T RangeInput(const char* msg, T min, T max)
{
	float result;
	
	std::stringstream hintSs;
	hintSs << msg << " [" << min << "; " << max << "]: ";
	
	while (true)
	{
		result = cga::SafeInput<float>(hintSs.str().c_str());
		if (result < 0.5f || result > 4.f)
		{
			std::cout << "Invalid range!" << std::endl;
		}
		else break;
	}

	return result;
}

}
