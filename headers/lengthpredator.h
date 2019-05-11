#ifndef lengthpredator_h
#define lengthpredator_h

#include "poppredator.h"

/**
 * \class LengthPredator
 * \brief This is the class used to model the predation of stocks by a fleet
 */
class LengthPredator : public PopPredator {
public:
  /**
   * \brief This is the LengthPredator constructor
   * \param givenname is the name of the predator
   * \param Areas is the IntVector of areas that the predator lives on
   * \param TimeInfo is the TimeClass for the current model
   * \param keeper is the Keeper for the current model
   * \param multscaler is the Formula that can be used to scale the biomass consumed
   */
  LengthPredator(const char* givenname, const IntVector& Areas,
    const TimeClass* const TimeInfo, Keeper* const keeper, Formula multscaler);
  /**
   * \brief This is the default LengthPredator destructor
   */
  virtual ~LengthPredator() {};
  /**
   * \brief This will calculate the amount of prey that is consumed by the predator
   * \param NumberInArea is the PopInfoVector giving the amount of prey in the area
   * \param area is the area that the prey consumption is being calculated on
   */
  virtual void Sum(const PopInfoVector& NumberInArea, int area);
  /**
   * \brief This will return the multiplicative value used scale the catch
   * \return multi
   */
  double getMultScaler() const { return multi; };
  /**
   * \brief This function will set the time multiplier values (used by the QuotaPredator)
   * \param TimeInfo is the TimeClass for the current model
   * \param quotastep is the time step for the quota
   * \param value is the time multiplier value
   */
  virtual void setTimeMultiplier(const TimeClass* const TimeInfo, int quotastep, double value);
  /**
   * \brief This function will reset the predation information
   * \param TimeInfo is the TimeClass for the current model
   */
  virtual void Reset(const TimeClass* const TimeInfo);
protected:
  /**
   * \brief This is the multiplicative constant that can be used to scale the biomass consumed
   */
  Formula multi;
  /**
   * \brief This is the DoubleVector of time multiplier values (only used by QuotaPredator)
   */
  DoubleVector timeMultiplier;
};

#endif
