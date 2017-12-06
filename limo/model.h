#pragma once

#include <memory>

// Implemented in liblinear
struct feature_node;

namespace limo {
    class Model {
    public:
        Model()
        {
        }

        virtual ~Model()
        {
        }

        virtual double Predict(const feature_node* x) const = 0;
    };

    typedef std::unique_ptr<Model> ModelPtr;
}  // namespace limo
