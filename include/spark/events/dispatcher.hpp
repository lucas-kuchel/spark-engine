#pragma once

#if SPARK_SUPPORTED

#include "pipeline_delegate.hpp"
#include "type_index.hpp"

#include <cstring>

namespace spark::events {
    class dispatcher {
    public:
        dispatcher() noexcept = default;
        ~dispatcher() noexcept = default;

        dispatcher(const dispatcher&) = delete;
        dispatcher(dispatcher&&) noexcept = default;

        dispatcher& operator=(const dispatcher&) = delete;
        dispatcher& operator=(dispatcher&&) noexcept = default;

        template <typename T>
        [[nodiscard]] pipeline<T>& acquire() noexcept {
            std::size_t index = type_index<T>();

            if (index >= pipelines_.size()) {
                pipelines_.resize(index + 1);
            }

            pipeline_delegate& pipeline = pipelines_[index];

            if (!pipeline) {
                pipeline.construct<T>();
            }

            return pipeline.acquire<T>();
        }

        void reset() noexcept {
            for (pipeline_delegate& pipeline : pipelines_) {
                if (pipeline) {
                    pipeline.reset();
                }
            }
        }

        void dispatch() noexcept {
            for (pipeline_delegate& pipeline : pipelines_) {
                if (pipeline) {
                    pipeline.dispatch();
                }
            }
        }

    private:
        std::vector<pipeline_delegate> pipelines_;
    };
}

#endif