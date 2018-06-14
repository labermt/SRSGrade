//
// Created by bessermt on 6/3/18.
//

#include "stdafx.h"
#include <string>
#include <chrono>
#include <sstream>
#include <gsl.h>
#include "timestamp.h"

namespace
{
    auto make_time_point
    (
        const int year,
        const int mon,
        const int mday,
        const int hour,
        const int min,
        const int sec
    )
    {
        tm time_value{};

        Expects(sec>=0 && sec<60);
		Expects(min>=0 && min<60);
		Expects(hour>=0 && hour<24);
		Expects(mday>=0 && mday<32);
		Expects(mon>=0 && mon<32);
		Expects(year>=1900 && year<2100);

        time_value.tm_sec  = sec;  // Seconds.     [0-60] (1 leap second)
        time_value.tm_min  = min;  // Minutes.     [0-59]
        time_value.tm_hour = hour; // Hours.	   [0-23]
        time_value.tm_mday = mday; // Day.         [1-31]
        time_value.tm_mon  = mon;  // Month.       [0-11]
        time_value.tm_year = year; // Year - 1900.

        const auto time_c{ mktime(&time_value) };
        const auto result{ std::chrono::system_clock::from_time_t(time_c) };

        return result;
    }

    auto make_time_point(const std::string date, const std::string time)
    {
        auto year{ 0 };
        auto mon{ 0 };
        auto mday{ 0 };
        auto hour{ 0 };
        auto min{ 0 };
        auto sec{ 0 };

        char delimiter;

        std::istringstream date_iss(date);
        date_iss >> mon >> delimiter >> mday >> delimiter >> year;
        Ensures(date_iss.eof());

        std::istringstream time_iss(time);
        time_iss >> hour >> delimiter >> min >> delimiter >> sec;
		Ensures(time_iss.eof());

        const auto result{ make_time_point(year, mon, mday, hour, min, sec) };

        return result;
    }
}

void Timestamp::set(const std::string date_string, const std::string time_string)
{
	time_point_ = make_time_point(date_string, time_string);
}
