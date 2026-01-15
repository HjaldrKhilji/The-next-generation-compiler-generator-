module;
#include <string>
#include <stdexcept>
#include <type_traits>
#include <memory>
#include <concepts>
#include <utility>
#include <variant>
#include <functional>

//i used AI to track the header files needed
#include<utility>
module Printer;
import All_declarations;
namespace printing_tools {
    namespace helper_templates_for_options {
        namespace helpers_for_arithmetic_options {
            template <absolute_base::Is_String_Or_Numeric T>
            inline T read_from_string(const std::string& string_to_read_from, std::string::size_type* pos) {
                if constexpr (std::is_same_v<T, std::string>) {
                    return read_string_from_string_at_a_position(string_to_read_from, pos);
                }
                else {
                    return absolute_base::read_number_from_string_at_a_position(string_to_read_from, pos);
                }
            }

            template <absolute_base::Is_String_Or_Numeric T, bool read_from_x_or_y>
            inline T read_from_string(const std::string& x, const std::string& y, std::string::size_type* x_pos, std::string::size_type* y_pos) {
                constexpr if (read_from_x_or_y) {
                    read_from_string(x, x_pos);
                }
                else {
                    read_from_string(y, y_pos);

                }
            }

            template <absolute_base::Numeric T>
            inline std::string convert_to_string(T source) {
                return std::to_string(source);

            }
            inline std::string convert_to_string(std::string source) {
                return source;
            }
            template <absolute_base::Numeric T>
            inline T convert_to_number(T source) {
                return source;

            }
            template <absolute_base::Numeric T>
            T convert_to_number(std::string source) {
                std::string::size_type position = 0;
                T result = absolute_base::read_number_from_string_at_a_position<T>(source, &position);
                if (position != source.length()) {
                    throw std::runtime_error{ "number mixed with (non numeric) charactor while taking input for an option" };
                }
                return result;

            }
            template <absolute_base::Is_String_Or_Numeric T_dest, absolute_base::Is_String_Or_Numeric T_source>
            inline T_dest convert_to_target(T_source source) {
                if constexpr (std::is_same_v<T_dest, std::string>) {
                    return convert_to_string(source);
                }
                else {
                    return convert_to_number<T_dest>(source);
                }
            }
           
           
          
            template <absolute_base::Is_String_Or_Numeric Internal_resperentation_type>
            struct Accumulator > {
                using Internal_resperentation = Internal_resperentation_type;
                Accumulator(Internal_resperentation arg) :internal_data{ std::move(arg) } {}

                template <absolute_base::Numeric Internal_resperentation_of_type_converted>
                Accumulator(Accumulator<Internal_resperentation_of_type_converted> arg) {
                    interal_data = convert_to_target<Internal_resperentation>(arg.internal_data);
                }
                void pump(std::string* string_to_pump_to) {
                    *string_to_pump_to += std::string{ internal_data };
                }
                Internal_resperentation internal_data;
                 Accumulator<Internal_resperentation> operator+(Accumulator<Internal_resperentation> y) {

                    return std::move(internal_data) + std::move(y.internal_data);// using move() due to std::strings 


                }
                 Accumulator<Internal_resperentation> operator-(Accumulator<Internal_resperentation> y)
                    requires absolute_base::Numeric<Internal_resperentation> {

                    return internal_data - y.internal_data;

                }
                 Accumulator<Internal_resperentation> operator*(Accumulator<Internal_resperentation> y)
                    requires absolute_base::Numeric<Internal_resperentation> {

                    return internal_data * y.internal_data;

                }
                 Accumulator<Internal_resperentation> operator/(Accumulator<Internal_resperentation> y)
                    requires absolute_base::Numeric<Internal_resperentation> {

                    return internal_data / y.internal_data;

                }
            };

            
            inline bool is_char_digit(const char c) {

                if (c >= '0' && c <= '9') {
                    return true;
                }
                else {
                    return false;
                }
            }
            struct Polymorphic_accumulator {
                std::variant<long long int, long double, std::string> internal_data;
                
                
                void pump(std::string* string_to_pump_to) {
                    std::visit([&](auto&& arg) {
                        *string_to_pump_to += std::string{ std::move(arg) };
                        }, internal_data);
                }

               
                template<typename Op_type>
                Polymorphic_accumulator all_operator_impl_generator(const Polymorphic_accumulator& lhs, const Polymorphic_accumulator& rhs, Op_type operator_name) {
                    return std::visit([&](auto&& a, auto&& b) -> Polymorphic_accumulator {
                        // This 'if constexpr' checks at compile-time if the operator works for these types
                        if constexpr (requires { operator_name(a, b); }) {
                            return Polymorphic_accumulator{ op(std::move(a), std::move( b)) };//used std::move() because of strings
                        }
                        else {
                            throw std::runtime_error("Type mismatch in arithmetic operation");
                        }
                        }, lhs.internal_data, rhs.internal_data);
                }
                Polymorphic_accumulator operator-(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_operator_impl_generator(*this, polymorphic_accumulator, std::plus <>{});

                }
                Polymorphic_accumulator operator-(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_operator_impl_generator(*this, polymorphic_accumulator, std::minus<>{});

                }
                Polymorphic_accumulator operator*(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_operator_impl_generator(*this, polymorphic_accumulator, std::multiplies<>{});

                }
                Polymorphic_accumulator operator*(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_operator_impl_generator(*this, polymorphic_accumulator, std::divides<>{});

                }
                Polymorphic_accumulator operator|(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_operator_impl_generator(*this, polymorphic_accumulator, std::bit_or<>{});

                }
                Polymorphic_accumulator operator&(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_operator_impl_generator(*this, polymorphic_accumulator, std::bit_and<>{});

                }
                Polymorphic_accumulator operator^(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_operator_impl_generator(*this, polymorphic_accumulator, std::bit_xor<>{});

                }
                
            };
             Polymorphic_accumulator read_polymorphically_from_string(const std::string& string_to_read_from, std::string::size_type* pos) {
          
                if (is_char_digit(string_to_read_from[*pos])) {
                    {
                        std::string::size_type previous_pos = *pos;
                        long long int int_read = read_from_string<long long int>(string_to_read_from, pos);
                        if (string_to_read_from[*pos] == '.') {
                            *pos = previous_pos:
                            long double double_read = read_from_string<long double>(string_to_read_from, pos);
                            return Polymorphic_accumulator{ double_read };

                        }
                        else {
                            return Polymorphic_accumulator{ int_read };
                        }
                    }


                else {
                    return  Polymorphic_accumulator{ read_from_string<std::string>(string_to_read_from, pos) };
                }
                }
            }

        }
    }


