//
// Created by wei on 3/28/19.
//

#include "ScalableTSDFVolumeCudaIO.h"
#include "ZlibIO.h"

namespace open3d {
namespace io {

bool WriteScalableTSDFVolumeToBIN(const std::string &filename,
                          cuda::ScalableTSDFVolumeCuda &volume,
                          bool use_zlib) {
    auto key_value = volume.DownloadVolumes();

    auto keys = key_value.first;
    auto values = key_value.second;
    assert(keys.size() == values.size());

    FILE *fid = fopen(filename.c_str(), "wb");
    if (fid == NULL) {
        utility::LogWarning("Write BIN failed: unable to open file: %s\n",
                              filename.c_str());
        return false;
    }

    /** metadata **/
    int num_volumes = keys.size(), volume_size = volume.N_;
    if (fwrite(&num_volumes, sizeof(int), 1, fid) < 1) {
        utility::LogWarning("Write BIN failed: unable to write num volumes\n");
        return false;
    }
    if (fwrite(&volume_size, sizeof(int), 1, fid) < 1) {
        utility::LogWarning("Write BIN failed: unable to write volume size\n");
        return false;
    }

    /** keys **/
    if (!Write(keys, fid, "keys")) {
        return false;
    }

    /** subvolumes **/
    std::vector<uchar> compressed_buf;
    if (use_zlib) {
        compressed_buf.resize( /* provide some redundancy */
            volume.N_ * volume.N_ * volume.N_ * sizeof(float) * 2);
    }

    utility::LogInfo("Writing {} subvolumes.\n", keys.size());
    for (auto &subvolume : values) {
        auto &tsdf = subvolume.tsdf_;
        auto &weight = subvolume.weight_;
        auto &color = subvolume.color_;

        if (!use_zlib) {
            if (!Write(tsdf, fid, "TSDF")) { return false; }
            if (!Write(weight, fid, "weight")) { return false; }
            if (!Write(color, fid, "color")) { return false; }
        } else {
            if (!CompressAndWrite(compressed_buf, tsdf, fid, "TSDF")) {
                return false;
            }
            if (!CompressAndWrite(compressed_buf, weight, fid, "weight")) {
                return false;
            }
            if (!CompressAndWrite(compressed_buf, color, fid, "color")) {
                return false;
            }
        }
    }

    fclose(fid);
    return true;
}

bool ReadScalableTSDFVolumeFromBIN(const std::string &filename,
                           cuda::ScalableTSDFVolumeCuda &volume,
                           bool use_zlib,
                           int batch_size) {
    FILE *fid = fopen(filename.c_str(), "rb");
    if (fid == NULL) {
        utility::LogWarning("Read BIN failed: unable to open file: %s\n",
                              filename.c_str());
        return false;
    }

    /** metadata **/
    int num_volumes, volume_size;
    if (fread(&num_volumes, sizeof(int), 1, fid) < 1) {
        utility::LogWarning("Read BIN failed: unable to read num volumes\n");
        return false;
    }
    if (fread(&volume_size, sizeof(int), 1, fid) < 1) {
        utility::LogWarning("Read BIN failed: unable to read volume size\n");
        return false;
    }
    assert(volume_size == volume.N_);

    int N = volume.N_;
    int NNN = N * N * N;

    /** keys **/
    std::vector<cuda::Vector3i> keys(num_volumes);
    if (!Read(keys, fid, "keys")) {
        return false;
    }

    /** values **/
    std::vector<float> tsdf_buf(NNN);
    std::vector<uchar> weight_buf(NNN);
    std::vector<cuda::Vector3b> color_buf(NNN);

    std::vector<int> failed_key_indices;
    std::vector<cuda::Vector3i> failed_keys;
    std::vector<cuda::ScalableTSDFVolumeCpuData> failed_subvolumes;

    /** Updating (key, value) pairs in parallel is tricky:
     * - thread lock can forbid some block to be allocated;
     * - split all the pairs them into batches can increase success rate;
     * - we should retry to insert stubborn failure pairs until they get in. **/
    if (batch_size <= 0) {
        batch_size = int(volume.hash_table_.bucket_count_ * 0.2f);
    }
    int num_batches = (num_volumes + batch_size - 1) / batch_size;

    std::vector<uchar> compressed_buf
        (volume.N_ * volume.N_ * volume.N_ * sizeof(float) * 2);
    for (int batch = 0; batch < num_batches; ++batch) {
        std::vector<cuda::Vector3i> batch_keys;
        std::vector<cuda::ScalableTSDFVolumeCpuData> batch_subvolumes;

        int begin = batch * batch_size;
        int end = std::min((batch + 1) * batch_size, num_volumes);

        for (int i = begin; i < end; ++i) {
            if (!use_zlib) {
                if (!Read(tsdf_buf, fid, "TSDF")) { return false; }
                if (!Read(weight_buf, fid, "weight")) { return false; }
                if (!Read(color_buf, fid, "color")) { return false; }
            } else {
                if (!ReadAndUncompress(compressed_buf, tsdf_buf, fid, "TSDF")) {
                    return false;
                }
                if (!ReadAndUncompress(compressed_buf, weight_buf, fid, "weight")) {
                    return false;
                }
                if (!ReadAndUncompress(compressed_buf, color_buf, fid, "uchar")) {
                    return false;
                }
            }
            batch_keys.emplace_back(keys[i]);
            batch_subvolumes.emplace_back(cuda::ScalableTSDFVolumeCpuData(
                tsdf_buf, weight_buf, color_buf));
        }

        volume.UploadVolumes(batch_keys, batch_subvolumes);
    }
    fclose(fid);

    return true;
}
} // io
} // open3d