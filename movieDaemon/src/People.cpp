#include "People.h"

People::People(std::string inName, std::string inPersonId, std::string inMovieId)
{
	mName = inName;
	mRfId = inPersonId;
    mCheckedOut.push_back(inMovieId);
}

People::~People(){}

string People::GetName()
{
	return mName;
}

string People::GetCardId()
{
	return mRfId;
}

std::vector<std::string> People::GetCheckedOut()
{
	return mCheckedOut;
}

