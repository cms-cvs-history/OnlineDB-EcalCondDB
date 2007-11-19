#include <stdexcept>
#include "OnlineDB/Oracle/interface/Oracle.h"

#include "OnlineDB/EcalCondDB/interface/FEConfigFgrInfo.h"
#include "OnlineDB/EcalCondDB/interface/Tm.h"
#include "OnlineDB/EcalCondDB/interface/DateHandler.h"

using namespace std;
using namespace oracle::occi;



FEConfigFgrInfo::FEConfigFgrInfo()
{
  m_conn = NULL;
  m_ID = 0;
  //
  m_iov_id =0 ;
  m_tag ="";

}



FEConfigFgrInfo::~FEConfigFgrInfo(){}


void FEConfigFgrInfo::setID(int id){ m_ID = id;  }
int FEConfigFgrInfo::getID(){ return m_ID ;  }

void FEConfigFgrInfo::setIOVId(int iov_id){ m_iov_id = iov_id;  }
int FEConfigFgrInfo::getIOVId()const {return m_iov_id;  }


Tm FEConfigFgrInfo::getDBTime() const{  return m_db_time;}

void FEConfigFgrInfo::setTag(std::string x) {
  if (x != m_tag) {
    m_ID = 0;
    m_tag = x;
  }
}

std::string FEConfigFgrInfo::getTag() const{  return m_tag;}


int FEConfigFgrInfo::fetchID()
  throw(runtime_error)
{

  // Return from memory if available
  if (m_ID) {
    return m_ID;
  }

  this->checkConnection();


  DateHandler dh(m_env, m_conn);

  try {
    Statement* stmt = m_conn->createStatement();

    stmt->setSQL("SELECT fgr_conf_id FROM fe_config_fgr_info "
		 "WHERE DB_TIMESTAMP = :db_time) " );
    stmt->setDate(1, dh.tmToDate(m_db_time));
		 
    ResultSet* rset = stmt->executeQuery();

    if (rset->next()) {
      m_ID = rset->getInt(1);
    } else {
      m_ID = 0;
    }
    m_conn->terminateStatement(stmt);
  } catch (SQLException &e) {
    throw(runtime_error("FEConfigFgrInfo::fetchID:  "+e.getMessage()));
  }

  return m_ID;
}

//
int FEConfigFgrInfo::fetchIDLast()
  throw(runtime_error)
{

  this->checkConnection();


  DateHandler dh(m_env, m_conn);

  try {
    Statement* stmt = m_conn->createStatement();
    stmt->setSQL("SELECT max( fgr_conf_id) FROM fe_config_fgr_info "	);
    ResultSet* rset = stmt->executeQuery();

    if (rset->next()) {
      m_ID = rset->getInt(1);
    } else {
      m_ID = 0;
    }
    m_conn->terminateStatement(stmt);
  } catch (SQLException &e) {
    throw(runtime_error("FEConfigFgrInfo::fetchIDLast:  "+e.getMessage()));
  }

  return m_ID;
}

//
int FEConfigFgrInfo::fetchIDFromTag()
  throw(runtime_error)
{
  // selects tha most recent config id with a given tag 

  this->checkConnection();

  DateHandler dh(m_env, m_conn);

  try {
    Statement* stmt = m_conn->createStatement();
    stmt->setSQL("SELECT max(fgr_conf_id) FROM fe_config_fgr_info "
		 "WHERE tag = :tag  ");
    stmt->setString(1, m_tag);
    ResultSet* rset = stmt->executeQuery();

    if (rset->next()) {
      m_ID = rset->getInt(1);
    } else {
      m_ID = 0;
    }
    m_conn->terminateStatement(stmt);
  } catch (SQLException &e) {
    throw(runtime_error("FEConfigFgrInfo::fetchIDFromTag:  "+e.getMessage()));
  }
  return m_ID;
}


void FEConfigFgrInfo::setByID(int id) 
  throw(std::runtime_error)
{
   this->checkConnection();

   DateHandler dh(m_env, m_conn);

   try {
     Statement* stmt = m_conn->createStatement();

     stmt->setSQL("SELECT iov_id, db_timestamp, tag FROM fe_config_fgr_info WHERE fgr_conf_id = :1");
     stmt->setInt(1, id);
     
     ResultSet* rset = stmt->executeQuery();
     if (rset->next()) {
       m_iov_id = rset->getInt(1);
       Date dbdate = rset->getDate(2);
       m_db_time = dh.dateToTm( dbdate );
       m_tag=rset->getString(3);
       m_ID = id;

     } else {
       throw(runtime_error("FEConfigFgrInfo::setByID:  Given config_id is not in the database"));
     }
     
     m_conn->terminateStatement(stmt);
   } catch (SQLException &e) {
     throw(runtime_error("FEConfigFgrInfo::setByID:  "+e.getMessage()));
   }
}



int FEConfigFgrInfo::writeDB()
  throw(runtime_error)
{
  this->checkConnection();

  // Check if this IOV has already been written
  if (this->fetchID()) {
    return m_ID;
  }
  
  // Validate the data, use infinity-till convention
  DateHandler dh(m_env, m_conn);

  try {

    // first reserve an id
    Statement* stmtq = m_conn->createStatement();
    stmtq->setSQL("SELECT FE_CONFIG_FGR_SQ.NextVal FROM dual "  );
    ResultSet* rsetq = stmtq->executeQuery();
    if (rsetq->next()) {
      m_ID = rsetq->getInt(1);
    } else {
      m_ID = 0;
    }
    m_conn->terminateStatement(stmtq);

    cout<< "going to use id "<<m_ID<<endl;


    // now insert 

    Statement* stmt = m_conn->createStatement();
    
    stmt->setSQL("INSERT INTO FE_CONFIG_FGR_INFO ( fgr_conf_id, iov_id, tag ) "
		 "VALUES (:1, :2 , :3 )");
    stmt->setInt(1, m_ID);
    stmt->setInt(2, m_iov_id);
    stmt->setString(3, m_tag );

    stmt->executeUpdate();


    m_conn->terminateStatement(stmt);

    int ii=m_ID;
    setByID(ii);
    // this is to recover also the time info

  } catch (SQLException &e) {
    throw(runtime_error("FEConfigFgrInfo::writeDB:  "+e.getMessage()));
  }

  return m_ID;
}




