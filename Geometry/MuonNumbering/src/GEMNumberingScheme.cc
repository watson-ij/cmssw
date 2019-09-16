#include "Geometry/MuonNumbering/interface/GEMNumberingScheme.h"
#include "Geometry/MuonNumbering/interface/MuonBaseNumber.h"
#include "Geometry/MuonNumbering/interface/MuonDDDConstants.h"
#include "DataFormats/MuonDetId/interface/GEMDetId.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//#define LOCAL_DEBUG

GEMNumberingScheme::GEMNumberingScheme(const MuonDDDConstants& muonConstants) { initMe(muonConstants); }

GEMNumberingScheme::GEMNumberingScheme(const DDCompactView& cpv) {
  MuonDDDConstants muonConstants(cpv);
  initMe(muonConstants);
}

void GEMNumberingScheme::initMe(const MuonDDDConstants& muonConstants) {
  int theLevelPart = muonConstants.getValue("level");
  theRegionLevel = muonConstants.getValue("mg_region") / theLevelPart;
  theStationLevel = muonConstants.getValue("mg_station") / theLevelPart;
  theRingLevel = muonConstants.getValue("mg_ring") / theLevelPart;
  theSectorLevel = muonConstants.getValue("mg_sector") / theLevelPart;
  theRollLevel = muonConstants.getValue("mg_roll") / theLevelPart;

  theME0RegionLevel = muonConstants.getValue("m0_region") / theLevelPart;
  theME0LayerLevel = muonConstants.getValue("m0_layer") / theLevelPart;
  theME0SectorLevel = muonConstants.getValue("m0_sector") / theLevelPart;
  theME0RollLevel = muonConstants.getValue("m0_roll") / theLevelPart;
  theME0NEtaPart = muonConstants.getValue("m0_nroll");
#ifdef LOCAL_DEBUG
  edm::LogVerbatim("GEMNumberingScheme") << "Initialize GEMNumberingScheme"
                                         << "\ntheRegionLevel " << theRegionLevel << "\ntheStationLevel "
                                         << theStationLevel << "\ntheRingLevel " << theRingLevel << "\ntheSectorLevel "
                                         << theSectorLevel << "\ntheRollLevel " << theRollLevel;
#endif
}



int GEMNumberingScheme::baseNumberToUnitNumberME0(const MuonBaseNumber& num) {
  // for (auto& n : num.sortedBaseNumber)
  //   std::cout<<n.myLevel<<" "<<n.mySuper<<" "<<n.myBase<<", ";
  edm::LogVerbatim("ME0NumberingScheme") << "ME0NumberingScheme::baseNumberToUnitNumber BEGIN ";
  // Debug using LOCAL_DEBUG
#ifdef LOCAL_DEBUG
  edm::LogVerbatim("ME0NumberingScheme") << "ME0Numbering " << num.getLevels();
  for (int level = 1; level <= num.getLevels(); level++) {
    edm::LogVerbatim("ME0NumberingScheme")
        << "level " << level << " " << num.getSuperNo(level) << " " << num.getBaseNo(level);
  }
#endif
  // -----------------------

  int maxLevel = theME0RollLevel;
  if (num.getLevels() != maxLevel) {
    throw cms::Exception("MuonNumbering") << "MuonME0NS::BNToUN "
                                          << "BaseNumber has " << num.getLevels() << " levels,"
                                          << "need " << maxLevel << std::endl;
    return 0;
  }

  int region(0), layer(0), chamber(0), roll(0);

  //decode significant ME0 levels

  if (num.getBaseNo(theME0RegionLevel) == 0)
    region = 1;
  else
    region = -1;
  layer = num.getBaseNo(theME0LayerLevel) + 1;
  chamber = num.getBaseNo(theME0SectorLevel);
  roll = num.getBaseNo(theME0RollLevel) + 1;

  // collect all info

  // Debug using LOCAL_DEBUG
#ifdef LOCAL_DEBUG
  edm::LogVerbatim("ME0NumberingScheme") << "ME0NumberingScheme: Region " << region << " Layer " << layer << " Chamber "
                                         << chamber << " Roll " << roll;
#endif
  // -----------------------

  // std::cout << "ME0NumberingScheme: Region " << region
  // 	     << " Layer " << layer << " Chamber " << chamber << " Roll " << roll<<std::endl;

  // Build the actual numbering
  GEMDetId id(region, 1, 0, layer, chamber, roll);

  // Debug using LOCAL_DEBUG
#ifdef LOCAL_DEBUG
  edm::LogVerbatim("ME0NumberingScheme") << " DetId " << id;
#endif
  // ---------------------

  return id.rawId();
}

int GEMNumberingScheme::baseNumberToUnitNumber(const MuonBaseNumber& num) {
#ifdef LOCAL_DEBUG
  edm::LogVerbatim("GEMNumberingScheme") << "GEMNumbering " << num.getLevels();
  for (int level = 1; level <= num.getLevels(); level++) {
    edm::LogVerbatim("GEMNumberingScheme") << level << " " << num.getSuperNo(level) << " " << num.getBaseNo(level);
  }
#endif
  // for (auto& n : num.sortedBaseNumber)
  //   std::cout<<n.myLevel<<" "<<n.mySuper<<" "<<n.myBase<<", ";
  // std::cout << std::endl;
  
  int maxLevel = theRollLevel;
  if (num.getLevels() != maxLevel) {
    edm::LogWarning("GEMNumberingScheme")
        << "MuonGEMNumberingScheme::BNToUN: BaseNumber has " << num.getLevels() << " levels, need " << maxLevel;
    return 0;
  }

  // Seems to always be 0 for gem
  if (num.getBaseNo(theME0SectorLevel) > 0)
    return baseNumberToUnitNumberME0(num);
  
  int region(0), ring(0), station(0), layer(0), chamber(0), roll(0);

  //decode significant GEM levels

  if (num.getBaseNo(theRegionLevel) == 0)
    region = 1;
  else
    region = -1;

  // All GEM super chambers in stations 1 and 2 are on ring 1.
  // The long super chambers in station 2 are assigned *station 3* due
  // to the current limitation in the definition of the GEMDetId,
  // i.e. only 2 layers available per station.
  //  ring    = num.getSuperNo(theRingLevel);
  // GEM are only on the first ring
  ring = 1;
  station = num.getSuperNo(theStationLevel);
#ifdef LOCAL_DEBUG
  edm::LogVerbatim("GEMNumberingScheme") << "GEMNumbering: Ring " << ring << " Station "
                                         << num.getSuperNo(theStationLevel) << ":" << station;
#endif

  roll = num.getBaseNo(theRollLevel) + 1;
  const int copyno = num.getBaseNo(theSectorLevel) + 1;
  if (copyno < 50) {
    if (copyno % 2 == 0) {
      layer = 2;
      chamber = copyno - 1;
    } else {
      layer = 1;
      chamber = copyno;
    }
  } else {
    int copynp = copyno - 50;
    if (copynp % 2 != 0) {
      layer = 2;
      chamber = copynp - 1;
    } else {
      layer = 1;
      chamber = copynp;
    }
  }

  // collect all info

#ifdef LOCAL_DEBUG
  edm::LogVerbatim("GEMNumberingScheme") << "GEMNumberingScheme: Region " << region << " Ring " << ring << " Station "
                                         << station << " Layer " << layer << " Chamber " << chamber << " Roll " << roll;
#endif
  // std::cout << "GEMNumberingScheme: Region " << region << " Ring " << ring << " Station "
  // 	    << station << " Layer " << layer << " Chamber " << chamber << " Roll " << roll<<std::endl;

  // Build the actual numbering
  GEMDetId id(region, ring, station, layer, chamber, roll);

#ifdef LOCAL_DEBUG
  edm::LogVerbatim("GEMNumberingScheme") << id.rawId() << " DetId " << id;
#endif

  return id.rawId();
}
