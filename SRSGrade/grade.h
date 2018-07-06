#pragma once
#include <string>
#include <type_traits>
#include <tuple>
#include <gsl.h>
#include "roster.h"
#include "timestamp.h"
#include <boost/multi_index/sequenced_index.hpp>

class Grade
{
public:
	class Key
	{
	private:
		Roster const& roster_;
		std::string grader_email_;
		std::string student_name_;

	private:
		friend bool operator==(const Key lhs, const Key rhs)
		{
			const auto result{ lhs.grader_email_ == rhs.grader_email_ && lhs.student_name_ == rhs.student_name_ };
			return result;
		}

	public:
		explicit Key(Roster const& roster);

		std::string getGraderEmail() const
		{
			Expects(!grader_email_.empty() && roster_.is_email_good(grader_email_));
			return grader_email_;
		}

		void setGraderEmail(const std::string grader_email)
		{
			Expects(!grader_email.empty() && roster_.is_email_good(grader_email));
			grader_email_ = grader_email;
		}

		std::string getStudentName() const
		{
			Expects(!student_name_.empty());
			return student_name_;
		}

		void setStudentName(const std::string student_name)
		{
			Expects(!student_name.empty());
			student_name_ = student_name;
		}

        bool is_good() const;
	};

private:
    Roster const& roster_;
	Key key_;
	Timestamp timestamp_{};
	unsigned score_{ 0 };

public:
	bool is_good() const;

	Key getKey() const
	{
		return key_;
	}

	Timestamp getTimestamp() const
	{
		return timestamp_;
	}

	unsigned getScore() const
	{
		return score_;
	}

private:
	void setTimestamp(const Timestamp timestamp)
	{
		Expects(timestamp.is_good());
		timestamp_ = timestamp;
	}

	friend std::istream& operator>>(std::istream& is, Grade& grade)
	{
		Timestamp timestamp;

		is >> timestamp;
		Ensures(timestamp.is_good());
		grade.setTimestamp(timestamp);
		
		char grader_email_cstr[256]{};
		constexpr auto max_grader_len{ std::extent<decltype(grader_email_cstr)>::value };
		is.getline(grader_email_cstr, max_grader_len, ',');
		auto const grader_len{ strlen(grader_email_cstr) };
		Ensures(grader_len > 0 && grader_len < max_grader_len);

		grade.key_.setGraderEmail(grader_email_cstr);

		is.ignore(std::numeric_limits<std::streamsize>::max(), ',');

		char student_name_cstr[256]{};
		constexpr auto max_student_name_len{ std::extent<decltype(student_name_cstr)>::value };
		is.getline(student_name_cstr, max_student_name_len, ',');
		auto const student_len{ strlen(student_name_cstr) };
		Ensures(student_len > 0 && student_len < max_student_name_len);

		grade.key_.setStudentName(student_name_cstr);

		Ensures(grade.is_good());

		is.seekg(-2, std::ios_base::end);

		auto const is_comma{ is.peek()==',' };

		if (is_comma)
		{
			is.ignore();
		}

		auto grade_value{ std::numeric_limits<int>::max() };

		is >> grade_value;

		Ensures(grade_value >= 0 && grade_value<(10+1) && is.eof());

		grade.score_ = 10 * grade_value;

		Ensures(grade.score_ >= 0 && grade.score_ < (100+1));

		return is;
	}

public:
	explicit Grade(Roster const& roster);
};

