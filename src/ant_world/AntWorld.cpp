//
// Created by reuben on 16/3/17.
//

#include "ant_world/AntWorld.hpp"

AntWorld::AntWorld(int width, int height, int seed) : environment((short) width, (short) height) {
	srand(seed);
	environment = Environment((short) width, (short) height);
	openCVEnvironment = new ui::OpenCV(this->environment);

	checkpointLocation = ".";
	iteration = 0;
	mutationPeriod = 500;
	minimumPopulation = (unsigned int) width;
	displayPeriod = 1;
	checkpointPeriod = 5000;

	environment.randomize();

	mutationEnabled = false;
	minimumPopulationEnabled = displayEnabled = fileCheckpointsEnabled = _isRunning = true;

	waitPeriod = 250;
	deficit = 0;
	previousWaitStartTimestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
	);
}

AntWorld::~AntWorld() {
	delete openCVEnvironment;
}

bool AntWorld::isRunning() {
	return _isRunning;
}

unsigned long long AntWorld::getIteration() {
	return iteration;
}

void AntWorld::skipIterations(unsigned long long skip) {
	iteration += skip;
}

bool AntWorld::performIteration() {
	Energy priorEnergy = environment.getTotalEnergy();

	unsigned long long antCount = ants.size();
	for (unsigned long long j = 0; j < antCount; j++) {

		ants[j].observeEnvironment(environment);
		ants[j].senseObservation(environment);
		ants[j].selectAction();
		ants[j].performAction((Agent::Action) ((Ant::Action) ants[j].getSelectedAction()));

		if (mutationEnabled && iteration % mutationPeriod == 0) {
			ants[j].mutate();
		}

	}

	Ant::realizeAntsAction(ants, environment);

	if (environment.getTotalEnergy() != priorEnergy) {
//		AsciiEnvironment::displayEnergyDeltas(oldEnvironment.getEnvironment(), environment);
		throw runtime_error(string("Environment energy is not conserved in iteration ") + to_string(iteration)
							+ " from " + to_string(priorEnergy) + " to " + to_string(environment.getTotalEnergy()));
	}

	iteration++;
	return true;
}

void AntWorld::maintainMinimumPopulation() {
	if (minimumPopulationEnabled && ants.size() < minimumPopulation) {
		Ant::sparkNLives(environment, ants, (unsigned int) (minimumPopulation - ants.size()));
	}
}

void AntWorld::displayPeriodically() {
	if (displayEnabled && iteration % displayPeriod == 0) {
		openCVEnvironment->displayEnvironment(ants, iteration);
	}
}

void AntWorld::checkpointPeriodically() {
	if (fileCheckpointsEnabled && iteration % checkpointPeriod == 0) {
		saveToFile();
	}
}

void AntWorld::saveToFile() {
	string environmentFilePath = checkpointLocation + "/" + "environment" + to_string(iteration) + ".txt";
	cout << "Saving to file " << environmentFilePath << endl;
	ofstream environmentFile(environmentFilePath);
	environment.save(environmentFile);
	environmentFile.close();

	string antFilePath = checkpointLocation + "/" + "ants" + to_string(iteration) + ".txt";
	cout << "Saving to file " << antFilePath << endl;
	ofstream antFile(antFilePath);
	Ant::save(antFile, ants);
	antFile.close();
}

void AntWorld::waitOnePeriod() {
	if (char(27) == waitKey((int) waitPeriod)) {
		_isRunning = false;
	}
	previousWaitStartTimestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
	);
}

void AntWorld::waitRemainingPeriod() {
	std::chrono::milliseconds waitStartTimestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
	);
	std::chrono::milliseconds elapsed = waitStartTimestamp - previousWaitStartTimestamp;
	deficit += waitPeriod - elapsed.count();

	long remainingTime = std::min(waitPeriod, (unsigned long) std::max(deficit, (long) 1));
	cout << deficit << " deficit" << " and waiting " << remainingTime << endl;
	if (char(27) == waitKey((int) remainingTime)) {
		_isRunning = false;
	}
	previousWaitStartTimestamp = waitStartTimestamp;
}

bool AntWorld::loadFromFile(unsigned long long iteration) {
	try {
		string environmentFilePath = checkpointLocation + "/" + "environment" + to_string(iteration) + ".txt";
		string antFilePath = checkpointLocation + "/" + "ants" + to_string(iteration) + ".txt";
		cout << "Loading from file " << environmentFilePath << endl;
		cout << "Loading from file " << antFilePath << endl;

		ifstream antFile(antFilePath);
		ifstream environmentFile(environmentFilePath);
		if (!environmentFile.is_open())
			return false;
		if (!antFile.is_open())
			return false;

		if (!environment.load(environmentFile))
			return false;
		if (!Ant::load(antFile, environment, ants))
			return false;
		this->iteration = iteration;

		return true;
	} catch (exception &e) {
		cout << e.what() << endl;
		return false;
	}
}

void AntWorld::setMutationEnabled(bool enabled) {
	mutationEnabled = enabled;
}

void AntWorld::setMutationPeriod(unsigned int period) {
	mutationPeriod = period;
}

void AntWorld::setMinimumPopulationEnabled(bool enabled) {
	minimumPopulationEnabled = enabled;
}

bool AntWorld::getMinimumPopulationEnabled() {
	return minimumPopulationEnabled;
}

void AntWorld::setMinimumPopulation(unsigned int minPopulation) {
	minimumPopulation = minPopulation;
}

unsigned int AntWorld::getMinimumPopulation() {
	return minimumPopulation;
}

void AntWorld::setDisplayEnabled(bool enabled) {
	displayEnabled = enabled;
}

bool AntWorld::getDisplayEnabled() {
	return displayEnabled;
}

void AntWorld::setDisplayPeriod(unsigned int period) {
	displayPeriod = period;
}

unsigned int AntWorld::getDisplayPeriod() {
	return displayPeriod;
}

void AntWorld::setFileCheckpointsEnabled(bool enabled) {
	fileCheckpointsEnabled = enabled;
}

bool AntWorld::getFileCheckpointsEnabled() {
	return fileCheckpointsEnabled;
}

void AntWorld::setCheckpointPeriod(unsigned int period) {
	checkpointPeriod = period;
}

unsigned int AntWorld::getCheckpointPeriod() {
	return checkpointPeriod;
}

void AntWorld::setWaitPeriod(unsigned long period) {
	waitPeriod = period;
}

unsigned long AntWorld::getWaitPeriod() {
	return waitPeriod;
}

void AntWorld::setCheckpointLocation(string path) {
	checkpointLocation = path;
}

string AntWorld::getCheckpointLocation() {
	return checkpointLocation;
}