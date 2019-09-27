#ifndef GEMRecHit_ME0SegmentAlgorithmBase_h
#define GEMRecHit_ME0SegmentAlgorithmBase_h

/** \class ME0SegmentAlgo derived from CSC
 * An abstract base class for algorithmic classes used to
 * build segments in one ensemble of ME0 detector
 *
 * Implementation notes: <BR>
 * For example, ME0SegmAlgoMM inherits from this class,
 *
 * \author Marcello Maggi
 *
 */

#include "DataFormats/GEMRecHit/interface/GEMRecHitCollection.h"
#include "DataFormats/GEMRecHit/interface/GEMSegment.h"
#include "Geometry/GEMGeometry/interface/GEMEtaPartition.h"
#include "Geometry/GEMGeometry/interface/GEMSuperChamber.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include <map>
#include <vector>

class ME0SegmentAlgorithmBase {
public:
  struct HitAndPosition {
    HitAndPosition(const GEMRecHit* rh, const LocalPoint& lp, const GlobalPoint& gp, unsigned int idx)
        : rh(rh), lp(lp), gp(gp), layer(rh->gemId().layer()), idx(idx) {}
    const GEMRecHit* rh;
    LocalPoint lp;
    GlobalPoint gp;
    unsigned int layer;
    unsigned int idx;
  };

  typedef std::vector<HitAndPosition> HitAndPositionContainer;
  typedef std::vector<const HitAndPosition*> HitAndPositionPtrContainer;

  /// Constructor
  explicit ME0SegmentAlgorithmBase(const edm::ParameterSet&){};
  /// Destructor
  virtual ~ME0SegmentAlgorithmBase(){};

  /** Run the algorithm = build the segments in this chamber
    */
  virtual std::vector<GEMSegment> run(const GEMSuperChamber* chamber, const HitAndPositionContainer& rechits) = 0;

private:
};
#endif
