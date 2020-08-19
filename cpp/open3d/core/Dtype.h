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

#pragma once

#include <string>

#include "open3d/core/Dispatch.h"
#include "open3d/utility/Console.h"

static_assert(sizeof(float) == 4,
              "Unsupported platform: float must be 4 bytes.");
static_assert(sizeof(double) == 8,
              "Unsupported platform: double must be 8 bytes.");
static_assert(sizeof(int) == 4, "Unsupported platform: int must be 4 bytes.");
static_assert(sizeof(int32_t) == 4,
              "Unsupported platform: int32_t must be 4 bytes.");
static_assert(sizeof(int64_t) == 8,
              "Unsupported platform: int64_t must be 8 bytes.");
static_assert(sizeof(uint8_t) == 1,
              "Unsupported platform: uint8_t must be 1 byte.");
static_assert(sizeof(uint16_t) == 2,
              "Unsupported platform: uint16_t must be 2 bytes.");
static_assert(sizeof(bool) == 1, "Unsupported platform: bool must be 1 byte.");

namespace open3d {
namespace core {

enum class Dtype {
    Undefined,  // Dtype for uninitialized Tensor
    Float32,
    Float64,
    Int32,
    Int64,
    UInt8,
    UInt16,
    Bool,
    Object,
    PyObject,
};

class DtypeUtil {
public:
    static bool IsObject(const Dtype &dtype) { return dtype == Dtype::Object; }

    static int64_t ByteSize(const Dtype &dtype) {
        int64_t byte_size = 0;
        switch (dtype) {
            case Dtype::Float32:
                byte_size = 4;
                break;
            case Dtype::Float64:
                byte_size = 8;
                break;
            case Dtype::Int32:
                byte_size = 4;
                break;
            case Dtype::Int64:
                byte_size = 8;
                break;
            case Dtype::UInt8:
                byte_size = 1;
                break;
            case Dtype::UInt16:
                byte_size = 2;
                break;
            case Dtype::Bool:
                byte_size = 1;
                break;
            case Dtype::PyObject:
                byte_size = 8;
                break;
            case Dtype::Object:
                utility::LogError(
                        "Please use sizeof(T) to get byte size for "
                        "object dtype.");
                break;
            default:
                utility::LogError("Unsupported data type.");
        }
        return byte_size;
    }

    /// Convert from C++ types to Dtype. Known types are explicitly specialized,
    /// e.g. DtypeUtil::FromType<float>(). Non built-in types will be regarded
    /// as custom types if is_custom_dtype is set to true. Otherwise they will
    /// result in an exception. is_custom_dtype is disregarded for built-in
    /// C++ types.
    template <typename T>
    static inline Dtype FromType(bool is_object = false) {
        if (is_object) {
            return Dtype::Object;
        }
        utility::LogError("Unsupported data type");
        return Dtype::Undefined;
    }

    static std::string ToString(const Dtype &dtype) {
        std::string str = "";
        switch (dtype) {
            case Dtype::Undefined:
                str = "Undefined";
                break;
            case Dtype::Float32:
                str = "Float32";
                break;
            case Dtype::Float64:
                str = "Float64";
                break;
            case Dtype::Int32:
                str = "Int32";
                break;
            case Dtype::Int64:
                str = "Int64";
                break;
            case Dtype::UInt8:
                str = "UInt8";
                break;
            case Dtype::UInt16:
                str = "UInt16";
                break;
            case Dtype::Bool:
                str = "Bool";
                break;
            case Dtype::Object:
                str = "Object";
                break;
            case Dtype::PyObject:
                str = "PyObject";
                break;
            default:
                utility::LogError("Unsupported data type");
        }
        return str;
    }
};

template <>
inline Dtype DtypeUtil::FromType<float>(bool is_object) {
    return Dtype::Float32;
}

template <>
inline Dtype DtypeUtil::FromType<double>(bool is_object) {
    return Dtype::Float64;
}

template <>
inline Dtype DtypeUtil::FromType<int32_t>(bool is_object) {
    return Dtype::Int32;
}

template <>
inline Dtype DtypeUtil::FromType<int64_t>(bool is_object) {
    return Dtype::Int64;
}

template <>
inline Dtype DtypeUtil::FromType<uint8_t>(bool is_object) {
    return Dtype::UInt8;
}

template <>
inline Dtype DtypeUtil::FromType<uint16_t>(bool is_object) {
    return Dtype::UInt16;
}

template <>
inline Dtype DtypeUtil::FromType<bool>(bool is_object) {
    return Dtype::Bool;
}

// PyObject* will be converted to void* to cpp
// and reconstructed with PyObject*
template <>
inline Dtype DtypeUtil::FromType<void *>(bool is_object) {
    return Dtype::PyObject;
}

}  // namespace core
}  // namespace open3d
