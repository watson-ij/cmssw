#ifndef GEMRecHit_ME0SegmentBuilder_h
#define GEMRecHit_ME0SegmentBuilder_h

/** \class ME0SegmentBuilder derived by CSCSegmentBuilder
 * Algorithm to build ME0Segment's from ME0RecHit collection
 * by implementing a 'build' function required by ME0SegmentProducer.
 *
 * Implementation notes: <BR>
 * Configured via the Producer's ParameterSet. <BR>
 * Presume this might become an abstract base class one day. <BR>
 *
 * $Date: 2014/02/04 13:45:31 $
 * $Revision: 1.1 $
 * \author Marcello Maggi
 *
 */

#include "DataFormats/GEMRecHit/interface/GEMRecHitCollection.h"
#include "DataFormats/GEMRecHit/interface/GEMSegmentCollection.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class ME0SegmentAlgorithmBase;

class ME0SegmentBuilder {
public:
  /** Configure the algorithm via ctor.
   * Receives ParameterSet percolated down from EDProducer
   * which owns this Builder.
   */
  explicit ME0SegmentBuilder(const edm::ParameterSet&);
  /// Destructor
  ~ME0SegmentBuilder();

  /** Find rechits in each ensemble of all ME0 layers, build ME0Segment's ,
   *  and fill into output collection.
   */
  void build(const GEMRecHitCollection* rechits, GEMSegmentCollection& oc);

  /** Cache pointer to geometry _for current event_
   */
  void setGeometry(const GEMGeometry* g);

private:
  std::unique_ptr<ME0SegmentAlgorithmBase> algo;
  const GEMGeometry* geom_;
};

#endif
