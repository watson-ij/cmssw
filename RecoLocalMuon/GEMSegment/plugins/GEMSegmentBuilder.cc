#include "RecoLocalMuon/GEMSegment/plugins/GEMSegmentBuilder.h"
#include "DataFormats/MuonDetId/interface/GEMDetId.h"
#include "DataFormats/GEMRecHit/interface/GEMRecHit.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"
#include "Geometry/GEMGeometry/interface/GEMEtaPartition.h"
#include "RecoLocalMuon/GEMSegment/plugins/GEMSegmentAlgorithmBase.h"
#include "RecoLocalMuon/GEMSegment/plugins/GEMSegmentBuilderPluginFactory.h"

#include "RecoLocalMuon/GEMSegment/plugins/ME0SegmentBuilder.h"
#include "RecoLocalMuon/GEMSegment/plugins/ME0SegmentAlgorithmBase.h"
#include "RecoLocalMuon/GEMSegment/plugins/ME0SegmentBuilderPluginFactory.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

GEMSegmentBuilder::GEMSegmentBuilder(const edm::ParameterSet& ps) : geom_(nullptr) {
  ////// ME0
  // Algo type (indexed)
  int me0chosenAlgo = ps.getParameter<int>("me0_algo_type") - 1;
  // Find appropriate ParameterSets for each algo type

  std::vector<edm::ParameterSet> me0algoPSets = ps.getParameter<std::vector<edm::ParameterSet> >("me0_algo_psets");

  edm::ParameterSet me0segAlgoPSet = me0algoPSets[me0chosenAlgo].getParameter<edm::ParameterSet>("algo_pset");
  std::string me0algoName = me0algoPSets[me0chosenAlgo].getParameter<std::string>("algo_name");
  LogDebug("ME0SegmentBuilder") << "ME0SegmentBuilder algorithm name: " << algoName;

  // Ask factory to build this algorithm, giving it appropriate ParameterSet
  me0algo = ME0SegmentBuilderPluginFactory::get()->create(me0algoName, me0segAlgoPSet);

  

  ////// GEM
  
  // Algo name
  algoName = ps.getParameter<std::string>("algo_name");

  edm::LogVerbatim("GEMSegmentBuilder") << "GEMSegmentBuilder algorithm name: " << algoName;

  // SegAlgo parameter set
  segAlgoPSet = ps.getParameter<edm::ParameterSet>("algo_pset");

  // Ask factory to build this algorithm, giving it appropriate ParameterSet
  algo = GEMSegmentBuilderPluginFactory::get()->create(algoName, segAlgoPSet);
}
GEMSegmentBuilder::~GEMSegmentBuilder() {}

void GEMSegmentBuilder::build(const GEMRecHitCollection* recHits, GEMSegmentCollection& oc) {
  std::cout << "Segment Builder " << std::endl;

  //// ME0


  std::map<GEMDetId, bool> foundChambers;
  for (GEMRecHitCollection::const_iterator it = recHits->begin(); it != recHits->end(); it++) {
    if (it->gemId().station() != 0) continue;

    std::cout << "  ME0 hit " << it->gemId() << std::endl;
    
    const auto chId = it->gemId().superChamberId();
    auto chIt = foundChambers.find(chId);
    if (chIt != foundChambers.end())
      continue;
    foundChambers[chId] = true;
    ME0SegmentAlgorithmBase::HitAndPositionContainer hitAndPositions;
    const GEMSuperChamber* chamber = geom_->superChamber(chId);
    for (GEMRecHitCollection::const_iterator it2 = it; it2 != recHits->end(); it2++) {
      if (it2->gemId().superChamberId() != chId)
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
    std::vector<GEMSegment> segv = me0algo->run(chamber, hitAndPositions);

    LogDebug("ME0Segment|ME0") << "found " << segv.size() << " segments in chamber " << chId;

    // Add the segments to master collection
    if (!segv.empty()) {
      std::cout << "  ME0 segment! " << std::endl;
      oc.put(chId, segv.begin(), segv.end());
    }
  }


  

  //// GEM
  
  edm::LogVerbatim("GEMSegmentBuilder") << "[GEMSegmentBuilder::build] Total number of rechits in this event: "
                                        << recHits->size();

  // Let's define the ensemble of GEM devices having the same region, chambers number (phi)
  // different eta partitions and different layers are allowed

  std::map<uint32_t, std::vector<const GEMRecHit*> > ensembleRH;

  // Loop on the GEM rechit and select the different GEM Ensemble
  for (GEMRecHitCollection::const_iterator it2 = recHits->begin(); it2 != recHits->end(); ++it2) {
    if (it2->gemId().station() == 0) continue;
    // GEM Ensemble is defined by assigning all the GEMDetIds of the same "superchamber"
    // (i.e. region same, chamber same) to the DetId of the first layer
    std::cout << "  gem hit " << it2->gemId().station() << " " << it2->gemId().chamber() << " " << it2->gemId().layer() << std::endl;

    // here a reference GEMDetId is created: named "id"
    // - Ring 1 (no other rings available for GEM)
    // - Layer 1 = reference layer (effective layermask)
    // - Roll 0  = reference roll  (effective rollmask)
    // - Station == 1 (GE1/1) or == 2 (GE2/1)
    // this reference id serves to link all GEMEtaPartitions
    // and will also be used to determine the GEMSuperChamber
    // to which the GEMSegment is assigned (done inside GEMSegAlgoXX)
    GEMDetId id(it2->gemId().superChamberId());
    // save current GEMRecHit in vector associated to the reference id
    ensembleRH[id.rawId()].push_back(&(*it2));
  }

  // Loop on the entire map <ref id, vector of GEMRecHits>
  for (auto enIt = ensembleRH.begin(); enIt != ensembleRH.end(); ++enIt) {
    std::vector<const GEMRecHit*> gemRecHits;
    std::map<uint32_t, const GEMEtaPartition*> ens;

    // all detIds have been assigned to the to chamber
    const GEMSuperChamber* chamber = geom_->superChamber(enIt->first);
    for (auto rechit = enIt->second.begin(); rechit != enIt->second.end(); ++rechit) {
      gemRecHits.push_back(*rechit);
      ens[(*rechit)->gemId()] = geom_->etaPartition((*rechit)->gemId());
    }

#ifdef EDM_ML_DEBUG  // have lines below only compiled when in debug mode
    LogTrace("GEMSegmentBuilder")
        << "[GEMSegmentBuilder::build] -----------------------------------------------------------------------------";
    LogTrace("GEMSegmentBuilder") << "[GEMSegmentBuilder::build] found " << gemRecHits.size()
                                  << " rechits in GEM Super Chamber " << chamber->id() << " ::";
    for (auto rh = gemRecHits.begin(); rh != gemRecHits.end(); ++rh) {
      auto gemid = (*rh)->gemId();
      // auto rhr = gemGeom->etaPartition(gemid);
      auto rhLP = (*rh)->localPosition();
      // auto rhGP = rhr->toGlobal(rhLP);
      // no sense to print local y because local y here is in the roll reference frame
      // in the roll reference frame the local y of a rechit is always the middle of the roll, and hence equal to 0.0
      LogTrace("GEMSegmentBuilder") << "[RecHit :: Loc x = " << std::showpos << std::setw(9)
                                    << rhLP.x() /*<<" Loc y = "<<std::showpos<<std::setw(9)<<rhLP.y()*/
                                    << " BX = " << (*rh)->BunchX() << " -- " << gemid.rawId() << " = " << gemid << " ]";
    }
#endif

    GEMSegmentAlgorithmBase::GEMEnsemble ensemble(
        std::pair<const GEMSuperChamber*, std::map<uint32_t, const GEMEtaPartition*> >(chamber, ens));

#ifdef EDM_ML_DEBUG  // have lines below only compiled when in debug mode
    LogTrace("GEMSegmentBuilder") << "[GEMSegmentBuilder::build] run the segment reconstruction algorithm now";
#endif

    // given the superchamber select the appropriate algo... and run it
    std::vector<GEMSegment> segv = algo->run(ensemble, gemRecHits);
#ifdef EDM_ML_DEBUG  // have lines below only compiled when in debug mode
    LogTrace("GEMSegmentBuilder") << "[GEMSegmentBuilder::build] found " << segv.size();
#endif

    GEMDetId mid(enIt->first);

#ifdef EDM_ML_DEBUG  // have lines below only compiled when in debug mode
    LogTrace("GEMSegmentBuilder") << "[GEMSegmentBuilder::build] found " << segv.size()
                                  << " segments in GEM Super Chamber " << mid;
    LogTrace("GEMSegmentBuilder")
        << "[GEMSegmentBuilder::build] -----------------------------------------------------------------------------";
#endif

    // Add the segments to master collection
    std::cout << "  GEM segment! " << std::endl;
    oc.put(mid, segv.begin(), segv.end());
  }
}

void GEMSegmentBuilder::setGeometry(const GEMGeometry* geom) { geom_ = geom; }
