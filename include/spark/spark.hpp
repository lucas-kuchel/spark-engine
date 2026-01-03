#pragma once

#if SPARK_SUPPORTED

#ifdef SPARK_ECS_AVAILABLE
#include <spark/ecs/interface.hpp>
#endif

#ifdef SPARK_EVENTS_AVAILABLE
#include <spark/events/interface.hpp>
#endif

#ifdef SPARK_GRAPHICS_AVAILABLE
#include <spark/graphics/interface.hpp>
#endif

#ifdef SPARK_MATH_AVAILABLE
#include <spark/math/interface.hpp>
#endif

#ifdef SPARK_TYPES_AVAILABLE
#include <spark/types/interface.hpp>
#endif

#endif