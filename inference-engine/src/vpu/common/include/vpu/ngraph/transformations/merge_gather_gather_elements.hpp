// Copyright (C) 2018-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include "ngraph/pass/pass.hpp"

namespace vpu {

class MergeGatherGatherElements : public ngraph::pass::FunctionPass {
public:
    NGRAPH_RTTI_DECLARATION;
    bool run_on_model(const std::shared_ptr<ngraph::Function>& m) override;
};

}  // namespace vpu
