#include <stdexcept>
#include <string>
#include "OnlineDB/Oracle/interface/Oracle.h"

#include "OnlineDB/EcalCondDB/interface/MonTimingCrystalDat.h"
#include "OnlineDB/EcalCondDB/interface/RunTag.h"
#include "OnlineDB/EcalCondDB/interface/RunIOV.h"

using namespace std;
using namespace oracle::occi;

MonTimingCrystalDat::MonTimingCrystalDat()
{
  m_env = NULL;
  m_conn = NULL;
  m_writeStmt = NULL;
  m_readStmt = NULL;

  m_timingMean = 0;
  m_timingRMS = 0;
 }



MonTimingCrystalDat::~MonTimingCrystalDat()
{
}



void MonTimingCrystalDat::prepareWrite()
  throw(runtime_error)
{
  this->checkConnection();

  try {
    m_writeStmt = m_conn->createStatement();
    m_writeStmt->setSQL("INSERT INTO mon_timing_crystal_dat (iov_id, logic_id, "
			"timing_mean, timing_rms ) "
			"VALUES (:iov_id, :logic_id, "
			":timing_mean, :timing_rms )");
  } catch (SQLException &e) {
    throw(runtime_error("MonTimingCrystalDat::prepareWrite():  "+e.getMessage()));
  }
}



void MonTimingCrystalDat::writeDB(const EcalLogicID* ecid, const MonTimingCrystalDat* item, MonRunIOV* iov)
  throw(runtime_error)
{
  this->checkConnection();
  this->checkPrepare();

  int iovID = iov->fetchID();
  if (!iovID) { throw(runtime_error("MonTimingCrystalDat::writeDB:  IOV not in DB")); }

  int logicID = ecid->getLogicID();
  if (!logicID) { throw(runtime_error("MonTimingCrystalDat::writeDB:  Bad EcalLogicID")); }
  
  try {
    m_writeStmt->setInt(1, iovID);
    m_writeStmt->setInt(2, logicID);

    m_writeStmt->setFloat(3, item->getTimingMean() );
    m_writeStmt->setFloat(4, item->getTimingRMS() );


    m_writeStmt->executeUpdate();
  } catch (SQLException &e) {
    throw(runtime_error("MonTimingCrystalDat::writeDB():  "+e.getMessage()));
  }
}



void MonTimingCrystalDat::fetchData(std::map< EcalLogicID, MonTimingCrystalDat >* fillMap, MonRunIOV* iov)
  throw(runtime_error)
{
  this->checkConnection();
  fillMap->clear();

  iov->setConnection(m_env, m_conn);
  int iovID = iov->fetchID();
  if (!iovID) { 
    //  throw(runtime_error("MonTimingCrystalDat::writeDB:  IOV not in DB")); 
    return;
  }

  try {

    m_readStmt->setSQL("SELECT cv.name, cv.logic_id, cv.id1, cv.id2, cv.id3, cv.maps_to, "
		 "d.timing_mean, d.timing_rms "
		 "FROM channelview cv JOIN mon_timing_crystal_dat d "
		 "ON cv.logic_id = d.logic_id AND cv.name = cv.maps_to "
		 "WHERE d.iov_id = :iov_id");
    m_readStmt->setInt(1, iovID);
    ResultSet* rset = m_readStmt->executeQuery();
    
    std::pair< EcalLogicID, MonTimingCrystalDat > p;
    MonTimingCrystalDat dat;
    while(rset->next()) {
      p.first = EcalLogicID( rset->getString(1),     // name
			     rset->getInt(2),        // logic_id
			     rset->getInt(3),        // id1
			     rset->getInt(4),        // id2
			     rset->getInt(5),        // id3
			     rset->getString(6));    // maps_to

      dat.setTimingMean( rset->getFloat(7) );
      dat.setTimingRMS( rset->getFloat(8) );

      p.second = dat;
      fillMap->insert(p);
    }


  } catch (SQLException &e) {
    throw(runtime_error("MonTimingCrystalDat::fetchData():  "+e.getMessage()));
  }
}

void MonTimingCrystalDat::writeArrayDB(const std::map< EcalLogicID, MonTimingCrystalDat >* data, MonRunIOV* iov)
  throw(runtime_error)
{
  this->checkConnection();
  this->checkPrepare();

  int iovID = iov->fetchID();
  if (!iovID) { throw(runtime_error("MonTimingCrystalDat::writeArrayDB:  IOV not in DB")); }


  int nrows=data->size(); 
  int* ids= new int[nrows];
  int* iovid_vec= new int[nrows];
  float* xx= new float[nrows];
  float* yy= new float[nrows];

  ub2* ids_len= new ub2[nrows];
  ub2* iov_len= new ub2[nrows];
  ub2* x_len= new ub2[nrows];
  ub2* y_len= new ub2[nrows];

  const EcalLogicID* channel;
  const MonTimingCrystalDat* dataitem;
  int count=0;
  typedef map< EcalLogicID, MonTimingCrystalDat >::const_iterator CI;
  for (CI p = data->begin(); p != data->end(); ++p) {
        channel = &(p->first);
	int logicID = channel->getLogicID();
	if (!logicID) { throw(runtime_error("MonTimingCrystalDat::writeArrayDB:  Bad EcalLogicID")); }
	ids[count]=logicID;
	iovid_vec[count]=iovID;

	dataitem = &(p->second);
	// dataIface.writeDB( channel, dataitem, iov);
	float x=dataitem->getTimingMean();
	float y=dataitem->getTimingRMS();

	xx[count]=x;
	yy[count]=y;

	ids_len[count]=sizeof(ids[count]);
	iov_len[count]=sizeof(iovid_vec[count]);
	
	x_len[count]=sizeof(xx[count]);
	y_len[count]=sizeof(yy[count]);

	count++;
     }


  try {
    m_writeStmt->setDataBuffer(1, (dvoid*)iovid_vec, OCCIINT, sizeof(iovid_vec[0]),iov_len);
    m_writeStmt->setDataBuffer(2, (dvoid*)ids, OCCIINT, sizeof(ids[0]), ids_len );
    m_writeStmt->setDataBuffer(3, (dvoid*)xx, OCCIFLOAT , sizeof(xx[0]), x_len );
    m_writeStmt->setDataBuffer(4, (dvoid*)yy, OCCIFLOAT , sizeof(yy[0]), y_len );

    m_writeStmt->executeArrayUpdate(nrows);

    delete [] ids;
    delete [] iovid_vec;
    delete [] xx;
    delete [] yy;

    delete [] ids_len;
    delete [] iov_len;
    delete [] x_len;
    delete [] y_len;

  } catch (SQLException &e) {
    throw(runtime_error("MonTimingCrystalDat::writeArrayDB():  "+e.getMessage()));
  }
}
