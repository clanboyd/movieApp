#include "dbClass.h"

//CALLBACKS

int dbClass::CallbackSingle(void *pObj, int argc, char **argv, char **azColName)
{
    printf("CallbackSingle result = %s\n", argv[0]);
    ((dbClass*)pObj)->SetResultSingle(argv[0]);
    return 0;
}

int dbClass::CallbackMulti(void *pObj, int argc, char **argv, char **azColName)
{
    int i;
    for(i=0; i<argc; i++)
    {
        ((dbClass*)pObj)->SetResultMulti(argv[i]);
    }
    return 0;
}

//Class Code

int dbClass::DbInit()
{
    return OpenDb();
}

int dbClass::OpenDb()
{
    int rc=0;
    rc = sqlite3_open((char*)DB_NAME.c_str(),&sqlDriver);
    return rc;
}

void dbClass::CloseDb()
{
    sqlite3_close(sqlDriver);
}

void dbClass::ListDb()
{
    int rc=0;
    char *zErrMsg = 0;
    rc=sqlite3_exec(sqlDriver,"select * from Movies;", CallbackMulti, 0, &zErrMsg);
    if( rc!=SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    std::vector<std::string> vec = GetResultMulti();
    for ( std::vector<std::string>::iterator it=vec.begin(); it!=vec.end(); ++it )
    {
        std::string tmp = (*it);
        printf("query result = %s\n", (char*)(tmp).c_str());
    }
}

std::string dbClass::QueryMovieByKey(const std::string &rKey)
{
    int rc=0;
    char *zErrMsg = 0;
    mQuery = DB_SELECT_STR;
    mQuery+= DB_MOVIES_TITLE_STR;
    mQuery+= DB_FROM_STR;
    mQuery+= DB_MOVIES_STR;
    mQuery+= DB_WHERE_STR;
    mQuery+= DB_MOVIES_KEY_STR;
    mQuery+= DB_EQUAL_STR;
    mQuery+= DB_QUOTE_STR;
    mQuery+= rKey;
    mQuery+= DB_QUOTE_STR;
    mQuery+= DB_TERM_STR;
    printf("Query Movie By Key = %s\n", (char*)mQuery.c_str());
    rc=sqlite3_exec(sqlDriver, (char*)mQuery.c_str(), CallbackSingle, this, &zErrMsg);
    if( rc!=SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    printf("query result = %s\n", (char*)(GetResultSingle()).c_str());
    return GetResultSingle();
}

std::string dbClass::QueryPersonByKey(const std::string &rKey)
{
    int rc=0;
    char *zErrMsg = 0;                                                                                                                 
    mQuery = DB_SELECT_STR;
    mQuery+= DB_PEOPLE_NAME_STR;
    mQuery+= DB_FROM_STR;
    mQuery+= DB_PEOPLE_STR;
    mQuery+= DB_WHERE_STR;
    mQuery+= DB_PEOPLE_NAME_KEY_STR;
    mQuery+= DB_EQUAL_STR;
    mQuery+= DB_QUOTE_STR;
    mQuery+= rKey;
    mQuery+= DB_QUOTE_STR;
    mQuery+= DB_TERM_STR;
    printf("Query Person By Key = %s\n", (char*)mQuery.c_str());
    rc=sqlite3_exec(sqlDriver, (char*)mQuery.c_str(), CallbackSingle, this, &zErrMsg);
    if( rc!=SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    printf("query result = %s\n", (char*)(GetResultSingle()).c_str());
    return mSingleRes;
}

std::string dbClass::CheckOutMovie(const std::string &rMovieKey, const std::string &rPersonKey)
{
    int rc=0;
    std::string result="FAILURE";
    char *zErrMsg = 0;
    bool cont_out=false;
    // First, make sure movie not already checked out
    mQuery = DB_SELECT_STR;
    mQuery+= DB_CHECKOUT_NAME_KEY_STR;
    mQuery+= DB_FROM_STR;
    mQuery+= DB_CHECKOUT_STR;
    mQuery+= DB_WHERE_STR;
    mQuery+= DB_CHECKOUT_MOVIE_KEY_STR;
    mQuery+= DB_EQUAL_STR;
    mQuery+= DB_QUOTE_STR;
    mQuery+= rMovieKey;
    mQuery+= DB_QUOTE_STR;
    mQuery+= DB_TERM_STR;
    ReSetResultSingle();
    printf("Check Out Query Check = %s\n", (char*)mQuery.c_str());
    rc=sqlite3_exec(sqlDriver, (char*)mQuery.c_str(), CallbackSingle, this, &zErrMsg);
    if( rc!=SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        // Should be empty unless found it was already checked out
        if ( "INIT" == GetResultSingle() )
        {
            cont_out=true;
        }
        else
        {
            return "FAILURE";
        }
    }

    if ( true == cont_out )
    {
        // Second, check it out
        mQuery = DB_INSERT_STR;
        mQuery+= DB_INTO_STR;
        mQuery+= DB_CHECKOUT_STR;
        mQuery+= DB_VALUES_STR;
        mQuery+= DB_OPEN_BRAC_STR;
        mQuery+= DB_QUOTE_STR;
        mQuery+= rPersonKey;
        mQuery+= DB_QUOTE_STR;
        mQuery+= DB_COMMA_STR;
        mQuery+= DB_QUOTE_STR;
        mQuery+= rMovieKey;
        mQuery+= DB_QUOTE_STR;
        mQuery+= DB_CLOSE_BRAC_STR;
        mQuery+= DB_TERM_STR;
        printf("Check Out Query = %s\n", (char*)mQuery.c_str());
        rc=sqlite3_exec(sqlDriver, (char*)mQuery.c_str(), 0, this, &zErrMsg);
        if( rc!=SQLITE_OK )
        {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
        else
        {
            result="SUCCESS";
        }
    }
    return result;
}

std::string dbClass::CheckInMovie(const std::string &rMovieKey, const std::string &rPersonKey)
{
    CALL_RESULT ciResults;
    int rc=0;
    std::string result="FAILURE";
    char *zErrMsg = 0;                                                                                                                 
    bool cont_in=false;
    // First, make sure movie not already checked out
    mQuery = DB_SELECT_STR;
    mQuery+= DB_CHECKOUT_NAME_KEY_STR;
    mQuery+= DB_FROM_STR;
    mQuery+= DB_CHECKOUT_STR;
    mQuery+= DB_WHERE_STR;
    mQuery+= DB_CHECKOUT_MOVIE_KEY_STR;
    mQuery+= DB_EQUAL_STR;
    mQuery+= DB_QUOTE_STR;
    mQuery+= rMovieKey;
    mQuery+= DB_QUOTE_STR;
    mQuery+= DB_TERM_STR;
    ReSetResultSingle();
    printf("Check In Query Check = %s\n", (char*)mQuery.c_str());
    rc=sqlite3_exec(sqlDriver, (char*)mQuery.c_str(), CallbackSingle, this, &zErrMsg);
    if( rc!=SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        // Should match the person key
        if ( rPersonKey == GetResultSingle() )
        {
            cont_in=true;
        }
        else
        {
            return "FAILURE";
        }
    }

    if ( true == cont_in )
    {
        // Second, check it in
        mQuery = DB_DELETE_STR;
        mQuery+= DB_FROM_STR;
        mQuery+= DB_CHECKOUT_STR;
        mQuery+= DB_WHERE_STR;
        mQuery+= DB_CHECKOUT_NAME_KEY_STR;
        mQuery+= DB_EQUAL_STR;
        mQuery+= DB_QUOTE_STR;
        mQuery+= rPersonKey;
        mQuery+= DB_QUOTE_STR;
        mQuery+= DB_AND_STR;
        mQuery+= DB_CHECKOUT_MOVIE_KEY_STR;
        mQuery+= DB_EQUAL_STR;
        mQuery+= DB_QUOTE_STR;
        mQuery+= rMovieKey;
        mQuery+= DB_QUOTE_STR;
        mQuery+= DB_TERM_STR;
        printf("query = %s\n", (char*)mQuery.c_str());
        rc=sqlite3_exec(sqlDriver, (char*)mQuery.c_str(), 0, this, &zErrMsg);
        if( rc!=SQLITE_OK )
        {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
        else
        {
            result="SUCCESS";
        }
    }
    return result;
}
