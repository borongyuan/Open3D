//
// Created by wei on 4/15/19.
//

#include "DrawGeometryPBR.h"

namespace open3d {
namespace visualization {

bool DrawGeometriesPBR(
    const std::vector<std::shared_ptr<const geometry::Geometry>> &geometry_ptrs,
    const std::vector<std::shared_ptr<geometry::Lighting>> &lightings,
    const std::string &window_name /* = "Open3D"*/,
    int width /* = 640*/,
    int height /* = 480*/,
    int left /* = 50*/,
    int top /* = 50*/) {

    VisualizerPBR visualizer;

    if (! visualizer.CreateVisualizerWindow(
        window_name, width, height, left, top)) {
        utility::PrintWarning(
            "[DrawGeometries] Failed creating OpenGL window.\n");
        return false;
    }

    for (int i = 0; i < geometry_ptrs.size(); ++i) {
        if (! visualizer.AddGeometryPBR(
            geometry_ptrs[i], lightings[i])) {
            utility::PrintWarning(
                "[DrawGeometriesPBR] Failed adding geometry.\n");
            utility::PrintWarning(
                "[DrawGeometriesPBR] Possibly due to bad geometry or wrong "
                "geometry type.\n");
            return false;
        }
    }

    visualizer.Run();
    visualizer.DestroyVisualizerWindow();
    return true;
}

}
}