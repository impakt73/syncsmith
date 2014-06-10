#pragma once

#include <QtWidgets>

class Track;

class TrackUtils
{
public:
    static void RenderTrackBackground(QPainter *painter, const QRect &rect, const Track* inTrack);
    static void RenderFloatTrack(QPainter *painter, const QRect &rect, const Track* inTrack, bool inIsEditMode);

    static inline float NormalizeDataValue(float inMinDataValue, float inMaxDataValue, float inDataValue)
    {
        return (inDataValue - inMinDataValue) / (inMaxDataValue - inMinDataValue);
    }
};
