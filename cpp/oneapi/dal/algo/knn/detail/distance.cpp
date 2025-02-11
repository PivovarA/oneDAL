/*******************************************************************************
* Copyright 2021 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include "oneapi/dal/algo/knn/detail/distance.hpp"
#include "oneapi/dal/algo/knn/backend/distance_impl.hpp"

namespace oneapi::dal::knn::detail {
namespace v1 {

template <typename F, typename M>
using minkowski_distance_t = minkowski_distance::descriptor<F, M>;

template <typename F, typename M>
using chebychev_distance_t = chebychev_distance::descriptor<F, M>;

class daal_interop_chebychev_distance_impl : public distance_impl {
public:
    daal_interop_chebychev_distance_impl() = default;

    daal_distance_t get_daal_distance_type() override {
        return daal_distance_t::chebychev;
    }

    double get_degree() override {
        return 0.0;
    }
};

class daal_interop_minkowski_distance_impl : public distance_impl {
public:
    daal_interop_minkowski_distance_impl(double degree) : degree_(degree) {}

    daal_distance_t get_daal_distance_type() override {
        return daal_distance_t::minkowski;
    }

    double get_degree() override {
        return degree_;
    }

private:
    double degree_;
};

template <typename F, typename M>
distance<minkowski_distance_t<F, M>>::distance(const minkowski_distance_t<F, M> &dist)
        : distance_(dist),
          impl_(new daal_interop_minkowski_distance_impl{ dist.get_degree() }) {}

template <typename F, typename M>
distance_impl *distance<minkowski_distance_t<F, M>>::get_impl() const {
    return impl_.get();
}

template <typename F, typename M>
distance<chebychev_distance_t<F, M>>::distance(const chebychev_distance_t<F, M> &dist)
        : distance_(dist),
          impl_(new daal_interop_chebychev_distance_impl{}) {}

template <typename F, typename M>
distance_impl *distance<chebychev_distance_t<F, M>>::get_impl() const {
    return impl_.get();
}

#define INSTANTIATE_MINKOWSKI(F, M) \
    template class ONEDAL_EXPORT distance<minkowski_distance_t<F, M>>;

#define INSTANTIATE_CHEBYCHEV(F, M) \
    template class ONEDAL_EXPORT distance<chebychev_distance_t<F, M>>;

INSTANTIATE_MINKOWSKI(float, minkowski_distance::method::dense)
INSTANTIATE_MINKOWSKI(double, minkowski_distance::method::dense)

INSTANTIATE_CHEBYCHEV(float, chebychev_distance::method::dense)
INSTANTIATE_CHEBYCHEV(double, chebychev_distance::method::dense)

} // namespace v1
} // namespace oneapi::dal::knn::detail
