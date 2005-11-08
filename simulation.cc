#include "ecosystem.h"
#include "errorhandler.h"
#include "gadget.h"
#include "interruptinterface.h"

extern ErrorHandler handle;

void Ecosystem::SimulateOneAreaOneTimeSubstep(int area) {
  int i;
  // calculate the number of preys and predators in area.
  for (i = 0; i < basevec.Size(); i++)
    if (basevec[i]->isInArea(area))
      basevec[i]->calcNumbers(area, Area, TimeInfo);
  for (i = 0; i < basevec.Size(); i++)
    if (basevec[i]->isInArea(area))
      basevec[i]->calcEat(area, Area, TimeInfo);
  for (i = 0; i < basevec.Size(); i++)
    if (basevec[i]->isInArea(area))
      basevec[i]->checkEat(area, TimeInfo);
  for (i = 0; i < basevec.Size(); i++)
    if (basevec[i]->isInArea(area))
      basevec[i]->adjustEat(area, TimeInfo);
  for (i = 0; i < basevec.Size(); i++)
    if (basevec[i]->isInArea(area))
      basevec[i]->reducePop(area, TimeInfo);
}

void Ecosystem::updatePopulationOneArea(int area) {
  int i;
  // under updates are movements to mature stock, renewal, spawning and straying.
  for (i = 0; i < basevec.Size(); i++)
    if (basevec[i]->isInArea(area))
      basevec[i]->Grow(area, Area, TimeInfo);
  for (i = 0; i < basevec.Size(); i++)
    if (basevec[i]->isInArea(area))
      basevec[i]->updatePopulationPart1(area, TimeInfo);
  for (i = 0; i < basevec.Size(); i++)
    if (basevec[i]->isInArea(area))
      basevec[i]->updatePopulationPart2(area, TimeInfo);
  for (i = 0; i < basevec.Size(); i++)
    if (basevec[i]->isInArea(area))
      basevec[i]->updatePopulationPart3(area, TimeInfo);
  for (i = 0; i < basevec.Size(); i++)
    if (basevec[i]->isInArea(area))
      basevec[i]->updatePopulationPart4(area, TimeInfo);
  for (i = 0; i < basevec.Size(); i++)
    if (basevec[i]->isInArea(area))
      basevec[i]->updatePopulationPart5(area, TimeInfo);
}

void Ecosystem::updateAgesOneArea(int area) {
  int i;
  // age related update and movements between stocks.
  for (i = 0; i < basevec.Size(); i++)
    if (basevec[i]->isInArea(area))
      basevec[i]->updateAgePart1(area, TimeInfo);
  for (i = 0; i < basevec.Size(); i++)
    if (basevec[i]->isInArea(area))
      basevec[i]->updateAgePart2(area, TimeInfo);
  for (i = 0; i < basevec.Size(); i++)
    if (basevec[i]->isInArea(area))
      basevec[i]->updateAgePart3(area, TimeInfo);
}

void Ecosystem::SimulateOneTimestep() {
  int i, j;
  for (i = 0; i < TimeInfo->numSubSteps(); i++) {
    for (j = 0; j < basevec.Size(); j++)
      basevec[j]->Migrate(TimeInfo);
    for (j = 0; j < Area->numAreas(); j++)
      this->SimulateOneAreaOneTimeSubstep(j);
    TimeInfo->IncrementSubstep();
  }

  for (j = 0; j < Area->numAreas(); j++)
    this->updatePopulationOneArea(j);
}

void Ecosystem::Simulate(int optimise, int print) {
  int i, j;

  handle.logMessage(LOGMESSAGE, "");  //write blank line to log file
  for (j = 0; j < likevec.Size(); j++)
    likevec[j]->Reset(keeper);

  if (optimise)
    for (j = 0; j < likevec.Size(); j++)
      likevec[j]->addLikelihoodKeeper(TimeInfo, keeper);

  for (j = 0; j < tagvec.Size(); j++)
    tagvec[j]->Reset();

  TimeInfo->Reset();
  for (i = 0; i < TimeInfo->numTotalSteps(); i++) {
    for (j = 0; j < basevec.Size(); j++)
      basevec[j]->Reset(TimeInfo);

    //Add in any new tagging experiments
    tagvec.updateTags(TimeInfo);

    if (print)
      for (j = 0; j < printvec.Size(); j++)
        printvec[j]->Print(TimeInfo, 1);  //start of timestep, so printtime is 1

    this->SimulateOneTimestep();
    if (optimise)
      for (j = 0; j < likevec.Size(); j++)
        likevec[j]->addLikelihood(TimeInfo);

    if (print)
      for (j = 0; j < printvec.Size(); j++)
        printvec[j]->Print(TimeInfo, 0);  //end of timestep, so printtime is 0

    for (j = 0; j < Area->numAreas(); j++)
      this->updateAgesOneArea(j);

    #ifdef INTERRUPT_HANDLER
      if (interrupted) {
        InterruptInterface ui(*this);
        if (ui.menu() == 0) {
          handle.logMessage(LOGMESSAGE, "\n** Gadget interrupted - quitting current simulation **");
          char interruptfile[15];
          strncpy(interruptfile, "", 15);
          strcpy(interruptfile, "interrupt.out");
          this->writeParams(interruptfile, 0);
          handle.logMessage(LOGMESSAGE, "** Gadget interrupted - quitting current simulation **");
          exit(EXIT_SUCCESS);
        }
        interrupted = 0;
      }
    #endif

    //Remove any expired tagging experiments
    tagvec.deleteTags(TimeInfo);

    //Increase the time in the simulation
    TimeInfo->IncrementTime();
  }

  //Remove all the tagging experiments - they must have expired now
  tagvec.deleteAllTags();

  if (optimise) {
    likelihood = 0.0;
    for (j = 0; j < likevec.Size(); j++)
      likelihood += likevec[j]->getLikelihood();
  }
  handle.logMessage(LOGMESSAGE, "The current overall likelihood score is", likelihood);
}
