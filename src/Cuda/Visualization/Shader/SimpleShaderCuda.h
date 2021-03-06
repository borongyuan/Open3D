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

#include <vector>
#include <Eigen/Core>
#include <Open3D/Visualization/Shader/ShaderWrapper.h>

#include <Cuda/Common/UtilsCuda.h>
#include <Cuda/Common/LinearAlgebraCuda.h>

#ifdef __CUDACC__
#include <cuda_gl_interop.h>
#endif

namespace open3d {
namespace visualization {

namespace glsl {

class SimpleShaderCuda : public ShaderWrapper {
public:
    ~SimpleShaderCuda() override { Release(); }

protected:
    SimpleShaderCuda(const std::string &name)
        : ShaderWrapper(name) { Compile(); }

protected:
    bool Compile() final;
    void Release() final;

    bool BindGeometry(const geometry::Geometry &geometry,
                      const RenderOption &option,
                      const ViewControl &view) final;
    bool RenderGeometry(const geometry::Geometry &geometry,
                        const RenderOption &option,
                        const ViewControl &view) final;
    void UnbindGeometry() final;

protected:
    virtual bool PrepareRendering(const geometry::Geometry &geometry,
                                  const RenderOption &option,
                                  const ViewControl &view) = 0;
    virtual bool PrepareBinding(const geometry::Geometry &geometry,
                                const RenderOption &option,
                                const ViewControl &view,
                                cuda::Vector3f *&vertices,
                                cuda::Vector3f *&colors,
                                cuda::Vector3i *&triangles,
                                int &vertex_size,
                                int &triangle_size) = 0;

protected:
    GLuint vertex_position_;
    GLuint vertex_position_buffer_;
    cudaGraphicsResource_t vertex_position_cuda_resource_;

    GLuint vertex_color_;
    GLuint vertex_color_buffer_;
    cudaGraphicsResource_t vertex_color_cuda_resource_;

    /** Only initialized for mesh **/
    GLuint triangle_buffer_;
    cudaGraphicsResource_t triangle_cuda_resource_;

    GLuint MVP_;
};

class SimpleShaderForPointCloudCuda : public SimpleShaderCuda {
public:
    SimpleShaderForPointCloudCuda() :
        SimpleShaderCuda("SimpleShaderForPointCloudCuda") {}

protected:
    bool PrepareRendering(const geometry::Geometry &geometry,
                          const RenderOption &option,
                          const ViewControl &view) final;
    bool PrepareBinding(const geometry::Geometry &geometry,
                        const RenderOption &option,
                        const ViewControl &view,
                        cuda::Vector3f *&vertices,
                        cuda::Vector3f *&colors,
                        cuda::Vector3i *&triangles, /* Place holder */
                        int &vertex_size,
                        int &triangle_size /* Place holder */) final;
};

class SimpleShaderForTriangleMeshCuda : public SimpleShaderCuda {
public:
    SimpleShaderForTriangleMeshCuda() :
        SimpleShaderCuda("SimpleShaderForTriangleMeshCuda") {}

protected:
    bool PrepareRendering(const geometry::Geometry &geometry,
                          const RenderOption &option,
                          const ViewControl &view) final;
    bool PrepareBinding(const geometry::Geometry &geometry,
                        const RenderOption &option,
                        const ViewControl &view,
                        cuda::Vector3f *&vertices,
                        cuda::Vector3f *&colors,
                        cuda::Vector3i *&triangles,
                        int &vertex_size,
                        int &triangle_size) final;
};

}    // namespace open3d::glsl
}    // visualization
}    // namespace open3d
