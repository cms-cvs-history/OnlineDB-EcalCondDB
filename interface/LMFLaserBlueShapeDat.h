#ifndef LMFLASERBLUESHAPEDAT_H
#define LMFLASERBLUESHAPEDAT_H

#include <map>
#include <stdexcept>

#include "OnlineDB/EcalCondDB/interface/IDataItem.h"
#include "OnlineDB/EcalCondDB/interface/LMFRunTag.h"
#include "OnlineDB/EcalCondDB/interface/LMFRunIOV.h"
#include "OnlineDB/EcalCondDB/interface/EcalLogicID.h"

class LMFLaserBlueShapeDat : public IDataItem {
 public:
  friend class EcalCondDBInterface;
  LMFLaserBlueShapeDat();
  ~LMFLaserBlueShapeDat();

  // User data methods
  inline void setAlpha(float alpha) { m_alpha = alpha; }
  inline float getAlpha() const { return m_alpha; }

  inline void setAlphaRMS(float rms) { m_alphaRMS = rms; }
  inline float getAlphaRMS() const { return m_alphaRMS; }

  inline void setBeta(float beta) { m_beta = beta; }
  inline float getBeta() const { return m_beta; }

  inline void setBetaRMS(float rms) { m_betaRMS = rms; }
  inline float getBetaRMS() const { return m_betaRMS; }

  
 private:
  void prepareWrite() 
    throw(std::runtime_error);

  void writeDB(const EcalLogicID* ecid, const LMFLaserBlueShapeDat* item, LMFRunIOV* iov)
    throw(std::runtime_error);

  void fetchData(std::map< EcalLogicID, LMFLaserBlueShapeDat >* fillVec, LMFRunIOV* iov)
     throw(std::runtime_error);

  // User data
  float m_alpha;
  float m_alphaRMS;
  float m_beta;
  float m_betaRMS;
  
};

#endif
