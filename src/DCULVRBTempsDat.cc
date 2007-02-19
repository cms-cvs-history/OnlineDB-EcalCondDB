#include <stdexcept>
#include <string>
#include "OnlineDB/Oracle/interface/Oracle.h"

#include "OnlineDB/EcalCondDB/interface/DCULVRBTempsDat.h"
#include "OnlineDB/EcalCondDB/interface/RunTag.h"
#include "OnlineDB/EcalCondDB/interface/RunIOV.h"

using namespace std;
using namespace oracle::occi;

DCULVRBTempsDat::DCULVRBTempsDat()
{
  m_env = NULL;
  m_conn = NULL;
  m_writeStmt = NULL;

  m_t1 = 0;
  m_t2 = 0;
  m_t3 = 0;
}



DCULVRBTempsDat::~DCULVRBTempsDat()
{
}



void DCULVRBTempsDat::prepareWrite()
  throw(runtime_error)
{
  this->checkConnection();

  try {
    m_writeStmt = m_conn->createStatement();
    m_writeStmt->setSQL("INSERT INTO dcu_lvrb_temps_dat (iov_id, logic_id, "
			"t1, t2, t3) "
			"VALUES (:iov_id, :logic_id, "
			":3, :4, :5)");
  } catch (SQLException &e) {
    throw(runtime_error("DCULVRBTempsDat::prepareWrite():  "+e.getMessage()));
  }
}



void DCULVRBTempsDat::writeDB(const EcalLogicID* ecid, const DCULVRBTempsDat* item, DCUIOV* iov)
  throw(runtime_error)
{
  this->checkConnection();
  this->checkPrepare();

  int iovID = iov->fetchID();
  if (!iovID) { throw(runtime_error("DCULVRBTempsDat::writeDB:  IOV not in DB")); }

  int logicID = ecid->getLogicID();
  if (!logicID) { throw(runtime_error("DCULVRBTempsDat::writeDB:  Bad EcalLogicID")); }
  
  try {
    m_writeStmt->setInt(1, iovID);
    m_writeStmt->setInt(2, logicID);

    m_writeStmt->setFloat(3, item->getT1() );
    m_writeStmt->setFloat(4, item->getT2() );
    m_writeStmt->setFloat(5, item->getT3() );

    m_writeStmt->executeUpdate();
  } catch (SQLException &e) {
    throw(runtime_error("DCULVRBTempsDat::writeDB():  "+e.getMessage()));
  }
}



void DCULVRBTempsDat::fetchData(std::map< EcalLogicID, DCULVRBTempsDat >* fillMap, DCUIOV* iov)
  throw(runtime_error)
{
  this->checkConnection();
  fillMap->clear();

  iov->setConnection(m_env, m_conn);
  int iovID = iov->fetchID();
  if (!iovID) { 
    //  throw(runtime_error("DCULVRBTempsDat::writeDB:  IOV not in DB")); 
    return;
  }

  try {
    Statement* stmt = m_conn->createStatement();
    stmt->setSQL("SELECT cv.name, cv.logic_id, cv.id1, cv.id2, cv.id3, cv.maps_to, "
		 "d.t1, d.t2, d.t3 "
		 "FROM channelview cv JOIN dcu_lvrb_temps_dat d "
		 "ON cv.logic_id = d.logic_id AND cv.name = cv.maps_to "
		 "WHERE d.iov_id = :iov_id");
    stmt->setInt(1, iovID);
    ResultSet* rset = stmt->executeQuery();
    
    std::pair< EcalLogicID, DCULVRBTempsDat > p;
    DCULVRBTempsDat dat;
    while(rset->next()) {
      p.first = EcalLogicID( rset->getString(1),     // name
			     rset->getInt(2),        // logic_id
			     rset->getInt(3),        // id1
			     rset->getInt(4),        // id2
			     rset->getInt(5),        // id3
			     rset->getString(6));    // maps_to

      dat.setT1( rset->getFloat(7) );
      dat.setT2( rset->getFloat(8) );
      dat.setT3( rset->getFloat(9) );

      p.second = dat;
      fillMap->insert(p);
    }
  } catch (SQLException &e) {
    throw(runtime_error("DCULVRBTempsDat::fetchData():  "+e.getMessage()));
  }
}
