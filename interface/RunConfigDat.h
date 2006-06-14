#ifndef RUNCONFGDAT_H
#define RUNCONFIGDAT_H

#include <vector>
#include <stdexcept>

#include "OnlineDB/EcalCondDB/interface/IDataItem.h"
#include "OnlineDB/EcalCondDB/interface/RunIOV.h"
#include "OnlineDB/EcalCondDB/interface/EcalLogicID.h"

class RunConfigDat : public IDataItem {
 public:
  friend class EcalCondDBInterface;
  RunConfigDat();
  ~RunConfigDat();

  // User data methods
  std::string getConfigTag() const { return m_configTag; }
  void setConfigTag(std::string tag) { m_configTag = tag; }

  int getConfigVersion() const { return m_configVer; }
  void setConfigVersion(int ver) { m_configTag = ver; }

 private:
  void prepareWrite() 
    throw(std::runtime_error);

  void writeDB(const EcalLogicID* ecid, const RunConfigDat* item, RunIOV* iov )
    throw(std::runtime_error);

  void fetchData(std::map< EcalLogicID, RunConfigDat >* fillMap, RunIOV* iov)
     throw(std::runtime_error);

  // User data
  std::string m_configTag;
  int m_configVer;

};

#endif
