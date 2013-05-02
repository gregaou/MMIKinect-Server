#include "audiomodule.h"

extern "C" IModule* create () {
	return new AudioModule();
}

extern "C" void destroy (IModule* module) {
	delete module;
}

void AudioModule::onNewPacket(Packet *p)  {
	if (!p) { io::warn << "Audio Module : Empty packet!" << io::endl; return; }

	uint16 action = p->getType() & 0x0F;
	uint16 type = p->getType() & 0xF0;

	if (type != AUDIO_TYPE && type != BROADCAST_TYPE) { return; }

	switch (action) {
		case LISTING_REQUEST :
			onListRequest(p);
			break;
		case TRAINING_REQUEST :
			onTrainRequest(p);
			break;
		case SCORING_REQUEST :
		onScoreRequest(p);
			break;
		default :
			io::dbg << "Audio Module : Nothing to do (id=" << p->getId() << ")" << io::endl;
			break;
	}
}

void AudioModule::onTrainRequest(Packet* p) {
	TrainRequestPacket trp(p);
	if (!trp.getTrainDataSize()) {
		io::warn << "Audio Module : data size = 0 (id=" << trp.getId() << ")" << io::endl;
		return;
	}
	io::dbg << "Train request for "
					<< "\"" << trp.getPerson()->getId() << "\"" << io::endl;

	std::string id = trp.getPerson()->getId();
	std::replace(id.begin(), id.end(), ' ', '_');

	AudioSample sample(trp.getTrainData(), trp.getTrainDataSize(), AUDIO_FORMAT_WAVE);
	sample.doTrain(id);

	TrainResultPacket reponse(&trp);
	reponse.doSend();
}

void AudioModule::onScoreRequest(Packet* p) {
	ScoreRequestPacket srp(p);
	if (!srp.getBodySize()) {
		io::warn << "Audio Module : data size = 0 (id=" << srp.getId() << ")" << io::endl;
		return;
	}
	io::dbg << "Score request (id=" << srp.getId() << ")" << io::endl;

	AudioSample sample(srp.getData(), srp.getBodySize(), AUDIO_FORMAT_WAVE);
	sample.doTest()->getScoring();

	ScoreResultPacket reponse(&srp);
	reponse.setScoringVector(sample.getScoring())->doSend();
}

void AudioModule::onListRequest(Packet *p) {
	ListResultPacket lrp(p);
	lrp.setPeopleVector(new PeopleVector())->doSend();
}
