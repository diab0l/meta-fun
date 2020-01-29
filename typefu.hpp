#ifndef __TYPEFU_HPP__
#define __TYPEFU_HPP__

#include <type_traits>

namespace Typefu {
	template<template<typename...> typename f>
	struct curry {
		private:
		template<typename g, typename... xs>
		struct is_saturated {
			private:
			template<typename C> static std::true_type check(typename C::template apply<xs...>*);
			template<typename C> static std::false_type check(...);

			public:
			static const bool value = decltype(check<g>(nullptr))::value;
		};

		template<template<typename...> typename g>
		struct meta_lift {
			struct type {
				template<typename... xs>
				using apply = g<xs...>;
			};
		};

		template<template<typename...> typename g, typename ...xs> 
		struct f_apply {
			using type = typename g<xs...>::type;
		};

		struct curry_one {
			template<typename x, typename... xs>
			struct application {
				template<typename y>
				using apply = 
					typename
					std::conditional_t<
						is_saturated<typename meta_lift<f>::type, x, xs..., y>::value,
						f_apply<f, x, xs..., y>,
						application<x, xs..., y>
					>::type;

				using type = application<x, xs...>;
			};

			template<typename x>
			using apply =
				typename
				std::conditional_t<
					is_saturated<meta_lift<f>, x>::value,
					f_apply<f, x>,
					application<x>
				>::type;
		};

		template<typename p, typename... xs>
		struct recursive_apply {
			using type = p;
		};

		template<typename p, typename x, typename... xs>
		struct recursive_apply<p, x, xs...> {
			using type = typename recursive_apply<typename p::template apply<x>, xs...>::type;
		};

		public:
		template<typename... xs>
		using apply = typename recursive_apply<curry_one, xs...>::type;
	};

	template<typename a, template<typename> typename... tids>
	struct is_type {
		static bool const value = false;
	};

	template<typename a, template<typename> typename tid, template<typename> typename... tids>
	struct is_type<a, tid, tids...> {
		private:
			template<typename C> static std::true_type check(tid<C>);
			template<typename> static std::false_type check(...);
		public:
			static bool const value = decltype(check<a>(std::declval<a>()))::value || is_type<a, tids...>::value;
	};

    template<typename a, template<typename> typename... tids>
	const auto is_type_v = is_type<a, tids...>::value;
}

#endif