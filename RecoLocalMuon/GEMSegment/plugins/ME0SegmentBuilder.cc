#include "RecoLocalMuon/GEMSegment/plugins/ME0SegmentBuilder.h"
#include "DataFormats/MuonDetId/interface/GEMDetId.h"
#include "DataFormats/GEMRecHit/interface/GEMRecHit.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"
#include "Geometry/GEMGeometry/interface/GEMEtaPartition.h"
#include "Geometry/GEMGeometry/interface/GEMSuperChamber.h"
#include "RecoLocalMuon/GEMSegment/plugins/ME0SegmentAlgorithmBase.h"
#include "RecoLocalMuon/GEMSegment/plugins/ME0SegmentBuilderPluginFactory.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

ME0SegmentBuilder::ME0SegmentBuilder(const edm::ParameterSet& ps) : geom_(nullptr) {
  // Algo type (indexed)
  int chosenAlgo = ps.getParameter<int>("algo_type") - 1;
  // Find appropriate ParameterSets for each algo type

  std::vector<edm::ParameterSet> algoPSets = ps.getParameter<std::vector<edm::ParameterSet> >("algo_psets");

  edm::ParameterSet segAlgoPSet = algoPSets[chosenAlgo].getParameter<edm::ParameterSet>("algo_pset");
  std::string algoName = algoPSets[chosenAlgo].getParameter<std::string>("algo_name");
  LogDebug("ME0SegmentBuilder") << "ME0SegmentBuilder algorithm name: " << algoName;

  // Ask factory to build this algorithm, giving it appropriate ParameterSet
  algo = ME0SegmentBuilderPluginFactory::get()->create(algoName, segAlgoPSet);
}

ME0SegmentBuilder::~ME0SegmentBuilder() {}

void ME0SegmentBuilder::build(const GEMRecHitCollection* recHits, GEMSegmentCollection& oc) {
  LogDebug("ME0SegmentBuilder") << "Total number of rechits in this event: " << recHits->size();

  std::map<GEMDetId, bool> foundChambers;
  for (GEMRecHitCollection::const_iterator it = recHits->begin(); it != recHits->end(); it++) {
    const auto chId = it->gemId().chamberId();
    auto chIt = foundChambers.find(chId);
    if (chIt != foundChambers.end())
      continue;
    foundChambers[chId] = true;
    ME0SegmentAlgorithmBase::HitAndPositionContainer hitAndPositions;
    const GEMSuperChamber* chamber = geom_->superChamber(chId);
    for (GEMRecHitCollection::const_iterator it2 = it; it2 != recHits->end(); it2++) {
      if (it2->gemId().chamberId() != chId)
        continue;

      const auto* part = geom_->etaPartition(it2->gemId());
      GlobalPoint glb = part->toGlobal(it2->localPosition());
      LocalPoint nLoc = chamber->toLocal(glb);
      hitAndPositions.emplace_back(&(*it2), nLoc, glb, hitAndPositions.size());
    }

    LogDebug("ME0Segment|ME0") << "found " << hitAndPositions.size() << " rechits in chamber " << chId;
    //sort by layer
    auto getLayer = [&](int iL) -> const GEMChamber* {  //function is broken in the geo currently
      for (auto layer : chamber->chambers()) {
        if (layer->id().layer() == iL)
          return layer;
      }
      return nullptr;
    };
    float z1 = getLayer(1)->position().z();
    float z6 = getLayer(6)->position().z();
    if (z1 < z6)
      std::sort(hitAndPositions.begin(),
                hitAndPositions.end(),
                [](const ME0SegmentAlgorithmBase::HitAndPosition& h1,
                   const ME0SegmentAlgorithmBase::HitAndPosition& h2) { return h1.layer < h2.layer; });
    else
      std::sort(hitAndPositions.begin(),
                hitAndPositions.end(),
                [](const ME0SegmentAlgorithmBase::HitAndPosition& h1,
                   const ME0SegmentAlgorithmBase::HitAndPosition& h2) { return h1.layer > h2.layer; });

    // given the chamber select the appropriate algo... and run it
    std::vector<GEMSegment> segv = algo->run(chamber, hitAndPositions);

    LogDebug("ME0Segment|ME0") << "found " << segv.size() << " segments in chamber " << chId;

    // Add the segments to master collection
    if (!segv.empty())
      oc.put(chId, segv.begin(), segv.end());
  }
}

void ME0SegmentBuilder::setGeometry(const GEMGeometry* geom) { geom_ = geom; }
