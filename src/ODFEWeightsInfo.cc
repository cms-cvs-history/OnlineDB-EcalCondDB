#include <stdexcept>
#include <string>
#include <string.h>
#include "OnlineDB/Oracle/interface/Oracle.h"

#include "OnlineDB/EcalCondDB/interface/ODFEWeightsInfo.h"

using namespace std;
using namespace oracle::occi;

ODFEWeightsInfo::ODFEWeightsInfo()
{
  m_env = NULL;
  m_conn = NULL;
  m_writeStmt = NULL;
  m_readStmt = NULL;
  m_config_tag="";
   m_ID=0;
   clear();   
}


void ODFEWeightsInfo::clear(){
   m_version=0;
   m_num=0;

}



ODFEWeightsInfo::~ODFEWeightsInfo()
{
}



int ODFEWeightsInfo::fetchNextId()  throw(std::runtime_error) {

  int result=0;
  try {
    this->checkConnection();

    m_readStmt = m_conn->createStatement(); 
    m_readStmt->setSQL("select COND2CONF_INFO_SQ.NextVal from DUAL ");
    ResultSet* rset = m_readStmt->executeQuery();
    while (rset->next ()){
      result= rset->getInt(1);
    }
    result++;
    m_conn->terminateStatement(m_readStmt);
    return result; 

  } catch (SQLException &e) {
    throw(runtime_error("ODFEWeightsInfo::fetchNextId():  "+e.getMessage()));
  }

}

void ODFEWeightsInfo::prepareWrite()
  throw(runtime_error)
{
  this->checkConnection();

  int next_id=0;
  if(getId()==0){
    next_id=fetchNextId();
  }

  try {
    m_writeStmt = m_conn->createStatement();
    m_writeStmt->setSQL("INSERT INTO "+getTable()+" ( rec_id, tag, version, number_of_groups ) " 
			" VALUES ( :1, :2, :3 , :4) " );

    m_writeStmt->setInt(1, next_id);
    m_ID=next_id;

  } catch (SQLException &e) {
    throw(runtime_error("ODFEWeightsInfo::prepareWrite():  "+e.getMessage()));
  }

}

void ODFEWeightsInfo::setParameters(std::map<string,string> my_keys_map){
  
  // parses the result of the XML parser that is a map of 
  // string string with variable name variable value 
  
  for( std::map<std::string, std::string >::iterator ci=
	 my_keys_map.begin(); ci!=my_keys_map.end(); ci++ ) {
    
    if(ci->first==  "VERSION") setVersion(atoi(ci->second.c_str()) );
    if(ci->first==  "TAG") setConfigTag(ci->second);
    if(ci->first==  "NUMBER_OF_GROUPS") setNumberOfGroups(atoi(ci->second.c_str()) );
    
  }
  
}

void ODFEWeightsInfo::writeDB()
  throw(runtime_error)
{
  this->checkConnection();
  this->checkPrepare();

  try {

    // number 1 is the id 
    m_writeStmt->setString(2, this->getConfigTag());
    m_writeStmt->setInt(3, this->getVersion());
    m_writeStmt->setInt(4, this->getNumberOfGroups());

    m_writeStmt->executeUpdate();


  } catch (SQLException &e) {
    throw(runtime_error("ODFEWeightsInfo::writeDB():  "+e.getMessage()));
  }
  // Now get the ID
  if (!this->fetchID()) {
    throw(runtime_error("ODFEWeightsInfo::writeDB:  Failed to write"));
  }


}


void ODFEWeightsInfo::fetchData(ODFEWeightsInfo * result)
  throw(runtime_error)
{
  this->checkConnection();
  result->clear();
  if(result->getId()==0 && (result->getConfigTag()=="") ){
    throw(runtime_error("ODFEWeightsInfo::fetchData(): no Id defined for this ODFEWeightsInfo "));
  }

  try {

    m_readStmt->setSQL("SELECT * FROM " + getTable() +   
                       " where ( rec_id = :1 or (tag=:2 AND version=:3 ) )" );
    m_readStmt->setInt(1, result->getId());
    m_readStmt->setString(2, result->getConfigTag());
    m_readStmt->setInt(3, result->getVersion());
    ResultSet* rset = m_readStmt->executeQuery();

    rset->next();

    // 1 is the id and 2 is the config tag and 3 is the version

    result->setId(rset->getInt(1));
    result->setConfigTag(rset->getString(2));
    result->setVersion(rset->getInt(3));
    result->setNumberOfGroups(rset->getInt(4));

  } catch (SQLException &e) {
    throw(runtime_error("ODFEWeightsInfo::fetchData():  "+e.getMessage()));
  }
}

int ODFEWeightsInfo::fetchID()    throw(std::runtime_error)
{
  // Return from memory if available
  if (m_ID!=0) {
    return m_ID;
  }

  this->checkConnection();

  try {
    Statement* stmt = m_conn->createStatement();
    stmt->setSQL("SELECT rec_id FROM "+ getTable()+
                 "WHERE  tag=:1 and version=:2 " );

    stmt->setString(1, getConfigTag() );
    stmt->setInt(2, getVersion() );

    ResultSet* rset = stmt->executeQuery();

    if (rset->next()) {
      m_ID = rset->getInt(1);
    } else {
      m_ID = 0;
    }
    m_conn->terminateStatement(stmt);
  } catch (SQLException &e) {
    throw(runtime_error("ODFEWeightsInfo::fetchID:  "+e.getMessage()));
  }

  return m_ID;
}