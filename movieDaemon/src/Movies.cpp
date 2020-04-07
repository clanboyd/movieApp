#include "Movies.h"

//Media
Media::Media()
{
	//m_ID=0;
	genre=0;
	media=0;
	medEnum.insert( std::make_pair ( mediaDVD, "DVD"));
	medEnum.insert( std::make_pair ( mediaBluRay, "BluRay"));
	medEnum.insert( std::make_pair ( mediaBluRay4k, "BluRay4k"));
	genEnum.insert( std::make_pair ( Action, "Action"));
	genEnum.insert( std::make_pair ( Horror, "Horror"));
	genEnum.insert( std::make_pair ( SciFi, "SciFi"));
	genEnum.insert( std::make_pair ( Children, "Children"));
	genEnum.insert( std::make_pair ( Drama, "Drama"));
	genEnum.insert( std::make_pair ( Documentary, "Documentary"));
	genEnum.insert( std::make_pair ( KungFu, "KungFu"));
	genEnum.insert( std::make_pair ( Disney, "Disney"));
	genEnum.insert( std::make_pair ( Suspense, "Suspense"));
	genEnum.insert( std::make_pair ( Comedy, "Comedy"));
	genEnum.insert( std::make_pair ( Musical, "Musical"));

}

Media::~Media(){}

string Media::getTitle()
{
	return title;
}

const std::string Media::getMedia()
{
    switch ( media )
    {
        case mediaDVD:
        {
            return "DVD";
        }
        case mediaBluRay:
        {
            return "BluRay";
        }
        case mediaBluRay4k:
        {
            return "BluRay4k";
        }
        default:
        {
            return "Unknown";
        }
    }
}
const std::string Media::getGenre()
{
    switch ( genre )
    {
        case Action:
        {
            return "Action";
        }
        case Horror:
        {
            return "Horror";
        }
        case SciFi:
        {
            return "SciFi";
        }
        case Children:
        {
            return "Children";
        }
        case Drama:
        {
            return "Drama";
        }
        case Documentary:
        {
            return "Documentary";
        }
        case KungFu:
        {
            return "KungFu";
        }
        case Disney:
        {
            return "Disney";
        }
        case Suspense:
        {
            return "Suspense";
        }
        case Comedy:
        {
            return "Comedy";
        }
        case Musical:
        {
            return "Musical";
        }
        default:
        {
            return "Unknown";
        }
    }
}

const Media::genretype Media::retGenreEnum(std::string inGenre)
{
	std::map< Media::genretype, std::string>::iterator myIt;
	for (myIt = Media::genEnum.begin(); myIt != Media::genEnum.end(); ++myIt){
		if (myIt-> second == inGenre){
			return myIt->first;
		}
	}
	return Media::UnkGenre;
}

const string Media::retGenreString(genretype inGenre)
{
	return genEnum.find(inGenre)->second;
}

const Media::mediatype Media::retMediaEnum(std::string inMedia)
{
	std::map< mediatype, std::string>::iterator myIt;
	for (myIt = Media::medEnum.begin(); myIt != Media::medEnum.end(); ++myIt)
    {
		if (myIt-> second == inMedia)
        {
			return myIt->first;
		}
	}
	return Media::UnkMedia;
}

const string Media::retMediaString(mediatype inMedia)
{
	return medEnum.find(inMedia)->second;
}

//DVD
DVD::~DVD(){}

DVD::DVD(std::string inTitle)
{
	media = mediaDVD;
	title = inTitle;
}

DVD::DVD(std::string inTitle, std::string inMovieId, std::string inPersonId)
{
	media = mediaDVD;
	title = inTitle;
	mMovieRfId = inMovieId;
	mPersonRfId = inPersonId;
}

//DVD::DVD(std::string inTitle, std::string inGenre)
//{
//	media = mediaDVD;
//	title = inTitle;
//	genre = this->retGenreEnum(inGenre);
//}

//BluRay
BluRay::~BluRay(){}

BluRay::BluRay(std::string inTitle)
{
	media = mediaBluRay;
	title = inTitle;
}

BluRay::BluRay(std::string inTitle, std::string inMovieId, std::string inPersonId)
{
	media = mediaDVD;
	title = inTitle;
	mMovieRfId = inMovieId;
	mPersonRfId = inPersonId;
}

//BluRay::BluRay(std::string inTitle, std::string inGenre)
//{
//	media = mediaBluRay;
//	title = inTitle;
//	genre = this->retGenreEnum(inGenre);
//}

//BluRay4k
BluRay4k::~BluRay4k(){}

BluRay4k::BluRay4k(std::string inTitle)
{
	media = mediaBluRay4k;
	title = inTitle;
}

BluRay4k::BluRay4k(std::string inTitle, std::string inMovieId, std::string inPersonId)
{
	media = mediaDVD;
	title = inTitle;
	mMovieRfId = inMovieId;
	mPersonRfId = inPersonId;
}

//BluRay4k::BluRay4k(std::string inTitle, std::string inGenre)
//{
//	media = mediaBluRay;
//	title = inTitle;
//	genre = this->retGenreEnum(inGenre);
//}

