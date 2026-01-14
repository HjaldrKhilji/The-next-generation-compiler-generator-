module;
#include <string>       // For std::string, std::string::size_type
#include <stdexcept>    // For std::runtime_error
#include <type_traits>
#include <memory>
#include<concepts>

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

                Polymorphic_accumulator operator+(Polymorphic_accumulator polymorphic_accumulator) {
                    //warning ugly implementation that I had to add:

                    if (internal_data.index() == 0) {
                   
                            return Polymorphic_accumulator{ static_cast<long long int>
                                (std::get<long long int>(internal_data)) + static_cast<long long int>
                                (std::get<long long int>(polymorphic_accumulator.internal_data)) };
                        }
                        else if (data.index() == 1) {
                            return Polymorphic_accumulator{ static_cast<long double>
                                (std::get<long double>(internal_data)) + static_cast<long double>
                                (std::get<long double>(polymorphic_accumulator.internal_data)) };
                        }
                        else  if (data.index() == 2) {
                            return Polymorphic_accumulator{ static_cast<std::string>
                                (std::get<std::string>(internal_data)) + static_cast<std::string>
                                (std::get<std::string>(polymorphic_accumulator.internal_data)) };
                        }

                       
                        }
                template< template<typename, typename> typename operator_func>
                Polymorphic_accumulator all_numeric_operator_impl_generator(Polymorphic_accumulator polymorphic_accumulator) {
                    //in case future operators were to be added, this is my goto template
                    //warning ugly implementation that I had to add:

                    if (internal_data.index() == 0) {
                        return Polymorphic_accumulator{ operator_func(static_cast<long  long int>
                                (std::get<long long int>(internal_data)) , static_cast<long  long int>
                                (std::get<long long int>(polymorphic_accumulator.internal_data)))};
                    }
                    else if (internal_data.index() == 1) {
                        return Polymorphic_accumulator{ operator_func(static_cast<long double>
                                (std::get<long double>(internal_data)) , static_cast<long double>
                                (std::get<long double>(polymorphic_accumulator.internal_data)) };
                    }
                    else {
                        throw std::runtime_error{ "string used in arithmetic other than addition" };
                    } 
                }

                Polymorphic_accumulator operator-(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_numeric_operator_impl_generator<std::minus>(polymorphic_accumulator);

                }
                Polymorphic_accumulator operator*(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_numeric_operator_impl_generator<std::multiplies>(polymorphic_accumulator);

                }
                Polymorphic_accumulator operator*(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_numeric_operator_impl_generator<std::divides>(polymorphic_accumulator);

                }
                Polymorphic_accumulator operator|(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_numeric_operator_impl_generator<std::logical_or>(polymorphic_accumulator);

                }
                Polymorphic_accumulator operator&(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_numeric_operator_impl_generator<std::logical_and>(polymorphic_accumulator);

                }
                Polymorphic_accumulator operator^(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_numeric_operator_impl_generator<std::logical_xor>(polymorphic_accumulator);

                }
                
            };
            inline Polymorphic_accumulator read_polymorphically_from_string(const std::string& string_to_read_from, std::string::size_type* pos) {
          
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


