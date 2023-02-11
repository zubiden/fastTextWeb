/**
 * Copyright (c) 2016-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <sstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <stdexcept>
#include "args.h"
#include "fasttext.h"

// emscripten
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>

// nodeFS
#ifdef NODERAWFS
#define CWD ""
#else
#define CWD "/working"
#endif
// end nodeFS

using namespace fasttext;

// our instance of fastText
static FastText fastTextInstance;

int isModelLoaded = 0;

std::string testFunction () {
  return "The test function worked.\n";
}

std::string printPredictions(
    const std::vector<std::pair<real, std::string>>& predictions,
    bool printProb,
    bool multiline) {
  std::stringstream ss;
  bool first = true;
  for (const auto& prediction : predictions) {
    if (!first && !multiline) {
      ss << " ";
    }
    first = false;
    ss << prediction.second;
    if (printProb) {
      ss << " " << prediction.first;
    }
    if (multiline) {
      ss << std::endl;
    }
  }
  if (!multiline) {
    ss << std::endl;
  }

  return ss.str();
}

// this function loads the model
void loadModel() {
  if (isModelLoaded) return;
  fastTextInstance.loadModel();
  isModelLoaded = 1;
  // std::cout << "The model has successfully been loaded!\n";
}

// this function will actually make a prediction
std::string makePrediction(std::string predictionType, std::string textToClassify, std::string numPredictions, std::string thresholdValue) {
  loadModel();
  int32_t k = std::stoi(numPredictions);
  real threshold = std::stof(thresholdValue);
  std::istringstream is(textToClassify);

  bool printProb = predictionType == "predict-prob";

  std::istream& in = is;
  std::vector<std::pair<real, std::string>> predictions;
  std::stringstream ss;
  while (fastTextInstance.predictLine(in, predictions, k, threshold)) {
    ss << printPredictions(predictions, printProb, true);
  }
  return ss.str();
}

int main(int argc, char** argv) {
  // nodeFS
  #ifndef NODERAWFS
    // mount the current folder as a nodeFS instance
    EM_ASM(
      FS.mount(NODEFS, { root: '.' }, '.');
    );
  #endif
  // end nodeFS
  return 0;
}

// emscripten bindings
EMSCRIPTEN_BINDINGS(myModule) {
  emscripten::function("testFunction", &testFunction);
  emscripten::function("makePrediction", &makePrediction);
}