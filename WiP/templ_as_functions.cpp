template<typename T> struct show_type_as_error_message;

#if 0
template<bool B, typename TypeInTrueCase, typename TypeInFalseCase>
struct either_or;

template<typename TypeInTrueCase, typename TypeInFalseCase>
struct either_or<true, TypeInTrueCase, TypeInFalseCase> {
    using type = TypeInTrueCase;
};
template<typename TypeInTrueCase, typename TypeInFalseCase>
struct either_or<false, TypeInTrueCase, TypeInFalseCase> {
    using type = TypeInFalseCase;
};

template<typename TypeOfParameter>
struct optimal_arg_type {
    using type = typename either_or<sizeof(TypeOfParameter) <= 4,
                                    TypeOfParameter, const TypeOfParameter&
                                   >::type;
};
#else
#include <type_traits>
template<typename TypeOfParameter>
struct optimal_arg_type {
    using type = std::conditional_t<std::is_arithmetic<TypeOfParameter>{},
                                    TypeOfParameter, const TypeOfParameter&
                                   >;
};
#endif

class point { short x; short y; /*....*/ };

show_type_as_error_message<     optimal_arg_type<short>::type    > dummy;

int main() {}
