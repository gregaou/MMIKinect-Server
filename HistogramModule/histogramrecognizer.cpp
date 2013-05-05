#include "histogramrecognizer.h"

HistogramRecognizer::HistogramRecognizer(uint8* data, int size, string folder)
	: _folder(folder), _histogram(data,size), _scoring(0) {}

HistogramRecognizer::HistogramRecognizer(string filename, string folder)
	: _folder(folder), _histogram(filename), _scoring(0) {}

HistogramRecognizer* HistogramRecognizer::doTrain (string id) {
	HistoPerson person = getHistoPerson(id);
	if (person.name != "" && person.filename != "") {
		Histogram h(_folder + person.filename);
		double weight = 1/(1+((double)person.size));
		h.update(_histogram, weight);
		person.size ++;
		h.saveToFile(_folder + person.filename);
	} else {
		person.name = id;
		person.filename = id + ".yaml";
		for (uint i = 0; i < person.filename.length(); ++i) {
			person.filename[i] = tolower(person.filename[i]);
		}
		person.size = 1;
		_histogram.saveToFile(_folder + person.filename);
	}
	updateRecord(person);

	return this;
}

HistogramRecognizer* HistogramRecognizer::doTest  () {
	vector<HistoPerson> vec = getHistoPeople();

	for (uint i = 0; i < vec.size(); ++i) {
		Histogram h(_folder + vec.at(i).filename);
		getScoring()->push_back(Score(vec.at(i).name, h.compare(_histogram)));
	}

	return this;
}

ScoringVector* HistogramRecognizer::getScoring() {
	if (!_scoring) {
		_scoring = new ScoringVector();
	}
	return _scoring;
}

PeopleVector* HistogramRecognizer::getPeopleVector (string folder) {
	string csvFile = folder + CSV_FILE;
	fstream csv(csvFile.c_str());
	string id;
	string line;
	PeopleVector* result = new PeopleVector();

	while (getline(csv,line)) {
		istringstream l(line);
		getline(l,id,';');
		result->push_back(Person(id));
	}
	csv.close();

	return result;
}

vector<HistoPerson> HistogramRecognizer::getHistoPeople () {
	string csvFile = _folder + CSV_FILE;
	fstream csv(csvFile.c_str());
	string line;
	vector<HistoPerson> result;

	while (getline(csv,line)) {
		istringstream l(line);
		HistoPerson person;
		getline(l,person.name,';');
		string filename;
		getline(l,filename,';');
		person.filename = filename;
		string size;
		getline(l,size);
		person.size = atoi(size.c_str());
		result.push_back(person);
	}
	csv.close();

	return result;
}

HistoPerson HistogramRecognizer::getHistoPerson (string id) {
	string csvFile = _folder + CSV_FILE;
	fstream csv(csvFile.c_str());
	string line;
	string name;
	HistoPerson person;

	person.name = "";
	person.filename = "";
	person.size = 0;

	while (getline(csv,line)) {
		istringstream l(line);
		getline(l,name,';');
		if (name == id) {
			person.name = name;
			string filename;
			getline(l,filename,';');
			person.filename = filename;
			string size;
			getline(l,size);
			person.size = atoi(size.c_str());
			break;
		}
	}
	csv.close();

	return person;
}


HistogramRecognizer* HistogramRecognizer::updateRecord(HistoPerson p) {
	string csvFile = _folder + CSV_FILE;
	fstream csv(csvFile.c_str());
	ostringstream out;
	string line;
	string id;
	bool found = false;

	while (getline(csv,line)) {
		istringstream l(line);
		getline(l,id,';');
		if (id == p.name) {
			found = true;
			out << p.name << ";" << p.filename << ";" << p.size << endl;
		} else {
			out << line << endl;
		}
	}
	if (!found) {
		out << p.name << ";" << p.filename << ";" << p.size << endl;
	}
	csv.close();
	csv.open(csvFile.c_str(), ios_base::out | ios_base::trunc);
	csv << out.str();
	csv.close();

	return this;
}
