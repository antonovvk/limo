#include "clar/clar.h"
#include "llinear.h"
#include "pwgmd.h"

using namespace std;
using namespace limo;

int Main(int argc, char* argv[]) {
    clar::Config cfg("limo", "LiMo playground");
    clar::Switch<'L'> liblinear(cfg, "liblinear", "Run default liblinear setup");
    clar::Switch<'P'> pwgmd(cfg, "pwgmd", "Run Pairwise GMD");
    clar::FreeInputFileArg<true> trainFile(cfg, "train", "Input train data in libsvm format");
    clar::FreeInputFileArg<true> testFile(cfg, "test", "Input test data in libsvm format");

    ostringstream err;
    if (!cfg.Parse(argc, argv, err)) {
        cerr << err.str() << endl;
        return 1;
    }

    auto problem = LoadProblem(trainFile, cerr);
    if (!problem) {
        return 1;
    }

    ModelPtr model;
    if (liblinear) {
        model.reset(new LibLinearModel(*problem));
    }
    if (pwgmd) {
        model.reset(new PairwiseGMD(*problem, 0.9, 10));
    }

    if (!model) {
        cerr << "No model selected" << endl;
        return 0;
    }

    auto testing = LoadProblem(testFile, cerr);
    if (!testing) {
        return 1;
    }

    size_t correct = 0;
    size_t total = 0;
    for (size_t i = 0; i < testing->Labels_.size(); ++i) {
        auto pred = model->Predict(testing->Features_[i]);
        if (pred == testing->Labels_[i]) {
            ++correct;
        }
        ++total;
    }
    if (total) {
        cerr << "Accuracy: " << (1.0 * correct / total) << ", correct: " << correct << " total: " << total << endl;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    try {
        return Main(argc, argv);
    } catch (const exception& e) {
        cerr << "Caught exception: " << e.what() << endl;
    }
    return 1;
}
