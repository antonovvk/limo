#include <fstream>
#include <sstream>
#include <string.h>

#include "linear.h"
#include "llinear.h"

using namespace std;

namespace limo {

LibLinearProblem::LibLinearProblem()
    : MaxIdx_(0)
{
}

LibLinearProblem::~LibLinearProblem() = default;

LibLinearProblemPtr LoadProblem(const string& fileName, ostream& err) {
    cerr << "Reading input file " << fileName << endl;
    ifstream input(fileName);
    if (!input) {
        err << "Failed to open input file: " << fileName << endl;
        return nullptr;
    }

    LibLinearProblemPtr p(new LibLinearProblem());

    size_t n = 0;
    string line;
    while (getline(input, line)) {
        ++n;
        auto error = [&err, n]() -> ostream& {
            err << "Line: " << n << ", error: ";
            return err;
        };

        istringstream fields(line);

        int label;
        try {
            fields >> label;
        } catch (const exception& e) {
            error() << e.what() << endl;
            return nullptr;
        }

        int maxIdx = 0;
        LibSvmFeaturesPtr features(new LibSvmFeatures());
        while (!fields.eof()) {
            string f;
            feature_node fn;
            try {
                fields >> f;
                auto col = f.find(':');
                fn.index = stoi(f.substr(0, col));
                fn.value = stod(f.substr(col + 1));
            } catch (const exception& e) {
                error() << e.what() << endl;
                return nullptr;
            }
            if (fn.index <= maxIdx) {
                error() << "Invalid feature index: " << fn.index << endl;
                return nullptr;
            }
            maxIdx = fn.index;
            features->push_back(fn);
        }
        feature_node lf;
        lf.index = -1;
        features->push_back(lf);

        p->Labels_.push_back(label);
        p->Features_.push_back(&(*features)[0]);
        p->Data_.emplace_back(move(features));
        if (p->MaxIdx_ < maxIdx) {
            p->MaxIdx_ = maxIdx;
        }
    }
    return p;
}

void LibLinearModel::LLModelDeleter::operator() (model* m) {
    free_and_destroy_model(&m);
}

LibLinearModel::LibLinearModel(LibLinearProblem& p)
{
    parameter params;
    memset(&params, 0, sizeof(parameter));
    params.solver_type = 1;
    params.eps = 0.1;
    params.C = 1;

    problem pr;
    pr.n = p.MaxIdx_;
    pr.l = p.Labels_.size();
    pr.y = &p.Labels_[0];
    pr.x = &p.Features_[0];

    LLModel_.reset(train(&pr, &params));
}

double LibLinearModel::Predict(const feature_node* x) const {
    return predict(LLModel_.get(), x);
}

}  // namespace limo
