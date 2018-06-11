#include "stdafx.h"
#include "grader_score.h"

unsigned GraderScore::getInstructor() const
{
    return instructor_;
}

unsigned GraderScore::getSelf() const
{
    return self_;
}

std::vector<unsigned> GraderScore::getPeer() const;
{
    return peer_;
}

void GraderScore::setInstructor(const unsigned value);
{
    instructor_ = value;
}

void GraderScore::setSelf(const unsigned value);            
{
    self_ = value;
}

void GraderScore::addPeer(const unsigned value);
{
    peer_.emplace_back(value);
}

