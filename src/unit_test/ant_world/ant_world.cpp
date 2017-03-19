//
// Created by reuben on 16/3/17.
//

#include <ant_world/AntWorld.hpp>

int main(int argc, char *argv[]) {
	AntWorld antWorld(50, 50);
	antWorld.setMinimumPopulation(50);
	antWorld.setDisplayPeriod(5);
	antWorld.setCheckpointPeriod(10000);
	antWorld.setWaitPeriod(1);
	antWorld.setMutationEnabled(true);
	antWorld.setCrossoverEnabled(false);
	antWorld.setCheckpointLocation("./checkpoints");

//	if (!antWorld.loadFromFile(100)) {
//		cout << "Failed to load from file\n";
//		return 0;
//	}

	while (antWorld.isRunning()) {
		antWorld.maintainMinimumPopulation();
		antWorld.performIteration();
		antWorld.displayPeriodically();
		antWorld.checkpointPeriodically();
		antWorld.waitOnePeriod();
	}
	return 0;
}