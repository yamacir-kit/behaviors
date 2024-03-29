#ifndef INCLUDED_BEHAVIORS_EXPRESSION_DISPATCH_HPP
#define INCLUDED_BEHAVIORS_EXPRESSION_DISPATCH_HPP

#include <type_traits>
#include <utility>

namespace behaviors { namespace expression
{
  template <typename...>
  struct dispatcher;

  template <typename F>
  struct dispatcher<F>
    : public F
  {
    constexpr dispatcher(F&& function)
      : F {std::forward<F>(function)}
    {}

    using F::operator();
  };

  template <typename F1, typename F2, typename... Fs>
  struct dispatcher<F1, F2, Fs...>
    : public F1
    , public dispatcher<F2, Fs...>
  {
    constexpr dispatcher(F1&& f1, F2&& f2, Fs&&... fs)
      : F1 {std::forward<F1>(f1)}
      , dispatcher<F2, Fs...> {std::forward<F2>(f2), std::forward<Fs>(fs)...}
    {}

    using F1::operator();
    using dispatcher<F2, Fs...>::operator();
  };

  template <typename... Fs>
  constexpr auto dispatch(Fs&&... fs)
    -> dispatcher<typename std::decay<Fs>::type...>
  {
    return {std::forward<Fs>(fs)...};
  }
}} // namespace behaviors::expression

#endif // INCLUDED_BEHAVIORS_EXPRESSION_DISPATCH_HPP

