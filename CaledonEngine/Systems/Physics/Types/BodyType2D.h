#pragma once

// Enumeration (BodyType2D)
// The physical behavior type of the RigidBody2D
//
// Controls the physical behavior of a RigidBody2D in how it should move, react to forces and interact with the rest of the physics simulation
//
// Properties:
// Dynamic			:		Sets the RigidBody2D to have dynamic behavior
//							Causes the RigidBody2D to react to gravity and applied forces
//							Should be moved using forces and never repositioned explicitly
//							Will collide with all other body types
//							When an attached Collider2D is set to trigger, it will always produce a trigger for any Collider2D attached to all other body types
//
// Kinematic		:		Sets the RigidBody2D to have kinematic behavior
//							Stops the RigidBody2D from reacting to gravity or applied forces
//							Can be moved by settings its RigidBody2D.linearVelocity or RigidBody2D.angularVelocity or by being repositioned explicitly
//							Will only collide with a dynamic body type
//							When an attached Collider2D is set to trigger, it will always produce a trigger for any Collider2D attached to all other body types
// 
// Static			:		Sets the RigidBody2D to have static behavior
//							Stops the RigidBody2D from reacting to gravity or applied forces
//							Should never be repositioned explicitly. It is designed to never move.
//							Will only collide with a dynamic body type
//							When an attached Collider2D is set to trigger, it will always produce a trigger for any Collider2D attached to Dynamic or Kinematic body types