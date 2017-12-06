#include <deque>
#include <map>
#include <cassert>

#include "pwgmd.h"
#include "llinear.h"
#include "dm_english.h"

using namespace std;

namespace limo {

struct PairwiseLLModel {
    const double L_;
    const double R_;
    double Score_;
    unique_ptr<LibLinearModel> Model_;

    PairwiseLLModel(size_t m, int maxIdx, double l, double r, const vector<feature_node*>& xf, const vector<feature_node*>& yf)
        : L_(l)
        , R_(r)
        , Score_(0)
    {
        vector<double> testLabels;
        vector<feature_node*> testFeatures;

        auto getTail = [m, &testLabels, &testFeatures](double l, const vector<feature_node*>& f) {
            if (f.size() >= m) {
                for (size_t k = f.size() / 2; k < f.size(); ++k) {
                    testLabels.push_back(l);
                    testFeatures.push_back(f[k]);
                }
                return f.size() / 2;
            }
            return f.size();
        };

        LibLinearProblem p(maxIdx);

        auto yn = getTail(1, yf);
        vector<double>(yn, 1).swap(p.Labels_);
        vector<feature_node*>(yf).swap(p.Features_);
        p.Features_.resize(yn);

        auto xn = getTail(0, xf);
        vector<double> xl(xn, 0);
        p.Labels_.insert(p.Labels_.end(), xl.begin(), xl.end());
        p.Features_.insert(p.Features_.end(), xf.begin(), xf.begin() + xn);

        Model_.reset(new LibLinearModel(p));

        size_t correct = 0;
        size_t total = 0;
        for (size_t i = 0; i < testLabels.size(); ++i) {
            auto pred = Model_->Predict(testFeatures[i]);
            if (pred == testLabels[i]) {
                ++correct;
            }
            ++total;
        }
        if (total) {
            Score_ = 1.0 * correct / total;
        }
    }
};

class PairwiseGMD::Impl {
public:
    Impl(LibLinearProblem& problem, double eps, size_t m)
    {
        vector<feature_node*> xf;
        double prevX = -1;
        size_t i = 0;
        for (auto x = problem.Labels_.begin(); x != problem.Labels_.end(); ++x, ++i) {
            if (prevX != *x && prevX >= 0) {
                vector<feature_node*> yf;
                double prevY = -1;
                size_t j = i;

                for (auto y = x; y != problem.Labels_.end(); ++y, ++j) {
                    if (prevY != *y && prevY >= 0) {
                        ModelIds_[{prevX, prevY}] = Models_.size();
                        Models_.emplace_back(new PairwiseLLModel(m, problem.MaxIdx_, prevX, prevY, xf, yf));
                        yf.clear();
                    }
                    yf.push_back(problem.Features_[j]);
                    prevY = *y;
                }
                if (prevY >= 0) {
                    ModelIds_[{prevX, prevY}] = Models_.size();
                    Models_.emplace_back(new PairwiseLLModel(m, problem.MaxIdx_, prevX, prevY, xf, yf));
                }
                xf.clear();
            }
            xf.push_back(problem.Features_[i]);
            prevX = *x;
            NodeIds_.insert({*x, NodeIds_.size()});
        }

        Graph_.resize(NodeIds_.size());
        for (auto it: ModelIds_) {
            auto x = NodeIds_.find(it.first.first);
            auto y = NodeIds_.find(it.first.second);
            assert(x != NodeIds_.end() && y != NodeIds_.end());

            auto add = [this](int p, int q) {
                Adj a;
                a.s = q;
                a.suiv = Graph_[p];
                Adjacency_.push_back(a);
                Graph_[p] = &Adjacency_.back();
            };

            if (Models_[it.second]->Score_ > eps) {
                add(x->second, y->second);
                add(y->second, x->second);
                cerr << x->second << ":" << y->second << " -> " << Models_[it.second]->Score_ << endl;
            }
        }

        graphe g;
        g.n = Graph_.size();
        g.G = &Graph_[0];

        printarbre(decomposition_modulaire(g));
    }

    double Predict(const feature_node* x) const {
        return 0;
    }

private:
    vector<unique_ptr<PairwiseLLModel>> Models_;
    map<pair<double, double>, size_t> ModelIds_;
    map<double, int> NodeIds_;
    deque<Adj> Adjacency_;
    vector<Adj*> Graph_;
};

PairwiseGMD::PairwiseGMD(LibLinearProblem& problem, double eps, size_t m)
    : Impl_(new PairwiseGMD::Impl(problem, eps, m))
{
}

PairwiseGMD::~PairwiseGMD() = default;

double PairwiseGMD::Predict(const feature_node* x) const {
    return Impl_->Predict(x);
}

}  // namespace limo
