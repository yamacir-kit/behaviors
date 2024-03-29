#ifndef INCLUDED_BEHAVIORS_SEMANTICS_TARGET_HPP
#define INCLUDED_BEHAVIORS_SEMANTICS_TARGET_HPP

#include <algorithm>

#include <lgsvl_msgs/Detection3DArray.h>
#include <sensor_msgs/Joy.h>

#include <behaviors/semantics/define.hpp>

namespace behaviors { namespace semantics
{
  DEFINE_SEMANTICS_CATEGORY(target, vector_type::Zero());

  template <typename Target>
  struct target_traits
  {
    using vector_type = typename Target::vector_type;
  };

  DEFINE_SEMANTICS_CATEGORY_SPECIALIZATION(target, sensor_msgs::Joy,
  {
    return {message->axes[1], message->axes[2]};
  })

  DEFINE_SEMANTICS_CATEGORY_SPECIALIZATION(target, lgsvl_msgs::Detection3DArray,
  {
    // std::cerr << "message->detections.size() = " << message->detections.size() << std::endl;

    if (not message->detections.empty())
    {
      auto distance = [](const auto& detection)
      {
        return std::sqrt(
                 std::pow(detection.bbox.position.position.x, 2)
               + std::pow(detection.bbox.position.position.y, 2)
               );
      };

      const auto iter {std::min_element(
        message->detections.begin(),
        message->detections.end(),
        [&](const auto& lhs, const auto& rhs)
        {
          return distance(lhs) < distance(rhs);
        }
      )};

      // p.145
      auto magnitude = [&](const auto& detection)
      {
        // Sphere of influence (radial extent of force from the center of the obstacle).
        constexpr auto s {20.0};

        // Radius of obstacle.
        constexpr auto r {2.0};

        // Distance of robot to center of obstacle.
        const auto d {distance(detection)};

        if (s < d)
        {
          std::cerr << "0.0" << std::endl;
          return 0.0;
        }
        else if (r < d && d <= s)
        {
          std::cerr << (s - d) / (s - r) << std::endl;
          return (s - d) / (s - r);
        }
        else // d <= r
        {
          std::cerr << "1.0" << std::endl;
          return 1.0;
        }

        // return 1.0 / std::pow(distance(detection), 2);
      };

      return vector_type {iter->bbox.position.position.x, iter->bbox.position.position.y} * magnitude(*iter);
    }
    else
    {
      return vector_type::Zero();
    }
  })
}} // namespace behaviors::semantics

#endif // INCLUDED_BEHAVIORS_SEMANTICS_TARGET_HPP

