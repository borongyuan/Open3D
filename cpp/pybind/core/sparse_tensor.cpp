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

#include <pybind11/cast.h>
#include <pybind11/pytypes.h>

#include "open3d/core/SparseTensor.h"
#include "open3d/utility/Console.h"
#include "pybind/core/core.h"
#include "pybind/docstring.h"
#include "pybind/open3d_pybind.h"

namespace open3d {
namespace core {
void pybind_core_sparse_tensor(py::module& m) {
    py::class_<SparseTensor> sparse_tensor(
            m, "SparseTensor",
            "A SparseTensor is a map from coordinate tensors to element "
            "tensors without bounds of coordinates.");

    sparse_tensor.def(py::init<const Tensor&, const Tensor&, bool>(),
                      "coords"_a, "elems"_a, "insert"_a = false);

    sparse_tensor.def("insert_entries", &SparseTensor::InsertEntries);
    sparse_tensor.def("activate_entries", &SparseTensor::ActivateEntries);
    sparse_tensor.def("find_entries", &SparseTensor::FindEntries);
    sparse_tensor.def("erase_entries", &SparseTensor::EraseEntries);
    sparse_tensor.def("get_elems_list", &SparseTensor::GetElemsList);
}
}  // namespace core
}  // namespace open3d
