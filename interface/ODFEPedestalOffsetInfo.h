#ifndef ODFEPEDOFFINFO_H
#define ODFEPEDOFFINFO_H

#include <map>
#include <string>

#include "OnlineDB/EcalCondDB/interface/IODConfig.h"

class ODFEPedestalOffsetInfo : public IODConfig {
 public:
  friend class EcalCondDBInterface;
  ODFEPedestalOffsetInfo();
  ~ODFEPedestalOffsetInfo();

  // User data methods
  inline std::string getTable() { return "PEDESTAL_OFFSETS_INFO"; }

  inline void setId(int id) { m_ID = id; }
  inline int getId() const { return m_ID; }

  // the tag is already in IODConfig 

  inline void setVersion(int id) { m_version = id; }
  inline int getVersion() const { return m_version; }

  int fetchNextId() throw(std::runtime_error);
  void setParameters(std::map<string,string> my_keys_map);
  int fetchID()  throw(std::runtime_error);
  
 private:
  void prepareWrite()  throw(std::runtime_error);

  void writeDB()       throw(std::runtime_error);

  void clear();

  void fetchData(ODFEPedestalOffsetInfo * result)     throw(std::runtime_error);
  void fetchLastData(ODFEPedestalOffsetInfo * result)     throw(std::runtime_error);



  // User data
  int m_ID;
  int m_version;
  
};

#endif