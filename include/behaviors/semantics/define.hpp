#ifndef INCLUDED_BEHAVIORS_SEMANTICS_DEFINE_HPP
#define INCLUDED_BEHAVIORS_SEMANTICS_DEFINE_HPP

#include <utility>

#include <behaviors/expression/list.hpp>
#include <behaviors/semantics/facade.hpp>

#define DEFINE_SEMANTICS_CATEGORY(CATEGORY_NAME, DEFAULT_VECTOR)               \
                                                                               \
template <typename...>                                                         \
struct CATEGORY_NAME;                                                          \
                                                                               \
template <>                                                                    \
struct CATEGORY_NAME<expression::unit>                                         \
  : public facade<CATEGORY_NAME, expression::unit>                             \
  , public expression::unit                                                    \
{                                                                              \
  template <typename... Ts>                                                    \
  constexpr CATEGORY_NAME(Ts&&... xs)                                          \
    : expression::unit {std::forward<Ts>(xs)...}                               \
  {}                                                                           \
                                                                               \
  template <typename... Ts>                                                    \
  decltype(auto) operator()(Ts&&...) const noexcept                            \
  {                                                                            \
    return DEFAULT_VECTOR;                                                     \
  }                                                                            \
}


#define DEFINE_SEMANTICS_CATEGORY_SPECIALIZATION(CATEGORY, TYPE, ...)          \
template <>                                                                    \
struct CATEGORY<TYPE>                                                          \
  : public facade<CATEGORY, TYPE>                                              \
{                                                                              \
  vector_type operator()(const TYPE::ConstPtr& message) const                  \
  {                                                                            \
    if (message) __VA_ARGS__ else return CATEGORY<expression::unit> {}();      \
  }                                                                            \
};


#endif // INCLUDED_BEHAVIORS_SEMANTICS_DEFINE_HPP

