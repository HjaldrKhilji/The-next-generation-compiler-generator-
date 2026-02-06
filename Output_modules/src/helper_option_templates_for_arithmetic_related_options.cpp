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
import estd_regex;
namespace printing_tools {
    namespace helper_templates_for_options {
        namespace helpers_for_arithmetic_options {
            template <absolute_base::Is_String_Or_Numeric T>
            inline T read_from_string(const std::string& string_to_read_from, std::string::size_type* pos) {
                if constexpr (std::is_same_v<T, std::string>) {
                    return read_string_from_string_at_a_position(string_to_read_from, pos);
                }
                else {
                    return absolute_base::read_number_from_string_at_a_position<T>(string_to_read_from, pos);
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
            template <bool read_from_x_or_y>
            inline T read_from_string<bool>(const std::string& x, const std::string& y, std::string::size_type* x_pos, std::string::size_type* y_pos) {
                constexpr if (read_from_x_or_y) {
                    absolute_base::convert_to_bool(x, x_pos);
                }
                else {
                    absolute_base::convert_to_bool(y, y_pos);

                }
            }
            template <bool read_from_x_or_y>
            inline T read_from_string<char>(const std::string& x, const std::string& y, std::string::size_type* x_pos, std::string::size_type* y_pos) {
                constexpr if (read_from_x_or_y) {
                    absolute_base::convert_to_char(x, x_pos);
                }
                else {
                    absolute_base::convert_to_char(y, y_pos);

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
                    throw std::string{ "number mixed with (non numeric) charactor while taking input for an option" };
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
                void pump(std::string* string_to_pump_to, std::string::size_type* output_string_position) {
                    if constexpr (!std::is_same_v<std::string, Internal_resperentation>) {
                        std::string to_pump = std::move(internal_data);
                        *string_to_pump_to += to_pump;
                        *(static_cast<uint64_t*>(output_string_position)) += to_pump.length();
                    }
                    else {
                        std::string to_pump = std::to_string(internal_data);
                        *string_to_pump_to += to_pump;
                        *(static_cast<uint64_t*>(output_string_position)) += to_pump.length();

                    }
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
            std::vector<std::string> all_polymorphic_strings{}
            struct string_index{
            uint64_t index;
            }
            struct Polymorphic_accumulator {
                std::variant<uint64_t, long double, all_polymorphic_strings> internal_data;


                void pump(std::string* string_to_pump_to, std::string::size_type* output_string_position) {
                    std::visit([&](auto&& arg) {
                        if constexpr (!std::is_same_v<string_index, decltype(arg)>) {
                            std::string to_pump = std::move(all_polymorphic_strings[arg.index]);
                            *string_to_pump_to += to_pump;
                            *(static_cast<uint64_t*>(output_string_position)) += to_pump.length();
                        }
                        else {
                            std::string to_pump = std::to_string(all_polymorphic_strings[arg.index]);
                            *string_to_pump_to += to_pump;
                            *(static_cast<uint64_t*>(output_string_position)) += to_pump.length();

                        }
                        
                        }, internal_data);
                }
                inline bool regex_match(std::string& string_to_match, std::string& pattern_to_be_matched) {
                    estd::smatch match_info;
                    return estd::regex_search(string_to_match, match_info, estd::regex{ pattern_to_be_matched });

                }
                bool regex_match(const Polymorphic_accumulator& rhs) {
                  
                        Polymorphic_accumulator result = std::visit([&](auto&& a, auto&& b) -> Polymorphic_accumulator {

                            if constexpr (std::is_same_v<std::string, decltype(a)> && std::is_same_v<std::string, decltype(b)>) {
                                regex_match(a, b);
                            }
                            else {
                                if (std::is_same_v<std::string, decltype(b)>) {
                                    regex_match(std::to_string(a), b);

                                }
                                else {
                                    throw std::string{ "DYNAMIC ARETHIMETIC ENGINE: operand mismatch in regex match (both operands are strings) " };

                                }
                            }
                            }, *this, rhs);
                   
                
                }

                template<typename Op_type>
                bool all_comparision_imp_generator(const Polymorphic_accumulator& lhs, const Polymorphic_accumulator& rhs, Op_type operator_name) {
                    Polymorphic_accumulator result = std::visit([&](auto&& a, auto&& b) -> Polymorphic_accumulator {
                    if constexpr (std::is_same_v<std::string, decltype(a)>) {
                        if constexpr (!std::is_same_v<std::string, decltype(b)>) {
                            return operator_name(a, b);

                        }
                        else {
                            return operator_name(a, std::to_string{ b });

                        }
                    }
                    else {
                        if constexpr (std::is_same_v<std::string, decltype(b)>) {
                            try {
                                if constexpr (std::is_same_v<long long int, decltype(a)>) {
                                    return operator_name(a, convert_to_number<long long int>(b));
                                }
                                else if constexpr (std::is_same_v<long double, decltype(a)>) {
                                    return operator_name(b, convert_to_number<long long int>(b));

                                }
                            }
                                catch (std::string error_from_converter) {
                                    throw std::string{ "DYNAMIC ARETHIMETIC ENGINE: "+ error_from_converter+'.'};
                                }
                            
                        }
                        else {
                            return operator_name(a , b);

                        }
                    }
                }, lhs, rhs);
                return result;
                }
                inline bool operator==(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_comparision_imp_generator(*this, polymorphic_accumulator, std::equal_to <>{});

                }
                inline bool operator!=(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_comparision_imp_generator(*this, polymorphic_accumulator, std::not_equal_to <>{});

                }
                inline bool operator<(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_comparision_imp_generator(*this, polymorphic_accumulator, std::less <>{});

                }
                inline bool operator>(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_comparision_imp_generator(*this, polymorphic_accumulator, std::greater <>{});

                }
                inline bool operator<=(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_comparision_imp_generator(*this, polymorphic_accumulator, std::less_equal <>{});

                }
                inline bool operator>=(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_comparision_imp_generator(*this, polymorphic_accumulator, std::greater_equal <>{});

                }
                Polymorphic_accumulator operator+(Polymorphic_accumulator polymorphic_accumulator) {
                    Polymorphic_accumulator result = std::visit([&](auto&& a, auto&& b) -> Polymorphic_accumulator {

                        if constexpr (std::is_same_v<std::string, decltype(a)>) {
                            if constexpr (!std::is_same_v<std::string, decltype(b)>) {
                                return Polymorphic_accumulator{ std::move(a) + std::move(std::to_string(b) };//used std::move() because of strings

                            }
                            else {
                                return Polymorphic_accumulator{ std::move(a) + std::move(b) };//used std::move() because of strings

                            }
                        }
                        else {
                            if constexpr (std::is_same_v<std::string, decltype(b)>) {
                                throw std::string{"DYNAMIC ARETHIMETIC ENGINE std::visit all_opoerator_impl compile time error, first paremeter is std::string, type mismatch. ")};
                            }
                            else {
                                return Polymorphic_accumulator{ a+b };

                            }
                        }
                        }, *this, polymorphic_accumulator);
                    return result;

                }
                template<typename Op_type>
                Polymorphic_accumulator all_operator_impl_generator(const Polymorphic_accumulator& lhs, const Polymorphic_accumulator& rhs, Op_type operator_name) {
                    
                    Polymorphic_accumulator result = std::visit([&](auto&& a, auto&& b) -> Polymorphic_accumulator {
                        //I used && to enable the good old reference forwading
                        if constexpr (requires { operator_name(a, b); }) {
                           
                           return Polymorphic_accumulator{ operator_name(a, b) };
                            
                        }
                        

                            else {
                            throw std::string{ "DYNAMIC ARETHIMETIC ENGINE std::visit all_opoerator_impl compile time error, first paremeter is std::string, type mismatch. ") };
                        }}
                            , lhs.internal_data, rhs.internal_data);
                        return result;
                    
                    
                   }
                inline Polymorphic_accumulator operator-(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_operator_impl_generator(*this, polymorphic_accumulator, std::plus <>{});

                }
                inline Polymorphic_accumulator operator-(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_operator_impl_generator(*this, polymorphic_accumulator, std::minus<>{});

                }
                inline Polymorphic_accumulator operator*(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_operator_impl_generator(*this, polymorphic_accumulator, std::multiplies<>{});

                }
                inline Polymorphic_accumulator operator*(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_operator_impl_generator(*this, polymorphic_accumulator, std::divides<>{});

                }
                inline Polymorphic_accumulator operator|(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_operator_impl_generator(*this, polymorphic_accumulator, std::bit_or<>{});

                }
                inline Polymorphic_accumulator operator&(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_operator_impl_generator(*this, polymorphic_accumulator, std::bit_and<>{});

                }
                inline Polymorphic_accumulator operator^(Polymorphic_accumulator polymorphic_accumulator) {
                    return all_operator_impl_generator(*this, polymorphic_accumulator, std::bit_xor<>{});

                }

            };
            Polymorphic_accumulator read_polymorphically_from_string(const std::string& string_to_read_from, std::string::size_type* pos) {
              
                    if (is_char_digit(string_to_read_from[*pos])) 
                        {
                            return Polymorphic_accumulator{ read_from_string<long long int>(string_to_read_from, pos) };

                         }
                    else if (string_to_read_from[*pos] == '.') {
                        return Polymorphic_accumulator{ read_from_string<long double>(string_to_read_from, pos) };
                    }
                    


                    else {
                        return  Polymorphic_accumulator{ read_from_string<std::string>(string_to_read_from, pos) };
                    }
                    
                

                

            }
            template <bool read_from_x_or_y>
            inline Polymorphic_accumulator read_polymorphically_from_string(const std::string& x, const std::string& y, std::string::size_type* x_pos, std::string::size_type* y_pos) {
                constexpr if (read_from_x_or_y) {
                    read_polymorphically_from_string(x, x_pos);
                }
                else {
                    read_polymorphically_from_string(y, y_pos);

                }
            }
        }
    }
}





