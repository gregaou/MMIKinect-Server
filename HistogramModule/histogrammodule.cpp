#include "histogrammodule.h"

extern "C" IModule* create () {
	return new HistogramModule();
}

extern "C" void destroy (IModule* module) {
	delete module;
}

const std::string HistogramModule::getName() const { return "Histogram Module"; }

void HistogramModule::onNewPacket(Packet *p) {
	if (!p) { *this << WARNING << "Empty packet!" << std::endl; return; }

	uint16 action = p->getType() & 0x0F;
	uint16 type = p->getType() & 0xF0;

	if (type != HISTOGRAM_TYPE && type != BROADCAST_TYPE) { return; }

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

void HistogramModule::onTrainRequest(Packet* p) {
	TrainRequestPacket trp(p);
	if (!trp.getTrainDataSize()) {
		*this << WARNING << "data size = 0"
										 << " (id=" << trp.getId() << ")" << std::endl;
		return;
	}
	*this << DEBUG << "Train request for \"" << trp.getPerson()->getId() << "\""
								 << " (id=" << p->getId() << ")" << std::endl;

	std::string id = trp.getPerson()->getId();
	std::replace(id.begin(), id.end(), ' ', '_');

	*this << DEBUG << "Creating Histogram Recognizer "
								 << "(id=" << p->getId() << ")" << std::endl;
	HistogramRecognizer hr(trp.getTrainData(), trp.getTrainDataSize(), getFolder());
	*this << DEBUG << "Created"
								 << " (id=" << p->getId() << ")" << std::endl;

	*this << DEBUG << "Starting training"
								 << " (id=" << p->getId() << ")" << std::endl;
	hr.doTrain(id);
	*this << DEBUG << "Training done"
								 << " (id=" << p->getId() << ")" << std::endl;

	*this << DEBUG << "Creating answer"
								 << " (id=" << p->getId() << ")" << std::endl;
	TrainResultPacket reponse(&trp);
	*this << DEBUG << "Sending answer"
								 << " (id=" << p->getId() << ")" << std::endl;
	reponse.doSend();
	*this << DEBUG << "Answer sent"
								 << " (id=" << p->getId() << ")" << std::endl;
}

void HistogramModule::onScoreRequest(Packet* p) {
	ScoreRequestPacket srp(p);
	if (!srp.getBodySize()) {
		*this << WARNING << "data size = 0"
										 << " (id=" << p->getId() << ")" << std::endl;
		return;
	}
	*this << DEBUG << "Score request"
								 << " (id=" << p->getId() << ")" << std::endl;

	*this << DEBUG << "Creating Histogram Recognizer "
								 << "(id=" << p->getId() << ")" << std::endl;
	HistogramRecognizer hr(srp.getData(), srp.getBodySize(), getFolder());
	*this << DEBUG << "Created"
								 << " (id=" << p->getId() << ")" << std::endl;

	*this << DEBUG << "Starting Scoring"
								 << " (id=" << p->getId() << ")" << std::endl;
	hr.doTest();
	*this << DEBUG << "TScoringing done"
								 << " (id=" << p->getId() << ")" << std::endl;

	*this << DEBUG << "Creating answer"
								 << " (id=" << p->getId() << ")" << std::endl;
	ScoreResultPacket reponse(&srp);
	*this << DEBUG << "Sending answer"
								 << " (id=" << p->getId() << ")" << std::endl;
	reponse.setScoringVector(hr.getScoring())->doSend();
	*this << DEBUG << "Answer sent"
								 << " (id=" << p->getId() << ")" << std::endl;
}

void HistogramModule::onListRequest(Packet* p) {
	*this << DEBUG << "Creating answer (id=" << p->getId() << ")" << std::endl;
	ListResultPacket lrp(p);
	*this << DEBUG << "Sending answer (id=" << p->getId() << ")" << std::endl;
	lrp.setPeopleVector(HistogramRecognizer::getPeopleVector(getFolder()))->doSend();
	*this << DEBUG << "Answer sent (id=" << p->getId() << ")" << std::endl;
}
