add_subdirectory(debug)
# TODO: reformat and fix these files
# add_subdirectory(profile)

EXAMPLE_CUDA(ViewFragments ${CMAKE_PROJECT_NAME} ${OPEN3D_CUDA_MODULE} ${JSONCPP_TARGET})
EXAMPLE_CUDA(ViewPoseGraph ${CMAKE_PROJECT_NAME} ${OPEN3D_CUDA_MODULE} ${JSONCPP_TARGET})
EXAMPLE_CUDA(RunSystem     ${CMAKE_PROJECT_NAME} ${OPEN3D_CUDA_MODULE} ${JSONCPP_TARGET})
