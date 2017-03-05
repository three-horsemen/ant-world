//
// Created by reuben on 5/3/17.
//

#ifndef SFTP_ANT_HPP
#define SFTP_ANT_HPP

#include <shared/logger.hpp>
#include <shared/Utils.hpp>

#include <agent/Agent.hpp>
#include <agent/AgentCharacter.hpp>

#include <map/Coordinate.hpp>
#include <map/Energy.hpp>

using namespace std;

class Ant : public Agent {
protected:
	Coordinate coordinate;
	Energy potential;
	Energy shield;
	Energy fertility;
	Energy baby;
	AgentCharacter character;
public:
	enum Action {
		FORWARD,
		LEFT,
		RIGHT,
		EAT,
		ATTACK,
		FORTIFY,
		MATURE,
		GROW_BABY,
		GIVE_BIRTH
	};

	Ant(Environment &);

	bool isActionValid(Agent::Action action);

	void observeEnvironment(Environment &);

	PerceptiveField *getPerceptiveField();

	Coordinate getCoordinate();

	Energy getPotential();

	Energy getShield();

	Energy getFertility();

	Energy getBaby();

	AgentCharacter getCharacter();

	void setPotential(Energy newPotential);

	void setShield(Energy newShield);

	void setFertility(Energy newFertility);

	void setBaby(Energy newBaby);

	void setCharacter(AgentCharacter newCharacter);

	Energy getTotalEnergy();
};


#endif //SFTP_ANT_HPP
