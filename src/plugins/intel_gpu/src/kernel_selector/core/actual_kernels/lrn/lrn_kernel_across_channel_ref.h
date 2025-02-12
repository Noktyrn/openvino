﻿// Copyright (C) 2018-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once
#include <vector>
#include "lrn_kernel_base.h"

namespace kernel_selector {
class LRNKernelAcrossChannelRef : public LRNKernelBase {
public:
    using Parent = LRNKernelBase;

    LRNKernelAcrossChannelRef() : Parent("lrn_gpu_across_channel_ref") {}
    virtual ~LRNKernelAcrossChannelRef() {}

    KernelsData GetKernelsData(const Params& params, const optional_params& options) const override;
    KernelsPriority GetKernelsPriority(const Params& params, const optional_params& options) const override;
    ParamsKey GetSupportedKey() const override;

protected:
    DispatchData SetDefault(const lrn_params& params) const override;
    std::vector<FusedOpType> GetSupportedFusedOps() const override {
        return { FusedOpType::QUANTIZE,
                 FusedOpType::SCALE,
                 FusedOpType::ACTIVATION };
    }
    JitConstants GetJitConstants(const lrn_params& params, const DispatchData& dispatchData) const override;
};
}  // namespace kernel_selector
