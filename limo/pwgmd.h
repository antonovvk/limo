#pragma once

#include "model.h"

namespace limo {
    class LibLinearProblem;
    class PairwiseGMD: public Model {
    public:
        PairwiseGMD(LibLinearProblem& problem, double eps, size_t m);
        ~PairwiseGMD();
        virtual double Predict(const feature_node* x) const override;

    private:
        class Impl;
        std::unique_ptr<Impl> Impl_;
    };
}  // namespace limo
