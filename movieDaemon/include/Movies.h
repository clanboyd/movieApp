#ifndef MOVIES_H_
#define MOVIES_H_

#include <string>
#include <map>
#include "Definitions.h"

using namespace std;

class Media
{
public:
	Media();
	virtual ~Media();
	virtual string getTitle();
	virtual const std::string getMedia();
	virtual const std::string getGenre();
	enum genretype{ Action, Horror, SciFi, Children, Drama, Documentary, KungFu, Disney, Suspense, Comedy, Musical, UnkGenre };
	enum mediatype{ mediaDVD, mediaBluRay, mediaBluRay4k, UnkMedia};
	const string retGenreString(genretype inGenre);
	const genretype retGenreEnum(std::string inGenre);
	const string retMediaString(mediatype inMedia);
	const mediatype retMediaEnum(std::string inMedia);
	const std::string GetGenreStr(genretype &rGenre);
	const std::string GetMovieRfId(){return mMovieRfId;};
	const std::string GetPersonRfId(){return mPersonRfId;};
	const std::string GetTitle(){return title;};

protected:
	//int m_ID;
	std::string title;
	int genre;
	int media;
	std::string mMovieRfId;
	std::string mPersonRfId;
	std::map<genretype,std::string> genEnum;
	std::map<mediatype,std::string> medEnum;
};


class DVD : public Media
{
public:
	DVD(std::string inTitle);
	//DVD(std::string inTitle, std::string inGenre);
	DVD(std::string inTitle, std::string inId, std::string inPersonId);
	~DVD();

private:
};

class BluRay : public Media{
public:
	BluRay(std::string inTitle);
	//BluRay(std::string inTitle, std::string inId);
	BluRay(std::string inTitle, std::string inGenre, std::string inPersonId);
	~BluRay();

private:
};

class BluRay4k : public Media
{
public:
	BluRay4k(std::string inTitle);
	//BluRay4k(std::string inTitle, std::string inId);
	BluRay4k(std::string inTitle, std::string inGenre, std::string inPersonId);
	~BluRay4k();

private:
};

#endif /* MOVIES_H_ */
