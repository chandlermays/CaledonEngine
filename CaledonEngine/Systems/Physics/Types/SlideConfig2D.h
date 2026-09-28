/*------------------------------
| File: SlideConfig2D.h
| Author: Chandler Mays
------------------------------*/
#pragma once

// Struct (SlideConfig)
// The configuration that controls how a RigidBody2D.Slide method behaves
//
// Properties:
// Vec2 gravity							:			the gravity to be applied to the slide position
// float gravitySlipAngle				:			when the gravity movement causes a contact with a Collider2D, slippage may occur if the surface angle is greater than this angle
// int maxIterations					:			controls the maximum number of iterations to perform when determining how a RigidBody2D will slide
// Collider2D selectedCollider			:			the specific Collider2D attached to this RigidBody2D to be used to detect contacts
// Vec2 startPosition					:			the start position to slide the RigidBody2D from