#include "PoliceMeter.h"
#include <cstdlib>
#include <ctime>

PoliceMeter::PoliceMeter()
{
	MeterPercentage = 0;			// meter starts out 0%
	BribeSuccess = 100;				// chance of bribe succeeding starts at 100%
	BribeCost = 1000;				// default cost of bribe
}

PoliceMeter::~PoliceMeter()
{
}

void PoliceMeter::DailyIncreaseMP(int NoOfNoobWorkers, int NoOfExperiencedWorkers, int NoOfExpertWorkers, bool minus3boost)
{
	// formula based on the google doc
	int dailyIncrease = (NoOfNoobWorkers * 3) + (NoOfExperiencedWorkers * 2) + (NoOfExpertWorkers * 1);

	// if boost has been bought and the bool value is true,
	// the daily increase will - 3%
	if ((minus3boost == true) && (dailyIncrease > 3))
	{
		dailyIncrease = dailyIncrease - 10;
	}
	MeterPercentage += dailyIncrease;
}

void PoliceMeter::Bribe(void)
{
	int BribeChance;
	BribeChance = rand() % 100 + 1;

	// if the random number falls within the bribesuccess range, bribe succeeds and cost increases.
	//  If not, police meter + 5%.
	if (BribeChance <= BribeSuccess) 
	{
		MeterPercentage = MeterPercentage/2;
		BribeCost *= 1.1;
		if (BribeSuccess >= 40) {
			BribeSuccess -= 10;
		}
		policefail = false;
	}
	else
	{
		MeterPercentage = MeterPercentage + 50;
		policefail = true;
	}
}

void PoliceMeter::BoostBribeSuccess()
{
	BribeSuccess = BribeSuccess + 10;
}

void PoliceMeter::CustomIncreaseMP(float AmountToAdd)
{
	MeterPercentage = MeterPercentage + AmountToAdd;
}

void PoliceMeter::CustomDecreaseMP(float AmountToMinus)
{
	MeterPercentage = MeterPercentage - AmountToMinus;
	if (MeterPercentage < 0)
	{
		MeterPercentage = 0;
	}
}

float PoliceMeter::GetMP(void)
{
	return MeterPercentage;
}

int PoliceMeter::GetBribeCost(void)
{
	return BribeCost;
}


bool PoliceMeter::IsMeterFull(void)
{
	return (MeterPercentage >= 100);
}

bool PoliceMeter::getPoliceFail(void)
{
	return policefail;
}

void PoliceMeter::setPoliceFail(bool condition)
{
	policefail = condition;
}

int PoliceMeter::getSuccessChance(void)
{
		return BribeSuccess;
}
