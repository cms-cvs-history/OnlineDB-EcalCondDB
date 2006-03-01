#ifndef MONPEDESTALSONLINEDAT_H
#define MONPEDESTALSONLINEDAT_H

#include <map>
#include <stdexcept>

#include "OnlineDB/EcalCondDB/interface/IDataItem.h"
#include "OnlineDB/EcalCondDB/interface/MonRunTag.h"
#include "OnlineDB/EcalCondDB/interface/MonRunIOV.h"
#include "OnlineDB/EcalCondDB/interface/EcalLogicID.h"

class MonPedestalsOnlineDat : public IDataItem {
 public:
  friend class EcalCondDBInterface;
  MonPedestalsOnlineDat();
  ~MonPedestalsOnlineDat();

  // User data methods
  inline void setADCMeanG12(float mean) { m_adcMeanG12 = mean; }
  inline float getADCMeanG12() const { return m_adcMeanG12; }

  inline void setADCRMSG12(float rms) { m_adcRMSG12 = rms; }
  inline float getADCRMSG12() const { return m_adcRMSG12; }

  inline void setTaskStatus(bool status) { m_taskStatus = status; }
  inline bool getTaskStatus() const { return m_taskStatus; }

 private:
  void prepareWrite() 
    throw(std::runtime_error);

  void writeDB(const EcalLogicID* ecid, const MonPedestalsOnlineDat* item, MonRunIOV* iov )
    throw(std::runtime_error);

  void fetchData(std::map< EcalLogicID, MonPedestalsOnlineDat >* fillMap, MonRunIOV* iov)
     throw(std::runtime_error);

  // User data
  float m_adcMeanG12;
  float m_adcRMSG12;
  bool m_taskStatus;
};

#endif
