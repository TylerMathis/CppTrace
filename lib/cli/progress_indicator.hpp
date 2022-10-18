//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef RAYTRACER_LIB_CLI_PROGRESS_INDICATOR_HPP_
#define RAYTRACER_LIB_CLI_PROGRESS_INDICATOR_HPP_

#include <iostream>

struct ProgressIndicator {
  const int BAR_WIDTH = 70;

  int numSteps;
  explicit ProgressIndicator(const int numSteps)
      : numSteps(numSteps) { indicate(0); }

  void indicate(const int curStep) const {
    double progress = (double) curStep / numSteps;
    int intProgress = (int) (progress * 100);

    int barProgress = (int) (BAR_WIDTH * progress);
    std::cout << "[";
    for (int i = 0; i < BAR_WIDTH; i++) {
      if (i < barProgress)
        std::cout << "=";
      else if (i == barProgress)
        std::cout << ">";
      else
        std::cout << "-";
    }
    std::cout << "] ";
    std::cout << intProgress << "%\r";
    std::cout.flush();
  }

  void done() const {
    indicate(numSteps);
    std::cout << "\n";
    std::cout.flush();
  }
};

#endif //RAYTRACER_LIB_CLI_PROGRESS_INDICATOR_HPP_
