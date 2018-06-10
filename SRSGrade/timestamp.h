//
// Created by bessermt on 6/3/18.
//

#ifndef SRSCSV_TIMESTAMP_H
#define SRSCSV_TIMESTAMP_H

#include <iostream>
#include <iomanip>
#include <chrono>
#include <gsl.h>

class Timestamp
{
private:
    std::chrono::system_clock::time_point time_point_{};

    void set(const std::string date_string, const std::string time_string);

    friend std::istream& operator>>(std::istream& is, Timestamp& timestamp)
    {
        std::string date_string;

		is >> date_string;
		Ensures(!date_string.empty());

		char time_cstr[256]{};
		constexpr auto max_time_len{ std::extent<decltype(time_cstr)>::value };
		is >> std::ws;
		is.getline(time_cstr, max_time_len, ',');
		const auto time_len{ strlen(time_cstr) };
		Ensures(time_len > 0 && time_len < max_time_len);
    	
		const auto time_string{ std::string(time_cstr) };

        timestamp.set(date_string, time_string);

        return is;
    }

	friend bool operator<(const Timestamp lhs, const Timestamp rhs)
    {
		return lhs.time_point_ < rhs.time_point_;
    }

public:
    auto is_good() const
    {
		const auto is_past{ time_point_ < std::chrono::system_clock::now() };

        return is_past; // TODO: Any better tests?
    }
};


#endif //SRSCSV_TIMESTAMP_H
