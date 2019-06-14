#ifndef INCLUDED_BEHAVIOR_BASED_EXPRESSION_FOLD_HPP
#define INCLUDED_BEHAVIOR_BASED_EXPRESSION_FOLD_HPP

#include <utility>

#include <behavior_based/configure.hpp>

namespace NAMESPACE { namespace expression
{
  template <typename T, typename Initial, typename BinaryOperation>
  constexpr decltype(auto) fold_left(const list<T>& list, Initial&& initial, BinaryOperation&& operation)
  {
    return operation(std::forward<Initial>(initial), car(list));
  }

  template <typename T, typename U, typename... Ts, typename Initial, typename BinaryOperation>
  constexpr decltype(auto) fold_left(const list<T, U, Ts...>& list, Initial&& initial, BinaryOperation&& operation)
  {
    auto&& lhs {operation(std::forward<Initial>(initial), car(list))};
    return fold_left(cdr(list), std::forward<decltype(lhs)>(lhs), std::forward<BinaryOperation>(operation));
  }
}} // namespace NAMESPACE::expression

#endif // INCLUDED_BEHAVIOR_BASED_EXPRESSION_FOLD_HPP
