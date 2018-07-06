#include "stdafx.h"
#include "grade.h"
#include "roster.h"

Grade::Grade(Roster const& roster): roster_{ roster }, key_{ roster }
{
}

Grade::Key::Key(Roster const& roster): roster_{roster}
{
}

bool Grade::Key::is_good() const
{
	const auto result{ !student_name_.empty() && !grader_email_.empty() && roster_.is_email_good(grader_email_) };
	return result;
}

bool Grade::is_good() const
{
	auto const student_name{ key_.getStudentName() };
	auto const grader_email{ key_.getGraderEmail() };
	auto const result{ key_.is_good() && roster_.find_name(student_name).has_value() };
	return result;
}
