/*------------------------------
| File: BodyInterpolation2D.h
| Author: Chandler Mays
------------------------------*/
#pragma once

// Enumeration (BodyInterpolation2D)
// Interpolation mode for RigidBody2D objects
//
// Properties:
// None						:			Do not apply any smoothing to the object's movement
// Interpolate				:			Smooth movement based on the object's positions in previous frames
// Extrapolate				:			Smooth an object's movement based on an estimate of its position in the next frame