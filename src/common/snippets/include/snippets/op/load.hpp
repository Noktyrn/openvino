// Copyright (C) 2018-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <ngraph/op/op.hpp>

namespace ngraph {
namespace snippets {
namespace op {

/**
 * @interface Load
 * @brief Generated by Canonicalization step where explicit load instruction should be emmiteed
 * ScalarLoad == scalar instruction + post increment
 * Load (VectorLoad) == vector instruction + post increment
 * BroadcastLoad == scalar instruction - post increment
 * BlockedLoad == vector instruction - post increment
 * @ingroup snippets
 */
class Load : public ngraph::op::Op {
public:
    OPENVINO_OP("Load", "SnippetsOpset");

    Load(const Output<Node>& x);
    Load() = default;

    bool visit_attributes(AttributeVisitor& visitor) override;

    std::shared_ptr<Node> clone_with_new_inputs(const OutputVector& new_args) const override;

    void validate_and_infer_types() override;

    OPENVINO_SUPPRESS_DEPRECATED_START
    bool evaluate(const HostTensorVector& output_values, const HostTensorVector& input_values) const override;
    OPENVINO_SUPPRESS_DEPRECATED_END
};

} // namespace op
} // namespace snippets
} // namespace ngraph
