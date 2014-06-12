#pragma once

#include <QtWidgets>

class Track;

class TrackUtils
{
public:
    static void RenderTrackBackground(QPainter *painter, const QRect &rect, const Track* inTrack);
    static void RenderFloatTrack(QPainter *painter, const QRect &rect, const Track* inTrack, bool inIsEditMode, const QPoint& inMousePos);
    static void RenderFloatTrackHeader(QPainter *painter, const QRect &rect, const Track* inTrack);

    static inline float NormalizeDataValue(float inMinDataValue, float inMaxDataValue, float inDataValue)
    {
        return (inDataValue - inMinDataValue) / (inMaxDataValue - inMinDataValue);
    }

    static inline float Distance(const QPoint& inPointA, const QPoint& inPointB)
    {
        return sqrt(pow(inPointA.x() - inPointB.x(), 2) + pow(inPointA.y() - inPointB.y(), 2));
    }
};
