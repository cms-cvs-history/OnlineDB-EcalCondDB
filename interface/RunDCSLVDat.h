#ifndef RUNDCSLVEBDAT_H
#define RUNDCSLVEBDAT_H

#include <vector>
#include <stdexcept>

#include "OnlineDB/EcalCondDB/interface/IDataItem.h"
#include "OnlineDB/EcalCondDB/interface/RunIOV.h"
#include "OnlineDB/EcalCondDB/interface/EcalLogicID.h"
#include "OnlineDB/EcalCondDB/interface/Tm.h"
#include "OnlineDB/EcalCondDB/interface/DateHandler.h"
#include "OnlineDB/Oracle/interface/Oracle.h"

using namespace oracle::occi;

class RunDCSLVDat : public IDataItem {
 public:

  static const int maxDifference = 30*60*1000000; // 30 minutes
  static const int maxLVDifferenceEB = 1000;      // max LV tolerance in mV for EB
  static const int maxLVDifferenceEE = 1000;      // max LV tolerance in mV for EE
  static const int minLV = 2000;                  // if LV less than this value (in mV) LV is off
  
  static const int LVNOTNOMINAL = 1;
  static const int LVOFF        = 2;

  friend class EcalCondDBInterface;
  RunDCSLVDat();
  ~RunDCSLVDat();

  // User data methods
  inline std::string getTable() { return ""; }
  inline std::string getEBAccount() { return "CMS_ECAL_LV_PVSS_COND"; }
  inline std::string getEEAccount() { return "CMS_ECAL_LV_PVSS_COND"; }
  inline void setLV(float t) { m_lv = t; }
  inline void setStatus(int t) { m_status = t; }
  inline void setLVNominal(float t) { m_lvnom = t; }
  inline float getLV() const { return m_lv; }
  inline float getLVNominal() const { return m_lvnom; }
  inline int getStatus() const { return m_status; }
  int getTimeStatus() {return m_tstatus;}
  void setTimeStatus(int t ) {m_tstatus=t; } 

 private:
  void setStatusForBarrel(RunDCSLVDat&, Tm);
  void setStatusForEndcaps(RunDCSLVDat&, Tm);
  ResultSet* getBarrelRset();
  ResultSet* getEndcapRset();
  int nowMicroseconds();
  void fillTheMap(ResultSet *, std::map< EcalLogicID, RunDCSLVDat >* );
  void prepareWrite() 
    throw(std::runtime_error);

  void writeDB(const EcalLogicID* ecid, const RunDCSLVDat* item, RunIOV* iov )
    throw(std::runtime_error);

  void fetchData(std::map< EcalLogicID, RunDCSLVDat >* fillMap, RunIOV* iov)
     throw(std::runtime_error);

  void fetchLastData(std::map< EcalLogicID, RunDCSLVDat >* fillMap)
     throw(std::runtime_error);

  // User data
  float m_lv;
  float m_lvnom;
  int m_status;
  int m_tstatus;
};

#endif