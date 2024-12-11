#pragma once
class PoliceMeter
{


public:
	PoliceMeter(); // con
	~PoliceMeter(); // decon

	// the daily increase in meter %
	void DailyIncreaseMP(int NoOfNoobWorkers, int NoOfExperiencedWorkers, int NoOfExpertWorkers, bool minus3boost);
	
	int GetBribeCost(void);

	// call when Bribe is used
	void Bribe(void); 

	// increases bribe success by 10%(max 5 times not implemented here)
	void BoostBribeSuccess(void);

	// custom amount to increase MP
	void CustomIncreaseMP(float AmountToAdd);
	
	// custom amount to decrease MP
	void CustomDecreaseMP(float AmountToMinus);

	// getter for meter %
	float GetMP(void);

	// returns true when meter reaches 100% and can be used to check when game should end
	bool IsMeterFull(void);

	bool getPoliceFail(void);
	void setPoliceFail(bool condition);

    int getSuccessChance(void);

private:
	float MeterPercentage = 0;			// the police-meter "score"
	float BribeSuccess = 100;			// global variable
	int BribeCost = 1000;				// cost of activating bribe function
	bool policefail = false;
};
