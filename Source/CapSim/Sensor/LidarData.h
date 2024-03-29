#pragma once

#include "CapSim/Sensor/SemanticLidarData.h"

#include <cstdint>
#include <vector>
#include <numeric>

#include "Math/Vector.h"


/// Helper class to store and serialize the data generated by a Lidar.
///
/// The header of a Lidar measurement consists of an array of uint32_t's in
/// the following layout
///
///    {
///      Horizontal angle (float),
///      Channel count,
///      Point count of channel 0,
///      ...
///      Point count of channel n,
///    }
///
/// The points are stored in an array of floats
///
///    {
///      X0, Y0, Z0, I0
///      ...
///      Xn, Yn, Zn, In
///    }
///

class LidarDetection {
public:
    FVector point{};
    float intensity{};

    LidarDetection() :
        point(0.0f, 0.0f, 0.0f), intensity{ 0.0f } { }
    LidarDetection(float x, float y, float z, float intensity) :
        point(x, y, z), intensity{ intensity } { }
    LidarDetection(FVector p, float intensity) :
        point(p), intensity{ intensity } { }

    void WritePlyHeaderInfo(std::ostream& out) const {
        out << "property float32 x\n" \
            "property float32 y\n" \
            "property float32 z\n" \
            "property float32 I";
    }

    void WriteDetection(std::ostream& out) const {
        out << point.X << ' ' << point.Y << ' ' << point.Z << ' ' << intensity;
    }
};

class LidarData : public SemanticLidarData {

public:
    explicit LidarData(uint32_t ChannelCount = 0u)
        : SemanticLidarData(ChannelCount) {
    }

    LidarData& operator=(LidarData&&) = default;

    ~LidarData() = default;

    virtual void ResetMemory(std::vector<uint32_t> points_per_channel) {
        
        std::memset(_header.data() + Index::SIZE, 0, sizeof(uint32_t) * GetChannelCount());

        uint32_t total_points = static_cast<uint32_t>(
            std::accumulate(points_per_channel.begin(), points_per_channel.end(), 0));

        _points.clear();
        _points.reserve(total_points * 4);
    }

    void WritePointSync(LidarDetection& detection) {
        _points.emplace_back(detection.point.X);
        _points.emplace_back(detection.point.Y);
        _points.emplace_back(detection.point.Z);
        _points.emplace_back(detection.intensity);
    }

    virtual void WritePointSync(SemanticLidarDetection& detection) {
        (void)detection;
    }

    void PrintLidarDetections() override {
        
        //UE_LOG(LogTemp, Warning, TEXT("Number of detections: %d"), _points.size());


        FString PlyHeader = FString::Printf(TEXT("ply\nformat ascii 1.0\nelement vertex %d\nproperty float32 x\n"\
            "property float32 y\nproperty float32 z\nproperty float32 I\n" \
            "end_header\n"), _points.size()/4);

        FString lines = "";

        for (int i = 0; i < _points.size() / 4; i++) {
            FString line = FString::Printf(TEXT("%f %f %f %f\n"), _points[i * 4], _points[i * 4 + 1],
                _points[i * 4 + 2], _points[i * 4 + 3]);
            lines += line;
        }

        PointString = "";
        PointString += PlyHeader;
        PointString += lines;
        
    }

private:
    std::vector<float> _points;

};
