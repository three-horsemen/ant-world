//
// Created by soorya on 10/3/17.
//

#include <ui/OpenCV.hpp>

ui::OpenCV::OpenCV(Environment &newEnvironment) : environment(newEnvironment) {
	TILE_SIDE_PIXEL_WIDTH = WINDOW_WIDTH / environment.width;
	TILE_SIDE_PIXEL_HEIGHT = WINDOW_HEIGHT / environment.height;
	AGENT_HEIGHT_DISPLAY_PADDING = 2;
	AGENT_WIDTH_DISPLAY_PADDING = 2;
	HUD_HEIGHT_MARGIN = 15;
	HUD_HEIGHT_PADDING = 7;
	image = Mat::zeros(
			WINDOW_HEIGHT + (2 * TILE_SIDE_PIXEL_HEIGHT) + (HUD_HEIGHT_MARGIN + 2 * HUD_HEIGHT_PADDING),
			WINDOW_WIDTH + (2 * TILE_SIDE_PIXEL_WIDTH),
			CV_8UC3
	);
	HUDBackdrop = Rect2d(0, 0,
						 WINDOW_WIDTH + (2 * TILE_SIDE_PIXEL_WIDTH),
						 (TILE_SIDE_PIXEL_HEIGHT) + (HUD_HEIGHT_MARGIN + 2 * HUD_HEIGHT_PADDING)
	);
	HUDBackdrop.x = 0;
	HUDBackdrop.y = WINDOW_HEIGHT + TILE_SIDE_PIXEL_HEIGHT + HUD_HEIGHT_PADDING;
}

Environment ui::OpenCV::getEnvironment() const {
	return environment;
}

void ui::OpenCV::displayEnvironment(const vector<Ant> &ants, unsigned long long &iterationCount) {

	int i = 0, j = 0;
	Rect2d tileRect(0, 0, TILE_SIDE_PIXEL_WIDTH, TILE_SIDE_PIXEL_HEIGHT);
	Coordinate tileCoordinate;
	AgentCharacter character;
	Occupancy occupancy;
	Tile tile;
	Point agentCorners[1][3];
	vector<Point> contour(3);
	int npt[] = {3};
	int npts;
	for (i = 0; i < environment.height; i++) {
		for (j = 0; j < environment.width; j++) {
			tile = environment.getTile(Coordinate(j, i));
			character = tile.getAgentCharacter();
			occupancy = character.getOccupancy();
			tileCoordinate = tile.getGlobalCoordinate();
			tileRect.x = (1 + tileCoordinate.getX()) * TILE_SIDE_PIXEL_WIDTH;
			tileRect.y = (1 + tileCoordinate.getY()) * TILE_SIDE_PIXEL_HEIGHT;

			rectangle(
					image,
					tileRect,
					Scalar(
							128,
							0,
							(log(tile.getTotalEnergy()) / log(Ant::HYPOTHETICAL_MAX_ENERGY)) * 255
					),
					CV_FILLED,
					8
			);
			//If an agent exists, draw it here.
			if (occupancy != OCCUPANCY_DEAD) {
				if (occupancy == OCCUPANCY_NORTH) {
					//Bottom-left.
					agentCorners[0][0] = Point(
							(1 + tileCoordinate.getX()) * TILE_SIDE_PIXEL_WIDTH + AGENT_WIDTH_DISPLAY_PADDING,
							(2 + tileCoordinate.getY()) * TILE_SIDE_PIXEL_HEIGHT - AGENT_HEIGHT_DISPLAY_PADDING
					);
					//Bottom-right.
					agentCorners[0][1] = Point(
							(2 + tileCoordinate.getX()) * TILE_SIDE_PIXEL_WIDTH - AGENT_WIDTH_DISPLAY_PADDING,
							(2 + tileCoordinate.getY()) * TILE_SIDE_PIXEL_HEIGHT - AGENT_HEIGHT_DISPLAY_PADDING
					);
					//Top-center.
					agentCorners[0][2] = Point((int) ((1.5 + (float) tileCoordinate.getX()) * TILE_SIDE_PIXEL_WIDTH),
											   (1 + tileCoordinate.getY()) * TILE_SIDE_PIXEL_HEIGHT +
											   AGENT_HEIGHT_DISPLAY_PADDING
					);
				} else if (occupancy == OCCUPANCY_SOUTH) {
					//Top-left.
					agentCorners[0][0] = Point(
							(1 + tileCoordinate.getX()) * TILE_SIDE_PIXEL_WIDTH + AGENT_WIDTH_DISPLAY_PADDING,
							(1 + tileCoordinate.getY()) * TILE_SIDE_PIXEL_HEIGHT + AGENT_HEIGHT_DISPLAY_PADDING
					);
					//Top-right.
					agentCorners[0][1] = Point(
							(2 + tileCoordinate.getX()) * TILE_SIDE_PIXEL_WIDTH - AGENT_WIDTH_DISPLAY_PADDING,
							(1 + tileCoordinate.getY()) * TILE_SIDE_PIXEL_HEIGHT + AGENT_HEIGHT_DISPLAY_PADDING
					);
					//Bottom-center.
					agentCorners[0][2] = Point((int) ((1.5 + (float) tileCoordinate.getX()) * TILE_SIDE_PIXEL_WIDTH),
											   (2 + tileCoordinate.getY()) * TILE_SIDE_PIXEL_HEIGHT -
											   AGENT_HEIGHT_DISPLAY_PADDING
					);
				} else if (occupancy == OCCUPANCY_EAST) {
					//Top-left.
					agentCorners[0][0] = Point(
							(1 + tileCoordinate.getX()) * TILE_SIDE_PIXEL_WIDTH + AGENT_WIDTH_DISPLAY_PADDING,
							(1 + tileCoordinate.getY()) * TILE_SIDE_PIXEL_HEIGHT + AGENT_HEIGHT_DISPLAY_PADDING
					);
					//Bottom-left.
					agentCorners[0][1] = Point(
							(1 + tileCoordinate.getX()) * TILE_SIDE_PIXEL_WIDTH + AGENT_WIDTH_DISPLAY_PADDING,
							(2 + tileCoordinate.getY()) * TILE_SIDE_PIXEL_HEIGHT - AGENT_HEIGHT_DISPLAY_PADDING
					);
					//Right-center.
					agentCorners[0][2] = Point(
							(2 + tileCoordinate.getX()) * TILE_SIDE_PIXEL_WIDTH - AGENT_WIDTH_DISPLAY_PADDING,
							(int) ((1.5 + (float) tileCoordinate.getY()) * TILE_SIDE_PIXEL_HEIGHT)
					);
				} else { //Looking west.
					//Top-right.
					agentCorners[0][0] = Point(
							(2 + tileCoordinate.getX()) * TILE_SIDE_PIXEL_WIDTH - AGENT_WIDTH_DISPLAY_PADDING,
							(1 + tileCoordinate.getY()) * TILE_SIDE_PIXEL_HEIGHT + AGENT_HEIGHT_DISPLAY_PADDING
					);
					//Bottom-right.
					agentCorners[0][1] = Point(
							(2 + tileCoordinate.getX()) * TILE_SIDE_PIXEL_WIDTH - AGENT_WIDTH_DISPLAY_PADDING,
							(2 + tileCoordinate.getY()) * TILE_SIDE_PIXEL_HEIGHT - AGENT_HEIGHT_DISPLAY_PADDING
					);
					//Left-center.
					agentCorners[0][2] = Point(
							(1 + tileCoordinate.getX()) * TILE_SIDE_PIXEL_WIDTH + AGENT_WIDTH_DISPLAY_PADDING,
							(int) ((1.5 + (float) tileCoordinate.getY()) * TILE_SIDE_PIXEL_HEIGHT)
					);
				}
				const Point *ppt[1] = {agentCorners[0]};

				fillPoly(image, ppt, npt, 1, Scalar(
						character.getTrait(),
						tile.getTotalEnergy(),
						character.getAttitude()
				), 8);

				contour = {agentCorners[0][0], agentCorners[0][1], agentCorners[0][2]};
				const cv::Point *pts = (const cv::Point *) Mat(contour).data;
				npts = Mat(contour).rows;

				polylines(image, &pts, &npts, 1,
						  true,
						  Scalar(0, 255, 0),
						  1,
						  0, 0);
			}
		}
	}

	//Draw the HUD.
	rectangle(image, HUDBackdrop, Scalar(0, 0, 0), CV_FILLED, 8); //Clean the backdrop of the HUD.
	stringstream strStrm;

	string avgFertilityLabel = "Avg. fertility: ";
	float avgFertility = 0;
	for (unsigned int k = 0; k < ants.size(); k++) {
		const unsigned int childrenCount = ants[k].getFertility();
		avgFertility += childrenCount;
	}
	avgFertility = avgFertility / ants.size();
	strStrm << avgFertilityLabel << setw(6) << setfill('0') << fixed << setprecision(1) << avgFertility;
	putText(image,
			strStrm.str(),
			Point(
					(int) (WINDOW_WIDTH * 0.01f),
					WINDOW_HEIGHT + 1 * TILE_SIDE_PIXEL_HEIGHT + HUD_HEIGHT_MARGIN
			),
			FONT_HERSHEY_SIMPLEX,
			0.4,
			Scalar(0, 200, 200),
			1
	);
	strStrm.str("");

	string avgFetalLabel = "Avg. fetal: ";
	float avgFetal = 0;
	for (unsigned int k = 0; k < ants.size(); k++) {
		const unsigned int x = ants[k].getFetal();
		avgFetal += x;
	}
	avgFetal = avgFetal / ants.size();
	strStrm << avgFetalLabel << setw(9) << setfill('0') << fixed << setprecision(1) << avgFetal;
	putText(image,
			strStrm.str(),
			Point(
					(int) (WINDOW_WIDTH * 0.01f),
					WINDOW_HEIGHT + 2 * TILE_SIDE_PIXEL_HEIGHT + (HUD_HEIGHT_MARGIN + HUD_HEIGHT_PADDING)
			),
			FONT_HERSHEY_SIMPLEX,
			0.4,
			Scalar(0, 200, 200),
			1
	);
	strStrm.str("");

	string avgChildrenCountLabel = "Avg. children: ";
	float avgChildrenCount = 0;
	for (unsigned int k = 0; k < ants.size(); k++) {
		const unsigned int childrenCount = ants[k].getChildrenCount();
		avgChildrenCount += childrenCount;
	}
	avgChildrenCount = avgChildrenCount / ants.size();
	strStrm << avgChildrenCountLabel << setw(6) << setfill('0') << fixed << setprecision(2) << avgChildrenCount;
	putText(image,
			strStrm.str(),
			Point(
					(int) (WINDOW_WIDTH * 0.25f),
					WINDOW_HEIGHT + 1 * TILE_SIDE_PIXEL_HEIGHT + HUD_HEIGHT_MARGIN
			),
			FONT_HERSHEY_SIMPLEX,
			0.4,
			Scalar(0, 200, 200),
			1
	);
	strStrm.str("");

	string avgGenLabel = "Avg. gen.s: ";
	float avgGenCount = 0;
	for (unsigned int k = 0; k < ants.size(); k++) {
		unsigned int x = ants[k].getGenerationCount();
		avgGenCount += x;
	}
	avgGenCount = avgGenCount / ants.size();
	strStrm << avgGenLabel << setw(6) << setfill('0') << fixed << setprecision(2) << avgGenCount;
	putText(image,
			strStrm.str(),
			Point(
					(int) (WINDOW_WIDTH * 0.25f),
					WINDOW_HEIGHT + 2 * TILE_SIDE_PIXEL_HEIGHT + (HUD_HEIGHT_MARGIN + HUD_HEIGHT_PADDING)
			),
			FONT_HERSHEY_SIMPLEX,
			0.4,
			Scalar(0, 200, 200),
			1
	);
	strStrm.str("");

	string avgEnergyLabel = "Avg. Energy: ";
	float avgEnergy = 0;
	for (unsigned int k = 0; k < ants.size(); k++) {
		const unsigned int totalEnergy = ants[k].getTotalEnergy();
		avgEnergy += totalEnergy;
	}
	avgEnergy = avgEnergy / ants.size();
	strStrm << avgEnergyLabel << setw(9) << setfill('0') << fixed << setprecision(1) << avgEnergy;
	putText(image,
			strStrm.str(),
			Point(
					(int) (WINDOW_WIDTH * 0.5f),
					WINDOW_HEIGHT + 1 * TILE_SIDE_PIXEL_HEIGHT + HUD_HEIGHT_MARGIN
			),
			FONT_HERSHEY_SIMPLEX,
			0.4,
			Scalar(0, 200, 200),
			1
	);
	strStrm.str("");

	string avgAgeLabel = "Avg. Age: ";
	float avgAge = 0;
	for (unsigned int k = 0; k < ants.size(); k++) {
		const unsigned int age = ants[k].getAge();
		avgAge += age;
	}
	avgAge /= ants.size();
	strStrm << avgAgeLabel << setw(7) << setfill('0') << fixed << setprecision(2) << avgAge;
	putText(image,
			strStrm.str(),
			Point(
					(int) (WINDOW_WIDTH * 0.5f),
					WINDOW_HEIGHT + 2 * TILE_SIDE_PIXEL_HEIGHT + (HUD_HEIGHT_MARGIN + HUD_HEIGHT_PADDING)
			),
			FONT_HERSHEY_SIMPLEX,
			0.4,
			Scalar(0, 200, 200),
			1
	);
	strStrm.str("");

	string populationLabel = "Population: ";
	strStrm << populationLabel << setw(5) << setfill('0') << fixed << ants.size();
	putText(image,
			strStrm.str(),
			Point(
					(int) (WINDOW_WIDTH * 0.8f),
					WINDOW_HEIGHT + 1 * TILE_SIDE_PIXEL_HEIGHT + HUD_HEIGHT_MARGIN
			),
			FONT_HERSHEY_SIMPLEX,
			0.4,
			Scalar(0, 200, 200),
			1
	);
	strStrm.str("");

	string iterationLabel = "Iteration: ";
	strStrm << iterationLabel << setw(10) << setfill('0') << fixed << iterationCount;
	putText(image,
			strStrm.str(),
			Point(
					(int) (WINDOW_WIDTH * 0.8f),
					WINDOW_HEIGHT + 2 * TILE_SIDE_PIXEL_HEIGHT + (HUD_HEIGHT_MARGIN + HUD_HEIGHT_PADDING)
			),
			FONT_HERSHEY_SIMPLEX,
			0.4,
			Scalar(0, 200, 200),
			1
	);
	strStrm.str("");
	cvtColor(image, image, CV_HSV2BGR_FULL);
	imshow("AEvoSwarm-Environment", image);
}
