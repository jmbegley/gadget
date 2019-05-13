#ifndef proglikelihood_h
#define  proglikelihood_h

#include "likelihood.h"
#include "areatime.h"
#include "commentstream.h"
#include "charptrvector.h"
#include "intvector.h"
#include "stock.h"
#include "fleet.h"
#include "stockptrvector.h"
#include "fleetptrvector.h"
#include "doublevector.h"
#include "readword.h"
#include "formulamatrix.h"
#include "modelvariable.h"

/**
 * \class ProgLikelihood
 * \brief This is really a prediction class
 *
 */
class ProgLikelihood : public Likelihood {
public:
  /**
   * \brief This is the default ProgLikelihood contructor
   * \param infile is the CommentStream to read the likelihood parameters from
   * \param Area is the AreaClass for the current model
   * \param TimeInfo is the TimeClass for the current model
   * \param keeper is the Keeper for the current model
   * \param weight is the weight for the likelihood component
   * \param name is the name for the likelihood component
   */
  ProgLikelihood(CommentStream& infile, const AreaClass* const Area,
    const TimeClass* const TimeInfo, Keeper* const keeper, double weight, const char* name);
  /**
   * \brief This is the default ProgLikelihood destructor
   */
  virtual ~ProgLikelihood();
  /**
   * \brief This function will calculate the likelihood score for the current model
   * \param TimeInfo is the TimeClass for the current model
   */
  virtual void addLikelihood(const TimeClass* const TimeInfo);
  /**
   * \brief This will select the fleets and stocks required to calculate the ProgLikelihood likelihood score
   * \param Fleets is the FleetPtrVector of all the available fleets
   * \param Stocks is the StockPtrVector of all the available stocks
   */
  void setFleetsAndStocks(FleetPtrVector& Fleets, StockPtrVector& Stocks);
  /**
   * \brief This function will reset the ProgLikelihood likelihood information
   * \param keeper is the Keeper for the current model
   * \note This function is not used for this likelihood component
   */
  void Reset(const Keeper* const keeper) {};
  /**
   * \brief This function will reset the ProgLikelihood likelihood information
   * \param TimeInfo is the TimeClass for the current model
   */
  void Reset(const TimeClass* const TimeInfo) { asserr.Update(TimeInfo); implerr.Update(TimeInfo); };
  /**
   * \brief This function will print the summary ProgLikelihood likelihood information
   * \param outfile is the ofstream that all the model information gets sent to
   */
  virtual void Print(ofstream& outfile) const {};
  /**
   * \brief This function will print information from each ProgLikelihood calculation
   * \param outfile is the ofstream that all the model likelihood information gets sent to
   * \param TimeInfo is the TimeClass for the current model
   */
  virtual void printLikelihood(ofstream& outfile, const TimeClass* const TimeInfo);
protected:
  void CalcTac(const TimeClass* const TimeInfo);
  void AllocateTac(const TimeClass* const TimeInfo);
  void CalcBiomass(const TimeClass* const TimeInfo);
private:
  /**
   * \brief This is the CharPtrVector of the names of the fleets
   */
  CharPtrVector fleetnames;
  /**
   * \brief This is the CharPtrVector of the names of the stocks
   */
  CharPtrVector stocknames;  // names of stock for example lingimm lingmat
  FormulaVector fleetproportions;  // Proportion of TAC on each fleet
  FleetPtrVector fleets;
  StockPtrVector stocks;
  FormulaMatrix biocoeffs;  // Selection for fishable biomass i.e biomass number 1.  
  FormulaMatrix triggercoeffs;  // Selection for trigger biomass i.e biomass number 2.
  FormulaVector triggervalues;  // Triggerpoints in biomass.  
  FormulaVector harvestrates;  // Harvest proportions below trigger1, between trigger 2 and 3 trigger 4 and 5 etc
  IntVector quotasteps;  // Steps that the quota is put on  3 4 5 6  means 3-6 timesteps from current timestep
  DoubleVector quotaproportions;  //Proportion of quota on each timestep.  
  int assessmentstep;  // The timestep when quota is calculated
  DoubleVector tachistory;  // The tac each year.  Refers to the assessment year
  double lastyearstac;  // last Tac 
  Formula weightoflastyearstac; // Weight of last years tac 0.5 in icecod HCR
  double maxchange;   // Maximum change allowed 0.2 means lasttac/(1+0.2) < tac > lasttac*(1+0.2) 
  int functionnumber;  // Number of function.
  DoubleVector bio1;  //fishable biomass biocoeffs
  DoubleVector bio2;  // triggerbiomass triggercoeffs
  DoubleVector bio1werr; //fishable biomass biocoeffs+error
  DoubleVector bio2werr;  // triggerbiomass triggercoeffs+error
  DoubleVector historicalhr;  // Vector of Harvestrates
  double calculatedtac; // The tac calculated
  int firsttacyear; // The first year that TAC is compiled.  
  ModelVariable asserr;
  ModelVariable implerr;
  int firstyear; // First year of simulations just TimeInfo->getFirstYear()
};

#endif
