#ifndef DetLayers_GemDetRing_H
#define DetLayers_GemDetRing_H

/** \class GemDetRing
 *
 *  A ring of periodic, possibly overlapping vertical detectors.
 *  Designed for forward muon CSC/RPC chambers, with updates by IJW
 *  for GEM forward GEM detectors which may not form a complete ring.
 *
 *  \author I.J. Watson - USeoul
 *  \author N. Amapane - INFN Torino
 */

#include "TrackingTools/DetLayers/interface/ForwardDetRingOneZ.h"
#include "Utilities/BinningTools/interface/PeriodicBinFinderInPhi.h"

class GeomDet;

class GemDetRing : public ForwardDetRingOneZ {
 public:

  /// Construct from iterators on GeomDet*
  GemDetRing(std::vector<const GeomDet*>::const_iterator first,
	     std::vector<const GeomDet*>::const_iterator last,
	     bool _complete);

  /// Construct from a vector of GeomDet*
  GemDetRing(const std::vector<const GeomDet*>& dets, bool _complete);

  ~GemDetRing() override;

  // GeometricSearchDet interface

  const std::vector<const GeometricSearchDet*>& components() const override;

  std::pair<bool, TrajectoryStateOnSurface>
  compatible( const TrajectoryStateOnSurface& ts, const Propagator& prop, 
	      const MeasurementEstimator& est) const override;

  std::vector<DetWithState> 
  compatibleDets( const TrajectoryStateOnSurface& startingState,
		  const Propagator& prop, 
		  const MeasurementEstimator& est) const override;

  std::vector<DetGroup> 
  groupedCompatibleDets( const TrajectoryStateOnSurface& startingState,
			 const Propagator& prop,
			 const MeasurementEstimator& est) const override;


 private:
  typedef PeriodicBinFinderInPhi<float>   BinFinderType;
  BinFinderType theBinFinder;

  bool complete; /// Do the GEMs form a complete ring; they won't for slice test
  void init();

};
#endif

