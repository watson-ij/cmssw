/** \class ME0SegmentProducer derived by CSCSegmentProducer 
 * Produces a collection of ME0Segment's in endcap muon ME0s. 
 *
 * \author Marcello Maggi
 */
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/GEMRecHit/interface/GEMRecHitCollection.h"
#include "DataFormats/GEMRecHit/interface/GEMSegmentCollection.h"
#include "DataFormats/GEMRecHit/interface/GEMSegment.h"
#include "DataFormats/Common/interface/Handle.h"

#include "RecoLocalMuon/GEMSegment/plugins/GEMSegmentBuilder.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"

class ME0SegmentProducer : public edm::stream::EDProducer<> {
public:
  /// Constructor
  explicit ME0SegmentProducer(const edm::ParameterSet&);
  /// Destructor
  ~ME0SegmentProducer() override {}
  /// Produce the ME0Segment collection
  void produce(edm::Event&, const edm::EventSetup&) override;

private:
  int iev;  // events through
  edm::EDGetTokenT<GEMRecHitCollection> theME0RecHitToken;
  std::unique_ptr<GEMSegmentBuilder> segmentBuilder_;
};

ME0SegmentProducer::ME0SegmentProducer(const edm::ParameterSet& ps) : iev(0) {
  theME0RecHitToken = consumes<GEMRecHitCollection>(ps.getParameter<edm::InputTag>("me0RecHitLabel"));
  segmentBuilder_ = std::make_unique<GEMSegmentBuilder>(ps);  // pass on the Parameter Set

  // register what this produces
  produces<GEMSegmentCollection>();
}

void ME0SegmentProducer::produce(edm::Event& ev, const edm::EventSetup& setup) {
  LogDebug("ME0SegmentProducer") << "start producing segments for " << ++iev << "th event with ME0 data";

  // find the geometry (& conditions?) for this event & cache it in the builder
  edm::ESHandle<GEMGeometry> me0g;
  setup.get<MuonGeometryRecord>().get(me0g);
  const GEMGeometry* mgeom = &*me0g;
  segmentBuilder_->setGeometry(mgeom);

  // get the collection of ME0RecHit
  edm::Handle<GEMRecHitCollection> me0RecHits;
  ev.getByToken(theME0RecHitToken, me0RecHits);

  // create empty collection of Segments
  auto oc = std::make_unique<GEMSegmentCollection>();

  // fill the collection
  segmentBuilder_->build(me0RecHits.product(), *oc);  //@@ FILL oc

  // put collection in event
  ev.put(std::move(oc));
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(ME0SegmentProducer);
