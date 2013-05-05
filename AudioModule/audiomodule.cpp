#include "audiomodule.h"

extern "C" IModule* create () {
	return new AudioModule();
}

extern "C" void destroy (IModule* module) {
	delete module;
}

const std::string AudioModule::getName() const { return "Audio Module"; }

void AudioModule::onNewPacket(Packet *p)  {
	if (!p) { *this << WARNING << "Empty packet!" << std::endl; return; }

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
			*this << DEBUG << "Nothing to do (id=" << p->getId() << ")" << std::endl;
			break;
	}
}

void AudioModule::onTrainRequest(Packet* p) {
	TrainRequestPacket trp(p);
	if (!trp.getTrainDataSize()) {
		*this << WARNING << "data size = 0 (id=" << trp.getId() << ")" << std::endl;
		return;
	}
	*this << DEBUG << "Train request for "
								 << "\"" << trp.getPerson()->getId() << "\"" << std::endl;

	std::string id = trp.getPerson()->getId();
	std::replace(id.begin(), id.end(), ' ', '_');

	AudioSample sample(trp.getTrainData(), trp.getTrainDataSize(), AUDIO_FORMAT_WAVE, getFolder());
	sample.doTrain(id);

	TrainResultPacket reponse(&trp);
	reponse.doSend();
}

void AudioModule::onScoreRequest(Packet* p) {
	ScoreRequestPacket srp(p);
	if (!srp.getBodySize()) {
		*this << WARNING << "data size = 0 (id=" << srp.getId() << ")" << std::endl;
		return;
	}
	*this << DEBUG << "Score request (id=" << srp.getId() << ")" << std::endl;

	AudioSample sample(srp.getData(), srp.getBodySize(), AUDIO_FORMAT_WAVE, getFolder());
	sample.doTest()->getScoring();

	ScoreResultPacket reponse(&srp);
	reponse.setScoringVector(sample.getScoring())->doSend();
}

void AudioModule::onListRequest(Packet *p) {
	ListResultPacket lrp(p);
	lrp.setPeopleVector(new PeopleVector())->doSend();
}
