#ifndef __TPRELUDE_HPP__
#define __TPRELUDE_HPP__

#include <iostream>
#include <type_traits>

namespace TPrelude::TypeSystem {
	struct Type {};

	template<typename e, typename ua, typename bound>
	struct substitute {
		using type = e;
	};

	template<typename e, typename ua, typename bound>
	using substitute_t = typename substitute<e, ua, bound>::type;

	template<typename ua, typename e>
	struct ForAll {
		template<typename ba>
		using apply = substitute_t<e, ua, ba>;
	};

	template<uint i>
	struct free {
		using htype = Type;
	};

	template<typename... es>
	struct unbound {
	};

	template<typename... es>
	using unbound_t = typename unbound<es...>::type;

	template<>
	struct unbound<> {
		using type = free<0>; // HACK
	};

	template<typename e>
	struct unbound<e> {
		using type = free<0>; // HACK
	};

	template<typename ua, typename e>
	struct unbound<ForAll<ua, e>> {
		using type = unbound_t<e>;
	};

	template<typename ua, typename bound> 
	struct substitute<ua, ua, bound> {
		using type = bound;
	};

	template<typename e, typename ua, typename bound>
	struct substitute<ForAll<ua, e>, ua, bound> {
		using type = substitute_t<e, ua, bound>;
	};

	template<typename e, typename ua, typename ub, typename bound>
	struct substitute<ForAll<ua, e>, ub, bound> {
		private:
		using e_ = substitute_t<e, ub, bound>;

		public:
		using type = ForAll<ua, e_>;
	};

	template<template<typename> typename tpl, typename a0, typename ua, typename bound>
	struct substitute<tpl<a0>, ua, bound> {
		private:
		using a0_ = substitute_t<a0, ua, bound>;

		public:
		using type = tpl<a0_>;
	};

	template<template<typename, typename> typename tpl, typename a0, typename a1, typename ua, typename bound>
	struct substitute<tpl<a0, a1>, ua, bound> {
		private:
		using a0_ = substitute_t<a0, ua, bound>;
		using a1_ = substitute_t<a1, ua, bound>;

		public:
		using type = tpl<a0_, a1_>;
	};

	template<typename x, typename a>
	const auto is_v = std::is_same<typename x::htype, a>::value;

	template<typename x, typename a>
	using is = std::enable_if<is_v<x, a>, bool>;

	template<typename x, typename a>
	using is_t = typename is<x, a>::type;

	template<typename a, typename b>
	struct Tuple {
		using htype = Type;

		template<typename x, typename y, is_t<x, a> = true, is_t<y, b> = true>
		struct Pair {
			using htype = Tuple<a, b>;
		};

		template<typename, typename>
		struct match {};

		template<typename pattern, typename x, typename y>
		struct match<pattern, Pair<x, y>> {
			using type = typename pattern::template Pair<x, y>;
		};
	};

	template<typename a, typename b>
	struct Func {

	};

	template<typename a, typename b, typename ua, typename bound>
	struct substitute<Func<a, b>, ua, bound> {
		private:
		using a_ = substitute_t<a, ua, bound>;
		using b_ = substitute_t<b, ua, bound>;

		public:
		using type = Func<a_, b_>;
	};

	template<typename htype, template<typename...> typename f, typename... bound_type_args>
	struct import_fn {};

	template<typename a, typename b, template<typename...> typename f, typename... bas>
	struct import_fn<Func<a, b>, f, bas...> {
		private:
		template<typename t, typename... xs>
		struct curried_fn {};

		template<typename a_, typename b_, typename c, typename... xs>
		struct curried_fn<Func<a_, Func<b_, c>>, xs...> {
			using htype = Func<a_, Func<b_, c>>;

			using type = curried_fn<Func<a_, Func<b_, c>>, xs...>;

			template<typename y, is_t<y, a_> = true>
			using apply = typename curried_fn<Func<b_, c>, xs..., y>::type;
		};

		template<typename a_, typename b_, typename... xs>
		struct curried_fn<Func<a_, b_>, xs...> {
			using htype = Func<a_, b_>;

			using type = curried_fn<Func<a_, b_>, xs...>;

			template<typename y>
			using apply = typename f<bas..., xs..., y>::type;
		};

		public:
		using htype = Func<a, b>;

		template<typename x, is_t<x, a> = true>
		using apply = typename curried_fn<htype>::template apply<x>;
	};

	template<typename ua, typename e, template<typename...> typename f, typename... bas>
	struct import_fn<ForAll<ua, e>, f, bas...> {
		public:
		using htype = ForAll<ua, e>;
		
		private:
		template<typename a, is_t<a, Type> = true>
		using htype_ = typename htype::template apply<a>;;

		public:
		template<typename a, is_t<a, Type> = true>
		using apply = import_fn<htype_<a>, f, bas..., a>;
	};

	template<typename a, typename b, typename... cs>
	struct _NFunc {
		using type = Func<a, typename _NFunc<b, cs...>::type>;
	};

	template<typename a, typename b>
	struct _NFunc<a, b> {
		using type = Func<a, b>;
	};

	template<typename a, typename b, typename... cs>
	using NTuple = typename _NFunc<a, b, cs...>::type;

	template<typename e, typename ua, typename... uas>
	struct forAll_ {
		using type = ForAll<ua, forAll_<e, uas...>>;
	};

	template<typename e, typename ua>
	struct forAll_<e, ua> {
		using type = ForAll<ua, e>;
	};

	template<typename e, typename ua, typename... uas>
	using forAll_t = typename forAll_<e, ua, uas...>::type;
}

namespace TPrelude {
	using Type = TypeSystem::Type;

	template<typename a, typename b, typename... cs>
	using Func = TypeSystem::NTuple<a, b, cs...>;

	template<typename ua, typename e>
	using ForAll = TypeSystem::ForAll<ua, e>;

	template<typename e, typename ua, typename... uas>
	using ForAll_ = TypeSystem::forAll_t<e, ua, uas...>;

	using a_ = TypeSystem::free<0>;
	using b_ = TypeSystem::free<1>;
	using c_ = TypeSystem::free<2>;

	namespace Definition {
		template<typename a>
		struct List {
			using htype = Type;

			struct Nil {
				using htype = List<a>;
			};

			template<typename head, typename tail, TypeSystem::is_t<head, a> = true, TypeSystem::is_t<tail, List<a>> = true>
			struct Cons {
				using htype = List<a>;
			};

			template<typename pattern, typename x>
			struct match {};

			template<typename pattern>
			struct match<pattern, Nil> {
				using type = typename pattern::Nil;
			};

			template<typename pattern, typename head, typename tail>
			struct match<pattern, Cons<head, tail>> {
				using type = typename pattern::template Cons<head, tail>;
			};
		};

		template<typename a>
		struct Maybe {
			using htype = Type;

			struct Nothing {
				using htype = Maybe<a>;
			};

			template<typename x, TypeSystem::is_t<x, a> = true>
			struct Just {
				using htype = Maybe<a>;
			};

			template<typename pattern, typename x>
			struct match {};

			template<typename pattern>
			struct match<pattern, Nothing> {
				using type = typename pattern::Nothing;
			};

			template<typename pattern, typename x>
			struct match<pattern, Just<x>> {
				using type = typename pattern::template Just<x>;
			};
		};

		template<typename a, typename b>
		struct Either {
			using htype = TypeSystem::Type;

			template<typename x, TypeSystem::is_t<x, a> = true>
			struct Left {
				using type = Either<a, b>;
			};

			template<typename x, TypeSystem::is_t<x, b> = true>
			struct Right {
				using type = Either<a, b>;
			};

			template<typename, typename>
			struct match {};

			template<typename pattern, typename x>
			struct match<pattern, Left<x>> {
				using type = typename pattern::template Left<x>;
			};

			template<typename pattern, typename x>
			struct match<pattern, Right<x>> {
				using type = typename pattern::template Right<x>;
			};
		};
	}

	using Tuple = TypeSystem::ForAll<a_, ForAll<b_, TypeSystem::Tuple<a_, b_>>>;

	using List = TypeSystem::ForAll<a_, Definition::List<a_>>;

	struct Bool {
		using htype = Type;

		struct False {
			using htype = Bool;
		};
		struct True {
			using htype = Bool;
		};

		template<typename, typename>
		struct match {};

		template<typename pattern>
		struct match<pattern, False> {
			using type = typename pattern::False;
		};

		template<typename pattern>
		struct match<pattern, True> {
			using type = typename pattern::True;
		};
	};

	/// Boolean false
	using False = Bool::False;

	/// Boolean true
	using True = Bool::True;

	/*!
	 * Boolean "and"
	 */
	struct bool_and {
		private:
		template<typename x, typename y>
		struct fn {
			struct pattern {
				using False = Bool::False;
				using True = y;
			};

			public:
			using type = typename Bool::match<pattern, x>::type;
		};

		public:
		using htype = Func<Bool, Bool, Bool>;

		template<typename x>
		using apply = TypeSystem::import_fn<htype, fn>::apply<x>;
	};

	/*!
	 * Boolean "or"
	 */
	struct bool_or {
		private:
		template<typename x, typename y>
		struct fn {
			struct pattern {
				using False = y;
				using True = Bool::True;
			};

			using type = typename Bool::match<pattern, x>::type;
		};

		public:
		using htype = Func<Bool, Bool, Bool>;

		template<typename x>
		using apply = TypeSystem::import_fn<htype, fn>::apply<x>;
	};

	/*!
	 * Boolean "not"
	 */
	struct bool_not {
		private:
		template<typename x>
		struct fn {
			struct pattern {
				using False = Bool::True;
				using True = Bool::False;
			};

			using type = typename Bool::match<pattern, x>::type;
		};

		public:
		using htype = Func<Bool, Bool>;

		template<typename x>
		using apply = TypeSystem::import_fn<htype, fn>::apply<x>;
	};

	using otherwise = True;

	using Maybe = ForAll<a_, Definition::Maybe<a_>>;

	struct Nothing {
		using htype = Maybe;

		template<typename a, TypeSystem::is_t<a, TypeSystem::Type> = true>
		using apply = typename Maybe::apply<a>::Nothing;
	};

	struct Just {
		private:
		template<typename a, typename x>
		struct fn {
			using type = typename Maybe::apply<a>::template Just<x>;
		};

		public:
		using htype = ForAll<a_, Func<a_, Maybe::apply<a_>>>;

		template<typename a, TypeSystem::is_t<a, TypeSystem::Type> = true>
		using apply = TypeSystem::import_fn<htype, fn>::apply<a>;
	};

	struct maybe {
		public:
		using htype = ForAll<a_, ForAll<b_, Func<b_, Func<a_, b_>, Maybe::apply<a_>, b_>>>;

		private:
		template<typename a, typename b, typename y, typename f, typename mx>
		struct fn {
			struct pattern {
				using Nothing = y;

				template<typename x>
				using Just = typename f::template apply<x>;
			};

			using type = typename Maybe::apply<a>::template match<pattern, mx>::type;
		};

		public:
		template<typename a, TypeSystem::is_t<a, TypeSystem::Type> = true>
		using apply = TypeSystem::import_fn<htype, fn>::apply<a>;
	};

	using Either = ForAll<a_, ForAll<b_, Definition::Either<a_, b_>>>;

	struct Left {
		using htype = ForAll<a_, ForAll<b_, Func<a_, Either::apply<a_>::apply<b_>>>>;

		private:
		template<typename a, typename b, typename x>
		using fn = typename Either::template apply<a>::template apply<b>::template Left<x>;

		public:
		template<typename a, TypeSystem::is_t<a, TypeSystem::Type> = true>
		using apply = TypeSystem::import_fn<htype, fn>::apply<a>;
	};

	struct Right {
		using htype = ForAll<a_, ForAll<b_, Func<b_, Either::apply<a_>::apply<b_>>>>;

		private:
		template<typename a, typename b, typename x>
		using fn = typename Either::template apply<a>::template apply<b>::template Right<x>;

		public:
		template<typename a, TypeSystem::is_t<a, TypeSystem::Type> = true>
		using apply = TypeSystem::import_fn<htype, fn>::apply<a>;
	};

	struct Ordering {
		using htype = Type;

		struct LT {
			using htype = Ordering;
		};

		struct EQ {
			using htype = Ordering;
		};

		struct GT {
			using htype = Ordering;
		};

		template<typename, typename>
		struct match {};

		template<typename pattern>
		struct match<pattern, LT> {
			using type = typename pattern::LT;
		};

		template<typename pattern>
		struct match<pattern, EQ> {
			using type = typename pattern::EQ;
		};

		template<typename pattern>
		struct match<pattern, GT> {
			using type = typename pattern::GT;
		};
	};

	using LT = Ordering::LT;
	using EQ = Ordering::EQ;
	using GT = Ordering::GT;

	struct Char {
		/* magic goes here */
	};

	typedef List::apply<Char> String;

	struct fst {
		using htype = ForAll<a_, ForAll<b_, Func<Tuple::apply<a_>::apply<b_>, a_>>>;

		private:
		template<typename a, typename b, typename xy>
		struct fn {
			struct pattern {
				template<typename x, typename y>
				using Pair = x;
			};

			using type = 
				typename Tuple::
				template apply<a>::
				template apply<b>::
				template match<pattern, xy>::
				type;
		};

		public:
		template<typename a, TypeSystem::is_t<a, Type> = true>
		using apply = TypeSystem::import_fn<htype, fn>::apply<a>;
	};

	struct snd {
		using htype = ForAll<a_, ForAll<b_, Func<Tuple::apply<a_>::apply<b_>, b_>>>;

		private:
		template<typename a, typename b, typename xy>
		struct fn {
			struct pattern {
				template<typename x, typename y>
				using Pair = y;
			};

			using type = 
				typename Tuple::
				template apply<a>::
				template apply<b>::
				template match<pattern, xy>::
				type;
		};

		public:
		template<typename a, TypeSystem::is_t<a, Type> = true>
		using apply = TypeSystem::import_fn<htype, fn>::apply<a>;
	};

	// struct curry {
	// 	private:
	// 	using uncurried = Func<Tuple::apply<a_>::apply<b_>, c_>;

	// 	public:
	// 	using htype = ForAll_<Func<uncurried, a_, b_, c_>, a_, b_, c_>;

	// 	private:
	// 	template<typename a, typename b, typename c, typename f>
	// 	struct fn {

	// 	};
	// };
}

#endif