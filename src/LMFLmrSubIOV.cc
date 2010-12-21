#include "OnlineDB/EcalCondDB/interface/LMFLmrSubIOV.h"

void LMFLmrSubIOV::init() {
  m_className = "LMFLmrSubIOV";

  m_lmfIOV = 0;
  m_t[0] = Tm();
  m_t[1] = Tm();
  m_t[2] = Tm();
}

LMFLmrSubIOV::LMFLmrSubIOV() {
  init();
}

LMFLmrSubIOV::LMFLmrSubIOV(EcalDBConnection *c): LMFUnique(c) {
  init();
}

LMFLmrSubIOV::LMFLmrSubIOV(oracle::occi::Environment* env,
			   oracle::occi::Connection* conn): LMFUnique(env, conn)
{
  init();
}

LMFLmrSubIOV::~LMFLmrSubIOV() {
}

LMFLmrSubIOV& LMFLmrSubIOV::setLMFIOV(const LMFIOV &iov) {
  m_lmfIOV = iov.getID();
  return *this;
}

LMFLmrSubIOV& LMFLmrSubIOV::setTimes(Tm *t) {
  m_t[0] = t[0];
  m_t[1] = t[1];
  m_t[2] = t[2];
  return *this;
}

LMFLmrSubIOV& LMFLmrSubIOV::setTimes(Tm t1, Tm t2, Tm t3) {
  m_t[0] = t1;
  m_t[1] = t2;
  m_t[2] = t3;
  return *this;
}

std::string LMFLmrSubIOV::fetchIdSql(Statement *stmt) {
  if (!m_lmfIOV) {
    if (m_debug) {
      std::cout << m_className << ": LMFIOV not set" << std::endl;
    }
    return "";
  }

  std::string sql = "SELECT LMR_SUB_IOV_ID FROM LMF_LMR_SUB_IOV "
    "WHERE "
    "IOV_ID  = :1 AND "
    "T1      = :2 AND "
    "T2      = :3 AND "
    "T3      = :4";
  stmt->setSQL(sql);
  stmt->setInt(1, m_lmfIOV);
  DateHandler dh(m_env, m_conn);
  for (int i = 0; i < 3; i++) {
    oracle::occi::Date t = dh.tmToDate(m_t[i]);
    stmt->setDate(i + 2, t);
  }
  return sql;

}

std::string LMFLmrSubIOV::setByIDSql(Statement *stmt, int id) {
  std::string sql = "SELECT IOV_ID, T1, T2, T3 FROM LMF_LMR_SUB_IOV "
    "WHERE LMR_SUB_IOV_ID = :1";
  stmt->setSQL(sql);
  stmt->setInt(1, id);
  return sql;
}

std::string LMFLmrSubIOV::writeDBSql(Statement *stmt) {
  // Validate the data, use infinity-till convention
  DateHandler dh(m_env, m_conn);

  for (int i = 0; i < 3; i++) {
    if (m_t[i].isNull()) {
      m_t[i] = dh.getPlusInfTm();
    }
  }

  if (m_lmfIOV == 0) {
    throw(std::runtime_error(m_className + "::writeDB: LMFIOV not set"));
  }
  std::string sql = "INSERT INTO LMF_LMR_SUB_IOV (LMR_SUB_IOV_ID, "
    "IOV_ID, T1, T2, T3) "
    "VALUES (LMF_LMR_SUB_IOV_ID_SQ.NextVal, :1, :2, :3, :4)";
  stmt->setSQL(sql);
  stmt->setInt(1, m_lmfIOV);
  for (int i = 0; i < 3; i++) {
    stmt->setDate(i + 2, dh.tmToDate(m_t[i]));
  }
  return sql;
}

void LMFLmrSubIOV::getParameters(ResultSet *rset) {
  DateHandler dh(m_env, m_conn);
  m_lmfIOV = rset->getInt(1);
  for (int i = 0; i < 3; i++) {
    oracle::occi::Date t = rset->getDate(i + 2);
    m_t[i] = dh.dateToTm(t);
  }
}

std::list<int> LMFLmrSubIOV::getIOVIDsLaterThan(const Tm &t) 
  throw(std::runtime_error) {
  std::string sql = "SELECT LMR_SUB_IOV_ID FROM LMF_LMR_SUB_IOV WHERE T3 > :1";
  std::list<int> ret; 
  if (m_conn != NULL) {
    try {
      DateHandler dh(m_env, m_conn);
      Statement *stmt = m_conn->createStatement();
      stmt->setPrefetchRowCount(131072);
      stmt->setSQL(sql);
      stmt->setDate(1, dh.tmToDate(t));
      ResultSet *rset = stmt->executeQuery();
      while (rset->next()) {
	ret.push_back(rset->getInt(1));
      }
      stmt->setPrefetchRowCount(0);
      m_conn->terminateStatement(stmt);
    }
    catch (oracle::occi::SQLException e) {
      throw(std::runtime_error(m_className + "::getLmrSubIOVLaterThan: " +
			       e.getMessage()));
    }
  } else {
    throw(std::runtime_error(m_className + "::getLmrSubIOVLaterThan: " +
			     "Connection not set"));
  }
  ret.sort();
  return ret;
}


