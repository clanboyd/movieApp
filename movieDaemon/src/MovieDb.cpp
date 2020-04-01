#include<fstream>
#include "MovieDb.h"
#include "Movies.h"

void MovieDb::ImportFile()
{
	ParseFile("movieDaemon/data/Movies.csv",mMovieVector);
}

void MovieDb::ParseFile(std::string fileNameIn, std::vector<Media*> &inVector)
{   
    std::ifstream myReadFile;
    myReadFile.open(fileNameIn.c_str());
    std::string tempString;
    std::string inTitle;
    std::string inMedia;
    std::string inGenre;
    std::string inRfId;
    
    if (myReadFile.is_open()) 
	{
        while( !myReadFile.eof() )
		{
            getline(myReadFile, tempString);
            if( myReadFile ){
				size_t found = tempString.find(",");
				if ( found != std::string::npos )
				{
					inTitle=tempString.substr(0, found);
					tempString.erase(0,found+1);
					found = tempString.find(",");
					if ( found != std::string::npos )
					{
						inMedia=tempString.substr(0, found);
						tempString.erase(0,found+1);
						found = tempString.find(",");
						if ( found != std::string::npos )
						{
							inGenre=tempString.substr(0, found);
							tempString.erase(0,found+1);
							inRfId=tempString;
						}
					}
				}
                if(inMedia=="BluRay")
				{
                    Media *myblu = new BluRay(inTitle,inRfId);
                    inVector.push_back(myblu);
                }
                else if(inMedia=="BluRay4k")
				{
                    Media *myblu4k = new BluRay4k(inTitle,inRfId);
                    inVector.push_back(myblu4k);
                }
                else
				{
                    Media *mydvd = new DVD(inTitle,inRfId);
                    inVector.push_back(mydvd);
                
                }
            }
        }
    }
    myReadFile.close();
}

std::string MovieDb::FindTitleById(const std::string &rInId)
{
	std::string title="Unknown Movie";
//	std::cout<<"Looking For: "<<rInId << std::endl;;
	for ( std::vector<Media*>::iterator it= mMovieVector.begin();it!= mMovieVector.end(); ++it)
	{
//		std::cout<<"Have: "<<(*it)->GetTitle() << " with RFID: "<< (*it)->GetRfId() << std::endl;;
		if ( (*it)->GetRfId() == rInId )
		{
			title = (*it)->GetTitle();
			break;
		}
	}
	return title;

}

