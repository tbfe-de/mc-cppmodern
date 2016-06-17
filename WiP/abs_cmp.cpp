auto abs_cmp_external_function(double left, double right) {
#if 1
    return left < right;
#else
    return (std::abs(left) < std::abs(right)
        || (std::abs(left) == std::abs(right) && left < right));
#endif
}
