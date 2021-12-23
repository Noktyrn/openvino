// Copyright (C) 2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "mkldnn_shapeof.h"
#include <ngraph/opsets/opset1.hpp>

using namespace MKLDNNPlugin;
using namespace InferenceEngine;

bool MKLDNNShapeOfNode::isSupportedOperation(const std::shared_ptr<const ngraph::Node>& op, std::string& errorMessage) noexcept {
    try {
        if (!one_of(op->get_type_info(),
                    ngraph::op::v0::ShapeOf::get_type_info_static(),
                    ngraph::op::v3::ShapeOf::get_type_info_static())) {
            errorMessage = "Node is not an instance of ShapeOf form the operation set v1 or v3.";
            return false;
        }
    } catch (...) {
        return false;
    }
    return true;
}

MKLDNNShapeOfNode::MKLDNNShapeOfNode(const std::shared_ptr<ngraph::Node>& op, const mkldnn::engine& eng,
                                     MKLDNNWeightsSharing::Ptr &cache) : MKLDNNNode(op, eng, cache) {
    std::string errorMessage;
    if (isSupportedOperation(op, errorMessage)) {
        errorPrefix = "ShapeOf layer with name '" + getName() + "' ";
        if (op->get_input_partial_shape(0).size() == 0)
            IE_THROW() << errorPrefix << "gets unsupported input 0D tensor (scalar)";
    } else {
        IE_THROW(NotImplemented) << errorMessage;
    }
}

void MKLDNNShapeOfNode::getSupportedDescriptors() {
    if (!descs.empty())
        return;
    if (getParentEdges().size() != 1)
        IE_THROW() << errorPrefix << "has incorrect number of input edges: " << getParentEdges().size();
    if (getChildEdges().empty())
        IE_THROW() << errorPrefix << "has incorrect number of output edges: " << getChildEdges().size();
}

void MKLDNNShapeOfNode::initSupportedPrimitiveDescriptors() {
    if (!supportedPrimitiveDescriptors.empty())
        return;

    Precision precision = getOriginalInputPrecisionAtPort(0);

    const LayoutType dataFormats[4] = { LayoutType::ncsp, LayoutType::nspc, LayoutType::nCsp16c, LayoutType::nCsp8c };
    for (const auto &df : dataFormats) {
        addSupportedPrimDesc({{df, precision}},
                             {{LayoutType::ncsp, Precision::I32}},
                             impl_desc_type::ref);
    }
}

bool MKLDNNShapeOfNode::isExecutable() const {
    return true;
}

void MKLDNNShapeOfNode::execute(mkldnn::stream strm) {
    auto inPtr = getParentEdgeAt(0)->getMemoryPtr();
    auto outPtr = getChildEdgeAt(0)->getMemoryPtr();
    auto inDims = inPtr->getStaticDims();
    size_t dimsCount = inDims.size();
    if (outPtr->getStaticDims().size() != 1 || dimsCount != outPtr->getStaticDims()[0])
        IE_THROW() << errorPrefix << "has inconsistent input shape and output size";

    auto *dst = reinterpret_cast<int *>(getChildEdgeAt(0)->getMemoryPtr()->GetPtr());

    for (size_t i = 0; i < dimsCount; i++) {
        dst[i] = inDims[i];
    }
}

bool MKLDNNShapeOfNode::created() const {
    return getType() == ShapeOf;
}

REG_MKLDNN_PRIM_FOR(MKLDNNShapeOfNode, ShapeOf)
