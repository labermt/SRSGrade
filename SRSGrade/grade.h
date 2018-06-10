#pragma once
#include <string>
#include <type_traits>
#include <tuple>
#include <gsl.h>
#include "timestamp.h"

class Grade
{
public:
	class Key
	{
	private:
		const std::string srs_;
		std::string grader_;
		std::string student_;

	private:
		friend bool operator==(const Key lhs, const Key rhs)
		{
			const auto result{ lhs.srs_ == rhs.srs_ && lhs.grader_ == rhs.grader_ && lhs.student_ == rhs.student_ };
			return result;
		}

	public:
		explicit Key(const std::string srs): srs_{srs}{}

		std::string getSrs() const
		{
			return srs_;
		}

		std::string getGrader() const
		{
			Expects(!grader_.empty());
			return grader_;
		}

		void setGrader(const std::string grader)
		{
			Expects(!grader.empty());
			grader_ = grader;
		}

		std::string getStudent() const
		{
			Expects(!student_.empty());
			return student_;
		}

		void setStudent(const std::string student)
		{
			Expects(!student.empty());
			student_ = student;
		}
	};

private:
	Key key_;
	Timestamp timestamp_;
	unsigned score_;

public:
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
		
		char grader_cstr[256]{};
		constexpr auto max_grader_len{ std::extent<decltype(grader_cstr)>::value };
		is.getline(grader_cstr, max_grader_len, ',');
		const auto grader_len{ strlen(grader_cstr) };
		Ensures(grader_len > 0 && grader_len < max_grader_len);

		grade.key_.setGrader(grader_cstr);

		is.ignore(std::numeric_limits<std::streamsize>::max(), ',');

		char student_cstr[256]{};
		constexpr auto max_student_len{ std::extent<decltype(student_cstr)>::value };
		is.getline(student_cstr, max_student_len, ',');
		const auto student_len{ strlen(grader_cstr) };
		Ensures(student_len > 0 && student_len < max_student_len);

		grade.key_.setStudent(student_cstr);

		is.seekg(-1, std::ios_base::end);

		std::string grade_str;

		is >> grade_str;

		Ensures(grade_str.length()==1 && is.eof());

		const auto grade_letter{ grade_str.at(0) };

		Ensures(grade_letter >= 'A' && grade_letter < 'G'); // TODO: Handle the zeroes. 

		grade.score_ = 95 - 10 * ( grade_letter - 'A' );

		return is;
	}

public:
	explicit Grade(const std::string srs);
};
