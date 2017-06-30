// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2017 Jaesik Park <syncle@gmail.com>
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

#include <iostream> 
#include <vector>
#include <tuple>
#include <Eigen/Core>
#include <Core/Odometry/OdometryOption.h>
#include <Core/Utility/Eigen.h>

namespace three {

class Image;

class RGBDImage;

typedef std::vector<Eigen::Vector4i> CorrespondenceSetPixelWise;

/// Base class that computes Jacobian from two RGB-D images
class RGBDOdometryJacobian
{
public:
	RGBDOdometryJacobian() {}
	virtual ~RGBDOdometryJacobian() {}

public:
	/// Function to compute JTJ and JTr
	virtual std::tuple<Eigen::Matrix6d, Eigen::Vector6d> 
			ComputeJacobianAndResidual(
			const RGBDImage &source, const RGBDImage &target,
			const Image &source_xyz,
			const RGBDImage &target_dx, const RGBDImage &target_dy,
			const Eigen::Matrix3d &intrinsic,
			const Eigen::Matrix4d &extrinsic,
			const CorrespondenceSetPixelWise &corresps) const = 0;
};

/// Function to Compute Jacobian using color term 
/// Energy: (I_p-I_q)^2
/// reference: 
/// F. Steinbrucker, J. Sturm, and D. Cremers. 
/// Real-time visual odometry from dense RGB-D images.
/// In ICCV Workshops, 2011.
class RGBDOdometryJacobianFromColorTerm : public RGBDOdometryJacobian
{
public:
	RGBDOdometryJacobianFromColorTerm() {}
	~RGBDOdometryJacobianFromColorTerm() override {}

public:
	std::tuple<Eigen::Matrix6d, Eigen::Vector6d> ComputeJacobianAndResidual(
			const RGBDImage &source, const RGBDImage &target,
			const Image &source_xyz,
			const RGBDImage &target_dx, const RGBDImage &target_dy,
			const Eigen::Matrix3d &intrinsic,
			const Eigen::Matrix4d &extrinsic,
			const CorrespondenceSetPixelWise &corresps) const override;
};

/// Function to Compute Jacobian using hybrid term 
/// Energy: (I_p-I_q)^2 + lambda(D_p-(D_q)')^2
/// reference: 
/// J. Park, Q.-Y. Zhou, and V. Koltun
/// anonymous submission
class RGBDOdometryJacobianFromHybridTerm : public RGBDOdometryJacobian
{
public:
	RGBDOdometryJacobianFromHybridTerm() {}
	~RGBDOdometryJacobianFromHybridTerm() override {}

public:
	std::tuple<Eigen::Matrix6d, Eigen::Vector6d> ComputeJacobianAndResidual(
			const RGBDImage &source, const RGBDImage &target,
			const Image &source_xyz,
			const RGBDImage &target_dx, const RGBDImage &target_dy,
			const Eigen::Matrix3d &intrinsic,
			const Eigen::Matrix4d &extrinsic,
			const CorrespondenceSetPixelWise &corresps) const override;
};

}	// namespace three