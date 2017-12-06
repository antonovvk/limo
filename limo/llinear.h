#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "model.h"

// Implemented in liblinear
struct model;

namespace limo {
    typedef std::vector<feature_node> LibSvmFeatures;
    typedef std::unique_ptr<LibSvmFeatures> LibSvmFeaturesPtr;

    struct LibLinearProblem {
        int MaxIdx_;
        std::vector<double> Labels_;
        std::vector<feature_node*> Features_;
        std::vector<LibSvmFeaturesPtr> Data_;

        LibLinearProblem(int maxIdx = 0);
        ~LibLinearProblem();
    };

    typedef std::unique_ptr<LibLinearProblem> LibLinearProblemPtr;

    LibLinearProblemPtr LoadProblem(const std::string& fileName, std::ostream& err);

    class LibLinearModel: public Model {
        struct LLModelDeleter {
            void operator() (model* m);
        };
        typedef std::unique_ptr<model, LLModelDeleter> LLModelPtr;

    public:
        LibLinearModel(LibLinearProblem& problem);

        virtual double Predict(const feature_node* x) const override;

    private:
        LLModelPtr LLModel_;
    };
}  // namespace limo
