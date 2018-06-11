#include "stdafx.h"
#include "grade.h"

Grade::Grade(const Roster& roster): roster_{roster}
{
}

bool Grade::Key::is_good() const
{
    const auto result{ roster_.find_name(student_).has_value() && roster_.is_email_good(grader_email_) };
    return result;
}

