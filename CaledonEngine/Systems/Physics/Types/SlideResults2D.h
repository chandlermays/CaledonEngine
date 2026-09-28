/*------------------------------
| File: SlideResults2D.h
| Author: Chandler Mays
------------------------------*/
#pragma once

// Struct (SlideResults2D)
// The results of a slide movement performed with RigidBody2D.Slide
// These results can be used to both tune movement configuration and to implemnet further logic to react to the specific surfaces encountered when a slide occurs
//
// Properties:
// int iterationsUsed			:			returns the number of iterations used when performing a Slide
// Vec2 position				:			the position that was calculated as a target position to move to when performing a Slide
// Vec2 remainingVelocity		:			returns the remaining velocity that couldn't be used when performing a Slide
// Struct slideHit				:			the specific contact found when performing a Slide; when a slide along a surface occurs, the slide may hit a surface tangent to the movement
// Struct surfaceHit			:			the specific contact found when performing a Slide; if the movement anchors to a surface or if graviy causes a contact with the surface