#include "stdafx.h"
#include "roster.h"
#include "grade.h"

Grade::Grade(const Roster& roster): roster_{roster}
{
}

bool Grade::Key::is_good() const
{
	const auto result{ !student_name_.empty() && !grader_email_.empty() };
	return result;
}

bool Grade::is_good() const
{
	const auto student_name{ key_.getStudentName() };
	const auto grader_email{ key_.getGraderEmail() };
	const auto result{ key_.is_good() && roster_.find_name(student_name).has_value() && roster_.is_email_good(grader_email) };
	return result;
}

