// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018 www.open3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#include "Open3D/Core/Hashmap/HashmapBase.h"
#include "Open3D/Core/Tensor.h"

namespace open3d {

class TensorHash {
public:
    // virtual TensorHash(Tensor coords, Tensor indices) = 0;
    virtual std::pair<Tensor, Tensor> Query(Tensor coords) = 0;

protected:
    std::shared_ptr<Hashmap<DefaultHash>> hashmap_;
    Dtype key_type_;
    Dtype value_type_;

    int64_t key_dim_;
};

class CPUTensorHash : public TensorHash {
public:
    CPUTensorHash(Tensor coords, Tensor indices);
    std::pair<Tensor, Tensor> Query(Tensor coords);
};

class CUDATensorHash : public TensorHash {
public:
    CUDATensorHash(Tensor coords, Tensor indices);
    std::pair<Tensor, Tensor> Query(Tensor coords);
};

/// Factory
std::shared_ptr<CPUTensorHash> CreateCPUTensorHash(Tensor coords,
                                                   Tensor indices);
std::shared_ptr<CUDATensorHash> CreateCUDATensorHash(Tensor coords,
                                                     Tensor indices);
std::shared_ptr<TensorHash> CreateTensorHash(Tensor coords, Tensor indices);

/// Legacy
std::shared_ptr<Hashmap<DefaultHash>> IndexTensorCoords(Tensor coords,
                                                        Tensor indices);

// Returns mapped indices and corresponding masks as Tensors
std::pair<Tensor, Tensor> QueryTensorCoords(
        std::shared_ptr<Hashmap<DefaultHash>> hashmap, Tensor coords);

}  // namespace open3d