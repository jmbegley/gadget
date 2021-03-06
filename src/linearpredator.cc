#include "linearpredator.h"
#include "keeper.h"
#include "prey.h"
#include "mathfunc.h"
#include "errorhandler.h"
#include "gadget.h"
#include "global.h"

LinearPredator::LinearPredator(CommentStream& infile, const char* givenname,
  const IntVector& Areas, const TimeClass* const TimeInfo, Keeper* const keeper, Formula multscaler)
  : LengthPredator(givenname, Areas, TimeInfo, keeper, multscaler) {

  type = LINEARPREDATOR;
  keeper->addString("predator");
  keeper->addString(givenname);
  this->readSuitability(infile, TimeInfo, keeper);
  keeper->clearLast();
  keeper->clearLast();
}

void LinearPredator::Eat(int area, const AreaClass* const Area, const TimeClass* const TimeInfo) {

  int inarea = this->areaNum(area);
  int prey, preyl;
  int predl = 0;  //JMB there is only ever one length group ...
  totalcons[inarea][predl] = 0.0;

  double tmp;
  tmp = prednumber[inarea][predl].N * multi * TimeInfo->getTimeStepSize() / TimeInfo->numSubSteps();
  if (isZero(tmp)) //JMB no predation takes place on this timestep
    return;
  if (tmp > 10.0) //JMB arbitrary value here ...
    handle.logMessage(LOGWARN, "Warning in linearpredator - excessive consumption required");

  for (prey = 0; prey < this->numPreys(); prey++) {
    if (this->getPrey(prey)->isPreyArea(area)) {
      (*predratio[inarea])[prey][predl] = tmp;
      for (preyl = 0; preyl < (*cons[inarea][prey])[predl].Size(); preyl++) {
        (*cons[inarea][prey])[predl][preyl] = (*predratio[inarea])[prey][predl] *
          this->getSuitability(prey)[predl][preyl] * this->getPrey(prey)->getBiomass(area, preyl);
        totalcons[inarea][predl] += (*cons[inarea][prey])[predl][preyl];
      }
      //inform the preys of the consumption
      this->getPrey(prey)->addBiomassConsumption(area, (*cons[inarea][prey])[predl]);

    } else {
      for (preyl = 0; preyl < (*cons[inarea][prey])[predl].Size(); preyl++)
        (*cons[inarea][prey])[predl][preyl] = 0.0;
    }
  }
}

void LinearPredator::adjustConsumption(int area, const TimeClass* const TimeInfo) {
  int prey, preyl;
  int inarea = this->areaNum(area);
  int predl = 0;  //JMB there is only ever one length group ...
  overcons[inarea][predl] = 0.0;

  if (isZero(totalcons[inarea][predl])) //JMB no predation takes place on this timestep
    return;

  double maxRatio, tmp;
  maxRatio = TimeInfo->getMaxRatioConsumed();

  for (prey = 0; prey < this->numPreys(); prey++) {
    if (this->getPrey(prey)->isOverConsumption(area)) {
      hasoverconsumption[inarea] = 1;
      DoubleVector ratio = this->getPrey(prey)->getRatio(area);
      for (preyl = 0; preyl < (*cons[inarea][prey])[predl].Size(); preyl++) {
        if (ratio[preyl] > maxRatio) {
          tmp = maxRatio / ratio[preyl];
          overcons[inarea][predl] += (1.0 - tmp) * (*cons[inarea][prey])[predl][preyl];
          (*cons[inarea][prey])[predl][preyl] *= tmp;
          (*usesuit[inarea][prey])[predl][preyl] *= tmp;
        }
      }
    }
  }

  if (hasoverconsumption[inarea]) {
    totalcons[inarea][predl] -= overcons[inarea][predl];
    overconsumption[inarea][predl] += overcons[inarea][predl];
  }

  totalconsumption[inarea][predl] += totalcons[inarea][predl];
  for (prey = 0; prey < this->numPreys(); prey++)
    if (this->getPrey(prey)->isPreyArea(area))
      for (preyl = 0; preyl < (*cons[inarea][prey])[predl].Size(); preyl++)
        (*consumption[inarea][prey])[predl][preyl] += (*cons[inarea][prey])[predl][preyl];
}

void LinearPredator::Print(ofstream& outfile) const {
  outfile << "LinearPredator\n";
  PopPredator::Print(outfile);
}
